ifdef coverage
	CVG = -O0 --coverage
else
	CVG = 
endif

CC = clang
cJSON_path = ../../cJSON.c

files = loadJson_test readbyte_test writebyte_test disconnected_test send_msg_all_test error_handling_test
all: $(files)

readbyte_test: readbyte_test.c
	$(CC) $(CVG) $^ -o $@

writebyte_test: writebyte_test.c
	$(CC) $(CVG) $^ -o $@

loadJson_test: loadJson_test.c
	$(CC) $(CVG) $^ $(cJSON_path) -o $@

disconnected_test: disconnected_test.c
	$(CC) $(CVG) $^ -o $@

send_msg_all_test: send_msg_all_test.c
	$(CC) $(CVG) $^ -o $@

error_handling_test: error_handling_test.c
	$(CC) $(CVG) $^ -o $@

clean:
	rm -f $(files) *.gcno *.gcda *.gcov
