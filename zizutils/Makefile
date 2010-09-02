onelib: zizwindow xml ttf texture 3dobjects partikel
	gcc -shared -Wl,-soname,libzizopenparty.so -o libzizopenparty.so ./zizwindow/zizwindow.o ./texture/texture.o ./3dobjects/3dobjects.o ./ttf/ttf.o ./xml/xml.o ./partikel/partikel.o `sdl-config --cflags --libs` -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU

zizwindow: ./zizwindow/zizwindow.c ./zizwindow/zizwindow.h
	cd zizwindow
	make
	cd ..

xml: ./xml/xml.c ./xml/xml.h
	make ./xml/Makefile

ttf: ./ttf/ttf.c ./ttf/ttf.h
	make ./ttf/Makefile

texture: ./texture/texture.c ./texture/texture.h
	make ./texture/Makefile

3dobjects: ./3dobjects/3dobjects.c ./3dobjects/3dobjects.h
	make ./3dobjects/Makefile

partikel: ./partikel/partikel.c ./partikel/partikel.h
	make ./partikel/Makefile

install:
	sudo cp libzizopenparty.so /usr/lib/libzizopenparty.so
