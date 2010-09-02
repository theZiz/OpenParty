all: lib
	
object: xml.c
	gcc -fPIC -g -c -Wall -O2 xml.c

lib: object
	gcc -shared -Wl,-soname,libzizxml.so -o libzizxml.so xml.o
