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
#include <fcntl.h>

#define BUF_SIZE 256
#define MAX_USER 4
#define NAME_SIZE 16
#define PATH_LENGTH 256


int main(){
	printf("TEST: disconnected\n");
/*
	for(int i = 0; i < 5; i++){
		char filename[20];
		sprintf(filename, "sock/clnt_socks_%d", i);
		if((clnt_socks[i] = open(filename, O_WRONLY|O_CREAT, 0666)) < 0) perror("open");
	}

	printf("\nenter sock: ");
	int sock;
	scanf("%d", &sock);
	disconnected(sock);

	for(int i = 0; i < clnt_cnt; i++) printf("%d ", clnt_socks[i]);
	*/
	clnt_cnt = 10;
	printf("%d", clnt_cnt);

	printf("\nDONE: disconnected\n");

}

