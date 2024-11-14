#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "edge.h"
#include "circularBuffer.h"

void usage(void);
edge parseEdge(char *argument);
void edgeParsingError(void);

int main(int argc, char **argv)
{
    

    // each positional argument is one edge
    // at least one edge must be given

    // generator repeatedly generates a random solution and writes it to the circular buffer
    // it writes the edge sets to the circular buffer one at a time.
    // therefore a set of edges is a single element of the circular buffer

    int edgeCount = argc - 1;
    edge *edges = malloc(edgeCount * sizeof(edge));

    int j = 0;
    for (size_t i = 1; i < argc; i++, j++)
    {
        edges[j] = parseEdge(argv[i]);
    }

    free(edges);
}

edge parseEdge(char *argument)
{
    // the format of the argument must be: two integers separated by a dash
    int n1;
    int n2;
    if (sscanf(argument, "%d-%d", &n1, &n2) == 2)
    {
        vertex v1 = {n1, randomColor()};
        vertex v2 = {n2, randomColor()};
        edge e = {v1, v2};
        return e;
    }
    edgeParsingError();
}

edgeSet selectOnlyValidEdges(){
    
}

static COLOR randomColor(void){
    int randy = rand();
    switch (randy % 3)
    {
    case 0:
        return RED;
        break;
    case 1:
        return BLUE;
        break;
    case 2:
        return GREEN;
        break;
    default:
        perror("Weird error. This line of code should never be reached");
        exit(EXIT_FAILURE);
        break;
    }
}

void edgeParsingError(void){
    usage();
    exit(EXIT_FAILURE);
}

static void writeToBuffer(edgeSet *edgeSet, circularBuffer *circularBuffer)
{
    int err1 = sem_wait(circularBuffer->freeSpace);
    int err2 = sem_wait(circularBuffer->writeMutex);

    circularBuffer->buffer[circularBuffer->writeIndex] = *edgeSet;
    (*circularBuffer).writeIndex = (circularBuffer->writeIndex + 1) % BUFFER_SIZE;

    int err3 = sem_post(circularBuffer->writeMutex);
    int err4 = sem_post(circularBuffer->usedSpace);
    // TODO error handling
}

void usage(void)
{
    printf("SYNOPSIS:\n");
    printf("    generator EDGE1...\n");
    printf("EXAMPLE\n");
    printf("    generator 0-1 0-2 0-3 1-2 1-3 2-3\n");
}