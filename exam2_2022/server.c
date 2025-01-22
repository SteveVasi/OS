#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>


#define READ_END 0
#define WRITE_END 1


typedef struct {
    pid_t pid;
    int parent_to_child[2];
    int child_to_parent[2];
} child_t;

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

    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    
    if(bind(sockfd, address_info->ai_addr, address_info->ai_addrlen) < 0){
        perror("bind error");
        return -1;
    }
    if(listen(sockfd, 1) < 0){
        perror("listen error");
        return -1;        
    }
    freeaddrinfo(address_info);
    printf("listening on port %s\n", port);
    fflush(stdout);
    return sockfd;
}

int create_child(child_t *child){
    if(pipe(child->child_to_parent)){
        error("pipe error");
    }
    if(pipe(child->parent_to_child)){
        close(child->parent_to_child[READ_END]);
        close(child->parent_to_child[WRITE_END]);
        error("pipe error");
    }

    child->pid = fork();
    if(child->pid < 0){
        error("fork error");
    }

    if(child->pid == 0) {
        // in child
        close(child->child_to_parent[READ_END]);
        close(child->parent_to_child[WRITE_END]);

        dup2(child->child_to_parent[WRITE_END], STDOUT_FILENO);
        dup2(child->parent_to_child[READ_END], STDIN_FILENO);


    } else {
        // in parent
        close(child->parent_to_child[READ_END]);
        close(child->child_to_parent[WRITE_END]);
    }

    return 0;
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


int process_message(int client_fd, child_t child) {

    FILE *connFile = fdopen(client_fd, "r+");
    
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
    return 0;
}

int take_request(char* request, int fd){
    FILE *stream = fdopen(connfd, "r+");
    if(stream == NULL){
        perror("fdopen error");
        return -1;
    }

    read(connfd, request, MAX_ARGUMENT_LEN);
    printf("Server received request: %s\n", request);
    fflush(stdout);
    fclose(stream);
    return 0;
}

int execute_command(char* request){
    execlp("ls", "-l", "-a", (char *) NULL);
    perror("execlp error");
    exit(EXIT_FAILURE);
}

int execute_and_wait(child_t *child, char* request){
    if(child->pid == 0){
        // were in child process
        printf("We are in child process, about to execute command\n");
        fflush(stdout);
        execute_command(request);
    } else {
        // parent process
        int status;
        printf("waiting on child process\n");
        fflush(stdout);
        fflush(stderr);
        if(wait(&status)){
            fprintf(stderr, "wait failed\n");
            return NULL;
        };
    }
    
    return 0;
}

int accept_client(socket_fd){
    printf("Accepting connections now!\n");
    fflush(stdout);
    int connfd = accept(sockfd, NULL, NULL);
    if (connfd < 0)
    {
        error("accept error");
        return -1;
    }
    printf("Accepted client\n");
    fflush(stdout);
    return connfd;
}

int main(int argc, char *argv[])
{
    char* port = NULL;
    char request[MAX_ARGUMENT_LEN+1];
    child_t child;

    if(argc != 2) {
        fprintf(stderr, "wrong arguments!\n");
        exit(EXIT_FAILURE);
    }
    port = argv[1];

    int socket_fd = setup(port);
    int client_fd = accept_client(socket_fd);
    take_request(request, fd)
    create_child(&child);
    execute_and_wait(&child, request);
    process_message(client, child);
    
}
