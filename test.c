#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>
#include "cJSON.h"

#define NAME_SIZE 16
#define CELL_SIZE 8
#define MAP_WIDTH 32
#define MAP_HEIGHT 16
#define SCORE_BOARD_WIDTH 64
#define INFO_LABEL_HEIGHT 16
#define BUF_SIZE 128
#define GDK_KEY_UP 65362
#define GDK_KEY_DOWN 65364
#define GDK_KEY_LEFT 65361
#define GDK_KEY_RIGHT 65363

typedef struct location{
    int x;
    int y;
} location_t;

typedef struct user{
    char name[NAME_SIZE];
    int score;
    location_t base_loc;
    location_t user_loc;
}user_t;

typedef struct object_data{
    int map_width;
	int map_height;
    int timeout;
    int max_user;
    struct user * users;
    location_t * item_locations;
    location_t * block_locations;
}object_data_t;

object_data_t Model;

// int map[MAP_WIDTH][MAP_HEIGHT]; 
int ** map; // map malloc 해주기 
char msg_info[BUF_SIZE] = "";
char buf[BUF_SIZE] = "";
int sock;
int my_id;
int num_item;
int num_block;
// char my_username[BUF_SIZE] = "me";
// int my_id = 2;
// char all_usernames[NUM_PLAYER][BUF_SIZE];
// char score[NUM_PLAYER];

enum entity {
	EMPTY = 0,
	BLOCK = -1,
	ITEM = -9, //item will be -10 ~ -110
	USER = 1, //user wil be 1 ~ 3
	BASE = 9, //base will be 10 ~ 30
};
char user_color[8][20] = {"#faa8a1", "#ffe479", "#dbe87c", "#a19b8b", "#ea9574", "#ffca79", "#c79465", "#e3dbcf"};
enum spans {UP, DOWN, LEFT, RIGHT};



int parseJson(char * jsonfile);

//for GUI
GtkWidget *window;
GtkWidget *mat_main, *mat_changed_screen, *mat_board, *label_info, *label_me, *mat_fixed_screen, *mat_screen;
GtkWidget *mat_ans_btn, *mat_sol_btn;
GtkWidget *btn_solve, *btn_exit, *btn_next, *btn_prev;
GtkWidget *btn_auto, *btn_up, *btn_down, *btn_left, *btn_right;
GtkWidget *label_name;
GtkWidget ** label_score; 
GdkPixbuf *icon, *icon_block[2], *icon_fruit[11];
GdkPixbuf ** icon_player;

GdkPixbuf *create_pixbuf(const gchar * filename);
int load_icons();
int check_map_valid();
void set_window();
GtkWidget* create_entity(int id);
static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void display_screen();
void add_mat_board();
void exit_game(GtkWidget* widget);

int check_validation(int cmd);
int move(int cmd, int movement);
void update_model(int id, int x, int y);
void update_cell();
int item_idxToId(int idx);
int item_idToIdx(int id);
void score_up(int user_idx);
void gameover();


int recv_bytes(int sock_fd, void * buf, size_t len);
int send_bytes(int sock_fd, void * buf, size_t len);
void handle_timeout(int signum);
void *recv_msg(void * arg);

// void test_set(){
// 	g_print(" ... test data loading ... \n");

// 	Model.max_user = 4;
// 	Model.user_locations = malloc(sizeof(location_t)*NUM_PLAYER);
// 	Model.base_locations = malloc(sizeof(location_t)*NUM_PLAYER);
// 	for(int i = 0; i < NUM_PLAYER; i++){
// 		score[i] = 0;
// 		strcpy(all_usernames[i], "user");
// 		g_print("score of %s -> %d\n", all_usernames[i], score[i]);

// 	}
// 	strcpy(all_usernames[my_id], my_username);	
// 	Model.user_locations[0].x = 1;
// 	Model.user_locations[0].y = 0;
// 	Model.base_locations[0].x = 0;
// 	Model.base_locations[0].y = 0;

// 	Model.user_locations[1].x = MAP_WIDTH-2;
// 	Model.user_locations[1].y = 0;
// 	Model.base_locations[1].x = MAP_WIDTH-1;
// 	Model.base_locations[1].y = 0;

// 	Model.user_locations[2].x = 1;
// 	Model.user_locations[2].y = MAP_HEIGHT-1;
// 	Model.base_locations[2].x = 0;
// 	Model.base_locations[2].y = MAP_HEIGHT-1;

// 	Model.user_locations[3].x = MAP_WIDTH-2;
// 	Model.user_locations[3].y = MAP_HEIGHT-2;
// 	Model.base_locations[3].x = MAP_WIDTH-1;
// 	Model.base_locations[3].y = MAP_HEIGHT-1;



//     int randx, randy;
// 	for(int i = 0; i < MAP_WIDTH; i++){
// 		for(int j = 0; j < MAP_HEIGHT; j++){
// 			map[i][j] = EMPTY;
// 		}
// 	}

// 	Model.num_block = 50;
// 	Model.block_locations = malloc(sizeof(location_t)*Model.num_block);
// 	for (int i = 0; i < Model.num_block; i++) {
//         randx = rand() % MAP_WIDTH;
//         randy = rand() % MAP_HEIGHT;
// 		Model.block_locations[i].x = randx;
// 		Model.block_locations[i].y = randy;
//     }

// 	Model.num_item = 20;
// 	Model.item_locations = malloc(sizeof(location_t)*Model.num_item);	
// 	for (int i = 0; i < Model.num_item; i++) {
//         randx = rand() % MAP_WIDTH;
//         randy = rand() % MAP_HEIGHT;
// 		Model.item_locations[i].x = randx;
// 		Model.item_locations[i].y = randy;
//     }

// 	update_cell();

// }

int main(int argc, char *argv[]) {

	//get the username from stdin 
	//TODO maybe need change to args

	// signal(SIGALRM, handle_timeout);

    
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	 }
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
	
	while(1){
		printf("enter your name: ");
		if((scanf("%s", buf) != 1) || 0 /*TODO need another checking?*/){
			printf("invalid name. please pick another one.");	
			continue;
		}else break;
	}
	
    //todo errer handle
	int name_size = strlen(buf);
	if(send_bytes(sock, (void *)&name_size, sizeof(int)) == -1)
		return 1;

	if(send_bytes(sock, buf, strlen(buf)) == -1)
		return 1;

    //recv my id
	if (recv_bytes(sock, (void*)&my_id, sizeof(int)) == -1) 
    	return 1;
		
	fprintf(stderr, "id : %d\n", my_id);
	

    // recv json file
    int json_size;
    if (recv_bytes(sock, (void*)&(json_size), sizeof(int)) == -1)
		return (void*)-1;

    char * json_format = malloc(sizeof(char) * json_size);
    if (recv_bytes(sock, json_format, json_size) == -1)
		return (void*)-1;

	parseJson(json_format);
	
	// receive all player's name size, name 
	for (int i = 0; i < Model.max_user; i++) {
		int name_size;
		if (recv_bytes(sock, (void*)&(name_size), sizeof(name_size)) == -1)
			return (void*)-1;

	
		if (recv_bytes(sock, (void*)(Model.users[i].name), name_size) == -1)
			return (void*)-1;

		printf("id : %d name : %s\n",i,Model.users[i].name);
	}

	map = (int **) malloc (sizeof(int *) * Model.map_width);
	for(int i=0;i<Model.map_width;i++){
		map[i] =(int *) malloc(sizeof(int) * Model.map_height);
	} 

	label_score = (GtkWidget **) malloc(Model.max_user* sizeof(GtkWidget *));
	icon_player = (GdkPixbuf **) malloc(Model.max_user * sizeof(GdkPixbuf *));

//-------------------------------
	//load icons from icons dir
	// if(load_icons()) {
	// 	g_print("failed to load icons\n");
	// 	return -1;
  	// }


 	return 0;
}


int parseJson(char * jsonfile) {
    cJSON* root;

	root = cJSON_Parse(jsonfile);
	if (root == NULL) {
		printf("JSON 파싱 오류: %s\n", cJSON_GetErrorPtr());
        	return 1;
    	}
        
	cJSON* timeout = cJSON_GetObjectItem(root, "timeout");
	Model.timeout = timeout->valueint;
	cJSON* max_user = cJSON_GetObjectItem(root, "max_user");
	Model.max_user = max_user->valueint;

	cJSON* map = cJSON_GetObjectItem(root, "map");
	cJSON* map_width = cJSON_GetObjectItem(map, "map_width");
	Model.map_width = map_width->valueint;	
	cJSON* map_height = cJSON_GetObjectItem(map, "map_height");
	Model.map_height = map_height->valueint;

	cJSON* user = cJSON_GetObjectItem(root, "user");
	int num_user = cJSON_GetArraySize(user);
	Model.users = (struct user *)malloc(sizeof(struct user) * Model.max_user);
	for(int i = 0; i < num_user; i++){
		memset(Model.users[i].name, 0, sizeof(NAME_SIZE));
		Model.users[i].score = 0;
		cJSON* user_array = cJSON_GetArrayItem(user,i);
	       	cJSON* base = cJSON_GetObjectItem(user_array,"base"); 
		cJSON* base_x = cJSON_GetArrayItem(base, 0);
		cJSON* base_y = cJSON_GetArrayItem(base, 1);
		Model.users[i].base_loc.x = base_x->valueint;
		Model.users[i].base_loc.y = base_y->valueint;
	#ifdef DEBUG
		printf("name: %s\n",Model.users[i].name);
		printf("base x: %d\n",Model.users[i].base_loc.x);
		printf("base y: %d\n",Model.users[i].base_loc.y);
	#endif
	}
	
	cJSON * item = cJSON_GetObjectItem(root, "item_location");
	num_item = cJSON_GetArraySize(item);
	Model.item_locations = (struct location *)malloc(sizeof(struct location) * num_item); 
	for(int i = 0; i < num_item; i++){
		cJSON* item_array = cJSON_GetArrayItem(item,i);
		cJSON* item_x = cJSON_GetArrayItem(item_array, 0);
		cJSON* item_y = cJSON_GetArrayItem(item_array, 1);
		Model.item_locations[i].x = item_x->valueint;
		Model.item_locations[i].y = item_y->valueint;
	#ifdef DEBUG
		printf("item x: %d\n",Model.item_locations[i].x);
		printf("item y: %d\n",Model.item_locations[i].y);
		#endif
	}	

	cJSON * block = cJSON_GetObjectItem(root, "block_location");
	num_block = cJSON_GetArraySize(block);
	Model.block_locations = (struct location *)malloc(sizeof(struct location) * num_block); 
	for(int i = 0; i < num_block; i++){
		cJSON* block_array = cJSON_GetArrayItem(block,i);
		cJSON* block_x = cJSON_GetArrayItem(block_array, 0);
		cJSON* block_y = cJSON_GetArrayItem(block_array, 1);
		Model.block_locations[i].x = block_x->valueint;
		Model.block_locations[i].y = block_y->valueint;
	#ifdef DEBUG
		printf("block x: %d\n",Model.block_locations[i].x);
		printf("block y: %d\n",Model.block_locations[i].y);
	#endif
	}	
	
	return 0;
}

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

