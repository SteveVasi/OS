#include "edge.h"
#include <stdio.h>

void printEdge(edge e){
    printf("%d-%d ", e.v1, e.v2);
}

void printEdgeSet(edgeSet *es){
    for(int i = 0; i < es->edgeCount; i++){
        printEdge(es->edgeArray[i]);
    }
    printf("");
}

void insertEdgeIntoGraph(edge *e, graph *g) {

}

void insertEdgeSetIntoGraph(edge *e, graph *g) {

}