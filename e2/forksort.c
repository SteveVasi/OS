#include <stdio.h>
#include <stdlib.h>

#define bool int
#define errorCode bool

int get_input_length(int argc){
    return argc - 1;
}

static void usage(void)
{
    fprintf(stderr, "SYNOPSIS\n");
    fprintf(stderr, "\tforksort\n");

    exit(EXIT_FAILURE);
}


int getMidpoint(int size){
    if (size % 2 == 0)
    {
        return size / 2;
    } else {
        size / 2 + 1;
    }
}


/*
 * recursive variant of merge sort

    input: stdin until EOF encountered
 * 
 */
int main(int argc, char *argv[]) {

    int length = get_input_length(argc);

    if(length == 0){
        usage();
        return EXIT_FAILURE;
    }

    if(length == 1){
        //Write line to stdout
        return EXIT_SUCCESS;
    }

    if(length > 1){
        //split into two parts with length n/2
        // if odd, then one part is longer
        int midpoint = getMidpoint(length);
        
        // use fork and execlp to:
        // execute each part in a new child process (2 new child processes are created)
        
        
        
        // create 2 unnamed pipes per child to redirect stdin and stdout with dup2

    }
}