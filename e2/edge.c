#include "edge.h"
#include <stdio.h>

void printEdge(edge e){
    printf("%d-%d ", e.v1.vertex_number, e.v2.vertex_number);
}
