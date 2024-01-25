#include "../server.h"
#include "fcntl.h"


int main(){
	printf("TEST: disconnected\n");
	for(int i = 0; i < 5; i++){
		char filename[20];
		clnt_cnt++;
		sprintf(filename, "sock/clnt_socks_%d", i);
		if((clnt_socks[i] = open(filename, O_WRONLY|O_CREAT, 0666)) < 0) perror("open");
		printf("%d ", clnt_socks[i]);
	}

	printf("\nenter sock: ");
	int sock;
	scanf("%d", &sock);
	printf("remove %d...\n", sock);
	disconnected(sock);

	for(int i = 0; i < clnt_cnt; i++) printf("%d ", clnt_socks[i]);

	printf("\nDONE: disconnected\n");

}

