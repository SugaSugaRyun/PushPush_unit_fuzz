TEST = "test/map1.txt"

make:

test:
	gcc -o game pushpush.c `pkg-config --libs --cflags gtk+-2.0`

r:
	./game
future_run:
	./game $(TEST)

clean:
	rm a game
