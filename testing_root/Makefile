ifdef coverage
	GCOV = -O0 -fprofile-arcs -ftest-coverage
else
	GCOV =
endif

CC = gcc
AFL = ../AFLplusplus/afl-$(CC)
FUZZ = ../../AFLplusplus/afl-fuzz 
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

smod: $(SERVER).c $(TARGET) $(cJSON_path)
	$(CC) $(GCOV) -o mod.out  $^  -pthread -DTEST
cmod: $(CLIENT).c $(TARGET) $(cJSON_path)
	$(CC) $(GCOV) -o mod.out $^  $(LIBS) -DTEST

run: $(TARGET)
	./mod.out < $(TARGET)


#check coverage
scov:
	gcov -b -c -f $(SERVER).gcno
ccov:
	gcov -b -c -f $(CLIENT).gcno

sfuzz: $(SERVER).c $(TARGET) $(cJSON_path)
	$(AFL) -o target.out $^ -pthread -DTEST
cfuzz: $(CLIENT).c $(TARGET) $(cJSON_path)
	$(AFL) -o target.out $^  $(LIBS) -DTEST

rfuzzarg:
	$(FUZZ) -i $(INPUT) -o outputs ./target.out @@ 
rfuzzstd:
	$(FUZZ) -i $(INPUT) -o outputs ./target.out 

clean:
	find . -name "*.out" -exec rm -f {} \;
	find . -name "*.gcno" -exec rm -f {} \;
	find . -name "*.gcda" -exec rm -f {} \;
	find . -name "*.gcov" -exec rm -f {} \;
	find . -name "*.o" -exec rm -f {} \;
