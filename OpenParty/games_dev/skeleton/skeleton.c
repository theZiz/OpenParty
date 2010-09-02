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

#include "./../../../zizutils/ttf/ttf.h"
#include "./../../../zizutils/texture/texture.h"
#include "./../../../zizutils/partikel/partikel.h"

#include "./../../testgame/minigames.h"

typedef struct sdata *pdata;
typedef struct sdata {
  int step;
  float fade;
  int countdown;
  ZWtext text;
  pgameinfo gameinfo;
  Mix_Music *backgroundmusic;
  int old_music_volume;
} tdata;


EXPORT void op_get_gameinfos(pminigameinfo gameinfo)
{
  int a,b;
  for (a=0;a<8;a++)
    for (b=0;b<4;b++)
      gameinfo->playable[a][b]=1;
}

EXPORT int op_get_gamedata_size(void)
{
  return sizeof(tdata);
}

EXPORT void op_init_game(pdata data,pgameinfo gameinfo)
{
  //Text initialisieren
  data->text.texture=gameinfo->texttexture;
  int a;
  for (a=0;a<256;a++)
    data->text.breite[a]=gameinfo->textwidth[a];
    
  //Gameinforeferenzpointer speicher:
  data->gameinfo=gameinfo;
  
	char buffer[1024];
	
  //Ton vorbereiten
  sprintf(buffer,"%ssounds/Vegas Glitz.ogg",gameinfo->datafolder);
  data->backgroundmusic = Mix_LoadMUS(buffer);
  data->old_music_volume=Mix_VolumeMusic((int)(trunc(96.0*gameinfo->volume)));
  Mix_FadeInMusic(data->backgroundmusic,-1,500);

  
  //Spiel ansich vorbereiten
  data->step=0;
  data->fade=1;
  data->countdown=3000;  
}

EXPORT int op_calc_game(pdata data)
{ 
  if (data->step==0) //Fade In
  {
    data->fade-=(float)(*(data->gameinfo->steps))/500.0;
    if (data->fade<=0.0)
    {
      data->fade=0.0;
      data->step=1;
    }
    return 0;
  }
  if (data->step==2) //Fade Out
  {
    data->fade+=(float)(*(data->gameinfo->steps))/500.0;
    if (data->fade>=1.0)
    {
      data->fade=1.0;
      return 1;
    }
    return 0;
  }
  if (data->countdown>0) //Countdown
  {
    data->countdown-=*(data->gameinfo->steps);
    return 0;
  }

  //zeitunabhängige Bewegung:
  for (int a=0;a<data->gameinfo->playernum;a++)
    for (int b=0;b<4;b++)
      if (*(data->gameinfo->joystick[a].button[b]))
      {
        data->step=2;
        Mix_FadeOutMusic(500);
        return 0;
      }


  int steps;
  for (steps=0;steps<*(data->gameinfo->steps);steps++)
  {
    //zeitabhängige Berechnungen
  }



  return 0;
}

EXPORT int op_calc_game_thread(pdata data)
{ 
  if (*(data->gameinfo->shouldend))
    return 1;
  
  return 0;
}

EXPORT void op_draw_game(pdata data)
{
  char buffer[256];
 
  glColor4f(1,1,1,1);

	glEnable(GL_LIGHTING);
	ZWsetlightambient(0,0.1,0.1,0.1);
	ZWsetlightdiffuse(0,1,1,1);
	ZWsetlightposition(0,0,1,2);
	ZWenablelight(0);
	
	glTranslatef(-(float)(data->gameinfo->playernum+1)*1.1,0,-12);
	ZWusetexture(data->gameinfo->facetex);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	int a;
	for (a=0;a<data->gameinfo->playernum;a++)
	{
		glColor4f(data->gameinfo->color[a].r,data->gameinfo->color[a].g,data->gameinfo->color[a].b,1);
		glTranslatef(2.2,0.0,0.0);
		glCallList(data->gameinfo->ball[a]);
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glDepthFunc(GL_ALWAYS); 

  //Countdown
  glLoadIdentity();
	glColor4f(1,1,1,1);
  if (data->countdown>0)
  {
    sprintf(buffer,"%i",data->countdown/1000+1);
    ZWdrawtextmiddle((ZWtext)(data->text),0,-0.4,-4,buffer,2.0);  
  }

  //Fade
  glLoadIdentity();
  if (data->step!=1)
  {
    glDisable(GL_TEXTURE_2D);
    glColor4f(0,0,0,data->fade);
    glBegin(GL_QUADS);
      glVertex3f(-1,1,-1);
      glVertex3f(-1,-1,-1);
      glVertex3f(1,-1,-1);
      glVertex3f(1,1,-1);      
    glEnd();
    glEnable(GL_TEXTURE_2D);
  }

  glDepthFunc(GL_LEQUAL);
 
}

EXPORT void op_reset_game(pdata data,pgameresult gameresult)
{
  int b;
  for (b=0;b<data->gameinfo->playernum;b++)
    gameresult->winner[b]=1; //Bei dem Skeleton sind _alle_ Gewinner ^.^  
    
  //Musik zurücksetzen:
  Mix_VolumeMusic(data->old_music_volume);
  Mix_FreeMusic(data->backgroundmusic);
  
}
