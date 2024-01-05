JSON = "file.json"
IP = "127.0.0.1"
PORT = "9999"

make:

server:
	gcc -o server chat_serv.c cJSON.c -pthread

client:
	gcc -o client GUI_done.c cJSON.c `pkg-config --libs --cflags gtk+-2.0` -DDEBUG

rs:
	echo $(JSON) | ./server $(PORT)
rc:
	./client $(IP) $(PORT) 

clean:
	rm server client
