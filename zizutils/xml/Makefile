all: lib
	
object: xml.c xml.o
	gcc -fPIC -g -c -Wall -O2 xml.c

lib: object
	gcc -shared -Wl,-soname,libzizxml.so -o libzizxml.so xml.o
