all: module
module: kugel_krieg.c cpu.h
	g++ -fPIC -g -c -Wall kugel_krieg.c
	g++ -shared -Wl,-soname,kugel_krieg.so -o kugel_krieg.so kugel_krieg.o

4install: kugel_krieg.c cpu.h
	g++ -DFORINSTALL -fPIC -g -c -Wall kugel_krieg.c
	g++ -shared -Wl,-soname,kugel_krieg.so -o kugel_krieg.so kugel_krieg.o
