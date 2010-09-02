all: lib
	
object: partikel.c
	gcc -fPIC -g -c -Wall -O2 partikel.c

lib: object
	gcc -shared -Wl,-soname,libzizpartikel.so -o libzizpartikel.so partikel.o
