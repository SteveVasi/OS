#ifndef EDGE_H
#define EDGE_H

#define bool int
#define errorCode int
#define MAX_REMOVED_EDGES 8

// TYPE DEFS

typedef int vertex;

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
    edge *array;
    int max;
    int size;
} edgeSet;

typedef edgeSet removedEdgeSet;

typedef struct vertex_set_t {
    vertex *array;
    int max;
    int size;
} vertexSet;

typedef struct colored_vertex_set_t {
    coloredVertex *array;
    int max;
    int size;
} coloredVertexSet;

typedef struct graph_t {
    edgeSet edgeSet;
    // ideally vertexSet should be implemented as a map for better performance
    // but this exercise is already long enough
    
    vertexSet vertexSet; 
 
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
errorCode initEdgeSet(edgeSet *es, int maximum);
errorCode initVertexSet(vertexSet *vs, int maximum);
errorCode initColoredVertexSet(coloredVertexSet *vs, int maximum);
errorCode initGraph(graph **g, int maxEdges, int maxVertices);
void freeEdgeSet(edgeSet *es);
void freeVertexSet(vertexSet *vs);
void freeColoredVertexSet(coloredVertexSet *vs);
void freeGraph(graph *g) ;

void constructGraph(edgeSet *set, graph *g);

void getVerticesFromEdges(edgeSet *set, vertexSet *vertices);
bool containsVertex(vertexSet *set, vertex v);
errorCode addVertexToSet(vertexSet *set, vertex v);
errorCode addEdgeToSet(edge e, edgeSet *set);
void copyEdgeSet(edgeSet *destination, edgeSet *source);
#endif