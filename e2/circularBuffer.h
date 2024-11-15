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
    edge edgeArray[MAX_EDGES];
    int edgeCount;
} edgeSet;

typedef struct {
    edge edgeArray[MAX_EDGES];
    int edgeCount;
    vertex vertexArray[MAX_EDGES * 2];
    int vertexCount;
    /*
        The formula to determine the number of edges e in a complete graph is given by e = n * ( n − 1 )/ 2 
        where e is the number of edges and n is the number of vertices
        if we solve for n we get:
        n = +- 1/2 * (1 + sqrt(1 + 8e))
        thus the maximum number of vertices given the edges is:
        v_max = floor(1/2*(1+sqrt(1+8e)))
    */

} graph;

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