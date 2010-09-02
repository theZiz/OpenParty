/*
 The contents of this file are subject to the Mozilla Public License        
 Version 1.1 (the "License"); you may not use this file except in           
 compliance with the License. You may obtain a copy of the License at       
 http://www.mozilla.org/MPL/                                                
                                                                            
 Software distributed under the License is distributed on an "AS IS"        
 basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the    
 License for the specific language governing rights and limitations         
 under the License.                                                         
                                                                            
 Alternatively, the contents of this file may be used under the terms       
 of the GNU Lesser General Public license (the  "LGPL License"), in which case the  
 provisions of LGPL License are applicable instead of those                  
 above.                                                                     
                                                                            
 For feedback and questions about my Files and Projects please mail me,     
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com                         
*/
#define GL_GLEXT_PROTOTYPES

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#ifndef WIN32
  #include <dlfcn.h>
#endif

#include "./../zizutils/zizwindow/zizwindow.h"
#include "./../zizutils/texture/texture.h"
#include "./../zizutils/3dobjects/3dobjects.h"
#include "./../zizutils/ttf/ttf.h"
#include "./../zizutils/xml/xml.h"
#include "./../zizutils/partikel/partikel.h"

//testboard
#include "var.h"
#include "light.h"
#include "objects.h"
#include "sets.h"
#include "camera.h"
#include "texts.h"
#include "message.h"
#include "choosecircle.h"
#include "sounds.h"

//testgame
#include "minigames.h"
#include "minigamescontrol.h"
#include "minigamespre.h"
#include "minigameslast.h"

//testboard
#include "play_minigame.h"
#include "dice.h"
#include "choose.h"
#include "scriptdefs.h"
#include "script.h"
#include "game.h"
#include "level.h"
#include "partikel_funcs.h"
#include "turn.h"
#include "ingame_kalib.h"
#include "main.h"

//eigene Includes
#include "opvar.h"
#include "ballon_background.h"
#include "all_control.h"
#include "options.h"
#include "about.h"
#include "newgame_calib.h"
#include "newgame_choose.h"
#include "newgame_cpu.h"
#include "newgame_field.h"
#include "newgame_gamekind.h"
#include "newgame_games.h"
//#include "newgame_language.h"
#include "newgame_login.h"
#include "newgame_rounds.h"
#include "newgame_teams_2.h"
#include "newgame_teams_3.h"
#include "newgame_teams_4.h"
#include "newgame_turnsort.h"
#include "aftergame.h"
#include "opminigames.h"
#include "menu.h"
#include "joytest.h"

int main( int argc, char* argv[])
{
  //Initialisieren
    ZWattribute=ZWgetattribute();
    load_options();
    ZWinitparameter();
    ZWattribute->strech=stretched;
    if (fullscreen)
    {
      ZWattribute->width=resolutionx;
      ZWattribute->height=resolutiony;
      ZWattribute->sdlflags|=SDL_FULLSCREEN;
    }
    else
    {
      ZWattribute->width=windowx;
      ZWattribute->height=windowy;
    }
    ZWattribute->quit_function=quit_function;
    ZWattribute->initflags|=SDL_INIT_AUDIO;
		
		load_mapping();
		
    ZWinitsdl();
    
    SDL_WM_SetCaption((char*)"OpenParty",(char*)"OpenParty");
    
    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS; 
    int audio_channels = 2;
    int audio_buffers = 2048;
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
    {
      printf("Unable to open audio!\n");
      exit(1);
    }
    
    if ((ZWattribute->sdlflags & SDL_FULLSCREEN)==0) //SDL_FULLSCREEN nicht mehr gesetzt? AAAAHHHH!
    {
      fullscreen=0;
      windowx=ZWattribute->width;
      windowy=ZWattribute->height;
      resolutionx=ZWattribute->resolution[0].x;
      resolutiony=ZWattribute->resolution[0].y;
    }
		ZWsetstdreduce(texquali);
    ZWinitopengl();    
    setup_opengl();
    glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
    init_var();    
    init_op_var();

		//Minispielliste laden
	  load_minigames();
		save_minigames();
    prepare_minigames();

		//Hauptmenu:
    //Zeit einstellen:
    Mix_PlayMusic(snd_menumusic, -1);
    ZWattribute->newticks=SDL_GetTicks();
		
		printf("Run \"openparty --joytest\" for getting button numbers to simulate axis by editing \"mapping.xml\" (e.g. for dance mats).\n");

    if (argc>1 && strcmp(argv[1],(char*)"--joytest")==0)
		{
			int j;
			for (j=0;j<ZWattribute->realjoystickcount;j++)
			{
			  printf("Joystick: \"%s\"\n",ZWattribute->realjoystick[j].name);
				printf("  Buttons: %i\n",ZWattribute->realjoystick[j].buttoncount);
				printf("  Axis:    %i\n",ZWattribute->realjoystick[j].axiscount);
				printf("    \"Real\" Axis: %i\n",ZWattribute->realjoystick[j].only_axiscount);
				printf("    Hat Axis:    %i\n",ZWattribute->realjoystick[j].hatcount*2);
				printf("    Virt. Axis:  %i\n",ZWattribute->realjoystick[j].axiscount-ZWattribute->realjoystick[j].hatcount*2-ZWattribute->realjoystick[j].only_axiscount);
			}
			ZWschleifeex((void(*)(void*))draw_joytest,(int(*)(void*))calc_joytest_thread,(int(*)(void*))calc_joytest,
									 (void*)(&menudata),sizeof(tmenudata));
		}
		else
			ZWschleifeex((void(*)(void*))draw_menu,(int(*)(void*))calc_menu_thread,(int(*)(void*))calc_menu,
									 (void*)(&menudata),sizeof(tmenudata));
    Mix_HaltMusic();
    del_op_objects();
    del_objects();
    after_minigames();
    del_sounds();
    Mix_CloseAudio();
    ZWquitfont();
    ZWquitsdl();
  return 0;
}
