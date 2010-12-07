all: lib

lib: object
	gcc -shared -Wl,-soname,libzizttf.so -o libzizttf.so ttf.o

object: ttf.c ttf.h
	gcc -fPIC -g -c -Wall -O2 ttf.c
