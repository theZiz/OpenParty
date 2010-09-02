all: module
module: skeleton.c
	g++ -fPIC -g -c -Wall skeleton.c
	g++ -shared -Wl,-soname,skeleton.so -o skeleton.so skeleton.o
