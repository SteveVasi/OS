#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "circularBuffer.h"
#include <sys/stat.h>
#include <fcntl.h>

#define bool int

void usage(void);
circularBuffer *memoryMapBuffer(int sharedMemoryFileDescriptor, circularBuffer *circularBuffer);
int openSharedMemory();
void truncateSharedMemory(int sharedMemoryFileDescriptor, circularBuffer *circularBuffer);
bool shouldRun(int solutions, int maxSolutions);
void handleSignal(int signal);
void exitOnSemError(void);
void checkForSemError(circularBuffer *circularBuffer);
void initSharedBuffer(circularBuffer *circularBuffer);
void writeToBuffer(edgeSet *edgeSet, circularBuffer *circularBuffer);
edgeSet readFromBuffer(circularBuffer *circularBuffer);
void cleanUp(circularBuffer *circularBuffer);

volatile sig_atomic_t quit = 0;

int main(int argc, char **argv)
{

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

    circularBuffer buffer;
    initSharedBuffer(&buffer);
    
    int sharedMemoryFileDescriptor = openSharedMemory();
    truncateSharedMemory(sharedMemoryFileDescriptor, &buffer);
    circularBuffer *sharedBuffer = memoryMapBuffer(sharedMemoryFileDescriptor, &buffer);

    sleep(delay);
    volatile unsigned int solutions_count = 0;

    // edgeSet bestSolution;
    while (shouldRun(solutions_count, limit))
    {
    }

    // cleanUpAllResources:
    cleanUp(&buffer);
    close(sharedMemoryFileDescriptor); // TODO error handling
    shm_unlink(CIRCULAR_ARRAY_BUFFER); // TODO error handling
        
    _exit(1);
}

circularBuffer *memoryMapBuffer(int sharedMemoryFileDescriptor, circularBuffer *cb)
{
    circularBuffer *sharedBuffer = mmap(NULL,
                                          sizeof(*cb),
                                          PROT_READ | PROT_WRITE,
                                          MAP_SHARED,
                                          sharedMemoryFileDescriptor,
                                          0);
    if (sharedBuffer == MAP_FAILED)
    {
        perror("Memory mapping failed");
        _exit(EXIT_FAILURE);
    }
    return sharedBuffer;
}

int openSharedMemory()
{
    int shm_fd = shm_open(CIRCULAR_ARRAY_BUFFER, O_RDWR | O_CREAT, 0600);
    if (shm_fd == -1)
    {
        perror("Failed to create shared memory");
        _exit(EXIT_FAILURE);
    }
    return shm_fd;
}

void truncateSharedMemory(int sharedMemoryFileDescriptor, circularBuffer *circularBuffer)
{
    int truncation = ftruncate(sharedMemoryFileDescriptor, sizeof(*circularBuffer));
    if (truncation < 0)
    {
        perror("Failed to truncate shared memory");
        close(sharedMemoryFileDescriptor);
        _exit(EXIT_FAILURE);
    }
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

void exitOnSemError(void)
{
    perror("An error with a semaphore has occurred. Exiting program now");
    _exit(EXIT_FAILURE);
}

void checkForSemError(circularBuffer *circularBuffer)
{
    if (circularBuffer->freeSpace == SEM_FAILED ||
        circularBuffer->usedSpace == SEM_FAILED ||
        circularBuffer->writeMutex == SEM_FAILED)
    {
        exitOnSemError();
    }
}

void initSharedBuffer(circularBuffer *circularBuffer)
{
    circularBuffer->writeIndex = 0;
    circularBuffer->readIndex = 0;
    circularBuffer->freeSpace = sem_open(SEM_FREE_SPACE, O_CREAT, 777, BUFFER_SIZE);
    circularBuffer->usedSpace = sem_open(SEM_USED_SPACE, O_CREAT, 777, 0);
    circularBuffer->writeMutex = sem_open(SEM_WRITE_MUTEX, O_CREAT, 777, 1);
    checkForSemError(circularBuffer);
}

edgeSet readFromBuffer(circularBuffer *circularBuffer)
{
    edgeSet result;

    sem_wait(circularBuffer->usedSpace);  // TODO error handling

    result = circularBuffer->buffer[circularBuffer->readIndex];
    circularBuffer->readIndex = (circularBuffer->readIndex + 1) % BUFFER_SIZE;

    sem_post(circularBuffer->freeSpace);  // TODO error handling
    // TODO error handling
    return result;
}

void cleanUp(circularBuffer *circularBuffer)
{
    munmap(circularBuffer, sizeof(*circularBuffer));
    sem_close(circularBuffer->freeSpace);
    sem_close(circularBuffer->usedSpace);
    sem_close(circularBuffer->writeMutex);
    sem_unlink(SEM_FREE_SPACE);
    sem_unlink(SEM_USED_SPACE);
    sem_unlink(SEM_WRITE_MUTEX);
    // TODO error handling for munmap, sem_close  and sem_unlink
}
