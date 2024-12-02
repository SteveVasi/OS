#include "edge.h"
#include <stdio.h>

#define bool int

void printEdge(edge e){
    printf("%d-%d ", e.v1, e.v2);
}

void printEdgeSet(edgeSet *es){
    for(int i = 0; i < es->edgeCount; i++){
        printEdge(es->edgeArray[i]);
    }
    printf("\n");
}

void constructGraph(edgeSet *set, graph *g){
    g->edgeSet = *set;
    getVerticesFromEdges(set, &(g->vertexSet));
}

void getVerticesFromEdges(edgeSet *eSet, vertexSet *vSet){
    // ideally you would use a hash map for this to have a better time complexity but we dont care about that here
    for(int i = 0; i < eSet -> edgeCount; i++){
        edge e = eSet->edgeArray[i];
        if(!containsVertex(vSet, e.v1)){
            addVertexToSet(vSet, e.v1);
        }
        if(!containsVertex(vSet, e.v2)){
            addVertexToSet(vSet, e.v2);
        }
    }
 
}

void addVertexToSet(vertexSet *set, vertex v){
    set->vertexArray[set->vertexCount - 1] = v;
}

bool containsVertex(vertexSet *set, vertex v){
    for(int j = 0; j < set->vertexCount; j++){
        if(set->vertexArray[j] == v){
            return 1;
        }
    }
    return 0;
}
