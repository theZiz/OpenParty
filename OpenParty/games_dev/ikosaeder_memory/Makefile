all: ikosaeder_memory.pp
	fpc -Fu"../../../zizutils/pascal_headers/" -Fl"../../../zizutils/" ikosaeder_memory.pp -oikosaeder_memory.so -Xs -XX -fPIC

debug: ikosaeder_memory.pp
	fpc -Fu"../../../zizutils/pascal_headers/" -Fl"../../../zizutils/" ikosaeder_memory.pp -oikosaeder_memory.so -gw -fPIC
