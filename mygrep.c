#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <bits/getopt_core.h>

#define bool int
#define true 1
#define false 0

// forward declarations
bool grepFile(FILE *input, FILE *output, char *keyord, bool isCaseSensitive);
static void parseFlags(int argc, char **argv);
static FILE *findOutput(void);
static void usage(void);
static char *findKeyword(char **argv);
static void fopenFail(void);
static bool compareInsensitive(char *keyword, char *line);
static bool compareSensitive(char *keyword, char *line);

// flags
bool isCaseSensitive = true;
bool wantsOutFile = false;
char *outPath;
char *inPath;

struct flags
{
    bool isCaseSensitive;
    bool wantsOutFile;
    char *outPath;
    char *inPath;
};

int main(int argc, char *argv[])
{

    struct flags flags;
    flags.isCaseSensitive = true;
    flags.wantsOutFile = false;
    

    parseFlags(argc, argv);
    FILE *output = findOutput();
    char *keyword = findKeyword(argv);

    int numberOfPositionalArguments = argc - optind;

    if (numberOfPositionalArguments < 1)
    {
        usage();
    }
    else if (numberOfPositionalArguments == 1)
    {
        grepFile(stdin, output, keyword, isCaseSensitive);
    }
    else
    {
        for (int i = optind + 1; i < argc; i++) // i is (optind + 1) because optind is the index of the keyword
        {
            inPath = argv[i];
            FILE *input = fopen(inPath, "r");
            if (input == NULL)
            {
                fopenFail();
            }
            grepFile(input, output, keyword, isCaseSensitive);
        }
    }

    fclose(output);
}

bool grepFile(FILE *input, FILE *output, char *keyword, bool isCaseSensitive)
{
    bool (*compare)(char *keyword, char *line);

    if (isCaseSensitive)
    {
        compare = &compareSensitive;
    }
    else
    {
        compare = &compareInsensitive;
    }

    char *currentLine = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&currentLine, &len, input)) != -1)
    {
        if (compare(keyword, currentLine))
        {
            fprintf(output, "%s", currentLine);
        }
    }

    free(currentLine);
    fclose(input);
    return EXIT_SUCCESS;
}

static bool compareSensitive(char *keyword, char *line)
{
    return strstr(line, keyword) != NULL;
}

static bool compareInsensitive(char *keyword, char *line)
{
    return strcasestr(line, keyword) != NULL;
}

static void parseFlags(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "io:")) != -1)
    {
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

static FILE *findOutput(void)
{
    if (wantsOutFile)
    {
        FILE *outfile = fopen(outPath, "w");

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
