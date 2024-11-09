#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <semaphore.h>
#include <stdlib.h> 
#include "edge.h"

#define CIRCULAR_ARRAY_BUFFER "/circulararraybuffer"
#define MAX_EDGES (8)
#define BUFFER_SIZE (10)
#define SEM_FREE_SPACE "/semfreespace"
#define SEM_USED_SPACE "/semusedspace"
#define SEM_WRITE_MUTEX "/semwritemutex"

typedef struct
{
    edge edgeSet[MAX_EDGES];
    int edgeCount;
} edgeSet;

/*
    only (n-1) elements can be stored at a given time
*/
typedef struct
{
    edgeSet buffer[BUFFER_SIZE];
    int writeIndex;
    int readIndex;
    sem_t *freeSpace;
    sem_t *usedSpace;
    sem_t *writeMutex;
} circularBuffer;

void exitOnSemError(void){
    perror("An error with a semaphore has occurred. Exiting program now");
    _exit(EXIT_FAILURE);
}

void checkForSemError(circularBuffer *circularBuffer){
    if (circularBuffer->freeSpace == SEM_FAILED ||
        circularBuffer->usedSpace == SEM_FAILED ||
        circularBuffer->writeMutex == SEM_FAILED)
    {
        exitOnSemError();
    }
}


void initCircularBuffer(circularBuffer *circularBuffer)
{
    circularBuffer->writeIndex = 0;
    circularBuffer->readIndex = 0;
    circularBuffer->freeSpace = sem_open(SEM_FREE_SPACE, O_CREAT | O_EXCL, 0600, BUFFER_SIZE);
    circularBuffer->usedSpace = sem_open(SEM_USED_SPACE, O_CREAT | O_EXCL, 0600, 0);
    circularBuffer->writeMutex = sem_open(SEM_WRITE_MUTEX, O_CREAT | O_EXCL, 0600, 1);
    checkForSemError(circularBuffer);
}

void writeToBuffer(edgeSet *edgeSet, circularBuffer *circularBuffer){
    int err1 = sem_wait(&circularBuffer->freeSpace);
    int err2 = sem_wait(&circularBuffer->writeMutex);

    circularBuffer->buffer[circularBuffer->writeIndex] = *edgeSet;
    (*circularBuffer).writeIndex = (circularBuffer->writeIndex + 1) % BUFFER_SIZE;

    int err3 = sem_post(circularBuffer->writeMutex);
    int err4 = sem_post(circularBuffer->usedSpace);
    // TODO error handling
}

edgeSet readFromBuffer(circularBuffer *circularBuffer){
    edgeSet result;
    
    int err1 = sem_wait(&circularBuffer->usedSpace);

    result = circularBuffer->buffer[circularBuffer->readIndex];
    circularBuffer->readIndex = (circularBuffer->readIndex + 1) % BUFFER_SIZE;

    int err2 = sem_post(&circularBuffer->freeSpace);
    // TODO error handling
    return result;
}

void cleanUp(circularBuffer *circularBuffer){
    int err1 = sem_destroy(&circularBuffer->freeSpace );
    int err2 = sem_destroy(&circularBuffer->usedSpace );
    int err3 = sem_destroy(&circularBuffer->writeMutex);
    int err4 = munmap(circularBuffer, sizeof(circularBuffer));
    // TODO error handling
}




#endif // CIRCULARBUFFER_H