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

//Signalkontrolle
volatile sig_atomic_t quit = 0;
struct sigaction sa;
static void handleSig(int sig){
    quit = 1;
}

static void description(char* msg){
    fprintf(stderr, "%s : %s\n", msg, progName);
    fprintf(stderr, "Commandusage: %s\n", progName);
    exit(EXIT_FAILURE);
}

static void error(char *msg){
    fprintf(stderr, "%s : %s\n", msg, progName);
    exit(EXIT_FAILURE);
}

// Repräsentiert den Server
int main( int argc, char **argv){
    progName = argv[0];
    //fprintf(stdout, "This worked");

    if(argc > 1){description("Invalid option. None is supposed");}

    //Signalverarbeitung 
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleSig;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    //Filedescriptor für Shared Memory eröffnen
    int shmfd = shm_open(SEM_PATH, O_RDWR | O_CREAT, PERMISSION);
    if(shmfd == -1){
            error("Filedescriptor failed!");
    }

    //Speichergröße anpassen
    if(ftruncate(shmfd, sizeof(shm_t)) == -1){
        close(shmfd);
        error("Truncation failure");
        }

    //Mapping durchführen
    shm_t *shared = mmap(NULL, sizeof(shm_t), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if(shared == MAP_FAILED){
        close(shmfd);
        error("Mapping error");
    }

    //Semaphore öffnen
    sem_t *client = sem_open(SEM_CLIENT_READY, O_CREAT, PERMISSION, 0);
    sem_t *server = sem_open(SEM_SERVER_READY, O_CREAT, PERMISSION, 1);

    if(client == SEM_FAILED || server == SEM_FAILED){
        munmap(shared, sizeof(shm_t));
        close(shmfd);
        error("Semaphoren fehler");
    }

    //Engegennehmen der Nachrichten
    //int sval;
    while(!quit){
        //sem_getvalue(server, &sval);
        //printf("Server zustand: %d\n", sval);
        //sem_getvalue(client, &sval);
        //printf("Client zustand: %d\n", sval);

        sem_wait(client);
        //sem_getvalue(client, &sval);
        //printf("Client verringert: %d\n", sval);
        //fprintf(stdout, "LISTEN: ");
        if(strcmp(shared->buffer, "EOF")==0){ //Falls eine Datei ihr Ende hat 
            fprintf(stdout, "===================================================================================\n");
            sem_post(server);
            memset(shared->buffer, 0, BUFFER_SIZE); //Reset buffer;
            //sem_getvalue(server, &sval);
            //printf("Server erhöht: %d\n", sval);
            continue;        
        }
        if(strcasecmp(shared->buffer, "Server-Kill\n") == 0){
            fprintf(stdout, "Server will shutdown\n");
            break;
        }
        fprintf(stdout, "%s", shared->buffer);
        sem_post(server);
        //sem_getvalue(server, &sval);
        //printf("Server erhöht: %d\n", sval);
    }

    //Clean-Up
    munmap(shared, sizeof(shm_t));
    //close(shmfd);
    //sem_close(client);
    sem_close(server);
    sem_unlink(SEM_CLIENT_READY);
    sem_unlink(SEM_SERVER_READY);
    exit(EXIT_SUCCESS);

    
}