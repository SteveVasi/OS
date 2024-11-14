typedef struct edge_t
{
    vertex v1;
    vertex v2;
} edge;

typedef struct vertex_t {
    unsigned int vertex_number;
    COLOR;
} vertex;

typedef enum {
    RED,
    GREEN,
    BLUE
} COLOR;