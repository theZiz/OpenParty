all: module
module: reflexruebe.c
	g++ -fPIC -g -c -Wall reflexruebe.c
	g++ -shared -Wl,-soname,reflexruebe.so -o reflexruebe.so reflexruebe.o

4install: reflexruebe.c
	g++ -DFORINSTALL -fPIC -g -c -Wall reflexruebe.c
	g++ -shared -Wl,-soname,reflexruebe.so -o reflexruebe.so reflexruebe.o
