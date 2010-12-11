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
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <dlfcn.h>

#include "./../zizutils/zizwindow/zizwindow.h"
#include "./../zizutils/texture/texture.h"
#include "./../zizutils/3dobjects/3dobjects.h"
#include "./../zizutils/ttf/ttf.h"
#include "./../zizutils/xml/xml.h"
#include "./../zizutils/partikel/partikel.h"

#include "var.h"
#include "light.h"
#include "objects.h"
#include "sets.h"
#include "camera.h"
#include "texts.h"
#include "message.h"
#include "choosecircle.h"
#include "minigames.h"
#include "minigamescontrol.h"
#include "minigamespre.h"
#include "minigameslast.h"
#include "play_minigame.h"
#include "dice.h"
#include "choose.h"
#include "scriptdefs.h"
#include "script.h"
#include "game.h"
#include "sounds.h"
#include "level.h"
#include "partikel_funcs.h"
#include "turn.h"
#include "ingame_kalib.h"
#include "main.h"

int main( int argc, char* argv[] )
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
    ZWinitopengl();    
    setup_opengl();
    glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
    init_var();
        
  //Level laden
    if (argc<2)
    {
      printf("Not enough arguments\n");
      printf("testboard file.opxml [player]\n");
      ZWquitfont();
      ZWquitsdl();      
      return 1;
    }
    if (loadlevel(argv[1])==1)
    {
      printf("File %s not found\n",argv[1]);
      ZWquitfont();
      ZWquitsdl();   
      return 1;
    }  
    refreshsetlist();

  //Minispielliste laden
	  load_minigames();
		save_minigames();

  /*//zu Debugzwecken Set 0 bis 60 ausgeben:
  for (int a=0;a<61;a++)
  {
    if (setbynr[a]==NULL)
      printf("Set %i: NULL\n",a);
    else
      printf("Set %i: \"%s\"\n",a,setbynr[a]->type);  
  }*/

  //Debug
    if (argc>3 && argv[3][0]=='c')
      all_are_cpu=1;

  //Spielvorbereitungen
    if (argc>2)
      playernum=atoi(argv[2]);
    playernum=(playernum-1)%8+1;

		
		if (argc>3 && argv[3][1]=='2')
		{
			playernum=((playernum+1)/2)*2;
			if (playernum<4)
			  playernum=4;
			teamsize=2;
		}
		if (argc>3 && argv[3][1]=='3')
		{
			playernum=6;
			teamsize=3;
		}
		if (argc>3 && argv[3][1]=='4')
		{
			playernum=8;
			teamsize=4;
		}		

    pset startset=getsetbyname((char*)"start");
    if (startset==NULL) startset=firstset;
		prepare_game(startset,1);
		
    
  ZWreset_virtuell_joytick();
  //Controller festlegen:
  for (int a=0;a<playernum;a++)
  {
		if (all_are_cpu)
		  maindata.player[a].joystick_nr=-1;
		else		  
		  maindata.player[a].joystick_nr=a%ZWattribute->realjoystickcount;
  }
	calib_all_controllers();
  //Spiel starten
    Mix_FadeInMusic(backgroundmusic, -1,FADE_IN_TIME);
    ZWattribute->newticks=SDL_GetTicks();
    ZWschleifeex((void(*)(void*))draw_main,(int(*)(void*))calc_main_thread,(int(*)(void*))calc_main,
                 (void*)(&maindata),sizeof(tmaindata));    
    Mix_HaltMusic();
  //alles wieder freigeben
    reset_after_game();
    del_objects();
    del_sounds();
    Mix_CloseAudio();
    ZWquitfont();
    ZWquitsdl();
  return 0;
}
