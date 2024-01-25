#include <fcntl.h>
#include "../server.h"

int main(void){

	printf("TEST: handle_clnt()\n");
	char buf[128];
	max_user = 4;
	user_name = (char**)malloc(sizeof(char*)*max_user);
	json_serialize = (char*)malloc(sizeof(char)*BUF_SIZE);
	strcpy(json_serialize, "test: json file for handle_clnt()\n");
	json_size = strlen(json_serialize);

	for(clnt_cnt = 0; clnt_cnt < 5; clnt_cnt++){
		sprintf(buf, "sock/handle_clnt_user%d", clnt_cnt);
		user_name[clnt_cnt] = (char*)malloc(sizeof(char)*BUF_SIZE);
		sprintf(user_name[clnt_cnt], "user%d", clnt_cnt);
		if((clnt_socks[clnt_cnt] = open(buf, O_RDWR|O_CREAT|O_TRUNC, 0666)) < 0) perror("open");
		sprintf(buf, "5 user%d 1 2 3 %d", clnt_cnt, max_user*4);
		write(clnt_socks[clnt_cnt], buf, strlen(buf)); 
		printf("%d ", clnt_socks[clnt_cnt]);
	}

	int sock;
	printf("select target socket: ");
	scanf("%d", &sock);
	handle_clnt((void*)&sock);
	printf("DONE: handle_clnt()\n");

	

}


