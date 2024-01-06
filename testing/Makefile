JSON = "../game_data/map1.json"
IP = "127.0.0.1"
PORT = "9999"

run: server client

server:
	gcc -o server game_server.c ../cJSON.c -pthread

client:
	gcc -o client game_with_server.c ../cJSON.c `pkg-config --libs --cflags gtk+-2.0` -DDEBUG

rs:
	echo $(JSON) | ./server $(PORT)
rc:
	./client $(IP) $(PORT)

clean:
	rm server client
