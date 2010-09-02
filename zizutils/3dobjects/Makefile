all: lib
	
object: 3dobjects.c
	gcc -fPIC -g -c -Wall -O2 3dobjects.c

lib: object
	gcc -shared -Wl,-soname,libziz3dobjects.so -o libziz3dobjects.so 3dobjects.o
