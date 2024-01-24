#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <fcntl.h>

#define PATH_LENGTH 256
#define BUF_SIZE 256

//write_byte: write datas to socket, guarantee that all the byte is sent. 
int write_byte(int sock, void * buf, int size)
{

	int write_size = 0;
	int str_len = 0;
	while(write_size < size)
	{
		str_len = write(sock, buf + write_size, size - write_size);
		if( str_len == 0)
		{
			return 0;
		}
		if( str_len == -1)
		{
			// disconnected(sock);
		}
		write_size += str_len;
	}
	return write_size;
}

int main(void){
    int fd = open("testcases/writebyte_test_output", O_WRONLY | O_CREAT, 0666);
    char buf[BUF_SIZE];
	int readlen = read(STDIN_FILENO, buf, BUF_SIZE-1);
    int read_size = write_byte(fd, buf, readlen);
    close(fd);
	
    return 0;
}