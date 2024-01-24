#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <fcntl.h>
#include <string.h>

#define PATH_LENGTH 256
#define BUF_SIZE 256

//read_byte: read datas from socket, guarantee that all byte is accepted.
int read_byte(int sock, void * buf, int size)
{
	int read_size = 0;
	int str_len = 0;
	while(read_size < size)
	{
		str_len = read(sock, buf + read_size, size - read_size);
		if( str_len == 0)
		{
			// disconnected(sock);
			return 0;
		}
		if( str_len == -1)
		{

		}
		read_size += str_len;
	}
	return read_size;
}

int main(void){
    char buf[BUF_SIZE];

    int read_size = read_byte(STDIN_FILENO, buf, BUF_SIZE-1);
	
	for(int i=0; i<read_size; i++){
		printf("%c", buf[i]);
	}

    return 0;
}