#define bool int

typedef struct flags_t
{
    bool isCaseSensitive;
    bool wantsOutFile;
    char *outPath;
    char *inPath;
    FILE* inStream;
    FILE* outStream;
    char* keyword;
} flags;
