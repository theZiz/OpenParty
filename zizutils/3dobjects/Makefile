all: lib

lib: object
	gcc -shared -Wl,-soname,libziz3dobjects.so -o libziz3dobjects.so 3dobjects.o

object: 3dobjects.c 3dobjects.h
	gcc -fPIC -g -c -Wall -O2 3dobjects.c
