#include <stdio.h>
#include <stdlib.h>


//error_handling: print error message
void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(void){
	
	char buf[256];
	fgets(buf, 256, stdin);
	error_handling(buf);

}
