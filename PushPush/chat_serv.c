#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 256

typedef struct {
	int x;
	int y;
}location;

typedef struct {
	int map_size;
	int timeout;
	int num_user;
	location * base_loactions; 
	location * user_locations; 
	int num_item;
	location * item_locations; 
	int num_block;
	location * block_locations;
}object_data;

object_data game_data;

int clnt_cnt = 0;
int clnt_socks[4];
pthread_mutex_t mutx;



void disconnected(int sock)
{
	pthread_mutex_lock(&mutx);
	for (int i = 0; i < clnt_cnt; i++)   // remove disconnected client
	{
		if (sock == clnt_socks[i])
		{
			while (i < clnt_cnt-1)
			{
				clnt_socks[i] = clnt_socks[i+1];
				i++;
			}
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(sock);
}

// yeon
int recv_bytes(int sock_fd, void * buf, size_t len){
    char * p = (char *)buf;
    size_t acc = 0;

    while(acc < len)
    {
        size_t recved;
        recved = recv(sock_fd, p, len - acc, 0);
        if(recved  == -1 || recved == 0){
            return 1;
        }
        p+= recved ;
        acc += recved ;
    }
    return 0;
}

int send_byte(int sock_fd, void * buf, size_t len){
    char * p = (char *) buf;
    size_t acc = 0;

    while(acc < len)
    {
        size_t sent;
        sent = send(sock_fd, p, len - acc, 0);
        if(sent == -1 || sent == 0){
            return 1;
        }

        p+= sent;
        acc += sent;
    }
    return 0;
}

//yeon 

int read_byte(int sock, void * buf, int size)
{
	int read_size = 0;
	int str_len = 0;
	while(read_size < size)
	{
		str_len = read(sock, buf + read_size, size - read_size);
		if( str_len == 0)
		{
			disconnected(sock);
			return 0;
		}
		if( str_len == -1)
		{

		}
		read_size += str_len;
	}
	return read_size;
}//상훈님 파이팅 

void send_msg_all(char * msg, int len)   // send to all
{
	pthread_mutex_lock(&mutx);
	for (int i = 0; i < clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}

void *handle_clnt(void * arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0;
	int i;
	char msg[BUF_SIZE];
	
	int name_size = 0;
	str_len = read_byte(clnt_sock, (void *)&name_size, sizeof(int));

	char user_name[20]; //TODO 나중에 글로벌 변수에 ㄸ따로 저장해줘야해요
	str_len = read_byte(clnt_sock, (void *)&user_name, name_size);


	pthread_mutex_lock(&mutx);
	for (int i = 0; i < clnt_cnt; i++)   // remove disconnected client
	{
		if (clnt_sock == clnt_socks[i])
		{
			// printf("id : %d\n", i);
			write(clnt_sock, (void *)&i, sizeof(int));
			break;
		}
	}
	pthread_mutex_unlock(&mutx);
	
	

	
	while ((str_len = read_byte(clnt_sock, &msg, sizeof(msg))) != 0)
		send_msg_all(msg, str_len);
	
	
	return NULL;
}



void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
  
	pthread_mutex_init(&mutx, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; 
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	
	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		
		pthread_mutex_lock(&mutx); //
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);
		
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
		// if(clnt_cnt == game_data.num_user)
		// {
		// 	while(1);
		// }
		
	}
	close(serv_sock);
	return 0;
}
	