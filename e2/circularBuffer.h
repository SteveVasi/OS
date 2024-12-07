#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <semaphore.h>
#include <stdlib.h> 
#include "edge.h"

#define CIRCULAR_ARRAY_BUFFER "/circulararraybuffer"
#define SEM_FREE_SPACE "/semfreespace"
#define SEM_USED_SPACE "/semusedspace"
#define SEM_WRITE_MUTEX "/semwritemutex"

#define BUFFER_SIZE 10


/*
    only (n-1) elements can be stored at a given time
*/
typedef struct circular_buffer_t
{
    edgeSet buffer[BUFFER_SIZE];
    int writeIndex;
    int readIndex;
    sem_t *freeSpace;
    sem_t *usedSpace;
    sem_t *writeMutex;
} circularBuffer;

errorCode memoryMapBuffer(int sharedMemoryFileDescriptor, circularBuffer *circularBuffer);
int openSharedMemory();
int truncateSharedMemory(int sharedMemoryFileDescriptor);
void exitOnSemError(void);
int checkForSemError(circularBuffer *circularBuffer);
int initSharedBufferServer(circularBuffer *circularBuffer);
int initSharedBufferClient(circularBuffer *circularBuffer);
void writeToBuffer(edgeSet *edgeSet, circularBuffer *circularBuffer);
edgeSet readFromBuffer(circularBuffer *circularBuffer);

#endif // CIRCULARBUFFER_H