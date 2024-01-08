JSON = "../game_data/map1.json"
IP = "127.0.0.1"
HARMONY = "203.252.112.11"
PEACE = "203.252.112.10"
PORT = "9990"

compile: server client

server:
	gcc -o server game_server.c ../cJSON.c -pthread
client:
	gcc -o client game_with_server.c ../cJSON.c `pkg-config --libs --cflags gtk+-2.0` -DDEBUG
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
