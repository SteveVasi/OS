#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>
#include <signal.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "circularBuffer.h"

#define bool int

void usage(void);
void initCircularBuffer(circularBuffer *circularBuffer);

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

    // every time a better solution is found, the supervisor writes it to stderr
    // if generator writes a solution with 0 edges to circular buffer, then print("The graph is 3-colorable!") to stdout

    // otherwise the supervisor keeps reading results from the ciruclar buffer until:
    // SIGINT, SIGTERM or number of read solutions from the circular buffer exceeds limit
    // then print("The graph might not be 3-colorable, best solution removes N edges.") to stdout

    // supervisor remembers the best solution

    // processes communicate with eachother with the circular buffer which uses shared semaphores and shared memory

    // supervisor sets up shared memory, semaphores and circular buffer


    volatile unsigned int solutions_count = 0;

    // initializations

  

    int shmfd = shm_open(CIRCULAR_ARRAY_BUFFER, O_RDWR | O_CREAT, 0600);
    if(shmfd == -1){
        shmError();
    }

    int truncation = ftruncate(shmfd, sizeof(circularBuffer));
    if (truncation < 0)
    {
        shmError();
    }
    
    
    //myshm = mmap(NULL, sizeof(circularBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    
    circularBuffer circularBuffer;
    initCircularbuffer(&circularBuffer);

    sleep(delay);

    // read solutions from circular buffer

    while (shouldRun(solutions_count, limit))
    {
        /* code */
    }

    int err1 = close(shmfd);
    int err2 = shm_unlink(CIRCULAR_ARRAY_BUFFER);
    _exit(1);
}


void shmError(void){
    // TODO perror()
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

