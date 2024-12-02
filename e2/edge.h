#ifndef EDGE_H
#define EDGE_H

#define MAX_EDGES (8)
#define BUFFER_SIZE (10)
#define MAX_VERTICES (MAX_EDGES * 2)

// TYPE DEFS

#define vertex unsigned int

typedef enum {
    RED,
    GREEN,
    BLUE
} COLOR;

typedef struct edge_t {
    vertex v1;
    vertex v2;
} edge;


typedef struct colored_vertex_t {
    vertex v;
    COLOR color;
} coloredVertex;

typedef struct edge_set_t {
    edge edgeArray[MAX_EDGES];
    int edgeCount;
} edgeSet;

typedef struct vertex_set_t {
    vertex vertexArray[MAX_VERTICES];
    int vertexCount;
} vertexSet;

typedef struct colored_vertex_set_t {
    coloredVertex coloredVertexArray[MAX_VERTICES];
    int coloredVertexCount;
} coloredVertexSet;

typedef struct graph_t {
    edgeSet edgeSet;
    coloredVertexSet coloredVertexSet;
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

void printEdge(edge e);
void printEdgeSet(edgeSet *es);
void insertEdgeIntoGraph(edge *e, graph *g);
void insertEdgeSetIntoGraph(edge *e, graph *g);

#endif