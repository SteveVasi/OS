#ifndef COMMONS_H
#define COMMONS_H

#define PERMISSION 0666
#define SHM_PATH "/shm_testprep"
#define SEM_REQUEST "/sem_request"
#define SEM_RESPONSE "/sem_response"
#define SEM_CLIENT "/sem_client"
#define SHM_SIZE 256

void errorExit(char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}


#endif