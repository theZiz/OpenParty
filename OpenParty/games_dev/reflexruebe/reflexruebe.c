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

#include "./../../minigames.h"

typedef struct sdata *pdata;
typedef struct sdata {
  int step;
  float fade;
  int countdown;
  ZWtext text;
  pgameinfo gameinfo;
  Mix_Chunk *snd_negative;
  Mix_Chunk *snd_positive;
  Mix_Music *backgroundmusic;
  int old_music_volume;
	object3d schild;
	object3d ruebe;
	GLuint texture;
	float schildposition;
	char the_button;
	int points[8];
	int gamestep;
	char blocked[8];
	int time_out;
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
	sprintf(buffer,"%ssounds/positive.ogg",gameinfo->datafolder);
  data->snd_positive=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_positive,(int)(round(128.0*gameinfo->volume)));
	sprintf(buffer,"%ssounds/negative.ogg",gameinfo->datafolder);
  data->snd_negative=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_negative,(int)(round(128.0*gameinfo->volume)));
	sprintf(buffer,"%ssounds/Radio Martini.ogg",gameinfo->datafolder);
  data->backgroundmusic = Mix_LoadMUS(buffer);
  data->old_music_volume=Mix_VolumeMusic((int)(trunc(96.0*gameinfo->volume)));
  Mix_FadeInMusic(data->backgroundmusic,-1,500);

  //Objekte laden  
  sprintf(buffer,"%stextures/reflexruebe.png",gameinfo->datafolder);
  data->texture=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
  sprintf(buffer,"%smodels/ruebe.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->ruebe),buffer,data->texture);
  ZWcreatedrawlist(&(data->ruebe),gameinfo->lightquali);
	sprintf(buffer,"%smodels/schild.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->schild),buffer,data->texture);
  ZWcreatedrawlist(&(data->schild),0);

  //Licht!
  ZWenablelight(0);
  ZWsetlightambient(0,0.0,0.0,0.0);
  ZWsetlightdiffuse(0,1.0,1.0,1.0);
  ZWsetlightposition(0,0.0,0.0,2.0);
  ZWsetmaterial(lnone);
	
	for (a=0;a<gameinfo->playernum;a++)
	{
	  data->points[a]=0;
		data->blocked[a]=0;
	}
  
  //Spiel ansich vorbereiten
  data->step=0;
  data->fade=1;
  data->countdown=3000;
	data->schildposition=100.0;  
	data->gamestep=0;
	data->the_button=rand()%4;
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
    return 0;
  }

  int winner=0;
  int steps;
  for (steps=0;steps<*(data->gameinfo->steps);steps++)
  {
		int player;
		switch (data->gamestep)
		{
			case 0: //Schild unten, WARTEN
			  if (rand()%2000==0)
				  data->gamestep=1;
		  break;
			case 1: //Schild geht nach oben
			  data->schildposition-=1.0;
				if (data->schildposition<=0.0)
				{
					data->schildposition=0.0;
					data->gamestep=2;
					data->time_out=30000;
				}
			break;
			case 2: //Schild ist oben und wir warten auf die dämlichen Spieler...
			  data->time_out-=50;
				if (data->time_out<=100)
				  data->time_out=100;
			break;
			case 3: //Pause, wo das Schild runtergeht
			  data->schildposition+=0.2;
				if (data->schildposition>=100.0)
				{
					data->schildposition=100.0;
					data->gamestep=0;
					data->the_button=rand()%4;
				}
			  
			break;
		}
		int all=0;
		for (player=0;player<data->gameinfo->playernum;player++)
		{
		  int a;
			if (data->blocked[player])
			  all++;
      else
			for (a=0;a<4;a++)
			{
				if (data->gameinfo->iscpu[player])
				{
					if (data->gamestep!=2 && rand()%500000==0)
					  *data->gameinfo->joystick[player].button[rand()%4]=1;
					if (data->gamestep==2 && rand()%data->time_out==0)
					  *data->gameinfo->joystick[player].button[data->the_button]=1;
				}
			  if (*(data->gameinfo->joystick[player].button[a]))
				{
					if (data->gamestep==2 && a==data->the_button)
					{
					  data->points[player]++;
						if (data->points[player]>=3)
						  winner=1;
						data->gamestep=3;
						Mix_PlayChannel(-1,data->snd_positive,0);
						int b;
						for (b=0;b<data->gameinfo->playernum;b++)
						{
							if (data->blocked[b]>0)
							  data->blocked[b]--;
						}
						*data->gameinfo->joystick[player].button[a]=0;
					}
					else
					if ((data->gamestep==2 && a!=data->the_button) || data->gamestep==0)
					{
					  data->blocked[player]=2;
						Mix_PlayChannel(-1,data->snd_negative,0);
						*data->gameinfo->joystick[player].button[a]=0;
					}
				}
			}
		}
		if (all==data->gameinfo->playernum)
		{
		  data->gamestep=3;
			int b;
			for (b=0;b<data->gameinfo->playernum;b++)
				data->blocked[b]=0;
		}
  }
	if (winner)
	{
    data->step=2;
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
  char buffer[256];

	glDisable(GL_LIGHTING);
	
	glPushMatrix();
  glColor4f(1,1,1,1);
  glTranslatef(0,0,-40);
  float dazu=(float)(SDL_GetTicks())/5000.0;
  dazu=dazu-trunc(dazu);
  ZWusetexture(data->gameinfo->sky_menu_tex);
  glBegin(GL_QUADS);
    glTexCoord2f(0+dazu,0);glVertex3f(-31,23,0);
    glTexCoord2f(0+dazu,1);glVertex3f(-31,-3,0);
    glTexCoord2f(1+dazu,1);glVertex3f(31,-3,0);
    glTexCoord2f(1+dazu,0);glVertex3f(31,23,0);  
  glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(-90,1,0,0);
	glColor4f(1,1,1,1);
	ZWdrawsprite(data->texture,0,20,-1,30.0,20.0,2,1);
	glPopMatrix();	

	glEnable(GL_LIGHTING);
	
	glColor4f(1,1,1,1);
		
	glPushMatrix();
	glTranslatef(1,0,-7.5);
	ZWdrawobject(data->ruebe);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.35,0,-7.5);
	glRotatef(data->schildposition,1,0,0);
	glScalef(4,4,4);
	ZWdrawobject(data->schild);
	sprintf(buffer,"[%c]",data->the_button+'A');
	glNormal3f(0,0,1);
	switch (data->the_button)
	{
		case 0: glColor4f(1,0,0,0.8); break;
		case 1: glColor4f(0,0,1,0.8); break;
		case 2: glColor4f(0,1,0,0.8); break;
		default: glColor4f(1,1,0,0.8); break;		
	}
	glDepthFunc(GL_ALWAYS);
	ZWdrawtextmiddle((ZWtext)(data->text),0.03,0.69,0.03,buffer,0.4);
	glDepthFunc(GL_LEQUAL);
	glPopMatrix();
	
	int player;
	for (player=0;player<data->gameinfo->playernum;player++)
	{
		glPushMatrix();
		glTranslatef(((float)player*2.0-((float)(data->gameinfo->playernum)-1.0))*0.27,-0.75,-3.0);
		sprintf(buffer,"%i",data->points[player]);
		glColor4f(1,1,1,1);
		glNormal3f(0,0,1);
		ZWdrawtextmiddle((ZWtext)(data->text),0,0.4,0.0,buffer,0.5);
		switch (data->points[player])
		{
			case 1: glScalef(0.17,0.17,0.17); break;
			case 2: glScalef(0.20,0.20,0.20); break;
			case 3: glScalef(0.23,0.23,0.23); break;
			default: glScalef(0.14,0.14,0.14); break;
		}
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glColor4f(data->gameinfo->color[player].r,data->gameinfo->color[player].g,data->gameinfo->color[player].b,1.0-(float)data->blocked[player]*0.4);
		ZWusetexture(data->gameinfo->facetex);
		glCallList(data->gameinfo->ball[player]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glPopMatrix();
	}

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
  int b;
  for (b=0;b<data->gameinfo->playernum;b++)
    gameresult->winner[b]=data->points[b]>=3;

		

  //Texturen und Objekte freigeben
  ZWdeleteobject(data->ruebe);    
  ZWdeleteobject(data->schild);    
  ZWdeletetexture(data->texture);

    
  //Musik zurÃ¼cksetzen:
  Mix_VolumeMusic(data->old_music_volume);
  Mix_FreeMusic(data->backgroundmusic);
  Mix_FreeChunk(data->snd_negative);  
  Mix_FreeChunk(data->snd_positive);  
}
