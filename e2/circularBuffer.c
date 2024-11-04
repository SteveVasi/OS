#include <stdio.h>

int main(int argc, char** argv){
   // is essentially an array of the elements we want to pass through
   // elements are written successively and starts from the beginning when it ends (FIFO)

    // we implement it in a way which makes reading and writing simultaneously possible by using 2 semaphores
    // one semaphore for tracking free space (value corresponds to the amount of free space in buffer array). its initialized to size of buffer 
    // and one semaphore for tracking used space (value corresponds to the amount of used space in buffer array). its initialized to 0


}

