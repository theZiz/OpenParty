all: lib

lib: object 3dobjects.o
	gcc -shared -Wl,-soname,libziz3dobjects.so -o libziz3dobjects.so 3dobjects.o
