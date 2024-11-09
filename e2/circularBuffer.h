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


#endif // CIRCULARBUFFER_H