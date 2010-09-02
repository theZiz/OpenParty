all: ziztexture
ziztexture: lib
	
object: texture.c
	gcc -fPIC -g -c -Wall -O2 texture.c

lib: object
	gcc -shared -Wl,-soname,libziztexture.so -o libziztexture.so texture.o
