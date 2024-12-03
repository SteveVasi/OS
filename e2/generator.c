#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "edge.h"
#include "circularBuffer.h"

#define bool int
#define Coco war hier


void usage(void);
int parseEdge(char *argument, edge *e);
void edgeParsingError(void);
errorCode parseAllEdges(int argc, char** argv, graph *g);
static COLOR randomColor(void);
void colorRandomly(vertexSet *vs, coloredVertexSet *c);

int main(int argc, char **argv)
{
    

    // each positional argument is one edge
    // at least one edge must be given

    // generator repeatedly generates a random solution and writes it to the circular buffer
    // it writes the edge sets to the circular buffer one at a time.
    // therefore a set of edges is a single element of the circular buffer

    graph *g = NULL;

    parseAllEdges(argc, argv, g);

    freeGraph(g);
}

coloredVertexSet generateValid3Coloring(graph *g)
{
    // monte carlo algorithm
    coloredVertexSet *coloredSet = NULL;
    initColoredVertexSet(coloredSet);
    colorRandomly(&(g->vertexSet), &coloredSet);

    return coloredSet;
}

selectInvalidEdges(coloredVertexSet *set) 
{

}

/*
edgeSet selectOnlyValidEdges(edgeSet *edges)
{
    
    // edgeSet valids = malloc(sizeof(*edgeSet));
    // int counter = 0;
    // for(int i = 0; i < edgeSet->edgeCount; i++){
    //     if(areValid(edgeSet->edgeArray[i])){
    //         valids[counter] = edgeSet->edgeArray[i];
    //         counter++;
    //     }
    // }
    // TODO dont forget to free valid;
    edgeSet s;
    return s;
}

*/

void colorRandomly(vertexSet *vs, coloredVertexSet *c) 
{
    for(int i = 0; i < vs -> size; i++){
        coloredVertex cv = {vs->array[i], randomColor()};
        c->array[i] = cv;
    }
}

errorCode parseAllEdges(int argc, char** argv, graph *g)
{
    int edgeCount = argc - 1;
    initGraph(g, edgeCount, edgeCount * 2);

    edge *edges = (g -> edgeSet.array);

    int j = 0;
    for (size_t i = 1; i < argc; i++, j++)
    {
        int err = parseEdge(argv[i], &edges[j]);
        if(err < 0){
            edgeParsingError();
        }
        printEdge(edges[j]);
    }
}


int parseEdge(char *argument, edge *e)
{
    // the format of the argument must be: two integers separated by a dash. ints are allowed to have a sign
    int n1;
    int n2;
    if (sscanf(argument, "%d-%d", &n1, &n2) == 2)
    {
        vertex v1 = {n1};
        vertex v2 = {n2};
        edge parsed = {v1, v2};
        *e = parsed;
        return 0;
    }
    return -1;
}




static bool areValid(edge *edge)
{
    // todo
    return 1;
}

static COLOR randomColor(void)
{
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

void edgeParsingError(void)
{
    perror("Edge parsing error");
    usage();
    exit(EXIT_FAILURE);
}

void usage(void)
{
    printf("SYNOPSIS:\n");
    printf("    generator EDGE1...\n");
    printf("EXAMPLE\n");
    printf("    generator 0-1 0-2 0-3 1-2 1-3 2-3\n");
}