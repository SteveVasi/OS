#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>
#include <signal.h>

#define bool int

void usage(void);

volatile sig_atomic_t quit = 0;

int main(int argc, char **argv)
{

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    int limit = INT_MAX;
    unsigned int delay = 0;
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

    // supervisor remembers the best solution

    // processes communicate with eachother with the circular buffer which uses shared semaphores and shared memory

    // supervisor sets up shared memory, semaphores and circular buffer
    //////////////////////

    volatile unsigned int solutions_count = 0;

    // initializations

    sleep(delay);

    // read solutions from circular buffer

    while (shouldRun(solutions_count, limit))
    {
        /* code */
    }

    _exit(1);
}




bool shouldRun(int solutions, int maxSolutions)
{
    return (solutions <= maxSolutions) && (quit != 0);
}

void signalHandler(int signal)
{
    quit = 1;
}

void usage(void)
{
    printf("SYNOPSIS:\n");
    printf("    supervisor [-n limit] [-w delay]\n");
}