#include <stdio.h>

void usage(void);

int main(int argc, char** argv){
    usage();

    // each positional argument is one edge
    // at least one edge must be given

    // generator repeatedly generates a random solution and writes it to the circular buffer
    // it writes the edge sets to the circular buffer one at a time.
    // therefore a set of edges is a single element of the circular buffer
}

void usage(void){
    printf("SYNOPSIS:\n");
    printf("    generator EDGE1...\n");
    printf("EXAMPLE");
    printf("    generator 0-1 0-2 0-3 1-2 1-3 2-3");
}