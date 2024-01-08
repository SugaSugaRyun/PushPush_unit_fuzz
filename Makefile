JSON = "file.json"
IP = "127.0.0.1"
PORT = "9999"

run: server client

server:
	gcc -o server chat_serv.c cJSON.c -pthread

client:
	gcc -o client GUI_done.c cJSON.c `pkg-config --libs --cflags gtk+-2.0` -DDEBUG

s:
	echo $(JSON) | ./server $(PORT)
c:
	./client $(IP) $(PORT) 

clean:
	rm server client
