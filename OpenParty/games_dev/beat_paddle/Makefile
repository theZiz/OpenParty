all: module
module: beat_paddle.c
	g++ -fPIC -g -c -Wall beat_paddle.c
	g++ -shared -Wl,-soname,beat_paddle.so -o beat_paddle.so beat_paddle.o
