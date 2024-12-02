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
edgeSet* parseAllEdges(int edgeCount, char** argv);

int main(int argc, char **argv)
{
    

    // each positional argument is one edge
    // at least one edge must be given

    // generator repeatedly generates a random solution and writes it to the circular buffer
    // it writes the edge sets to the circular buffer one at a time.
    // therefore a set of edges is a single element of the circular buffer

    edgeSet *edges = parseAllEdges(argc, argv);


    graph *g;
    constructGraph(edges, g);


    free(edges);
}

void constructGraph(edgeSet *set, graph *g){
    g->edgeSet = set;
}

void getVertexSet(edgeSet *set, vertexSet *vertices){
    // ideally you would use a hash map for this to have a better time complexity but we dont care about that here
    memset(vertices->vertexArray, -1, MAX_VERTICES);
    for(int i = 0; i < set->edgeCount; i++){
        edge *currentEdge = set->edgeArray[i];
        int vertex1 = currentEdge-> v1;
        int vertex2 = currentEdge-> v2;
        
    }
}

bool containsVertex(vertexSet *set, vertex v){
    for(int j = 0; j < vertices->vertexCount; j++){
        if(vertexArray[j] == vertex){
            return 1;
        }
    }
    return 0;
}


edgeSet* parseAllEdges(int argc, char** argv)
{
    edgeSet *set = malloc(sizeof(edgeSet)); // TODO check if i can maybe remove malloc?
    edge *edges = set->edgeArray;

    int j = 0;
    for (size_t i = 1; i < argc; i++, j++)
    {
        int err = parseEdge(argv[i], &edges[j]);
        if(err < 0){
            edgeParsingError();
        }
        printEdge(edges[j]);
    }
    return set;
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

edgeSet selectOnlyValidEdges(edgeSet *edges){
    /*
    edgeSet valids = malloc(sizeof(*edgeSet));
    int counter = 0;
    for(int i = 0; i < edgeSet->edgeCount; i++){
        if(areValid(edgeSet->edgeArray[i])){
            valids[counter] = edgeSet->edgeArray[i];
            counter++;
        }
    }*/
    // TODO dont forget to free valid;
    edgeSet s;
    return s;
}

static void colorGraph(graph graph){
    // todo
}

static bool areValid(edge *edge){
    // todo
    return 1;
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