#include <stdio.h>

int main(int argc, char *argv[]){

    int opt;

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


}


// strstr and strcasestr for comparison. tolower toupper