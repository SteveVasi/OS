#include <stdio.h>

// is essentially an array of the elements we want to pass through
// elements are written successively and starts from the beginning when it ends (FIFO)

// we implement it in a way which makes reading and writing simultaneously possible by using 2 semaphores
// one semaphore for tracking free space (value corresponds to the amount of free space in buffer array). its initialized to size of buffer
// and one semaphore for tracking used space (value corresponds to the amount of used space in buffer array). its initialized to 0

enum { n = 10};

int buffer[n]; // only (n-1) elements can be stored at a given time
int writeIndex = 0;
int readIndex = 0;


