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

errorCode memoryMapBuffer(int sharedMemoryFileDescriptor, circularBuffer **cb)
{
    *cb = mmap(NULL,
                                          sizeof(circularBuffer),
                                          PROT_READ | PROT_WRITE,
                                          MAP_SHARED,
                                          sharedMemoryFileDescriptor,
                                          0);
    if (*cb == MAP_FAILED)
    {
        perror("Memory mapping failed");
        *cb = NULL;
        return -1;
    }
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

    circularBuffer->freeSpace = sem_open(SEM_FREE_SPACE, O_CREAT, 0777, BUFFER_SIZE);
    if (circularBuffer->freeSpace == SEM_FAILED) {
        perror("Failed to initialize freeSpace semaphore");
        return -1;
    }

    circularBuffer->usedSpace = sem_open(SEM_USED_SPACE, O_CREAT, 0777, 0);
    if (circularBuffer->usedSpace == SEM_FAILED) {
        perror("Failed to initialize usedSpace semaphore");
        sem_close(circularBuffer->freeSpace);
        return -1;
    }

    circularBuffer->writeMutex = sem_open(SEM_WRITE_MUTEX, O_CREAT, 0777, 1);
    if (circularBuffer->writeMutex == SEM_FAILED) {
        perror("Failed to initialize writeMutex semaphore");
        sem_close(circularBuffer->freeSpace); // Cleanup
        sem_close(circularBuffer->usedSpace); // Cleanup
        return -1;
    }
    
    return 0;
}

errorCode initSharedBufferClient(circularBuffer *circularBuffer)
{
    circularBuffer->writeIndex = 0;
    circularBuffer->readIndex = 0;
    
    circularBuffer->freeSpace = sem_open(SEM_FREE_SPACE, O_CREAT);
    if (circularBuffer->freeSpace == SEM_FAILED) {
        perror("Failed to initialize freeSpace semaphore");
        return -1;
    }
    circularBuffer->usedSpace = sem_open(SEM_USED_SPACE, O_CREAT);
    if (circularBuffer->usedSpace == SEM_FAILED) {
        perror("Failed to initialize usedSpace semaphore");
        return -1;
    }
    circularBuffer->writeMutex = sem_open(SEM_WRITE_MUTEX, O_CREAT);
    if (circularBuffer->writeMutex == SEM_FAILED) {
        perror("Failed to initialize writeMutex semaphore");
        return -1;
    }
    
    return 0;
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

errorCode closeSemaphores(circularBuffer *cb)
{
    if (cb == NULL) {
        perror("Circular buffer is NULL");
        return -1;
    }

    if (cb->freeSpace != NULL 
        && cb->freeSpace != SEM_FAILED) {
        if (sem_close(cb->freeSpace) != 0) {
            perror("Error closing freeSpace semaphore");
        }
    }
    if (cb->usedSpace != NULL && cb->usedSpace != SEM_FAILED) {
        if (sem_close(cb->usedSpace) != 0) {
            perror("Error closing usedSpace semaphore");
        }
    }
    if (cb->writeMutex != NULL && cb->writeMutex != SEM_FAILED) {
        if (sem_close(cb->writeMutex) != 0) {
            perror("Error closing writeMutex semaphore");
        }
    }
    return 0;
}

errorCode unlinkSemaphores()
{
    int e1 = sem_unlink(SEM_FREE_SPACE);
    if(e1){
        perror("Error unlinking semaphore free space");
    }
    int e2 = sem_unlink(SEM_USED_SPACE);
    if(e2){
        perror("Error unlinking semaphore used space");
    }
    int e3 = sem_unlink(SEM_WRITE_MUTEX);
    if(e3){
        perror("Error unlinking semaphore write mutex");
    }
    return e1 || e2 || e3;
}

errorCode unmapSharedMemory(circularBuffer *b)
{
    if(b == NULL){
        perror("Error unmapping shared memory. Buffer is NULL");
    }
    int e = munmap(b, sizeof(*b));
    if(e){
        perror("Error unmapping shared memory. Unknown");
    }
    return e;
}

errorCode unlinkSharedMemory()
{
    int e = shm_unlink(CIRCULAR_ARRAY_BUFFER); 
    if(e){
        perror("Error unlinking shared memory");
    }
    return e;
}