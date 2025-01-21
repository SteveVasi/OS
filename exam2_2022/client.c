#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int setup(int port){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("error creating socket");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *ai;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int result = getaddrinfo("localhost", &port, &hints, &ai);
    if(result != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
    }


    struct sockaddr server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    if(connect(sockfd, &server_addr, sizeof(server_addr))){
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
    int opt;
    char* port = NULL;

    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                port = argv[optarg];
                break;
            case '?':
            default:
                perror("Invalid args");
                return EXIT_FAILURE;
        }
    }


    int sockfd = setup(port);
    talk(sockfd);
}