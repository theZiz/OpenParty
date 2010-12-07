all: lib

lib: object ttf.o
	gcc -shared -Wl,-soname,libzizttf.so -o libzizttf.so ttf.o
