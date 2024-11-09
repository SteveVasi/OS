#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "edge.h"

void usage(void);
edge getEdge(char *argument);
void edgeParsingError(void);

int main(int argc, char **argv)
{
    usage();

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
        edges[j] = getEdge(argv[i]);
    }

    free(edges);
}

edge getEdge(char *argument)
{
    // the format of the argument must be: two integers separated by a dash
    unsigned int v1;
    unsigned int v2;
    if (sscanf(argument, "%u-%u", &v1, &v2) == 2)
    {
        edge e = {v1, v2};
        return e;
    }
    edgeParsingError();
}

void edgeParsingError(void){
    // todo
}

void usage(void)
{
    printf("SYNOPSIS:\n");
    printf("    generator EDGE1...\n");
    printf("EXAMPLE\n");
    printf("    generator 0-1 0-2 0-3 1-2 1-3 2-3\n");
}