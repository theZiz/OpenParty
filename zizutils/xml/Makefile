all: lib
	
lib: object xml.o
	gcc -shared -Wl,-soname,libzizxml.so -o libzizxml.so xml.o
