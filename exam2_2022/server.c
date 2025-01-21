#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#define COMMAND ("./doStuff")
#define MAX_ARGUMENT_LEN (100)

static int setup(char *port){

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    struct addrinfo *address_info;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int result = getaddrinfo(NULL, port, &hints, &address_info);
    if(result != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
    }

    int sockfd = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);    
    if(sockfd < 0){
        perror("error creating socket");
        return -1;
    }
    
    if(bind(sockfd, address_info->ai_addr, address_info->ai_addrlen) < 0){
        perror("bind error");
        return -1;
    }
    freeaddrinfo(address_info);
    if(listen(sockfd, 1) < 0){
        perror("listen error");
        return -1;        
    }
    return sockfd;
}


/**
### Task 2: accept connections from the created socket as a server
Wait for connections on the received socket file descriptor and accept them.
Read the arguments transmitted by the client from the connection and save them in a buffer that can hold a C-string with `MAX_ARGUMENT_LEN` characters (excluding `\0`).
Then you should call `execute_command()` with the content of the clients request and read the `FILE*` returned by this function.
The execution of the command should be done in a forked child process.
Then you should read the content from the file stream and send it to the client waiting on the accepted connection.
(tip: use `fileno(FILE* f)` to read and write using the underlying file descriptors, when things don't work as expected. Another advantage is that you won't have to call `fflush`.)
 */


int communicate(int sockfd, char *buffer) {
    int connfd = accept(sockfd, NULL, NULL);
    FILE *stream = fdopen(connfd, "r+");
    
    if (connfd < 0)
    {
        perror("accept error");
        return -1;
    }

    fread(buffer, sizeof(char), MAX_ARGUMENT_LEN, stream);
    return 0;
}



int main(int argc, char *argv[])
{
    char* port = NULL;
    if(argc != 2) {
        perror("wrong arguments!");
        exit(EXIT_FAILURE);
    }
    port = argv[1];

    int fd = setup(port);
    char buf[MAX_ARGUMENT_LEN+1];
    communicate(fd, buf);
    printf("Server received: %s", buf);
}
