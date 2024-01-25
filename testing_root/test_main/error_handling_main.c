#include "../server.h"


int main(void){
	
	printf("TEST: error_handling\n");
	char buf[256];
	fgets(buf, 256, stdin);
	error_handling(buf);
	printf("DONE: error_handling\n");

}
