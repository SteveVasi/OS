#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


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
        return size / 2 + 1;
    }
}

bool compareLines(char* line1, char* line2){
    return strcmp(line1, line2) <= 0;
}


/*
 * recursive variant of merge sort

    input: stdin until EOF encountered
 * 
 */
int main(int argc, char *argv[]) {

    /*
        use fflush after printf to avoid weird behaviour
    
        gdb debugging:
            before fork is executed: set follow-fork-mode [child|parent]
    
     */

    int length = get_input_length(argc);

    if(length == 0){
        usage();
        return EXIT_FAILURE;
    }

    if(length == 1){ 
        fprintf(stdout, argv[0]);
        return EXIT_SUCCESS;
    }

    if(length > 1){
        int midpoint = getMidpoint(length);
        
        pid_t pid_first_child = fork();
        pid_t pid_second_child = fork();

        // [0] is read end, [1] is write end
        int pipefd_first_child[2];
        int pipefd_second_child[2];
        
        if (pipe(pipefd_first_child) == -1) {
               perror("pipe error");
               exit(EXIT_FAILURE);
        }
        if (pipe(pipefd_second_child) == -1) {
               perror("pipe error");
               exit(EXIT_FAILURE);
        }

        dup2(pipefd_first_child[0], STDIN_FILENO);
        dup2(pipefd_first_child[1], STDOUT_FILENO);
        dup2(pipefd_second_child[0], STDIN_FILENO);
        dup2(pipefd_second_child[1], STDOUT_FILENO);

        switch (pid_first_child)
        {
        case -1:
            perror("Could not fork!\n");
            break;
        case 0: // we are in child task        
        
            //execlp: l -> variable number of args, 
            //        p -> search env var $PATH for the program specified 
            // 1. argument is programs name argv[0]
            // 2. argument is NULL pointer

            // read from parent
            
            //exit
            break;
        default: // we are in parent task

            //write to child
            fwrite(argv[0], sizeof(char), 0, pipefd_first_child[1]);
            fwrite(argv[midpoint], sizeof(char), 0, pipefd_second_child[1]);

            char* first_line;
            char* second_line;
            fread()
            // parent wants to use child's output
            // wait for children to finish
            // merge the outputs of children
            break;
        }
        


        //exit with exit(EXIT_FAILURE/SUCCESS)
    }
}