#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
	
#define BUF_SIZE 100
#define NAME_SIZE 16

typedef struct location{
    int x;
    int y;
} location_t;

typedef struct object_data{
    int map_size;
    int timeout;
    int num_user;
    location_t * base_locations; 
    location_t * user_locations; 
    int num_item;
    location_t * item_locations; 
    int num_block;
    location_t * block_locations;
}object_data_t;
	
char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];
char my_username[BUF_SIZE] = "me";;
int my_id = 0;
object_data_t Model;
char ** all_username;
//new jinju's 
char buf[BUF_SIZE] = "";
char * score;
char msg_info[BUF_SIZE] = "";

enum entity {
	EMPTY = 0,
	BLOCK = -1,
	ITEM = -9, //item will be -10 ~ -110
	USER = 1, //user wil be 1 ~ 3
	BASE = 9, //base will be 10 ~ 30
};
char user_color[8][20] = {"#faa8a1", "#ffe479", "#dbe87c", "#dbe87c", "#dbe87c", "#dbe87c", "#c59365", "#f3ebde"};
enum spans {UP, DOWN, LEFT, RIGHT};

int recv_bytes(int sock_fd, void * buf, size_t len){
    char * p = (char *)buf;
    size_t acc = 0;

    while(acc < len)
    {
        size_t recved;
        recved = recv(sock_fd, p, len - acc, 0);
        if(recved  == -1 || recved == 0){
            return -1;
        }
        p+= recved ;
        acc += recved ;
    }
    return 0;
}

int send_bytes(int sock_fd, void * buf, size_t len){
    char * p = (char *) buf;
    size_t acc = 0;

    while(acc < len)
    {
        size_t sent;
        sent = send(sock_fd, p, len - acc, 0);
        if(sent == -1 || sent == 0){
            return -1;
        }

        p+= sent;
        acc += sent;
    }
    return 0;
}

int check_validation(int cmd){
    int user_idx = cmd/4;
    int span = cmd%4;

    int curr_x, curr_y, target_x, target_y, item_target_x, item_target_y;
    curr_x = target_x = item_target_x = Model.user_locations[user_idx].x;
    curr_y = target_y = item_target_y = Model.user_locations[user_idx].y;

    switch(span){
        case UP:
            if((target_y = (curr_y - 1)) < 0) return 1;//out of array
            if(map[target_x][target_y] == EMPTY) return 0; //empty
            else if(map[target_x][target_y] == ITEM){
                if((item_target_y = (target_y - 1)) < 0) return 1; //item and non-movabel
                if(map[item_target_x][item_target_y] == EMPTY) return 0; //item and movable
                else return 1;  //others (block, user, base)
            }else return 1;
            break;

        case DOWN:
            if((target_y = (curr_y + 1)) > MAP_LENGTH) return 1;//out of array
            if(map[target_x][target_y] == EMPTY) return 0; //empty
            else if(map[target_x][target_y] == ITEM){
                if((item_target_y = (target_y + 1)) > MAP_LENGTH) return 1; //item and non-movabel
                if(map[item_target_x][item_target_y] == EMPTY) return 0; //item and movable
                else return 1;  //others (block, user, base)
            }else return 1;
            break;


        case LEFT:
            if((target_x = (curr_x - 1)) < 0) return 1;//out of array
            if(map[target_x][target_y] == EMPTY) return 0; //empty
            else if(map[target_x][target_y] == ITEM){
                if((item_target_x = (target_x - 1)) < 0) return 1; //item and non-movabel
                if(map[item_target_x][item_target_y] == EMPTY) return 0; //item and movable
                else return 1;  //others (block, user, base)
            }else return 1;
            break;
        case RIGHT:
            if((target_x = (curr_x + 1)) > MAP_WIDTH) return 1;//out of array
            if(map[target_x][target_y] == EMPTY) return 0; //empty
            else if(map[target_x][target_y] == ITEM){
                if((item_target_x = (target_x + 1)) > MAP_WIDTH) return 1; //item and non-movabel
                if(map[item_target_x][item_target_y] == EMPTY) return 0; //item and movable
                else return 1;  //others (block, user, base)
            }else return 1;
            break;
    }

}

void move(int cmd){
	int user_idx = cmd/4;
	int span = cmd%4;	
	
	int curr_x, curr_y, target_x, target_y, item_target_x, item_target_y;
	curr_x = target_x = item_target_x = Model.user_locations[user_idx].x;
	curr_y = target_y = item_target_y = Model.user_locations[user_idx].y;

	switch(span){
		case UP:		
			target_y = curr_y - 1;
			item_target_y = target_y - 1;
			break;	
		case DOWN:
			target_y = curr_y + 1;
			item_target_y = target_y + 1;
			break;	
		case LEFT:
			target_x = curr_x - 1;
			item_target_x = target_x - 1;
			break;	
		case RIGHT:
			target_x = curr_x + 1;
			item_target_x = target_x + 1;
			break;	
	}

	int tmp;	
	if(map[target_x][target_y] == EMPTY){ //empty
//		swap(curr_x, curr_y, target_x, target_y);
		tmp = map[curr_x][curr_y];
		map[curr_x][curr_y] = map[target_x][target_y];
		map[target_x][target_y] = tmp;
	}else if(map[target_x][target_y] < ITEM){ //item
//		swap(target_x, target_y, item_target_x, item_target_y);
//		swap(curr_x, curr_y, target_x, target_y);
		if(map[item_target_x][item_target_y] > BASE){
			map[target_x][target_y] = EMPTY;
			score[user_idx] ++;
			//TODO label_update
			sprintf(msg_info, "%s got the score!", all_usernames[user_idx]);
			gtk_label_set_text((GtkLabel*)label_info, msg_info);
			sprintf(msg_info, "%s: %d", all_usernames[user_idx], score[user_idx]);
			gtk_label_set_text((GtkLabel*)label_score[user_idx], msg_info);
			
		}else{
			tmp = map[target_x][target_y];
			map[target_x][target_y] = map[item_target_x][item_target_y];
			map[item_target_x][item_target_y] = tmp;
		}
		tmp = map[curr_x][curr_y];
		map[curr_x][curr_y] = map[target_x][target_y];
		map[target_x][target_y] = tmp;

	}

	Model.user_locations[user_idx].x = target_x;
	Model.user_locations[user_idx].y = target_y;

	display_screen();
	g_print("moved~\n");
}

void *recv_msg(void * arg)   // read thread main
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	int str_len;

    //recv my id
	if (recv_bytes(sock, (void*)&my_id, sizeof(int)) == -1) 
    return (void*)-1;

	// receive map 
	if (recv_bytes(sock, (void*)&(Model.map_size), sizeof(int)) == -1)
		return (void*)-1;

	if (recv_bytes(sock, (void*)&(Model.timeout), sizeof(int)) == -1)
		return (void*)-1;

	if (recv_bytes(sock, (void*)&(Model.num_user), sizeof(int)) == -1)
		return (void*)-1;

	Model.base_locations = malloc (sizeof(location_t) * Model.num_user);

	for (int i = 0; i < Model.num_user; i++) {
		if (recv_bytes(sock, (void*)&(Model.base_locations[i]), sizeof(location_t)) == -1)
			return (void*)-1;
	}

	Model.user_locations = malloc (sizeof(location_t) * Model.num_user);

	for (int i = 0; i < Model.num_user; i++) {
		if (recv_bytes(sock, (void*)&(Model.user_locations[i]), sizeof(location_t)) == -1)
			return (void*)-1;
	}

	if (recv_bytes(sock, (void*)&(Model.num_item), sizeof(int)) == -1)
		return (void*)-1;

	Model.item_locations = malloc (sizeof(location_t) * Model.num_item);

	for (int i = 0; i < Model.num_item; i++) {
		if (recv_bytes(sock, (void*)&(Model.item_locations[i]), sizeof(location_t)) == -1)
			return (void*)-1;
	}

	if (recv_bytes(sock, (void*)&(Model.num_block), sizeof(int)) == -1)
		return (void*)-1;

	Model.block_locations = malloc (sizeof(location_t) * Model.num_block);

	for (int i = 0; i < Model.num_block; i++) {
		if (recv_bytes(sock, (void*)&(Model.block_locations[i]), sizeof(location_t)) == -1)
			return (void*)-1;
	}

	// receive all player's id, name 
	all_username = malloc(sizeof(char**) * Model.num_user);

	for (int i = 0; i < Model.num_user; i++) {
		int name_size;
		if (recv_bytes(sock, (void*)&(name_size), sizeof(name_size)) == -1)
			return (void*)-1;

		all_username[i] = malloc(sizeof(char * ) * (name_size + 1)); //여기서 error 발생 

		if (recv_bytes(sock, (void*)(all_username[i]), name_size) == -1)
			return (void*)-1;

		all_username[i][name_size] = '\0';
	}

	//now enter new move 
	int recv_move;
	while(1){
		if(recv_bytes(sock, (void *)&recv_move, sizeof(server_echo)) == -1)
			return (void*)-1;

        //move
        move(recv_move);
	}
	return NULL;
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {

    const gchar *greeting = NULL;
    int cmd = my_id;
    switch (event->keyval) {
        case GDK_KEY_UP:
            greeting = "up key pressed...";
            cmd += 0;
            break;
        case GDK_KEY_DOWN:
            greeting = "down key pressed...";
            cmd += 1;
            break;
        case GDK_KEY_LEFT:
            greeting = "left key pressed...";
            cmd += 2;
            break;
        case GDK_KEY_RIGHT:
            greeting = "right key pressed...";
            cmd += 3;
            break;
    }
    g_print("%s\n", greeting);
    //TODO temporary... need to move this to recv function
    if(check_validation(cmd)) 
        g_print("invalid movement!\n");
	else {
        if(send_bytes(sock, (void*)&cmd, sizeof(cmd)) == -1)
		    return TRUE;
    }


    return TRUE;
}


int main(int argc, char *argv[])
{

	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	 }
	
	sprintf(name, "[%s]", argv[3]);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	  
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		fprintf(stderr, "ERROR: connect() error\n");
		exit(1);
	}
	
	// pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	// pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);

	while(1){
		printf("enter your name: ");
		if((scanf("%s", my_name) != 1) || 0 /*TODO need another checking?*/){
			printf("invalid name. please pick another one.");	
			continue;
		}else break;
	}

	int name_size = strlen(my_name);
	if(send_bytes(sock, (void *)&name_size, sizeof(int)) == -1)
		return 1;

	if(send_bytes(sock, my_name, strlen(my_name)) == -1)
		return 1;


	close(sock);  
	return 0;
}
	

