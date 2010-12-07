all: lib

lib: object partikel.o
	gcc -shared -Wl,-soname,libzizpartikel.so -o libzizpartikel.so partikel.o
