#include "../server.h"
#include <fcntl.h>

int main(void){
    int fd = open("test_main/testcases/writebyte_main_output", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    char buf[BUF_SIZE];
	int readlen = read(STDIN_FILENO, buf, BUF_SIZE-1);
    int read_size = write_byte(fd, buf, readlen);
    close(fd);
	
    return 0;
}