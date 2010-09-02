all: module
module: spiral_spass.c
	g++ -fPIC -g -c -Wall spiral_spass.c
	g++ -shared -Wl,-soname,spiral_spass.so -o spiral_spass.so spiral_spass.o

4install: spiral_spass.c
	g++ -DFORINSTALL -fPIC -g -c -Wall spiral_spass.c
	g++ -shared -Wl,-soname,spiral_spass.so -o spiral_spass.so spiral_spass.o
