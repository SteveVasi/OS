#include <stdio.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>

#define PERMISSION 0600
#define SEM_PATH "/shm_12225126"
#define SEM_CLIENT_READY "/shm_client_ready"
#define SEM_SERVER_READY "/shm_server_ready"
#define BUFFER_SIZE 256

//Konstrukt um die Kommunikation zu steuern.
typedef struct {
    char buffer[BUFFER_SIZE]; //Zeilenspeicher
} shm_t;

char *progName = "<Not set yet>";


static void description(char* msg){
    fprintf(stderr, "%s : %s\n", msg, progName);
    fprintf(stderr, "Commandusage: %s [-u] | [-l] FILENAME\n", progName);
    exit(EXIT_FAILURE);
}

static void error(char *msg){
    fprintf(stderr, "%s : %s\n", msg, progName);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
    progName = argv[0];
   
    if(argc < 2 || argc > 3){description("Invalid amount of parameters");}

    bool upper = false;
    bool lower = false;

    int opt = 0;
    while((opt = getopt(argc, argv, "ul")) != -1){
        switch(opt){
            case 'u':
            if(upper || lower){description("A case was already chosen");}
            upper = true;
            break;

            case 'l':
            if(upper || lower){description("A case was already chosen");}
            lower = true;
            break;

            default:
            description("Invalid option");

        }
    }
    FILE *in = fopen(argv[argc-1], "r");
    if(!in){error("Failed to open file. May not exist!");}

    //initialize shared memory
    int shmfd = shm_open(SEM_PATH, O_RDWR | O_CREAT, PERMISSION);
    //Filedescription für Shared memory anlegen
    if(shmfd == -1){error("Shared memory blocked");}
    //Speichergröße anpassen
    if(ftruncate(shmfd, sizeof(shm_t)) == -1){
        close(shmfd);
        error("Truncation failure");
        }
    //Memory im speicher abbilden
    // Aufruf mmap(ADDRESS, Speichergröße, Zugriffsrechte, Maptyp, Filedescriptor, Startposition)
    //Ist Adresse NULL sucht sich das System selber die passende aus.
    shm_t *shared = mmap(NULL, sizeof(shm_t), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if(shared == MAP_FAILED){
        close(shmfd); // Ist im Fehlerfall zu schließen
        error("Failed to map memory");
        }

    //Initialisierung des Memory
    //Aufruf sem_open(Semaphorenname, Flags, Zugriffscode, Initialwert)
    sem_t *client = sem_open(SEM_CLIENT_READY, 0);
    sem_t *server = sem_open(SEM_SERVER_READY, 0);

    if(client == SEM_FAILED || server == SEM_FAILED){
        munmap(shared, sizeof(shm_t));//Mapping in gleicher Größe rückgängig machen
        close(shmfd);//Filedescriptor schließen.
        error("Semaphore failed");
    }

    char *line = NULL;
    size_t len = 0;
    //int sval;
    //sem_getvalue(server, &sval);
        //printf("Server zustand: %d\n", sval);
        //sem_getvalue(client, &sval);
        //rintf("Client zustand: %d\n", sval);

    while(getline(&line, &len, in) != -1){
        sem_wait(server); //Wartet dass der Server bereit ist.
        //sem_getvalue(server, &sval);
        //printf("Server veringert: %d\n", sval);
        if(upper){
            for(size_t i = 0; i < strlen(line); i++){
                line[i] = toupper(line[i]);
            }
        }
        if(lower){
             for(size_t i = 0; i < strlen(line); i++){
                line[i] = tolower(line[i]);
            }
        }
         //fprintf(stdout, "Sended\n");
        strncpy(shared->buffer, line, BUFFER_SIZE);
        sem_post(client);
        //sem_getvalue(client, &sval); //Liest den aktuellen Wert des Semaphores aus. (Hilfreich!)
        //printf("Client erhöht: %d\n", sval);
    }

    //Line kann ab hier freigegeben werden.
    free(line);

    //Das ende Signalisieren
    sem_wait(server);
    //sem_getvalue(server, &sval);
    //printf("Server veringert: %d\n", sval);
    strncpy(shared->buffer, "EOF", BUFFER_SIZE);
    sem_post(client);
    //sem_getvalue(client, &sval);
    //printf("Client erhöht: %d\n", sval);


    //Cleaning
    fclose(in); //Schließe Filedescriptor der Datei
    munmap(shared, sizeof(shm_t)); //Unmapping
    close(shmfd); //Schließe Filedescriptor des Shared Memory
    //shm_unlink(SEM_PATH); //unlink den 

    //Der auskommentierte Code ist nur dann einzusetzen, wenn der Semaphor nach gebrauch eines Clients
    //nicht länger gebraucht wird.

    //sem_unlink(SEM_CLIENT_READY); //Entferne link --> Gilt für alle Instanzen, heißt Semaphor ist weg!!
    //sem_unlink(SEM_SERVER_READY); //Entferne link --> Gilt für alle Instanzen, heißt Semaphor ist weg!!
    sem_close(client); //Schließe Semaphor
    //sem_close(server); //Schließe Semaphor

    exit(EXIT_SUCCESS);
    
}





