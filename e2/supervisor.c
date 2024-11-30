#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>
#include <limits.h>
#include "circularBuffer.h"


#define bool int

void usage(void);
void handleSignal(int signal);
bool shouldRun(int solutions, int maxSolutions);

volatile sig_atomic_t quit = 0;

int main(int argc, char **argv)
{
    // https://youtu.be/AKJhThyTmQw?si=JGGSNW6eBqxMThib
    // goto (further down but not up!)
    int return_value = 0;

    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);

    int limit = INT_MAX;
    unsigned int delay = 0;
    int opt;
    int allowedArgCount = 0;
    bool flag_n = 0;
    bool flag_w = 0;
    while ((opt = getopt(argc, argv, "n:w:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            limit = atoi(optarg);
            flag_n = 1;
            break;
        case 'w':
            delay = atoi(optarg);
            flag_w = 1;
            break;
        default:
            usage();
            exit(EXIT_FAILURE);
            break;
        }
    }

    if(flag_n){
        allowedArgCount += 2;
    }if(flag_w){
        allowedArgCount += 2;
    }

    if ((argc - 1) != allowedArgCount)
    {
        usage();
        exit(EXIT_FAILURE);
    }

    // every time a better solution is found, the supervisor writes it to stderr
    // if generator writes a solution with 0 edges to circular buffer, then print("The graph is 3-colorable!") to stdout

    // otherwise the supervisor keeps reading results from the ciruclar buffer until:
    // SIGINT, SIGTERM or number of read solutions from the circular buffer exceeds limit
    // then print("The graph might not be 3-colorable, best solution removes N edges.") to stdout

    // supervisor remembers the best solution

    // processes communicate with eachother with the circular buffer which uses shared semaphores and shared memory

    // supervisor sets up shared memory, semaphores and circular buffer

    
    
    int sharedMemoryFileDescriptor = openSharedMemory();
    truncateSharedMemory(sharedMemoryFileDescriptor);
    circularBuffer *sharedBuffer;
    sharedBuffer = memoryMapBuffer(sharedMemoryFileDescriptor, sharedBuffer); // error handling check / check return value for MAP_FAILED

    sleep(delay);
    volatile unsigned int solutions_count = 0;

    edgeSet bestSolution;
    while (shouldRun(solutions_count, limit))
    {
        edgeSet newValue = readFromBuffer(sharedBuffer);
    }

    /*
    cleanUpAllResources:
        shm_unlink(CIRCULAR_ARRAY_BUFFER); // TODO error handling
    clean_up_shared_memory_fd:
        close(sharedMemoryFileDescriptor); // TODO error handling
    */

    return return_value;
}

bool isBetterThan(edgeSet *es1, edgeSet *es2){
    return 0;
}


bool shouldRun(int solutions, int maxSolutions)
{
    return (solutions <= maxSolutions) && (quit == 0);
}

void handleSignal(int signal)
{
    // goto cleanUpAllResources; doesnt work :(
    quit = 1;
}

void usage(void)
{
    printf("SYNOPSIS:\n");
    printf("    supervisor [-n limit] [-w delay]\n");
}
