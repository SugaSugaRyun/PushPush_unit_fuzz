ifdef coverage
	CVG = -O0 --coverage
else
	CVG = 
endif

CC = clang
cJSON_path = ../../cJSON.c

files = loadJson_test readbyte_test writebyte_test
all: $(files)

readbyte_test: readbyte_test.c
	$(CC) $(CVG) $^ -o $@

writebyte_test: writebyte_test.c
	$(CC) $(CVG) $^ -o $@

loadJson_test: loadJson_test.c
	$(CC) $(CVG) $^ $(cJSON_path) -o $@

clean:
	rm $(files) *.gcno *.gcda *.gcov