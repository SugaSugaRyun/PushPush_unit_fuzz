#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int clnt_cnt = 5;
int clnt_socks[5];
int max_user = 4;
int usr_cnt = 0;
char user_name[5][20];
char json_serialize[128] = "\nthis is json serialize file\n";
int json_size = 128;



//read_byte: read datas from socket, guarantee that all byte is accepted.
int read_byte(int sock, void * buf, int size)
{

	int read_size = 0;
	int str_len = 0;

	while(read_size < size)
	{
		str_len = read(sock, buf + read_size, size - read_size);
	fprintf(stderr, "start read byte\n");
		if( str_len == 0)
		{
//			disconnected(sock);
			return 0;
		}
		if( str_len == -1)
		{
			fprintf(stderr, "exit with failure\n");
			exit(EXIT_FAILURE);
			return -1;
		}
		read_size += str_len;
	}
	fprintf(stderr, "end read byte\n");
	return read_size;
}


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
			char I = i + '0';
			write(STDERR_FILENO, (void*)&I ,sizeof(int));

			usr_cnt++;
			break;
		}
	}

//	pthread_mutex_unlock(&mutx);
	printf("function call: pthread_mutex_unlock\n");

	//send json
	//write_byte(clnt_sock, (void *)&json_size, sizeof(int));
	write(STDERR_FILENO, (void *)&json_size, sizeof(int));
	//write_byte(clnt_sock, json_serialize, json_size);
	write(STDERR_FILENO, json_serialize, json_size);

	//wait untill all user is connected
	//while(usr_cnt < max_user); 

	//send connected user information
	for(int i=0; i< max_user; i++){
		int len = strlen(user_name[i]);
		//write_byte(clnt_sock, &len,sizeof(int));
		//write_byte(clnt_sock,user_name[i], len);
		write(STDERR_FILENO, &len,sizeof(int));
		write(STDERR_FILENO,user_name[i], len);
		fprintf(stderr, "\n");
	}

	read_byte(clnt_sock, (void *)&event, sizeof(int)){

	}

/*
	//receive and echo command
	while (read_byte(clnt_sock, (void *)&event, sizeof(int))) 
	//while (read(clnt_sock, (void *)&event, sizeof(int))) 
	{
		printf("move: %d\n", event);
		//send_msg_all((void *)&event, sizeof(int));
		printf("function call: send_msg_all\n");
		//detect end flag
		if(event == max_user*4)
		{
			printf("end game!\n");
			//disconnected(clnt_sock);
			printf("function call: disconnected\n");
		}
	}
*/	
	printf("done: handle_clnt\n");
	return NULL;
}


int main(void){

	char buf[128];
	for(int i = 0; i < 5; i++){
		sprintf(buf, "file/handle_clnt_user%d", i);
		sprintf(user_name[i], "user%d", i);
		if((clnt_socks[i] = open(buf, O_WRONLY|O_CREAT, 0666)) < 0) perror("open");
		printf("%d ", clnt_socks[i]);
	}

	int sock;
	printf("select target socket: ");
	scanf("%d", &sock);
	handle_clnt((void*)&sock);

	

}


