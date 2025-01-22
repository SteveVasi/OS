#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_RESPONSE_LEN 1024 // Maximum size for server response


static int setup(char* port){

    struct addrinfo hints;
    struct addrinfo *address_info;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo("localhost", port, &hints, &address_info);
    if(result != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
    }

    int sockfd = socket(address_info->ai_family, 
                        address_info->ai_socktype, 
                        address_info->ai_protocol);
    if(sockfd < 0){
        perror("error creating socket");
        exit(EXIT_FAILURE);
    }

    printf("Attempting to connect to server\n");
    fflush(stdout);
    if(connect(sockfd, address_info->ai_addr, address_info->ai_addrlen) < 0){
        perror("error connecting");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");
    fflush(stdout);
    return sockfd;
}

static int communicate(int sockfd, char *buffer, char* msg){
    FILE *stream = fdopen(sockfd, "r+");
    if(stream == NULL){
        perror("error with fdopen");
        return -1;
    }

    printf("Attempting to write to server: %s\n", msg);
    fflush(stdout);
    int sent_count = fwrite(msg, sizeof(char), strlen(msg), stream);
    fflush(stream);
    if(sent_count != strlen(msg)){
        perror("error with fwrite");
    }

    int received_count = fread(buffer, sizeof(char), MAX_RESPONSE_LEN, stream);
    fflush(stream);
    buffer[received_count] = '\0';
    if(received_count < 0){
        perror("error with fread");
    }
    

    fclose(stream);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 3) {
        perror("error connecting");
        exit(EXIT_FAILURE);
    }
    char *port = argv[1];
    char *msg = argv[2];

    int sockfd = setup(port);
    char buffer[MAX_RESPONSE_LEN];
    communicate(sockfd, buffer, msg);

    printf("Received: %s\n", buffer);
    close(sockfd);
    return 0;
}