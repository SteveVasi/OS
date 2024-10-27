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
char* findKeyword(char** argv);
void fopenFail(void);
bool grepFile(FILE* input, FILE* output, char* keyord, bool isCaseSensitive);
bool compareInsensitive(char* keyword, char* line);
bool compareSensitive(char* keyword, char* line);

// flags
bool isCaseSensitive = true;
bool wantsOutFile = false;
char* outPath;
char* inPath;

int main(int argc, char *argv[]){

    parseFlags(argc, argv);
    FILE* output = findOutput();
    char* keyword = findKeyword(argv);

    int numberOfPositionalArguments = argc - optind;

    if (numberOfPositionalArguments < 1)
    {
        usage();
    } else if (numberOfPositionalArguments == 1)
    {
        grepFile(stdin, output, keyword, isCaseSensitive);
    } else{
        for (int i = optind + 1; i < argc; i++) // i is optind + 1 because optind is the index of the keyword
        {
            inPath = argv[i];
            FILE* input = fopen(inPath, "r");
            if (input == NULL)
            {
                fopenFail();
            }
            grepFile(input, output, keyword, isCaseSensitive);
        }
    }
    
    fclose(output);
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
    bool hasNext = fgets(currentLine, BUFFER_MAX, input) != NULL;
    while (hasNext){
        if (compare(keyword, currentLine))
        {
            fprintf(output, currentLine);
        }
        hasNext = fgets(currentLine, BUFFER_MAX, input) != NULL;
    } 
    
    
    free(currentLine);
    fclose(input);
    return EXIT_SUCCESS;
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
            isCaseSensitive = false;
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

char* findKeyword(char** argv){
    return argv[optind];
}

void usage(void){
    fprintf(stderr, "SYNOPSIS\n");
    fprintf(stderr, "\tmygrep [-i] [-o outfile] keyword [file...]\n");

    exit(EXIT_FAILURE);
}

void fopenFail(void){
    fprintf(stderr, "fopen failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}


