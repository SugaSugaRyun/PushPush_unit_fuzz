#include "../client.h"

int main(void){

	int fd[2];
	int event;

	if(pipe(fd) < 0) perror("pipe");

	for(int i = 0; i < 5; i++){
		fscanf(stdin, "%d", &event);
		write(fd[1], &event, sizeof(int));
	}

	close(fd[1]);	
	recv_msg((void*)&fd[0]);


	for(int i = front+1; i < rear+1; i++) printf("%d ", event_arry[i]);


}
