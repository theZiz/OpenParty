all: module
module: wettschwimmen.c
	g++ -fPIC -g -c -Wall wettschwimmen.c
	g++ -shared -Wl,-soname,wettschwimmen.so -o wettschwimmen.so wettschwimmen.o

4install: wettschwimmen.c
	g++ -DFORINSTALL -fPIC -g -c -Wall wettschwimmen.c
	g++ -shared -Wl,-soname,wettschwimmen.so -o wettschwimmen.so wettschwimmen.o
