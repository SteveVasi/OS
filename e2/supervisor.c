#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "circularBuffer.h"


#define bool int

void usage(void);
void handleSignal(int signal);
bool shouldRun(int solutions, int maxSolutions, int bestSolutionSize, volatile sig_atomic_t *quitFlag);
bool isBetterThan(edgeSet *removedEdges1, edgeSet *removedEdges2);

volatile sig_atomic_t quitFlag = 0;

int main(int argc, char **argv)
{
    // https://youtu.be/AKJhThyTmQw?si=JGGSNW6eBqxMThib
    // goto (further down but not up!)
    int return_value = 0;


    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);

    int limit = -1;
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
    if(sharedMemoryFileDescriptor < 0){
        return_value = -1;
        return EXIT_FAILURE;
    }
    if(truncateSharedMemory(sharedMemoryFileDescriptor)){
        return_value = -1;
        goto close_shared_memory;
    };
    circularBuffer *sharedBuffer = NULL;
    if(memoryMapBuffer(sharedMemoryFileDescriptor, &sharedBuffer)){
        perror("mmap err");
        goto close_shared_memory;
    }
    if(initSharedBufferServer(sharedBuffer)){
        perror("initBufferServer err");
        return_value = -1;
        goto unmap_shared_memory;
    }

    sleep(delay);
    volatile unsigned int solutions_count = 0;

    removedEdgeSet bestSolution;
    initRemovedEdgeSet(&bestSolution);
    bestSolution.size = MAX_REMOVED_EDGES;

    while (shouldRun(solutions_count, limit, bestSolution.size, &quitFlag))
    {
        printf("In loop!");
        fflush(stdout);

        if(quitFlag) {
            break;
        }

        edgeSet readSet;
        
        if(readFromBuffer(sharedBuffer, &readSet)){
            goto cleanUpAllResources;
        }

        if(isBetterThan(&readSet, &bestSolution)){
            copyEdgeSet(&bestSolution, &readSet);
        }
    }
    printf("Out of the loop!");
    fflush(stdout);

    
    cleanUpAllResources:
    // close_semaphores:
        if(closeSemaphores(sharedBuffer)){
            return_value = -1;
        };
    // unlink_semaphores:
        if(unlinkSemaphores()){
            return_value = -1;
        }
    unmap_shared_memory:
        if(unmapSharedMemory(sharedBuffer)){
            return_value = -1;
        };
    close_shared_memory:
        if(close(sharedMemoryFileDescriptor)){
            return_value = -1;
        }; 
    // unlink_shared_memory:
        if(unlinkSharedMemory()){
            return_value = -1;
        };

    
    
    if(bestSolution.size == 0){
        printf("The graph is 3-colorable");
    } else {
        printf("The best solution removes these edges:");
        printEdgeSet(&bestSolution);
    }
    fflush(stdout);

    freeEdgeSet(&bestSolution);

    return return_value;
}

bool isBetterThan(edgeSet *removedEdges1, edgeSet *removedEdges2)
{
    return removedEdges1->size < removedEdges2->size;
}


bool shouldRun(int solutions, int maxSolutions, int bestSolutionSize, volatile sig_atomic_t *quitFlag)
{
    if (*quitFlag) {
        return 0;
    }
    if (maxSolutions < 0) {
        return bestSolutionSize != 0;
    } else {
        return (solutions <= maxSolutions) || bestSolutionSize != 0;
    }
}

void handleSignal(int signal)
{
    quitFlag = 1;
    char* msg = "Received signal";
    write(STDOUT_FILENO, msg, strlen(msg));
    // TODO send signal to generators
    return;
}

void usage(void)
{
    printf("SYNOPSIS:\n");
    printf("    supervisor [-n limit] [-w delay]\n");
}
