#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>

void usage(void);

int main(int argc, char** argv){
    int limit = INT_MAX;
    int delay = 0;
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
    

    usage();
}

void usage(void){
    printf("SYNOPSIS:\n");
    printf("    supervisor [-n limit] [-w delay]\n");
}