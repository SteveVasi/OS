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

char *progName = "sem_training_server";

char *shmp = MAP_FAILED;
int shmfd = -1;
sem_t *sem_request = SEM_FAILED;
sem_t *sem_client = SEM_FAILED;
sem_t *sem_response = SEM_FAILED;

//Signalkontrolle
volatile sig_atomic_t running = 1;

static void usage(char* msg){
    fprintf(stderr, "%s : %s\n", msg, progName);
    fprintf(stderr, "Commandusage: %s\n", progName);
    exit(EXIT_FAILURE);
}

void cleanUpShm(void){
    close(shmfd);
    shm_unlink(SHM_PATH);
}
void cleanUpSems(void){
    sem_close(sem_client);
    sem_close(sem_request);
    sem_close(sem_response);
    sem_unlink(SEM_CLIENT);
    sem_unlink(SEM_REQUEST);
    sem_unlink(SEM_RESPONSE);
}

void freeResources(){
    cleanUpShm();
    cleanUpSems();
}

void allocateResources(void){
    
    shmfd = shm_open(SHM_PATH, O_CREAT | O_RDWR, PERMISSION);
    if(shmfd == -1) {
        errorExit("error opening shm");
    }

    if(ftruncate(shmfd, SHM_SIZE) == -1){
        close(shmfd);
        shm_unlink(SHM_PATH);
        errorExit("error opening shm");
    }

    shmp = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if(shmp == MAP_FAILED){
        close(shmfd);
        shm_unlink(SHM_PATH);
        errorExit("mmap failed");
    }

    sem_client = sem_open(SEM_CLIENT, O_CREAT, PERMISSION, 1);
    if(sem_client == SEM_FAILED){
        cleanUpShm();
        errorExit("open sem_client failed");
    }

    sem_request = sem_open(SEM_REQUEST, O_CREAT, PERMISSION, 0);
    if(sem_request == SEM_FAILED){
        sem_close(sem_client);
        cleanUpShm();
        errorExit("open sem_request failed");
    }
    
    sem_response = sem_open(SEM_RESPONSE, O_CREAT, PERMISSION, 0);
    if(sem_response == SEM_FAILED){
        sem_close(sem_client);
        sem_close(sem_request);
        cleanUpShm();
        errorExit("open sem_response failed");
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

// Repräsentiert den Server
int main( int argc, char **argv){
    if(argc > 1){
        usage("Program does not take any arguments");
    }


    allocateResources();

    while(running == 1){
        printSems();
        if (sem_wait(sem_request) != 0) {
            perror("sem_wait failed in server");
            break;
        }
        
        if (strlen(shmp) > 0) {
            printf("Received from client: %s", shmp);
        }

        if (sem_post(sem_client) != 0) {
            perror("sem_post failed in server");
            break;
        }
    }

    // rm /dev/shm/*
    freeResources();
    return EXIT_SUCCESS;
}