#include "circularBuffer.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>

int openSharedMemory()
{
    int shm_fd = shm_open(CIRCULAR_ARRAY_BUFFER, O_RDWR | O_CREAT, 0600);
    if (shm_fd == -1)
    {
        perror("Failed to create shared memory");
    }
    return shm_fd;
}

int truncateSharedMemory(int sharedMemoryFileDescriptor)
{
    int truncation = ftruncate(sharedMemoryFileDescriptor, sizeof(circularBuffer));
    if (truncation < 0)
    {
        perror("Failed to truncate shared memory");
        // return -1 and then caller has to check return value 
        return truncation;
    }
    return EXIT_SUCCESS;
}

errorCode memoryMapBuffer(int sharedMemoryFileDescriptor, circularBuffer *cb)
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
        return -1;
    }
    cb = sharedBuffer;
    return 0;
}

int checkForSemError(circularBuffer *circularBuffer)
{
    if (circularBuffer->freeSpace == SEM_FAILED ||
        circularBuffer->usedSpace == SEM_FAILED ||
        circularBuffer->writeMutex == SEM_FAILED)
    {
        return -1;
    }
    return 0;
}

int initSharedBufferServer(circularBuffer *circularBuffer)
{
    circularBuffer->writeIndex = 0;
    circularBuffer->readIndex = 0;
    circularBuffer->freeSpace = sem_open(SEM_FREE_SPACE, O_CREAT, 777, BUFFER_SIZE);
    circularBuffer->usedSpace = sem_open(SEM_USED_SPACE, O_CREAT, 777, 0);
    circularBuffer->writeMutex = sem_open(SEM_WRITE_MUTEX, O_CREAT, 777, 1);
    return checkForSemError(circularBuffer);
}

int initSharedBufferClient(circularBuffer *circularBuffer)
{
    circularBuffer->writeIndex = 0;
    circularBuffer->readIndex = 0;
    circularBuffer->freeSpace = sem_open(SEM_FREE_SPACE, BUFFER_SIZE);
    circularBuffer->usedSpace = sem_open(SEM_USED_SPACE, 0);
    circularBuffer->writeMutex = sem_open(SEM_WRITE_MUTEX, 1);
    return checkForSemError(circularBuffer);
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

void writeToBuffer(edgeSet *edgeSet, circularBuffer *circularBuffer)
{
    sem_wait(circularBuffer->freeSpace);
    sem_wait(circularBuffer->writeMutex);

    circularBuffer->buffer[circularBuffer->writeIndex] = *edgeSet;
    (*circularBuffer).writeIndex = (circularBuffer->writeIndex + 1) % BUFFER_SIZE;

    sem_post(circularBuffer->writeMutex);
    sem_post(circularBuffer->usedSpace);
    // TODO error handling
}


void cleanUpSemaphores(circularBuffer *circularBuffer)
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
