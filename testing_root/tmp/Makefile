ifdef coverage
	CVG = -O0 --coverage
else
	CVG = 
endif
CC = clang
cJSON_path = ../../cJSON.c
LIBS = `pkg-config --libs --cflags gtk+-2.0`
FLAGS = -pthread
executable = .out

files = origin_clnt origin_serv
all: $(files)

origin_clnt: pushpush_client.c $(cJSON_path)
	$(CC) $(CVG) $^ -o $@$(executable) $(LIBS) $(FLAGS) 

origin_serv: pushpush_server.c $(cJSON_path)
	$(CC) $(CVG) $^ -o $@$(executable) $(FLAGS) 

test: pushpush_server.c mains/test.c $(cJSON_path)
	
	$(CC) $(CVG) $^ -o $@$(executable) $(FLAGS) -DTEST

cov:
	gcov -b -c $(target).gcno

clean:
	rm -f *.out *.gcno *.gcda *.gcov
