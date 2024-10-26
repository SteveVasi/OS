#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <bits/getopt_core.h>

#define bool int
#define true 1
#define false 0
#define BUFFER_MAX 2000

// forward declarations
void parseFlags(int argc, char** argv);
FILE* findOutput();
void usage(void);
FILE* findInput(int argc, char** argv);
char* findKeyword(char** argv);
void fopenFail(void);

// flags
bool isCaseSensitive = false;
bool wantsOutFile = false;
char* outPath;
char* inPath;



int main(int argc, char *argv[]){

    parseFlags(argc, argv);
    FILE* output = findOutput();
    FILE* input = findInput(argc, argv);
    char* keyword = findKeyword(argv);
    

    char* currentLine = malloc(BUFFER_MAX * sizeof(char));
    bool hasNext;

    do {
        // strstr and strcasestr for comparison. tolower toupper
        hasNext = fgets(currentLine, BUFFER_MAX, input) != NULL;
        bool (*compare)(char*, char*);

        if (isCaseSensitive)
        {
            /* code */
        } else{
            
        }
        
    } 
    while (hasNext); 
    
    
    free(currentLine);
}

void parseFlags(int argc, char** argv){
    int opt;
    while(( opt = getopt(argc, argv, "io:")) != -1){
        switch (opt)
        {
        case 'i':
            isCaseSensitive = true;
            break;
        case 'o':
            wantsOutFile = true;
            outPath = optarg;
            break; 
        default:
            usage();
            break;
        }
    }
}

FILE* findOutput(){
    if (wantsOutFile)
    {
        FILE* outfile = fopen(outPath, "w");

        if (outfile == NULL)
        {
            fopenFail();
        }
        
        return outfile;
    } else{
        return stdout;
    }

}

FILE* findInput(int argc, char** argv){
    int pos_args_count = argc - optind;

    if (pos_args_count < 1)
    {
        usage();
    }

    if (pos_args_count == 1)
    {
        return stdin;
    }

    if ( (argc - optind) == 2 ) { /* number of positional arguments is 2 */
        inPath = argv[optind+1];
        FILE* input = fopen(inPath, "r");

        if (input == NULL)
        {
            fopenFail();
        }

        return input;
        
    } else { 
        printf("Not implemented yet!");
        exit(EXIT_FAILURE);
    }
}

char* findKeyword(char** argv){
    return argv[optind];
}

void usage(void){
    fprintf(stderr, "Usage Message");
    exit(EXIT_FAILURE);
}

void fopenFail(void){
    fprintf(stderr, "fopen failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}


