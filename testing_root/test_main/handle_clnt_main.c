#include <fcntl.h>

int clnt_cnt = 5;
int clnt_socks[5];
int max_user = 4;
int usr_cnt = 0;
char user_name[5][20];
char json_serialize[128] = "\nthis is json serialize file\n";
int json_size = 128;

int main(void){

	char buf[128];
	for(clnt_cnt = 0; clnt_cnt < 5; clnt_cnt++){
		sprintf(buf, "file/handle_clnt_user%d", clnt_cnt);
		sprintf(user_name[i], "user%d", clnt_cnt);
		if((clnt_socks[clnt_cnt] = open(buf, O_RDWR|O_CREAT|O_TRUNC, 0666)) < 0) perror("open");
		printf("%d ", clnt_socks[clnt_cnt]);
		
	}

	int sock;
	printf("select target socket: ");
	scanf("%d", &sock);
	handle_clnt((void*)&sock);

	

}


