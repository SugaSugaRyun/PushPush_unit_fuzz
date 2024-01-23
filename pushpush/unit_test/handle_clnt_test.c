#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int clnt_cnt;
int clnt_socks[5];
int max_usr = 4;
char user_name[5][20];

//handle_clnt: thread function; handle one client per one thread. send json datas and recv commands, broadcast it to all... 
void *handle_clnt(void * arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0;
	int event;
	int name_size = 0;

	//recive name size
	//str_len = read_byte(clnt_sock, (void *)&name_size, sizeof(int));
	str_len = read(clnt_sock, (void *)&name_size, sizeof(int));

	//reciev name, send id
//	pthread_mutex_lock(&mutx);
	printf("function call: pthread_mutex_lock\n");

	for (int i = 0; i < clnt_cnt; i++) {
		if (clnt_sock == clnt_socks[i]){
			//read_byte(clnt_sock, (void *)user_name[i], name_size);
			read(clnt_sock, (void *)user_name[i], name_size);
			printf("%s is enter\n",user_name[i]);
			//write_byte(clnt_sock, (void *)&i, sizeof(int));
			write(clnt_sock, (void *)&i, sizeof(int));
			usr_cnt++;
			break;
		}
	}

/*
//	pthread_mutex_unlock(&mutx);
	printf("function call: pthread_mutex_unlock\n");

	//send json
	write_byte(clnt_sock, (void *)&json_size, sizeof(int));
	write_byte(clnt_sock, json_serialize, json_size);

	while(usr_cnt < max_user); //wait untill all user is connected
	
	//send connected user information
	for(int i=0; i< max_user; i++)
	{
		int len = strlen(user_name[i]);
		//write_byte(clnt_sock, &len,sizeof(int));
		//write_byte(clnt_sock,user_name[i], len);
		write(clnt_sock, &len,sizeof(int));
		write(clnt_sock,user_name[i], len);
	}

	//receive and echo command
	//while (read_byte(clnt_sock, (void *)&event, sizeof(int))) 
	while (read(clnt_sock, (void *)&event, sizeof(int))) 
	{
		printf("move: %d\n", event);
		//send_msg_all((void *)&event, sizeof(int));
		printf("function call: send_msg_all\n");
		//detect end flag
		if(event == max_user*4)
		{
			printf("end game!\n");
			disconnected(clnt_sock);
		}
	}
*/	
	return NULL;
}


int main(void){

	for(int i = 0; i < 5; i++){
		char filename[20];
		sprintf(filename, "file/test%d.txt", i);
		sprintf(user_name[i], "user%d", i);
		if((clnt_socks[i] = open(filename, O_WRONLY|O_CREAT, 0666)) < 0) perror("open");
		printf("%d ", clnt_socks[i]);
	}

	int sock;
	printf("select target socket: ");
	scanf("%d", &sock);
	handle_clnt((void*)&sock);

	printf("done: handle_clnt\n");
	

}


