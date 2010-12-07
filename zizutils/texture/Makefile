all: lib
	
lib: object texture.o
	gcc -shared -Wl,-soname,libziztexture.so -o libziztexture.so texture.o
