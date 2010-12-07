all: lib
	
lib: object
	gcc -shared -Wl,-soname,libziztexture.so -o libziztexture.so texture.o

object: texture.c texture.h
	gcc -fPIC -g -c -Wall -O2 texture.c
