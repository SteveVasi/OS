#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define COMMAND ("./doStuff")
#define MAX_ARGUMENT_LEN (100)

static int listen_socket(int listen_port){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("error creating socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in *sa;
    if(bind(sockfd, sa, sizeof(struct sockaddr_in) < 0)){
        perror("bind error");
        exit(EXIT_FAILURE);
        // TODO close maybe?
    }
    if(listen(sockfd, 0) < 0){
        perror("listen error");
        exit(EXIT_FAILURE);
        // TODO close maybe?
    }
    return sockfd;
}


/**
 * ### Task 2: accept connections from the created socket as a server

Wait for connections on the received socket file descriptor and accept them.

Read the arguments transmitted by the client from the connection and save them in a buffer that can hold a C-string with `MAX_ARGUMENT_LEN` characters (excluding `\0`).

Then you should call `execute_command()` with the content of the clients request and read the `FILE*` returned by this function.

The execution of the command should be done in a forked child process.

Then you should read the content from the file stream and send it to the client waiting on the accepted connection.

(tip: use `fileno(FILE* f)` to read and write using the underlying file descriptors, when things don't work as expected. Another advantage is that you won't have to call `fflush`.)

 * 
 * 
 */


void communicate(int sockfd) {
    int connfd = accept(sockfd, NULL, NULL);
    if (connfd < 0)
    {
        perror("accept error");
        exit(EXIT_FAILURE);
        // TODO close maybe?
    }

    char buf[MAX_ARGUMENT_LEN+1];
    fread(buf, sizeof(char), MAX_ARGUMENT_LEN, connfd);

}



int main(int argc, char *argv[])
{
    int port = strtol(argv[1], NULL, 10);
    int fd = listen_socket(argv[1]);
    communicate(fd);
}
