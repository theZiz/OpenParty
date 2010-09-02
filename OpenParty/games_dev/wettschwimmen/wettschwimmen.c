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
#include "./../../../zizutils/3dobjects/3dobjects.h"
#include "./../../../zizutils/zizwindow/zizwindow.h"

#include "./../../testgame/minigames.h"

typedef struct sdata *pdata;
typedef struct sdata {
  int step;
  float fade;
  int countdown;
  ZWtext text;
  pgameinfo gameinfo;
  Mix_Chunk *snd_peng;
  Mix_Chunk *snd_yeah;
  Mix_Music *backgroundmusic;
  int old_music_volume;
	object3d fish[8];
	GLuint water;
	GLuint goal;
	float position[8];
	float momposition[8];
	char mombutton[8];
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
  sprintf(buffer,"%ssounds/explosion.ogg",gameinfo->datafolder);
  data->snd_peng=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_peng,(int)(round(128.0*gameinfo->volume)));
  sprintf(buffer,"%ssounds/yeah.ogg",gameinfo->datafolder);
  data->snd_yeah=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_yeah,(int)(round(64.0*gameinfo->volume)));
  sprintf(buffer,"%ssounds/Radio Martini.ogg",gameinfo->datafolder);
  data->backgroundmusic = Mix_LoadMUS(buffer);
  data->old_music_volume=Mix_VolumeMusic((int)(trunc(96.0*gameinfo->volume)));
  Mix_FadeInMusic(data->backgroundmusic,-1,500);

  //Objekte laden  
  sprintf(buffer,"%stextures/water.png",gameinfo->datafolder);
  data->water=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
  sprintf(buffer,"%stextures/goal.png",gameinfo->datafolder);
  data->goal=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
  for (a=0;a<(data->gameinfo->playernum);a++)
  {
		sprintf(buffer,"%smodels/fish.3dm",gameinfo->datafolder);
    ZWload3dm(&(data->fish[a]),buffer,gameinfo->facetex);
    int b;
    for (b=0;b<data->fish[a].pointnum;b++)
    {
      float dazu=((float)(gameinfo->facenr[a]%4))*0.25;
      data->fish[a].p[b].u+=dazu;
      dazu=((float)(gameinfo->facenr[a]/4))*0.25;
      data->fish[a].p[b].v+=dazu;    
    }
    ZWcreatedrawlist(&(data->fish[a]),gameinfo->lightquali);
		data->position[a]=0;
		data->momposition[a]=0;
		data->mombutton[a]=0;
  }

  //Licht!
  ZWenablelight(0);
  ZWsetlightambient(0,0.0,0.0,0.0);
  ZWsetlightdiffuse(0,1.0,1.0,1.0);
  ZWsetlightposition(0,0.0,0.0,2.0);
  ZWsetmaterial(lnone);
  
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
    float temp=data->fade;
    data->fade+=(float)(*(data->gameinfo->steps))/500.0;
    if (temp<0.0 && data->fade>=0.0)
      Mix_FadeOutMusic(500);
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
		if (data->countdown<=0)
		  Mix_PlayChannel(-1,data->snd_peng,0);
    return 0;
  }

  int winner=0;
  int steps;
  for (steps=0;steps<*(data->gameinfo->steps);steps++)
  {
		int player;
		for (player=0;player<data->gameinfo->playernum;player++)
		{
			if ((*(data->gameinfo->joystick[player].button[data->mombutton[player]]) && !(*(data->gameinfo->joystick[player].button[1-data->mombutton[player]])))
			    || (data->gameinfo->iscpu[player] && rand()%120==0))
			{
				data->position[player]+=M_PI/4.0;
				data->mombutton[player]=1-data->mombutton[player];
				if (data->position[player]/M_PI>=76.0)
				  winner=1;
			}
			if (data->momposition[player]<data->position[player])
				data->momposition[player]+=(data->position[player]-data->momposition[player])*0.01;
		}
  }
	if (winner)
	{
    data->step=2;
		Mix_PlayChannel(-1,data->snd_yeah,0);
    data->fade=-3.0;
		return 0;
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
  glLoadIdentity();
	glEnable(GL_LIGHTING);
	
	int a;
	for (a=0;a<data->gameinfo->playernum;a++)
	{
		glPushMatrix();
		  glTranslatef(data->momposition[a]/M_PI*0.15-6.0,(((float)a)*2.0-((float)(data->gameinfo->playernum)-1.0))*0.7,-10.0);
			glColor4f(data->gameinfo->color[a].r,data->gameinfo->color[a].g,data->gameinfo->color[a].b,1.0);
			glRotatef(90,1,0,0);
			glRotatef(sin(data->momposition[a])*20.0,0,1,0);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			ZWdrawobject(data->fish[a]);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glPopMatrix();
	}


  glLoadIdentity();
	glNormal3f(0,0,1);
	glColor4f(1,1,1,1);
	ZWdrawsprite(data->water,0,0,-10.6,10.0,10.0,1,0);

	glPushMatrix();
	glRotatef(-90,0,0,1);
	glRotatef(90,1,0,0);
	glColor4f(1,1,1,1);
	ZWdrawsprite(data->goal,0,-9.0,-6.0,6.0,1.5,1,0);
	glPopMatrix();

	glColor4f(1,1,1,0.6);
	ZWdrawsprite(data->water,sin((float)(SDL_GetTicks())/500.0)*1.3,cos((float)(SDL_GetTicks())/500.0)*0.7,-10.3,10.0,10.0,1,0);
	glColor4f(1,1,1,0.4);
	ZWdrawsprite(data->water,sin((float)(SDL_GetTicks())/700.0)*0.7,cos((float)(SDL_GetTicks())/700.0)*1.3,-10.0,10.0,10.0,1,0);
	
	
  char buffer[256];
	glDisable(GL_LIGHTING);
 
  glDepthFunc(GL_ALWAYS);  

  //Countdown
  glLoadIdentity();
	glColor4f(1,1,1,0.8);
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

	int player;
	for (player=0;player<data->gameinfo->playernum;player++)
	  gameresult->winner[player]=(data->position[player]/M_PI>=76.0);
		

  //Texturen und Objekte freigeben
  int a;
  for (a=0;a<(data->gameinfo->playernum);a++)
    ZWdeleteobject(data->fish[a]);    
  ZWdeletetexture(data->water);
  ZWdeletetexture(data->goal);

    
  //Musik zurücksetzen:
  Mix_VolumeMusic(data->old_music_volume);
  Mix_FreeMusic(data->backgroundmusic);
  Mix_FreeChunk(data->snd_peng);  
  Mix_FreeChunk(data->snd_yeah);  
}
