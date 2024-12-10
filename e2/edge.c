#include "edge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printEdge(edge e)
{
    printf("%d-%d ", e.v1, e.v2);
}

void printEdgeSet(edgeSet *es)
{
    if(es->size == 0){
        printf("Edge set is empty");
    }
    for(int i = 0; i < es->size; i++){
        printEdge(es->array[i]);
    }
    printf("\n");
    fflush(stdout);
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
        perror("Failed to allocate memory for edge set");
        return -1;
    }
    es -> size = 0; 
    es -> max = maximum;
    return 0;
}

errorCode initRemovedEdgeSet(edgeSet *es)
{
    es->array = malloc(MAX_REMOVED_EDGES * sizeof(edge));
    if(es->array == NULL) {
        perror("Failed to allocate memory for removed edge set");
        return -1;
    }
    es -> size = MAX_REMOVED_EDGES; 
    es -> max = MAX_REMOVED_EDGES;
    return 0;
}

void freeEdgeSet(edgeSet *es) 
{
    free(es -> array);
    es -> array = NULL;
    es -> size = 0;
    es -> max = 0;
}

void copyEdgeSet(edgeSet *destination, edgeSet *source){
    memcpy(destination->array, source->array, source->size);
    destination->size = source->size;
    destination->max = source->max;
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

errorCode initGraph(graph **g, int maxEdges, int maxVertices)
{
    *g = malloc(sizeof(graph));
    if(*g == NULL){
        perror("Failed to allocate memory for graph");
        return -1;
    }
    errorCode es = initEdgeSet(&((*g)->edgeSet), maxEdges);
    if (es != 0) {
        free(*g);
        return es; // Return error from initEdgeSet
    }

    errorCode vs = initVertexSet(&((*g)->vertexSet), maxVertices);
    if (vs != 0) {
        freeEdgeSet(&((*g)->edgeSet)); // Cleanup previously initialized edgeSet
        free(*g);
        return vs; // Return error from initVertexSet
    }
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