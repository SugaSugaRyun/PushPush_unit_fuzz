#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int clnt_socks[5];
int clnt_cnt = 5;

//send_msg_all: send msg to all connected users
void send_msg_all(void * event, int len){
//	pthread_mutex_lock(&mutx);
	for (int i = 0; i < clnt_cnt; i++){
		write(clnt_socks[i], event, len);
		//write_byte(clnt_socks[i], event, len);
	}
//	pthread_mutex_unlock(&mutx);
}

int main(void){

	for(int i = 0; i < 5; i++){
		char filename[20];
		sprintf(filename, "file/test%d.txt", i);
		if((clnt_socks[i] = open(filename, O_WRONLY|O_CREAT, 0666)) < 0) perror("open");
	}

	char event[256];
	fgets(event, 256, stdin);
	send_msg_all(event, strlen(event));
	printf("done: sending all\n");
	

}
