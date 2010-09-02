all: module
module: roller.c
	g++ -fPIC -g -c -Wall roller.c
	g++ -shared -Wl,-soname,roller.so -o roller.so roller.o
