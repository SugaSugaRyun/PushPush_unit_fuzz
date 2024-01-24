ifdef coverage
	CVG = -O0 --coverage
else
	CVG = 
endif

target=

CC = clang
cJSON_path = ../../cJSON.c
executable = .out

files = loadJson_test readbyte_test writebyte_test disconnected_test send_msg_all_test error_handling_test
all: $(files)

readbyte_test: readbyte_test.c
	$(CC) $(CVG) $^ -o $@$(executable)

writebyte_test: writebyte_test.c
	$(CC) $(CVG) $^ -o $@$(executable)

loadJson_test: loadJson_test.c
	$(CC) $(CVG) $^ $(cJSON_path) -o $@$(executable)

disconnected_test: disconnected_test.c
	$(CC) $(CVG) $^ -o $@$(executable)

send_msg_all_test: send_msg_all_test.c
	$(CC) $(CVG) $^ -o $@$(executable)

error_handling_test: error_handling_test.c
	$(CC) $(CVG) $^ -o $@$(executable)

cov:
	gcov -b -c $(target).gcno


clean:
	rm -f *.out *.gcno *.gcda *.gcov
