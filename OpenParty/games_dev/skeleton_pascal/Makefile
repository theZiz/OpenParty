all: skeleton_pascal.pp
	fpc -Fu"../../../zizutils/pascal_headers/" -Fl"../../../zizutils/" skeleton_pascal.pp -oskeleton_pascal.so -Xs -XX -fPIC

debug: skeleton_pascal.pp
	fpc -Fu"../../../zizutils/pascal_headers/" -Fl"../../../zizutils/" skeleton_pascal.pp -oskeleton_pascal.so -gw -fPIC
