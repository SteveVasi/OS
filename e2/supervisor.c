#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>

void usage(void);

int main(int argc, char** argv){
    int limit = INT_MAX;
    int delay = 0;
    int opt;
    while ((opt = getopt(argc, argv, "n:w:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            limit = atoi(optarg);
            break;
        case 'w':
            delay = atoi(optarg);
            break;
        default:
            usage();
            exit(EXIT_FAILURE);
            break;
        }
    }
    
    // limit is for the number of generated solutions
    // delay is for reading the first solution from the buffer after initialization
    
    // every time a better solution is found, the supervisor writes it to stderr
    // if generator writes a solution with 0 edges to circular buffer, then print("The graph is 3-colorable!") to stdout
    
    // otherwise the supervisor keeps reading results from the ciruclar buffer until: 
    // SIGINT, SIGTERM or number of read solutions from the circular buffer exceeds limit
    // then print("The graph might not be 3-colorable, best solution removes N edges.") to stdout

    //supervisor remembers the best solution

    // processes communicate with eachother with the circular buffer which uses shared semaphores and shared memory

    // supervisor sets up shared memory, semaphores and circular buffer
    

    usage();
}



void usage(void){
    printf("SYNOPSIS:\n");
    printf("    supervisor [-n limit] [-w delay]\n");
}