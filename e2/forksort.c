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
        //Write line to stdout
        return EXIT_SUCCESS;
    }

    if(length > 1){
        int midpoint = getMidpoint(length);

        
        // execute each part in a new child process (2 new child processes are created)
        
        pid_t pid_first_child = fork();
        pid_t pid_second_child = fork();

        // create 2 unnamed pipes per child to redirect stdin and stdout with dup2
        // [0] is read end [1] is write end
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



        switch (pid_first_child)
        {
        case -1:
            perror("Could not fork!\n");
            break;
        case 0: //child task
            //exec
        
        
            //execlp: l -> variable number of args, 
            //        p -> search env var $PATH for the program specified 
            // 1. argument is programs name argv[0]
            // 2. argument is NULL pointer
            
            //exit
            break;
        default: //parent task
            // parent wants to use childs output
            // redirect stdin (fd 0, stdin_fileno)
            // and or (fd 1, stout_fileno) in new process
            //wait for child to finish
            break;
        }
        


        //exit with exit(EXIT_FAILURE/SUCCESS)
    }
}