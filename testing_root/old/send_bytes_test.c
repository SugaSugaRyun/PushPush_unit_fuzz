#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>
#include "../cJSON.h"
#include <fcntl.h>

#define NAME_SIZE 16
#define queue_size 20
#define BUF_SIZE 128
#define GDK_KEY_UP 65362
#define GDK_KEY_DOWN 65364
#define GDK_KEY_LEFT 65361
#define GDK_KEY_RIGHT 65363

int send_bytes(int sock_fd, void * buf, size_t len){
    char * p = (char *) buf;
    size_t acc = 0;

    while(acc < len){
        size_t sent;
        sent = send(sock_fd, p, len - acc, 0);
        if(sent == -1 || sent == 0){
            return -1;
        }

        p+= sent;
        acc += sent;
    }
    return 0;
}

int main(int argc, char *argv[]){
    struct stat status;
    if(stat(argv[1], &status) == -1){
        perror("stat");
        return 1;
    }
    int inputfd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC);
    
}