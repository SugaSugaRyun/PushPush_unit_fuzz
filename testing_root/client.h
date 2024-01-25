#ifndef GAME_H
#define GAME_H

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
#include "../cJSON.h"

#define NAME_SIZE 16
#define queue_size 20
#define BUF_SIZE 128
#define GDK_KEY_UP 65362
#define GDK_KEY_DOWN 65364
#define GDK_KEY_LEFT 65361
#define GDK_KEY_RIGHT 65363

// object data structures
typedef struct location {
    int x;
    int y;
} location_t;

typedef struct user {
    char name[NAME_SIZE];
    int score;
    location_t base_loc;
    location_t user_loc;
} user_t;

typedef struct object_data {
    int map_width;
    int map_height;
    int timeout;
    int max_user;
    struct user *users;
    location_t *item_locations;
    location_t *block_locations;
} object_data_t;

enum entity {
    EMPTY = 0,
    BLOCK = -1,
    ITEM = -9, // item will be -10 ~ -110
    USER = 1,  // user wil be 1 ~ 3
    BASE = 9,  // base will be 10 ~ 30
};

enum spans {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

extern char user_color[8][20];
extern int **map; // cell
extern object_data_t Model; // model
extern char msg_info[BUF_SIZE];
extern char buf[BUF_SIZE];
extern int sock;
extern int my_id;
extern int num_item, num_block;
extern int current_num_item;

extern pthread_mutex_t mutx;
extern pthread_cond_t cond;
extern int event_arry[queue_size];
extern int rear;
extern int front;

// GUI functions
extern GtkWidget *window;
extern GtkWidget *mat_main, *mat_changed_screen, *mat_board, *label_info, *label_me, *mat_fixed_screen, *mat_screen;
extern GtkWidget *mat_ans_btn, *mat_sol_btn;
extern GtkWidget *btn_solve, *btn_exit, *btn_next, *btn_prev;
extern GtkWidget *btn_auto, *btn_up, *btn_down, *btn_left, *btn_right;
extern GtkWidget *label_name;
extern GtkWidget **label_score; 
extern GdkPixbuf *icon, *icon_block[2], *icon_fruit[11];
extern GdkPixbuf **icon_player;
extern GdkPixbuf *create_pixbuf(const gchar *filename);
extern GtkWidget *create_entity(int id);
extern int load_icons();
extern int check_map_valid();
extern void set_window();
extern gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
extern void display_screen();
extern void add_mat_board();
extern void exit_game(GtkWidget* widget);
extern void gameover();

// for move handling
extern int check_validation(int cmd);
extern int move(int cmd, int movement);
extern void update_model(int id, int x, int y);
extern void update_cell();
extern int item_idxToId(int idx);
extern int item_idToIdx(int id);
extern void score_up(int user_idx);
extern gboolean handle_cmd(gpointer user_data);

// for networking
extern int recv_bytes(int sock_fd, void *buf, size_t len);
extern int send_bytes(int sock_fd, void *buf, size_t len);
extern void handle_timeout(int signum);
extern int parseJson(char *jsonfile);
extern void *recv_msg(void *arg);
extern void cannot_enter();

#endif /* GAME_H */
