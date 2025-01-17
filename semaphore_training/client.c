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
#include "commons.h"

#define errorCode int

char *progName = "sem_training_client";

char *shmp = MAP_FAILED;
int shmfd;
sem_t *sem_request = SEM_FAILED;
sem_t *sem_response = SEM_FAILED; // this one is actually not used
sem_t *sem_client = SEM_FAILED;

static void usage (char* msg){
    fprintf(stderr, "%s : %s\n", msg, progName);
    fprintf(stderr, "Commandusage: %s [-u] | [-l] FILENAME\n", progName);
    exit(EXIT_FAILURE);
}

FILE *openFile(char *name) {
    FILE *rv = fopen(name, "r");
    if(rv == NULL) {
        perror("Error opening file. Did you provide the right name?");
        exit(EXIT_FAILURE);
    }
    return rv;
}

errorCode writeToBuffer(char* line){

    if(shmp == MAP_FAILED){
        perror("Shared memory is not initialized");
        return -1;
    }

    if(sem_wait(sem_client)){
        perror("sem_wait failed in writeToBuffer");
        return -1;
    }
    
    strncpy(shmp, line, SHM_SIZE-1); // buffer overflow possible, too lazy to fix
    shmp[SHM_SIZE - 1] = '\0';

    if(sem_post(sem_request)){
        perror("sem_post failed in writeToBuffer");
        return -1;
    };

    return 0;
}

void cleanUpShm() {
    munmap(shmp, SHM_SIZE);
    close(shmfd);
}

void cleanUpSems() {
    sem_close(sem_client);
    sem_close(sem_request);
    sem_close(sem_response);
}

void freeResources(){
    cleanUpShm();
    cleanUpSems();
}

void allocate_resources(void) {
    shmfd = shm_open(SHM_PATH, O_RDWR | O_CREAT, PERMISSION);
    if(shmfd == -1){
        errorExit("Could not open shared memory");
    }

    if(ftruncate(shmfd, SHM_SIZE) == -1){
        close(shmfd);
        shm_unlink(SHM_PATH);
        errorExit("Could not truncate shm");
    }

    shmp = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if(shmp == MAP_FAILED){
        close(shmfd);
        shm_unlink(SHM_PATH);
        errorExit("Could not map shm");
    }

    sem_request = sem_open(SEM_REQUEST, O_CREAT);
    if(sem_request == SEM_FAILED){
        cleanUpShm();
        errorExit("Could not open sem");
    }

    sem_response = sem_open(SEM_RESPONSE, O_CREAT);
    if(sem_request == SEM_FAILED){
        sem_close(sem_request);
        cleanUpShm();
        errorExit("Could not open sem");
    }

    sem_client = sem_open(SEM_CLIENT, O_CREAT);
    if(sem_request == SEM_FAILED){
        sem_close(sem_request);
        sem_close(sem_response);
        cleanUpShm();
        errorExit("Could not open sem");
    }


}

void printSems(){
    int r = -1;
    int c = -1;
    sem_getvalue(sem_request, &r);
    sem_getvalue(sem_client, &c);
    printf("request:%d\n", r);
    printf("client: %d\n", c);
}

// start 14:30
int main(int argc, char **argv){
    int opt;
    int toUpperFlag = 0;
    int toLowerFlag = 0;
    
    while ((opt = getopt(argc, argv, "ul")) != -1) {
        switch (opt) {
            case 'u':
                toUpperFlag++;
                break;
            case 'l':
                toLowerFlag++;
                break;
            default:
                usage("improper argument provided");
                exit(EXIT_FAILURE);
        }
    }

    if(toLowerFlag > 1) {
        usage("-l provided more than once");
        exit(EXIT_FAILURE);
    }

    if(toUpperFlag > 1) {
        usage("-u provided more than once");
        exit(EXIT_FAILURE);
    }

    if(argc > 3) {
        usage("too many arguments");
        exit(EXIT_FAILURE);
    }

    char *filePath = argv[optind];

    FILE *file = openFile(filePath);

    allocate_resources();
    
    printSems();
    char *line = NULL;
    size_t size = 0;
    size_t readCount;
    while((readCount = getline(&line, &size, file)) != -1) {
        if(writeToBuffer(line)){
            perror("error writing to buffer");
            break;
        }
    }

/*
    if (writeToBuffer("END\n") != 0) {
        perror("Error writing termination signal");
    }
*/
    printSems();
    free(line);
    freeResources();
    fclose(file);
    return EXIT_SUCCESS;
}





