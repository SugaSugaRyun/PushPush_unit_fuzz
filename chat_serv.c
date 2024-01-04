#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <pthread.h>
#include "cJSON.h"

#define BUF_SIZE 256
#define MAX_USER 4

typedef struct {
	int x;
	int y;
}location_t;

typedef struct {
	int map_width;
	int map_height;
	int timeout;
	int num_user;
	char ** user_name;
	location_t * base_locations; 
	location_t * user_locations; 
	int num_item;
	location_t * item_locations; 
	int num_block;
	location_t * block_locations;
}object_data_t;

int arraySize;
cJSON* root;
object_data_t *Object;

int clnt_cnt = 0;
int clnt_socks[MAX_USER];
pthread_mutex_t mutx;



void parseEntries(cJSON* entriesArray, location_t* entries, int* numEntries)
{
	if (entriesArray == NULL || !cJSON_IsArray(entriesArray)) {
			entries = NULL;
			*numEntries = 0;
			return;
	}

	int element_cnt = *numEntries;

	for (int i = 0; i < element_cnt; i++) {
		cJSON* entry = cJSON_GetArrayItem(entriesArray, i);
		cJSON* inode = cJSON_GetObjectItem(entry, "x");
		entries[i].x = inode->valueint;
		inode = cJSON_GetObjectItem(entry, "y");
		entries[i].y = inode->valueint;

	}
}

int parseJson() 
{
	char filepath[256] = "file.json";
	FILE *file = fopen(filepath,"r");
	if(file == NULL){
		fprintf(stderr,"ERROR: open file");
		return 1;
	}
	struct stat st;
  if(stat(filepath, &st) == -1){
  	fprintf(stderr,"ERROR: stat()\n");
  	return 1;
  }
  int size = st.st_size;

	char* jsonfile = (char*)malloc(size+1);
	if(jsonfile == NULL){
		fprintf(stderr,"ERROR: memory allocation\n");
		return 1;
	}

	int read_size = fread(jsonfile, 1, size, file);
	if(read_size != size){
		fprintf(stderr, "ERROR: read file\n");
		return 1;
	}

	fclose(file);
	jsonfile[size] = '\0';
	
	root = cJSON_Parse(jsonfile);
	if (root == NULL) {
			printf("JSON 파싱 오류: %s\n", cJSON_GetErrorPtr());
      return 1;
  }
	if(cJSON_IsArray(root)){
		Object = (object_data_t *)malloc(sizeof(object_data_t));
		arraySize = cJSON_GetArraySize(root);

		for (int i = 0; i < arraySize; i++) {
			cJSON* item = cJSON_GetArrayItem(root, i);
			if(item != NULL){
        cJSON* inode = cJSON_GetObjectItem(item, "map_width");
				Object->map_width = inode->valueint;
				inode = cJSON_GetObjectItem(item, "map_height");
				Object->map_height = inode->valueint;
				inode = cJSON_GetObjectItem(item, "timeout");
				Object->timeout = inode->valueint;
				inode = cJSON_GetObjectItem(item, "num_user");
				Object->num_user = inode->valueint;
				//printf("%d %d %d\n",Object->map_size, Object->timeout, Object->num_user);
				cJSON* entries = cJSON_GetObjectItem(item, "base");
				Object->base_locations = (location_t *)malloc(sizeof(location_t)*Object->num_user);
				parseEntries(entries, Object->base_locations, &(Object->num_user));
				
				entries = cJSON_GetObjectItem(item, "user_location");
				Object->user_locations = (location_t *)malloc(sizeof(location_t)*Object->num_user);
				parseEntries(entries, Object->user_locations, &(Object->num_user));

				inode = cJSON_GetObjectItem(item, "num_item");
				Object->num_item = inode->valueint;

				entries = cJSON_GetObjectItem(item, "item_location");
				Object->item_locations = (location_t *)malloc(sizeof(location_t)*Object->num_item);
				parseEntries(entries, Object->item_locations, &(Object->num_item));
				
				inode = cJSON_GetObjectItem(item, "num_block");
				Object->num_block = inode->valueint;

				entries = cJSON_GetObjectItem(item, "block_location");
				Object->block_locations = (location_t *)malloc(sizeof(location_t)*Object->num_block);
				parseEntries(entries, Object->block_locations, &(Object->num_block));
			}

    }
  }
	return 0;
}

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


int write_byte(int sock, void * buf, int size){

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

		}
		write_size += str_len;
	}
	return write_size;
}

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

void send_msg_all(void * event, int len)   // send to all
{
	pthread_mutex_lock(&mutx);
	for (int i = 0; i < clnt_cnt; i++)
		write_byte(clnt_socks[i], (void *)event, len);
	pthread_mutex_unlock(&mutx);
}

void *handle_clnt(void * arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0;
	int event;
	int name_size = 0;
	str_len = read_byte(clnt_sock, (void *)&name_size, sizeof(int));

	//reciev name
	char username[20];
	read_byte(clnt_sock, (void *)username, name_size);

	//send id
	pthread_mutex_lock(&mutx);
	for (int i = 0; i < clnt_cnt; i++) 
	{
		if (clnt_sock == clnt_socks[i])
		{
			// printf("id : %d\n", i);
			// read_byte(clnt_sock, (void *)&Object->user_name[i], name_size);
			write_byte(clnt_sock, (void *)&i, sizeof(int));
			break;
		}
	}
	pthread_mutex_unlock(&mutx);
	
	//send data
	write_byte(clnt_sock, (void *)&Object->map_width, sizeof(int));
	write_byte(clnt_sock, (void *)&Object->map_height, sizeof(int));
	write_byte(clnt_sock, (void *)&Object->timeout, sizeof(int));
	write_byte(clnt_sock, (void *)&Object->num_user, sizeof(int));
	for(int i =0; i< Object->num_user; i++)
	{
		write_byte(clnt_sock, (void *)&Object->base_locations[i], sizeof(location_t));
	}
	for(int i =0; i< Object->num_user; i++)
	{
		write_byte(clnt_sock, (void *)&Object->user_locations[i], sizeof(location_t));
	}	
	write_byte(clnt_sock, (void *)&Object->num_item, sizeof(int));
	for(int i =0; i< Object->num_item; i++)
	{
		write_byte(clnt_sock, (void *)&Object->item_locations[i], sizeof(location_t));
	}
	write_byte(clnt_sock, (void *)&Object->num_block, sizeof(int));
	for(int i =0; i< Object->num_item; i++)
	{
		write_byte(clnt_sock, (void *)&Object->block_locations[i], sizeof(location_t));
	}		

	//waiting until all user enter
	// while(clnt_cnt < Object->num_user);

	//send all name 
	// for(int i=0; i<Object->num_user; i++)
	// {
	// 	int name_len = strlen(Object->user_name[i]);
	// 	send_msg_all((void*)&name_len, sizeof(int));
	// 	send_msg_all(&Object->num_user[i], name_len);
	// }
	//event echo
	while (read_byte(clnt_sock, (void *)&event, sizeof(int))) 
	{
		printf("move: %d\n", event);
		send_msg_all(&event, sizeof(int));
	}
	
	
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
	unsigned int clnt_adr_sz;
	pthread_t t_id;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	if( parseJson())
		exit(1);

	// Object->user_name = malloc((char*)Object->num_user);

	// for(int i=0; i<Object->num_user; i++)
	// {
	// 	Object->user_name[i] = malloc((char) * 16);
	// }

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
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		
		pthread_mutex_lock(&mutx); //
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);
		
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
		// if(clnt_cnt == Object->.num_user)
		// {
		// 	while(1);
		// }
		
	}
	free(Object);
	close(serv_sock);
	return 0;
}
	