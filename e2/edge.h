typedef struct edge_t
{
    vertex v1;
    vertex v2;
} edge;

typedef struct vertex_t {
    unsigned int vertex_number;
} vertex;

typedef struct colored_vertex_t
{
    vertex v;
    COLOR;
} coloredVertex;


typedef enum {
    RED,
    GREEN,
    BLUE
} COLOR;