all: lib

lib: object
	gcc -shared -Wl,-soname,libzizpartikel.so -o libzizpartikel.so partikel.o

object: partikel.c partikel.h
	gcc -fPIC -g -c -Wall -O2 partikel.c
