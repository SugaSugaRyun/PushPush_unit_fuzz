#include "../server.h"
#include <fcntl.h>

int main(void){

	printf("TEST: sending_msg_all\n");
	for(int i = 0; i < 5; i++){
		clnt_cnt++;
		char filename[20];
		sprintf(filename, "sock/clnt_socks_%d", i);
		if((clnt_socks[i] = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0) perror("open");
	}

	char event[256];
	fgets(event, 256, stdin);
	send_msg_all(event, strlen(event));
	for(int i = 0; i < 5; i++) close(clnt_socks[i]);
	printf("DONE: sending_msg_all\n");
	

}
