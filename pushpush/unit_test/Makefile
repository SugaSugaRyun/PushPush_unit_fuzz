ifdef coverage
	CVG = -O0 --coverage
else
	CVG = 
endif

CC = clang
cJSON_path = ../../cJSON.c

all: loadJson_test


loadJson_test: loadJson_test.c
	$(CC) $(CVG) $^ $(cJSON_path) -o $@

clean:
	rm loadJson_test *.gcno *.gcda *.gcov