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
bool grepFile(FILE* input, FILE* output, char* keyord, bool isCaseSensitive);
bool compareInsensitive(char* keyword, char* line);
bool compareSensitive(char* keyword, char* line);

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
    
    grepFile(input, output, keyword, isCaseSensitive);
    
}

bool grepFile(FILE* input, FILE* output, char* keyword, bool isCaseSensitive){
    bool (*compare)(char* keyword, char* line);
    
    if (isCaseSensitive)
    {
        compare = &compareSensitive;
    } else{
        compare = &compareInsensitive;
    }

    char* currentLine = malloc(BUFFER_MAX * sizeof(char));
    bool hasNext;
    do {
        hasNext = fgets(currentLine, BUFFER_MAX, input) != NULL;
        if (compare(keyword, currentLine))
        {
            fprintf(output, currentLine);
        }
    } 
    while (hasNext); 
    
    free(currentLine);
}


bool compareSensitive(char* keyword, char* line){
    return strstr(line, keyword) != NULL; 
}

bool compareInsensitive(char* keyword, char* line){
    return strcasestr(line, keyword) != NULL;
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
    fprintf(stderr, "Usage Message\n");
    exit(EXIT_FAILURE);
}

void fopenFail(void){
    fprintf(stderr, "fopen failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}


