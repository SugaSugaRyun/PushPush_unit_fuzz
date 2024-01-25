#include "../server.h"
#include <fcntl.h>

int main(void){
    char buf[BUF_SIZE];
	char buf2[BUF_SIZE];
	int fd[2];

	pipe(fd);
	fgets(buf, BUF_SIZE, stdin);
    write_byte(fd[1], buf, strlen(buf));

	read(fd[0], buf2, strlen(buf));
	printf("%s", buf2);

    return 0;
}
