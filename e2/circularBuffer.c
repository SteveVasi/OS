#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "edge.h"
#include "circularBuffer.h"



// this is the "server", generator and supervisor are clients

// is essentially an array of the elements we want to pass through
// elements are written successively and starts from the beginning when it ends (FIFO)

// we implement it in a way which makes reading and writing simultaneously possible by using 2 semaphores
// one semaphore for tracking free space (value corresponds to the amount of free space in buffer array). its initialized to size of buffer
// and one semaphore for tracking used space (value corresponds to the amount of used space in buffer array). its initialized to 0


// each element in the buffer is a set of edges!

// set up shared memory

