all: module
module: mini_game.c
	g++ -fPIC -g -c -Wall mini_game.c
	g++ -shared -Wl,-soname,mini_game.so -o mini_game.so mini_game.o
