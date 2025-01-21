#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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

    if(connect(sockfd, address_info->ai_addr, address_info->ai_addrlen) < 0){
        perror("error connecting");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

static int talk(int sockfd){
    
    return sockfd;
}

int main(int argc, char *argv[])
{
    char* port = NULL;

    if(argc != 2) {
        perror("error connecting");
        exit(EXIT_FAILURE);
    }


    int sockfd = setup(port);
    talk(sockfd);
}