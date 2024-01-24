ifdef coverage
	GCOV = -O0 -fprofile-arcs -ftest-coverage
else
	GCOV =
endif

CC = clang
JSON = "../game_data/map2.json"
IP = "127.0.0.1"
PORT = "9999"
SERVER = pushpush_server
CLIENT = pushpush_client
LIBS = `pkg-config --libs --cflags gtk+-2.0`
cJSON_path = ../cJSON.c
cJSON_object = cJSON.o
.PHONY : rcs rcc

compile: server client

#origin version compile...
server: $(SERVER).c $(cJSON_path)
	$(CC) $(GCOV) -o $@.out  $^  -pthread
client: $(CLIENT).c $(cJSON_path)
	$(CC) $(GCOV) -o $@.out $^  $(LIBS) 

#run normally
rs:
	echo $(JSON) | ./server.out $(PORT)
rc:
	./client.out $(IP) $(PORT)

#TODO test


test: mkobj run

mkobj: $(SERVER).c $(MAIN_TARGET)
	$(CC) -c -o $(cJSON_object) $(cJSON_path)
	$(CC) -c -o $(SERVER).o $(SERVER).c -I ../ -pthread -DTEST
	$(CC) -o $(MAIN_TARGET).out $(SERVER).o $(cJSON_object) $(MAIN_TARGET) -pthread -DTEST

run: $(MAIN_TARGET)
	./$(MAIN_TARGET).out
	

#check coverage
scov:
	gcov -b -c -f $(SERVER).gcno
ccov:
	gcov -b -c -f $(CLIENT).gcno

clean:
	find . 	-name "*.out" -o -name "*.gcno" -o 
		-name "*.gcda" -o 
		-name "*.gcov" -o 
		-name "*.o" -exec rm -f {} \;