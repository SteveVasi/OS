#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#define COMMAND ("cat")
#define MAX_ARGUMENT_LEN (100)
#define BUFFER_SIZE (1024)

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
    printf("accepting connections on port %s\n", port);
    return sockfd;
}

FILE* execute_command(char *cmd){
    int pipefd[2];
    if(pipe(pipefd) < 0){
        perror("pipe error");
        return NULL;
    }
    
    
    pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        return NULL;
    }

    if(pid == 0){
        // were in child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
        dup2(pipefd[1], STDERR_FILENO); // redirect stderr to pipe
        close(pipefd[1]);

        execlp("/bin/sh", "sh", "-c", cmd, NULL);
        perror("execlp error");
        return NULL;
    } else {
        // parent process
        close(pipefd[1]);
        return fdopen(pipefd[0], "r");
    }
    
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


int communicate(int sockfd, char *request) {
    int connfd = accept(sockfd, NULL, NULL);
    FILE *stream = fdopen(connfd, "r+");
    
    if (connfd < 0)
    {
        perror("accept error");
        return -1;
    }

    fread(request, sizeof(char), MAX_ARGUMENT_LEN, stream);
    FILE* output = execute_command(request);
    if(output == NULL){
        char *ret_msg = "Error executing command\n";
        fwrite(ret_msg, sizeof(char), strlen(ret_msg), stream);
        fclose(stream);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    size_t n;

    while((n = fread(buffer, sizeof(char), sizeof(buffer), output)) > 0){
        fwrite(buffer, sizeof(char), n, stream);
    }
    
    fclose(output);
    fclose(stream);
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
    

    
        


    printf("Server received: %s\n", buf);
}
