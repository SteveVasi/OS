#include <stdio.h>
#include <unistd.h>

#define EXIT_FAILURE -1

extern int optind; // removes inline error in vscode

int main(int argc, char *argv[]){

    int opt;
    char *input;
    

    while(( opt = getopt(argc, argv, "io:")) != -1){
        switch (opt)
        {
        case 'i':
            break;
        case 'o':
            break; 
        default:
            break;
        }
    }

    int pos_args_count = argc - optind;

    if (pos_args_count < 2)
    {
        usage();
        return;
    }
    
    input = argv[optind];

    if ( (argc - optind) == 2 ) { /* number of positional arguments is 2 */
        char *input = argv[optind];
        char *output = argv[optind+1];
    } else { /* error handling */}


}

void usage(void){
    exit(EXIT_FAILURE);
}


// strstr and strcasestr for comparison. tolower toupper