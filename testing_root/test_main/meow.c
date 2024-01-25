#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>

#define BUF_SIZE 256
#define MAX_USER 4
#define NAME_SIZE 16
#define PATH_LENGTH 256
int main(){

	printf("hehe\n");
	clnt_cnt = 0;
	write_byte(STDERR_FILENO, "HELLO", 6);

}
