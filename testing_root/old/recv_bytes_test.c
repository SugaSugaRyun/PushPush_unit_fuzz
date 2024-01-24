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

#define NAME_SIZE 16
#define queue_size 20
#define BUF_SIZE 128
#define GDK_KEY_UP 65362
#define GDK_KEY_DOWN 65364
#define GDK_KEY_LEFT 65361
#define GDK_KEY_RIGHT 65363

int recv_bytes(int sock_fd, void * buf, size_t len){
    char * p = (char *)buf;
    size_t acc = 0;

    while(acc < len)
    {
        size_t recved;
        recved = recv(sock_fd, p, len - acc, 0);
        if(recved  == -1 || recved == 0){
            return -1;
        }
        p+= recved ;
        acc += recved ;
    }
    return 0;
}

int main(void){
    
}