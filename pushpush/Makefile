JSON = "../game_data/map2.json"
IP = "127.0.0.1"
PORT = "9999"

GCOV = -O0 -fprofile-arcs -ftest-coverage

LIBS = `pkg-config --libs --cflags gtk+-2.0`

cJSON_path = ../cJSON.c

.PHONY : rcs rcc

compile: server client

server: pushpush_server.c $(cJSON_path)
	gcc -o $@  $^  -pthread
client: pushpush_client.c $(cJSON_path)
	gcc -o $@ $^  $(LIBS) -DDEBUG
rs:
	echo $(JSON) | ./server $(PORT)
rc:
	./client $(IP) $(PORT)
cs: pushpush_server.c $(cJSON_path) #coverage server
	gcc $(GCOV) -o $@ $^ -pthread
rcs:
	echo $(JSON) | ./cs $(PORT)
cc: pushpush_client.c $(cJSON_path) #coverage client
	gcc $(GCOV) -o $@ $^ $(LIBS) -DDEBUG
rcc:
	./cc $(IP) $(PORT)

scov:
	gcov -b -c -f 
ccov:

clean:
	rm server client cc cs
