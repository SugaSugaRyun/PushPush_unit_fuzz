#include <gtk/gtk.h>

#define BOARD_SIZE_X 5
#define BOARD_SIZE_Y 5
#define image_size 16

typedef enum {
    EMPTY,
    PLAYER,
    BOX,
    TARGET,
    OBSTACLE
} CellState;

CellState board[BOARD_SIZE_Y][BOARD_SIZE_X] = {
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, PLAYER, EMPTY, EMPTY, EMPTY},
    {EMPTY, OBSTACLE, TARGET, EMPTY, EMPTY},
    {EMPTY, TARGET, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
};

GtkWidget *window;
GtkWidget *drawingArea;
GdkPixbuf *images[6];  // 6개의 이미지를 저장할 배열

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {

    for (int y = 0; y < BOARD_SIZE_Y; y++) {
        for (int x = 0; x < BOARD_SIZE_X; x++) {
            double startX = x * image_size;
            double startY = y * image_size;

            switch (board[y][x]) {
                case EMPTY:
                    gdk_cairo_set_source_pixbuf(cr, images[0], startX, startY);
                    break;
                case PLAYER:
                    gdk_cairo_set_source_pixbuf(cr, images[1], startX, startY);
                    break;
                case OBSTACLE:
                    gdk_cairo_set_source_pixbuf(cr, images[2], startX, startY);
                    break;
                case TARGET:
                    gdk_cairo_set_source_pixbuf(cr, images[3], startX, startY);
                    break;
            }

            cairo_rectangle(cr, startX, startY, image_size, image_size);
            cairo_fill(cr);

            // cairo_set_source_rgb(cr, 0, 0, 0);  // 검은색
            // cairo_rectangle(cr, startX, startY, cellWidth, cellHeight);
            // cairo_stroke(cr);
        }
    }

    return FALSE;
}

void load_images() {
    images[0] = gdk_pixbuf_new_from_file("background.png", NULL);
    images[1] = gdk_pixbuf_new_from_file("player.png", NULL);
    images[2] = gdk_pixbuf_new_from_file("obstacle.png", NULL);
    images[3] = gdk_pixbuf_new_from_file("jewel.png", NULL);
}

gboolean key_press_callback(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    // 여기에 키 이벤트 처리 로직을 추가합니다.
    gtk_widget_queue_draw(widget);
    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    load_images();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "PushPush Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 16 * 5, 16 * 5);
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
