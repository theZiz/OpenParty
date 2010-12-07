all: lib
	
lib: object
	gcc -shared -Wl,-soname,libzizxml.so -o libzizxml.so xml.o

object: xml.c xml.h
	gcc -fPIC -g -c -Wall -O2 xml.c
