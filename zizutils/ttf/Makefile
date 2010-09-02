all: ziztexture
ziztexture: lib
	
object: ttf.c
	gcc -fPIC -g -c -Wall -O2 ttf.c

lib: object
	gcc -shared -Wl,-soname,libzizttf.so -o libzizttf.so ttf.o
