CC = clang
JSON = "../game_data/map2.json"
IP = "127.0.0.1"
PORT = "9999"
SERVER = pushpush_server
CLIENT = pushpush_client
GCOV = -O0 -fprofile-arcs -ftest-coverage
LIBS = `pkg-config --libs --cflags gtk+-2.0`
cJSON_path = ../cJSON.c
.PHONY : rcs rcc

compile: server client


server: $(SERVER).c $(cJSON_path)
	$(CC) -o $@  $^  -pthread
client: $(CLIENT).c $(cJSON_path)
	$(CC) -o $@ $^  $(LIBS) -DDEBUG

#run normally
rs:
	echo $(JSON) | ./server $(PORT)
rc:
	./client $(IP) $(PORT)


cov: cc cs
cs: $(SERVER).c $(cJSON_path) #coverage server
	$(CC) $(GCOV) -o $@ $^ -pthread
cc: $(CLIENT).c $(cJSON_path) #coverage client
	$(CC) $(GCOV) -o $@ $^ $(LIBS) -DDEBUG

#run with coverage
rcc:
	./cc $(IP) $(PORT)
rcs:
	echo $(JSON) | ./cs $(PORT)


scov:
	gcov -b -c -f $(SERVER).gcno
ccov:
	gcov -b -c -f $(CLIENT).gcno

clean:
	rm -f server client cc cs *.gcno *.gcda *.gcov
