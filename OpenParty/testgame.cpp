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
#ifndef WIN32
  #include <dlfcn.h>
#endif

#include "./../zizutils/zizwindow/zizwindow.h"
#include "./../zizutils/texture/texture.h"
#include "./../zizutils/3dobjects/3dobjects.h"
#include "./../zizutils/ttf/ttf.h"
#include "./../zizutils/xml/xml.h"
#include "./../zizutils/partikel/partikel.h"

#include "var.h"
#include "texts.h"

#include "minigames.h"
#include "minigamespre.h"
#include "minigamescontrol.h"
#include "minigameslast.h"

void prepare_game(void)
{
  int a;
  for (a=0;a<playernum;a++)
  {
    maindata.player[a].money=42;
    maindata.player[a].rubins=23;
    maindata.player[a].color.r=(float)(a%2);
    maindata.player[a].color.g=(float)(a/2%2);
    maindata.player[a].color.b=(float)(a/4%2);
    maindata.player[a].facenr=rand()%MAX_FACES;
    ZWload3dm(&(maindata.player[a].ball),(char*)DATAFOLDER"models/ball.3dm",facetex);
    int b;
    for (b=0;b<maindata.player[a].ball.pointnum;b++)
    {
      float dazu=((float)(maindata.player[a].facenr%4))*0.25;
      maindata.player[a].ball.p[b].u+=dazu;
      dazu=((float)(maindata.player[a].facenr/4))*0.25;
      maindata.player[a].ball.p[b].v+=dazu;
    }
    ZWcreatedrawlist(&(maindata.player[a].ball),lightquali);
    maindata.player[a].iscpu=all_are_cpu;
    maindata.player[a].color.a=1.0;
    maindata.player[a].team=a%(playernum/teamsize);
  }
}

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
    ZWattribute->initflags|=SDL_INIT_AUDIO;
    ZWattribute->quit_function=quit_function;

		load_mapping();

    ZWinitsdl();
		
    if ((ZWattribute->sdlflags & SDL_FULLSCREEN)==0) //SDL_FULLSCREEN nicht mehr gesetzt? AAAAHHHH!
    {
      fullscreen=0;
      windowx=ZWattribute->width;
      windowy=ZWattribute->height;
      resolutionx=ZWattribute->resolution[0].x;
      resolutiony=ZWattribute->resolution[0].y;
    }

    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS; 
    int audio_channels = 2;
    int audio_buffers = 2048;
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
    {
      printf("Unable to open audio!\n");
      exit(1);
    }
   
    ZWinitopengl();    
    setup_opengl();
    glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
    init_var();    
  //Debug
    if (argc>3 && argv[3][0]=='c')
      all_are_cpu=1;
        
  //Spielvorbereitungen
    if (argc>2)
      playernum=atoi(argv[2]);
    playernum=(playernum+7)%8+1;

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

    prepare_game();

  
  //Controller festlegen:
	ZWreset_virtuell_joytick();
  for (int a=0;a<playernum;a++)
  {
		if (all_are_cpu || a>=ZWattribute->realjoystickcount)
		  maindata.player[a].joystick_nr=-1;
		else		  
		  maindata.player[a].joystick_nr=a%ZWattribute->realjoystickcount;
  }
	calib_all_controllers();
  for (int a=ZWattribute->realjoystickcount;a<playernum;a++)
    maindata.player[a].iscpu=1;

  int result=0;
  tgameresult gameresult;
  while (result!=1 && (shouldend==0))
  {
		for (int player=0;player<playernum;player++)
		{
			if (!(maindata.player[player].iscpu))
				continue;
			for (int a=0;a<4;a++)
				*(ZWattribute->joystick[player].button[a])=0;
			for (int a=0;a<2;a++)
				*(ZWattribute->joystick[player].axis[a])=0;
		}
    //Minispielvorauswahl
      result=prepare_minigame(argv[1],&maindata);
    //Minispiel starten
      play_minigame(argv[1],0,&gameresult,result-1);
  }   
  //Minispielergebnis
  after_minigame(&maindata,&gameresult);
  
  //alles wieder freigeben
    Mix_CloseAudio();
    ZWquitfont();
    ZWquitsdl();
  return 0;
}
