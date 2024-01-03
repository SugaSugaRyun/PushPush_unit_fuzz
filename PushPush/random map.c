#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define image_size 16
   
#define BACKGROUND 0
#define PLAYER 1
#define HOME 2
#define TARGET 3
#define OBSTACLE 4


#define TARGET_NUM 10
#define OBSTACLE_NUM 20
#define MAP_SIZE 16
// 16x16
// 0,0은 플레이어 위치로 고정
// 타켓은 각 열에 하나씩 먼저 생성 되게끔

int map[MAP_SIZE][MAP_SIZE/8] ={0};
// short map_cell[MAP_SIZE][MAP_SIZE];

void make_map()
{
    //player start
    map[0][0] |= 0x10000000;
    
    int x, y, z;
    srand(time(NULL));


    
    for(int i=0; i<OBSTACLE_NUM;)
    {
      
        y = rand() % MAP_SIZE;
        x = y % ((int)MAP_SIZE/8);
        z = rand() % 8;

        if( (map[y][x] >> (4*z) & 0xF) == 0)
        {
            map[y][x] |= (0x4 << (4*z));
            i++;
        }
    }

    int check_target[MAP_SIZE] = {0};
    
    for(int i=0; i<TARGET_NUM;)
    {
        while(1)
        {
            y = rand() % MAP_SIZE;
            if(check_target[y] <= i/(int)MAP_SIZE)
            {
                check_target[y]++;
                break;
            }
        }
        x = y % ((int)MAP_SIZE/8);
        z = rand() % 8;

        if( (map[y][x] >> (4*z) & 0xF) == 0)
        {
            map[y][x] |= (0x3 << (4*z));
            i++;
        }
    }
    // for(int i=0; i<MAP_SIZE; i++)
    // {
    //     for(int j = 0; j<2; j++)
    //     {
    //         printf("%8x ",map[i][j]);
    //     }
    //     printf("\n");
    // }
}
GtkWidget *window;
GtkWidget *drawingArea;
GdkPixbuf *images[6];  // 6개의 이미지를 저장할 배열

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {

    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            double startX = x * image_size;
            double startY = y * image_size;
            
            int A = map[y][x / 8];
            A = (A >> (4 * ((MAP_SIZE-x-1)% 8))) & 0xF;
            // fprintf(stderr,"%d",A);
            switch ( A ) {
                case BACKGROUND:
                    gdk_cairo_set_source_pixbuf(cr, images[BACKGROUND], startX, startY);
                    break;
                case PLAYER:
                    gdk_cairo_set_source_pixbuf(cr, images[PLAYER], startX, startY);
                    break;
                case HOME:
                    gdk_cairo_set_source_pixbuf(cr, images[HOME], startX, startY);
                    break;
                case OBSTACLE:
                    gdk_cairo_set_source_pixbuf(cr, images[OBSTACLE], startX, startY);
                    break;
                case TARGET:
                    gdk_cairo_set_source_pixbuf(cr, images[TARGET], startX, startY);
                    break;
            }

            cairo_rectangle(cr, startX, startY, image_size, image_size);
            cairo_fill(cr);

            // cairo_set_source_rgb(cr, 0, 0, 0);  // 검은색
            // cairo_rectangle(cr, startX, startY, cellWidth, cellHeight);
            // cairo_stroke(cr);
        }
        // fprintf(stderr,"\n");
    }

    return FALSE;
}

void load_images() {
    images[BACKGROUND] = gdk_pixbuf_new_from_file("background.png", NULL);
    images[PLAYER] = gdk_pixbuf_new_from_file("player.png", NULL);
    images[HOME] = gdk_pixbuf_new_from_file("flag.png", NULL);
    images[OBSTACLE] = gdk_pixbuf_new_from_file("obstacle.png", NULL);
    images[TARGET] = gdk_pixbuf_new_from_file("jewel.png", NULL);
}

gboolean key_press_callback(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    // 여기에 키 이벤트 처리 로직을 추가합니다.
    gtk_widget_queue_draw(widget);
    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    load_images();
    make_map();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "PushPush Game");
    gtk_window_set_default_size(GTK_WINDOW(window), image_size * MAP_SIZE, image_size * MAP_SIZE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    drawingArea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawingArea);

    g_signal_connect(drawingArea, "draw", G_CALLBACK(draw_callback), NULL);

    gtk_widget_set_events(drawingArea, GDK_KEY_PRESS_MASK);
    g_signal_connect(drawingArea, "key-press-event", G_CALLBACK(key_press_callback), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    // 이미지 메모리 해제
    for (int i = 1; i <= 3; i++) {
        g_object_unref(images[i]);
    }

    return 0;
}
