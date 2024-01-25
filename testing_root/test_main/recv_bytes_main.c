#include "../client.h"
#include <fcntl.h>

int main(void){
    char buf[BUF_SIZE];
	char buf2[BUF_SIZE];
	int fd[2];

	if(pipe(fd) == -1) perror("pipe");
	fgets(buf, BUF_SIZE, stdin);

	int len = write(fd[1], buf, strlen(buf));
	printf("writelen: %d\n", len);
	len = recv(fd[0], buf2, strlen(buf), 0);
    //len = recv_bytes(fd[0], buf2, strlen(buf));
	printf("readlen: %d\n", len);
	perror("recv");

	printf("*%s", buf2);

    return 0;
}
