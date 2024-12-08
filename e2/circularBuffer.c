#include "circularBuffer.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>

errorCode openSharedMemory()
{
    int shm_fd = shm_open(CIRCULAR_ARRAY_BUFFER, O_RDWR | O_CREAT, 0600);
    if (shm_fd == -1)
    {
        perror("Error opening shared memory file descriptor");
    }
    return shm_fd;
}

errorCode truncateSharedMemory(int sharedMemoryFileDescriptor)
{
    errorCode truncation = ftruncate(sharedMemoryFileDescriptor, sizeof(circularBuffer));
    if (truncation < 0)
    {
        perror("Failed to truncate shared memory");
        // return -1 and then caller has to check return value 
        return truncation;
    }
    return 0;
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

errorCode checkForSemError(circularBuffer *circularBuffer)
{
    if (circularBuffer->freeSpace == SEM_FAILED ||
        circularBuffer->usedSpace == SEM_FAILED ||
        circularBuffer->writeMutex == SEM_FAILED)
    {
        return -1;
    }
    return 0;
}

errorCode initSharedBufferServer(circularBuffer *circularBuffer)
{
    circularBuffer->writeIndex = 0;
    circularBuffer->readIndex = 0;
    circularBuffer->freeSpace = sem_open(SEM_FREE_SPACE, O_CREAT, 777, BUFFER_SIZE);
    circularBuffer->usedSpace = sem_open(SEM_USED_SPACE, O_CREAT, 777, 0);
    circularBuffer->writeMutex = sem_open(SEM_WRITE_MUTEX, O_CREAT, 777, 1);
    return checkForSemError(circularBuffer);
}

errorCode initSharedBufferClient(circularBuffer *circularBuffer)
{
    circularBuffer->writeIndex = 0;
    circularBuffer->readIndex = 0;
    circularBuffer->freeSpace = sem_open(SEM_FREE_SPACE, BUFFER_SIZE);
    circularBuffer->usedSpace = sem_open(SEM_USED_SPACE, 0);
    circularBuffer->writeMutex = sem_open(SEM_WRITE_MUTEX, 1);
    return checkForSemError(circularBuffer);
}

errorCode readFromBuffer(circularBuffer *circularBuffer, edgeSet *es)
{
    if(sem_wait(circularBuffer->usedSpace)){
        return -1;
    };

    *es = circularBuffer->array[circularBuffer->readIndex];
    circularBuffer->readIndex = (circularBuffer->readIndex + 1) % BUFFER_SIZE;

    if(sem_post(circularBuffer->freeSpace)){
        return -1;
    };
    
    return 0;
}

errorCode writeToBuffer(edgeSet *edgeSet, circularBuffer *circularBuffer)
{
    if(sem_wait(circularBuffer->freeSpace)){
        return -1;
    };
    if(sem_wait(circularBuffer->writeMutex)){
        return -1;
    };

    circularBuffer->array[circularBuffer->writeIndex] = *edgeSet;
    (*circularBuffer).writeIndex = (circularBuffer->writeIndex + 1) % BUFFER_SIZE;

    if(sem_post(circularBuffer->writeMutex)){
        return -1;
    };
    if(sem_post(circularBuffer->usedSpace)){
        return -1;
    };
    return 0;
}

errorCode closeSemaphores(circularBuffer *circularBuffer)
{
    int e1 = sem_close(circularBuffer->freeSpace);
    int e2 = sem_close(circularBuffer->usedSpace);
    int e3 = sem_close(circularBuffer->writeMutex);
    return e1 || e2 || e3;
}

errorCode unlinkSemaphores()
{
    int e1 = sem_unlink(SEM_FREE_SPACE);
    int e2 = sem_unlink(SEM_USED_SPACE);
    int e3 = sem_unlink(SEM_WRITE_MUTEX);
    return e1 || e2 || e3;
}

errorCode unmapSharedMemory(circularBuffer *b)
{
    munmap(b, sizeof(*b));

}

errorCode unlinkSharedMemory()
{
    shm_unlink(CIRCULAR_ARRAY_BUFFER); 
}