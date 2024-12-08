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
    int writeIndex;
    int readIndex;
    sem_t *freeSpace;
    sem_t *usedSpace;
    sem_t *writeMutex;
    removedEdgeSet array[BUFFER_SIZE];
} circularBuffer;


errorCode memoryMapBuffer(int sharedMemoryFileDescriptor, circularBuffer *circularBuffer);
errorCode openSharedMemory();
int truncateSharedMemory(int sharedMemoryFileDescriptor);

errorCode initSharedBufferServer(circularBuffer *circularBuffer);
errorCode initSharedBufferClient(circularBuffer *circularBuffer);
errorCode initRemovedEdgeSet(edgeSet *es);

errorCode writeToBuffer(edgeSet *edgeSet, circularBuffer *circularBuffer);
errorCode readFromBuffer(circularBuffer *circularBuffer, edgeSet *es);

errorCode unmapSharedMemory(circularBuffer *b);
errorCode unlinkSharedMemory();
errorCode closeSemaphores(circularBuffer *circularBuffer);
errorCode unlinkSemaphores();
#endif // CIRCULARBUFFER_H