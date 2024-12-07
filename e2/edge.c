#include "edge.h"
#include <stdio.h>
#include <stdlib.h>

void printEdge(edge e)
{
    printf("%d-%d ", e.v1, e.v2);
}

void printEdgeSet(edgeSet *es)
{
    for(int i = 0; i < es->size; i++){
        printEdge(es->array[i]);
    }
    printf("\n");
}

void constructGraph(edgeSet *set, graph *g)
{
    g->edgeSet = *set;
    getVerticesFromEdges(set, &(g->vertexSet));
}

void getVerticesFromEdges(edgeSet *eSet, vertexSet *vSet)
{
    // ideally you would use a hash map for this to have a better time complexity but we dont care about that here
    for(int i = 0; i < eSet -> size; i++){
        edge e = eSet->array[i];
        if(!containsVertex(vSet, e.v1)){
            addVertexToSet(vSet, e.v1);
        }
        if(!containsVertex(vSet, e.v2)){
            addVertexToSet(vSet, e.v2);
        }
    }
 
}

errorCode addVertexToSet(vertexSet *set, vertex v) // TODO CHANGE PARAMETER ORDER AND ALSO RETURN VALUE TO ERRORCODE/CHECK FOR MAX
{
    if(set -> size == set -> max) {
        return -1;
    }
    set -> array[set -> size] = v;
    set -> size++;
    return 0;
}

errorCode addEdgeToSet(edge e, edgeSet *set)
{
    if(set -> size == set -> max) {
        return -1;
    }
    set -> array[set -> size] = e;
    set -> size++;
    return 0;
}


bool containsVertex(vertexSet *set, vertex v)
{
    for(int j = 0; j < set->size; j++){
        if(set->array[j] == v){
            return 1;
        }
    }
    return 0;
}

errorCode initEdgeSet(edgeSet *es, int maximum) 
{
    es->array = malloc(maximum * sizeof(edge));
    if(es->array == NULL) {
        perror("Failed to allocate mempory for edge set");
        return -1;
    }
    es -> size = 0; 
    es -> max = maximum;
    return 0;
}

void freeEdgeSet(edgeSet *es) 
{
    free(es -> array);
    es -> array = NULL;
    es -> size = 0;
    es -> max = 0;
}

errorCode initVertexSet(vertexSet *vs, int maximum) 
{
    vs->array = malloc(maximum * sizeof(vertex));
    if(vs->array == NULL) {
        perror("Failed to allocate mempory for vertex set");
        return -1;
    }
    vs -> size = 0; 
    vs -> max = maximum;
    return 0;
}

void freeVertexSet(vertexSet *vs) 
{
    free(vs -> array);
    vs -> array = NULL;
    vs -> size = 0;
    vs -> max = 0;
}

errorCode initColoredVertexSet(coloredVertexSet *vs, int maximum) 
{
    vs->array = malloc(maximum * sizeof(coloredVertex));
    if(vs->array == NULL) {
        perror("Failed to allocate mempory for vertex set");
        return -1;
    }
    vs -> size = 0; 
    vs -> max = maximum;
    return 0;
}

void freeColoredVertexSet(coloredVertexSet *vs) 
{
    free(vs -> array);
    vs -> array = NULL;
    vs -> size = 0;
    vs -> max = 0;
}

errorCode initGraph(graph *g, int maxEdges, int maxVertices)
{
    errorCode es = initEdgeSet(&(g->edgeSet), maxEdges);
    errorCode vs = initVertexSet(&(g->vertexSet), maxVertices);
    return es + vs;
}

void freeGraph(graph *g) 
{
    free(g);
    /* TODO i think the commented code is incorrect bc es and vs are not pointers? not sure 
    freeEdgeSet(&(g->edgeSet));
    freeVertexSet(&(g->vertexSet));
    */
}