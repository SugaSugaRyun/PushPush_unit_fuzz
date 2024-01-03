CC = gcc

GTK_LIBS = `pkg-config --libs gtk+-3.0`
GTK_CFLAGS = `pkg-config --cflags gtk+-3.0`

SRC = move.c
TARGET = result

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(GTK_LIBS) $(GTK_CFLAGS)

clean:
	rm -f $(TARGET)
