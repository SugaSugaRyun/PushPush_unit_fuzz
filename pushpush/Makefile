JSON = "../game_data/map1.json"
IP = "127.0.0.1"
HARMONY = "203.252.112.11"
PEACE = "203.252.112.10"
PORT = "9991"

LIBS = `pkg-config --libs --cflags gtk+-2.0`

cJSON_path = ../cJSON.c

compile: server client

server: pushpush_server.c $(cJSON_path)
	gcc -o $@  $^  -pthread
client: pushpush_client.c $(cJSON_path)
	gcc -o $@ $^  $(LIBS) -DDEBUG
rs:
	echo $(JSON) | ./server $(PORT)
rc:
	./client $(IP) $(PORT)
rch:
	./client $(HARMONY) $(PORT)
rcp:
	./client $(PEACE) $(PORT)
clean:
	rm server client
