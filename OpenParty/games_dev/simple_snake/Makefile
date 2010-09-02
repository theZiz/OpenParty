all: module
module: simple_snake.c
	g++ -fPIC -g -c -Wall simple_snake.c
	g++ -shared -Wl,-soname,simple_snake.so -o simple_snake.so simple_snake.o

4install: simple_snake.c
	g++ -DFORINSTALL -fPIC -g -c -Wall simple_snake.c
	g++ -shared -Wl,-soname,simple_snake.so -o simple_snake.so simple_snake.o
