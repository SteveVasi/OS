#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <bits/getopt_core.h>
#include "mygrep.h"

#define bool int
#define true 1
#define false 0

// forward declarations
bool grepFile(flags flags);
static flags parseFlags(int argc, char **argv);
static FILE *findOutput(flags flags);
static void usage(void);
static char *findKeyword(char **argv);
static void fopenFail(void);
static bool compareInsensitive(char *keyword, char *line);
static bool compareSensitive(char *keyword, char *line);


int main(int argc, char *argv[])
{

    flags flags = parseFlags(argc, argv); // rename to options
    flags.outStream = findOutput(flags);
    flags.keyword = findKeyword(argv);

    int numberOfPositionalArguments = argc - optind;

    if (numberOfPositionalArguments < 1)
    {
        usage();
    }
    else if (numberOfPositionalArguments == 1)
    {
        flags.inStream = stdin;
        grepFile(flags);
    }
    else
    {
        for (int i = optind + 1; i < argc; i++) // i is (optind + 1) because optind is the index of the keyword
        {
            flags.inPath = argv[i];
            FILE *input = fopen(flags.inPath, "r");
            if (input == NULL)
            {
                fopenFail();
            }
            flags.inStream = input;
            grepFile(flags);
        }
    }

    fclose(flags.outStream);
}

bool grepFile(flags flags)
{
    bool (*compare)(char *keyword, char *line);


    
    if (flags.isCaseSensitive)
    {
        compare = &compareSensitive;
    }
    else
    {
        compare = &compareInsensitive;
    }

    // bool (*compare)(char* char*)

    char *currentLine = NULL;
    size_t len = 0; // size is unsigned 
    ssize_t read; // ssize is signed 


    while ((read = getline(&currentLine, &len, flags.inStream)) != -1) //getline conveniently allocates memory for us
    {
        if (compare(flags.keyword, currentLine))
        {
            fprintf(flags.outStream, "%s", currentLine);
        }
    }

    free(currentLine);
    fclose(flags.inStream);
    return EXIT_SUCCESS;
}

static bool compareSensitive(char *keyword, char *line)
{
    return strstr(line, keyword) != NULL;
}

static bool compareInsensitive(char *keyword, char *line)
{
    return strcasestr(line, keyword) != NULL; // this line gives a compiler warning because it is a comparison between an integer and pointer, but man 3 says that this function returns NULL upon not finding the keyword.
}

static flags parseFlags(int argc, char **argv)
{
    flags flags;
    flags.isCaseSensitive = true;
    flags.wantsOutFile = false;

    int opt;
    while ((opt = getopt(argc, argv, "io:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            flags.isCaseSensitive = false;
            break;
        case 'o':
            flags.wantsOutFile = true;
            flags.outPath = optarg;
            break;
        default:
            usage();
            break;
        }
    }
    return flags;
}

static FILE *findOutput(flags flags)
{
    if (flags.wantsOutFile)
    {
        FILE *outfile = fopen(flags.outPath, "w");

        if (outfile == NULL)
        {
            fopenFail();
        }

        return outfile;
    }
    else
    {
        return stdout;
    }
}

// has to be called after parseFlags() so optind is at the correct index
static char *findKeyword(char **argv)
{
    return argv[optind];
}

static void usage(void)
{
    fprintf(stderr, "SYNOPSIS\n");
    fprintf(stderr, "\tmygrep [-i] [-o outfile] keyword [file...]\n");

    exit(EXIT_FAILURE);
}

static void fopenFail(void)
{
    fprintf(stderr, "fopen failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}
