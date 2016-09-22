all: openparty optestboard optestgame

all-4install: openparty-4install testboard-4install testgame-4install

openparty: openparty.cpp ingame_kalib.h main.h play_minigame.h var.h camera.h game.h level.h light.h objects.h sets.h texts.h dice.h scriptdefs.h script.h message.h choose.h turn.h menu.h opminigames.h minigamescontrol.h minigamespre.h minigameslast.h opvar.h ballon_background.h options.h  all_control.h newgame_calib.h newgame_choose.h newgame_cpu.h newgame_gamekind.h newgame_rounds.h newgame_teams_2.h newgame_teams_3.h newgame_teams_4.h newgame_turnsort.h about.h aftergame.h joytest.h minigames.h newgame_login.h
	g++ openparty.cpp -o openparty `sdl-config --cflags --libs` -L. -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU -lzizopenparty -ldl

openparty-4install: openparty.cpp ingame_kalib.h main.h play_minigame.h var.h camera.h game.h level.h light.h objects.h sets.h texts.h dice.h scriptdefs.h script.h message.h choose.h turn.h menu.h opminigames.h minigamescontrol.h minigamespre.h minigameslast.h opvar.h ballon_background.h options.h  all_control.h newgame_calib.h newgame_choose.h newgame_cpu.h newgame_gamekind.h newgame_rounds.h newgame_teams_2.h newgame_teams_3.h newgame_teams_4.h newgame_turnsort.h about.h aftergame.h joytest.h minigames.h newgame_login.h
	g++ openparty.cpp -o openparty -DFORINSTALL `sdl-config --cflags --libs` -L. -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU -lzizopenparty -ldl

openparty-debug: openparty.cpp ingame_kalib.h main.h play_minigame.h var.h camera.h game.h level.h light.h objects.h sets.h texts.h dice.h scriptdefs.h script.h message.h choose.h turn.h menu.h opminigames.h minigamescontrol.h minigamespre.h minigameslast.h opvar.h ballon_background.h options.h all_control.h newgame_calib.h newgame_choose.h newgame_cpu.h newgame_gamekind.h about.h newgame_login.h newgame_rounds.h newgame_teams_2.h newgame_teams_3.h newgame_teams_4.h newgame_turnsort.h aftergame.h joytest.h minigames.h
	g++ openparty.cpp -ggdb -o openparty `sdl-config --cflags --libs` -L. -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU -lzizopenparty -ldl

optestboard: testboard.cpp  ingame_kalib.h play_minigame.h main.h var.h camera.h game.h level.h light.h objects.h sets.h texts.h dice.h scriptdefs.h script.h message.h choose.h turn.h partikel_funcs.h sounds.h choosecircle.h ./minigames.h ./minigamescontrol.h ./minigamespre.h ./minigameslast.h
	g++ testboard.cpp -o optestboard `sdl-config --cflags --libs` -L. -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU -lzizopenparty -ldl

optestboard-4install: testboard.cpp  ingame_kalib.h play_minigame.h main.h var.h camera.h game.h level.h light.h objects.h sets.h texts.h dice.h scriptdefs.h script.h message.h choose.h turn.h partikel_funcs.h sounds.h choosecircle.h ./minigames.h ./minigamescontrol.h ./minigamespre.h ./minigameslast.h
	g++ testboard.cpp -o optestboard -DFORINSTALL  `sdl-config --cflags --libs` -L.  -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU -lzizopenparty -ldl

optestboard-debug: testboard.cpp  ingame_kalib.h play_minigame.h main.h var.h camera.h game.h level.h light.h objects.h sets.h texts.h dice.h scriptdefs.h script.h message.h choose.h turn.h partikel_funcs.h sounds.h choosecircle.h ./minigames.h ./minigamescontrol.h ./minigamespre.h ./minigameslast.h
	g++ testboard.cpp -ggdb -o optestboard `sdl-config --cflags --libs` -L.  -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU -lzizopenparty -ldl

optestgame: testgame.cpp minigames.h minigamescontrol.h minigamespre.h minigameslast.h
	g++ testgame.cpp -o optestgame `sdl-config --cflags --libs` -L.  -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU -lzizopenparty -ldl

optestgame-4install: testgame.cpp minigames.h minigamescontrol.h minigamespre.h minigameslast.h
	g++ testgame.cpp -o optestgame -DFORINSTALL `sdl-config --cflags --libs` -L.  -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU -lzizopenparty -ldl

optestgame-debug: testgame.cpp minigames.h minigamescontrol.h minigamespre.h minigameslast.h
	g++ testgame.cpp -ggdb -o optestgame `sdl-config --cflags --libs` -L.  -lSDL_image -lSDL_ttf -lSDL_mixer -lGL -lGLU -lzizopenparty -ldl

