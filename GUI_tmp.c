#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#define NUM_PLAYER 4
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


int map[MAP_WIDTH][MAP_HEIGHT]; 
char msg_info[BUF_SIZE] = "";
char buf[BUF_SIZE] = "";
char my_username[BUF_SIZE] = "me";
int my_id = 0;
char all_usernames[NUM_PLAYER][BUF_SIZE];
char score[NUM_PLAYER];
void load_game_info();
enum entity {
	EMPTY = 0,
	BLOCK = -1,
	ITEM = -9, //item will be -10 ~ -110
	USER = 1, //user wil be 1 ~ 3
	BASE = 9, //base will be 10 ~ 30
};
char user_color[8][20] = {"#faa8a1", "#ffe479", "#dbe87c", "#a19b8b", "#ea9574", "#ffca79", "#c79465", "#e3dbcf"};
enum spans {UP, DOWN, LEFT, RIGHT};

typedef struct location{
    int x;
    int y;
} location_t;

typedef struct object_data{
    int map_size;
    int timeout;
    int num_user;
    location_t * base_loactions; 
    location_t * user_locations; 
    int num_item;
    location_t * item_locations; 
    int num_block;
    location_t * block_locations;
}object_data_t;
object_data_t Model;

//for GUI
GtkWidget *window;
GtkWidget *mat_main, *mat_changed_screen, *mat_board, *label_info, *label_me, *mat_fixed_screen, *mat_screen;
GtkWidget *mat_ans_btn, *mat_sol_btn;
GtkWidget *btn_solve, *btn_exit, *btn_next, *btn_prev;
GtkWidget *btn_auto, *btn_up, *btn_down, *btn_left, *btn_right;
GtkWidget *label_name, *label_score[NUM_PLAYER]; 
GdkPixbuf *icon, *icon_block[2], *icon_fruit[11], *icon_player[NUM_PLAYER]; 
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
void move(int cmd);

//for testing...
void test_set(){
	g_print(" ... test data loading ... \n");

	for(int i = 0; i < NUM_PLAYER; i++){
		score[i] = 0;
		strcpy(all_usernames[i], "user");
		g_print("score of %s -> %d\n", all_usernames[i], score[i]);
	}

	
	for(int i = 0; i < MAP_WIDTH; i++){
		for(int j = 0; j < MAP_HEIGHT; j++){
			map[i][j] = EMPTY;
		}
	}

    int randx, randy;
	for (int i = 0; i < 50; i++) {
        randx = rand() % MAP_WIDTH;
        randy = rand() % MAP_HEIGHT;
		map[randx][randy] = BLOCK;
    }
	for (int i = 0; i < 20; i++) {
        randx = rand() % MAP_WIDTH;
        randy = rand() % MAP_HEIGHT;
		int item_id = rand() % 12;
		item_id = (0 - item_id)*10;
		map[randx][randy] = item_id;
    }

	map[1][0] = my_id+1;
	map[0][0] = (my_id+1)*10;
	Model.user_locations = malloc(sizeof(location_t)*NUM_PLAYER);
	Model.user_locations[my_id].x = 1;
	Model.user_locations[my_id].y = 0;
	strcpy(all_usernames[my_id], my_username);	


/*
typedef struct object_data{
    int map_size; TODO
    int timeout; TODO
    int num_user;
    location_t * base_loactions; 
    location_t * user_locations; 
    int num_item;
    location_t * item_locations; 
    int num_block;
    location_t * block_locations;
}object_data_t;
object_data_t Model;
*/


}

int main(int argc, char *argv[]) {

	//get the username from stdin 
	//TODO maybe need change to args
	while(1){
		printf("enter your name: ");
		if((scanf("%s", my_username) != 1) || 0 /*TODO need another checking?*/){
			printf("invalid name. please pick another one.");	
			continue;
		}else break;
	}

	//load icons from icons dir
	if(load_icons()) {
		g_print("failed to load icons\n");
		return -1;
  	}

	//set the testing data 
	// TODO can be customize
	test_set();
	srand((unsigned int)time(0));

	//set the GUI
	gtk_init(&argc, &argv); //init GTK by args
	 //key pressed
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press), NULL);
	set_window();

 	return 0;
}


//get pixbuf(for load image) from filename
GdkPixbuf *create_pixbuf(const gchar * filename) {
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if (!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }
   return pixbuf;
}


//load icons needed
//0 on success, 1 on failure
int load_icons(){
   	GdkPixbuf *pixbuf;
	for(int i = 0; i < NUM_PLAYER; i++){
		sprintf(buf, "icons/user%d.png", i);
		if((pixbuf = create_pixbuf(buf)) == NULL) return 1;
		else g_print("loading %s...\n", buf);
		icon_player[i] = gdk_pixbuf_scale_simple(pixbuf, 32, 32, GDK_INTERP_BILINEAR);
		g_object_unref(pixbuf);
	}
	for(int i = 0; i < 2; i++){
		sprintf(buf, "icons/block%d.png", i);
		if((pixbuf = create_pixbuf(buf)) == NULL) return 1;
		else g_print("loading %s...\n", buf);
		icon_block[i] = gdk_pixbuf_scale_simple(pixbuf, 32, 32, GDK_INTERP_BILINEAR);
		g_object_unref(pixbuf);
	}
	for(int i = 0; i < 12; i++){
		sprintf(buf, "icons/fruit%d.png", i);
		if((pixbuf = create_pixbuf(buf)) == NULL) return 1;
		else g_print("loading %s...\n", buf);
		icon_fruit[i] = gdk_pixbuf_scale_simple(pixbuf, 32, 32, GDK_INTERP_BILINEAR);
		g_object_unref(pixbuf);
	}

	g_print("success to load all icons!\n");
	return 0;

}


//load game information from server and save map[]
//TODO replace test to this one when done
void load_game_info(){





}


//GUI: set the main window
void set_window(){

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);//make window
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);//for termination
  g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press), NULL);
 
  //set the window
  gtk_window_set_title(GTK_WINDOW(window), "pushpush HK");
  gtk_window_set_default_size(GTK_WINDOW(window), 1024, 512);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);

  //change the icon of page(for cuteness)
  icon = create_pixbuf("icons/catIcon.png");
  gtk_window_set_icon(GTK_WINDOW(window), icon);

  
  //set main matrix
  mat_main = gtk_table_new(8, 8-1, TRUE);
	
  strcpy(msg_info, "Welcome to PushPush HK!");
  label_info = gtk_label_new(msg_info);
  gtk_table_attach_defaults(GTK_TABLE(mat_main), label_info, 0, 11, 0, 1);
  
  display_screen();
  add_mat_board();
 
  label_me = gtk_label_new("23-winter capston study#2 leeejjju");
  gtk_misc_set_alignment(GTK_MISC(label_me), 0.0, 1.0);
  gtk_table_attach_defaults(GTK_TABLE(mat_main), label_me, 0, 8+1, 6, 8);

  gtk_container_add(GTK_CONTAINER(window), mat_main);
  gtk_widget_show_all(window); //is it dup with above
  g_object_unref(icon);
  gtk_main(); //enter the GTK main loop

}



//create entity Widget from ids, return widget* or NULL on id-empty
GtkWidget* create_entity(int id){

	GtkWidget* sprite;
	GdkColor color;
	int idx;

	if(id == EMPTY) return NULL;
	else if(id == BLOCK){
		idx = rand() % 2;
      	sprite = gtk_image_new_from_pixbuf(icon_block[idx]); 
	}else if(id < ITEM){
		idx = (0-id)/10-1;
      	sprite = gtk_image_new_from_pixbuf(icon_fruit[idx]); 
	}else if(id > BASE){
		idx = id/10 -1;
		sprite = gtk_event_box_new();
		//gtk_widget_set_size_request(sprite, 32, 32);		
		gdk_color_parse(user_color[idx], &color);
		gtk_widget_modify_bg(sprite, GTK_STATE_NORMAL, &color);
	}else{
		idx = id-1;
      	sprite = gtk_image_new_from_pixbuf(icon_player[idx]); 
	}
	return sprite;

}

//GUI: display screen from map[] model
//TODO need to be updated to display by map[]
void display_screen(){

  //set screen matrix
  if(mat_changed_screen == NULL){ //initially once
	mat_screen = gtk_fixed_new();
	mat_changed_screen = gtk_table_new(MAP_WIDTH, MAP_HEIGHT, TRUE);
	mat_fixed_screen = gtk_table_new(MAP_WIDTH, MAP_HEIGHT, TRUE);
    for (int i = 0; i < MAP_WIDTH; i++) {
      for (int j = 0; j < MAP_HEIGHT; j++) {
		if(map[i][j] != BLOCK) continue;
		GtkWidget* sprite = create_entity(map[i][j]);
		if(sprite != NULL) gtk_table_attach_defaults(GTK_TABLE(mat_fixed_screen), sprite, i, i+1, j, j+1);
		}
    }
	gtk_fixed_put(GTK_FIXED(mat_screen), mat_changed_screen, 0, 0);
	gtk_fixed_put(GTK_FIXED(mat_screen), mat_fixed_screen, 0, 0);
  }else gtk_container_foreach(GTK_CONTAINER(mat_changed_screen), (GtkCallback)gtk_widget_destroy, NULL); 

  for (int i = 0; i < MAP_WIDTH; i++) {
    for (int j = 0; j < MAP_HEIGHT; j++) {
		if(map[i][j] == BLOCK) continue;
		GtkWidget* sprite = create_entity(map[i][j]);
		if(sprite != NULL) gtk_table_attach_defaults(GTK_TABLE(mat_changed_screen), sprite, i, i+1, j, j+1);
    }
  }

  if(!gtk_widget_get_parent(mat_screen)) gtk_table_attach_defaults(GTK_TABLE(mat_main), mat_screen, 0, 9, 1, 10);
  gtk_widget_show_all(window); 

}

//GUI: set the score board
//TODO need to be updated from score received from server
void add_mat_board(){

  //set board vbox
  int board_width = 8;
  mat_board = gtk_table_new(board_width, 10, TRUE);

  GtkWidget* line1 = gtk_hseparator_new();
  sprintf(buf, "Good luck, %s!", my_username);
  label_name = gtk_label_new(buf);
  GtkWidget* sprite = gtk_image_new_from_pixbuf(icon_player[my_id]);
  GtkWidget* line2 = gtk_hseparator_new();
  GtkWidget *label_title = gtk_label_new(":: SCORE ::");
  
  gtk_table_attach_defaults(GTK_TABLE(mat_board), line1, 0, board_width+1, 0, 1);
  gtk_table_attach_defaults(GTK_TABLE(mat_board), label_name, 0, board_width+1, 1, 2);
  gtk_table_attach_defaults(GTK_TABLE(mat_board), line2, 0, board_width+1, 2, 3);
  gtk_table_attach_defaults(GTK_TABLE(mat_board), sprite, 0, board_width+1, 3, 4);
  gtk_table_attach_defaults(GTK_TABLE(mat_board), label_title, 0, board_width+1, 4, 5);
	
  GtkWidget* score_board = gtk_vbox_new(TRUE, 10);
  for(int i = 0; i < NUM_PLAYER; i++){
	//TODO this part display the score (global array)
	sprintf(msg_info, "%s: %d", all_usernames[i], score[i]);		
	label_score[i] = gtk_label_new(msg_info);
	gtk_container_add(GTK_CONTAINER(score_board), label_score[i]);
  } 
  gtk_container_add(GTK_CONTAINER(score_board), gtk_label_new(""));
  gtk_table_attach_defaults(GTK_TABLE(mat_board), score_board, 0, board_width+1, 5, 10);

  btn_exit = gtk_button_new_with_label("exit game");
  gtk_table_attach_defaults(GTK_TABLE(mat_board), btn_exit, 0, board_width+1, 10, 11);
  g_signal_connect(G_OBJECT(btn_exit), "clicked", G_CALLBACK(exit_game), NULL);
  gtk_table_attach_defaults(GTK_TABLE(mat_main), mat_board, 9, 11, 0, 8); 

}

void exit_game(GtkWidget* widget){
	printf("See you again!\n");
	exit(EXIT_SUCCESS);
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
			else if(map[target_x][target_y] < ITEM){ 
				if((item_target_y = (target_y - 1)) < 0) return 1; //item and non-movabel
				if(map[item_target_x][item_target_y] == EMPTY) return 0; //item and movable
				if((map[item_target_x][item_target_y] > BASE) && ((map[item_target_x][item_target_y]/10-1) == user_idx)) return 0;
				else return 1;	//others (block, user, base)
			}else return 1;	
			break;

		case DOWN:
			if((target_y = (curr_y + 1)) > MAP_HEIGHT) return 1;//out of array
			if(map[target_x][target_y] == EMPTY) return 0; //empty
			else if(map[target_x][target_y] < ITEM){ 
				if((item_target_y = (target_y + 1)) > MAP_HEIGHT) return 1; //item and non-movabel
				if(map[item_target_x][item_target_y] == EMPTY) return 0; //item and movable
				if((map[item_target_x][item_target_y] > BASE) && ((map[item_target_x][item_target_y]/10-1) == user_idx)) return 0;
				else return 1;	//others (block, user, base)
			}else return 1;	
			break;


		case LEFT:
			if((target_x = (curr_x - 1)) < 0) return 1;//out of array
			if(map[target_x][target_y] == EMPTY) return 0; //empty
			else if(map[target_x][target_y] < ITEM){ 
				if((item_target_x = (target_x - 1)) < 0) return 1; //item and non-movabel
				if(map[item_target_x][item_target_y] == EMPTY) return 0; //item and movable
				if((map[item_target_x][item_target_y] > BASE) && ((map[item_target_x][item_target_y]/10-1) == user_idx)) return 0;
				else return 1;	//others (block, user, base)
			}else return 1;	
			break;

		case RIGHT:
			if((target_x = (curr_x + 1)) > MAP_WIDTH) return 1;//out of array
			if(map[target_x][target_y] == EMPTY) return 0; //empty
			else if(map[target_x][target_y] < ITEM){ 
				if((item_target_x = (target_x + 1)) > MAP_WIDTH) return 1; //item and non-movabel
				if(map[item_target_x][item_target_y] == EMPTY) return 0; //item and movable
				if((map[item_target_x][item_target_y] > BASE) && ((map[item_target_x][item_target_y]/10-1) == user_idx)) return 0;
				else return 1;	//others (block, user, base)
			}else return 1;	
			break;

	}


}

//update cells by cmd(0-15), 
//return 0 on validation of moving, return 1 on invalid moving
//TODO 
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
		tmp = map[curr_x][curr_y];
		map[curr_x][curr_y] = map[target_x][target_y];
		map[target_x][target_y] = tmp;
	}else if(map[target_x][target_y] < ITEM){ //item
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
	//move(cmd);
	if(check_validation(cmd)) g_print("invalid movement!\n");
	else move(cmd);

    return TRUE;
}



