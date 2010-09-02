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

#define WAITTIME 1000
#ifdef WIN32
#define bool int
#define true 1
#define false 0
#endif

typedef struct sdata *pdata;
typedef struct sdata {
  int step;
  float fade;
  int countdown;
  ZWtext text;
  pgameinfo gameinfo;
  Mix_Chunk *snd_win;
  Mix_Chunk *snd_drum;
  Mix_Music *backgroundmusic;
  int old_music_volume;
	object3d schlaeger;
	object3d drum;
	object3d boat;
	object3d paddle;
	GLuint texture;
	GLuint water;
	char the_button;
	int schlaeger_step;
	float ist_position[8];
	float soll_position[8];
	float paddle_schlag[8];
	bool chance[8];
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
	sprintf(buffer,"%ssounds/i_dont_know_exactly_what_this_is.ogg",gameinfo->datafolder);
  data->snd_win=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_win,(int)(round(128.0*gameinfo->volume)));
	sprintf(buffer,"%ssounds/kettledrum.ogg",gameinfo->datafolder);
  data->snd_drum=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_drum,(int)(round(128.0*gameinfo->volume)));
	sprintf(buffer,"%ssounds/waves.ogg",gameinfo->datafolder);
  data->backgroundmusic = Mix_LoadMUS(buffer);
  data->old_music_volume=Mix_VolumeMusic((int)(trunc(96.0*gameinfo->volume)));
  Mix_FadeInMusic(data->backgroundmusic,-1,500);

  //Objekte laden  
  sprintf(buffer,"%stextures/water.png",gameinfo->datafolder);
  data->water=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
  sprintf(buffer,"%stextures/beat_paddle.png",gameinfo->datafolder);
  data->texture=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
  sprintf(buffer,"%smodels/drum.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->drum),buffer,data->texture);
  ZWcreatedrawlist(&(data->drum),gameinfo->lightquali);
	sprintf(buffer,"%smodels/schlaeger.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->schlaeger),buffer,data->texture);
  ZWcreatedrawlist(&(data->schlaeger),0);
	sprintf(buffer,"%smodels/boat.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->boat),buffer,data->texture);
  ZWcreatedrawlist(&(data->boat),0);
	sprintf(buffer,"%smodels/paddle.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->paddle),buffer,data->texture);
  ZWcreatedrawlist(&(data->paddle),0);

  //Licht!
  ZWenablelight(0);
  ZWsetlightambient(0,0.0,0.0,0.0);
  ZWsetlightdiffuse(0,1.0,1.0,1.0);
  ZWsetlightposition(0,0.0,0.0,2.0);
  ZWsetmaterial(lnone);
	
	for (a=0;a<gameinfo->playernum;a++)
	{
		data->soll_position[a]=0;
		data->ist_position[a]=0;
		data->paddle_schlag[a]=0;
		data->chance[a]=false;
	}
  
  //Spiel ansich vorbereiten
  data->step=0;
  data->fade=1;
  data->countdown=3000;
	data->schlaeger_step=600;
	data->the_button=-1;
}

EXPORT int op_calc_game(pdata data)
{ 
	int player;
  int steps;
  for (steps=0;steps<*(data->gameinfo->steps);steps++)
		for (player=0;player<data->gameinfo->playernum;player++)
			data->ist_position[player]+=(data->soll_position[player]-data->ist_position[player])*0.01;

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
  for (steps=0;steps<*(data->gameinfo->steps);steps++)
  {
		data->schlaeger_step++;
	  if (data->schlaeger_step>=400+2*WAITTIME)
		  data->schlaeger_step=0;
		if (data->schlaeger_step==0 || data->schlaeger_step==WAITTIME+200)
		{
			data->the_button=rand()%4;
		  Mix_PlayChannel(-1,data->snd_drum,0);
			for (player=0;player<data->gameinfo->playernum;player++)
			  data->chance[player]=true;
		}
		  
		
		for (player=0;player<data->gameinfo->playernum;player++)
		{
			if (data->chance[player] && (*(data->gameinfo->joystick[player].button[data->the_button]) || (data->gameinfo->iscpu[player] && rand()%150==0)))
			{
				*(data->gameinfo->joystick[player].button[data->the_button])=0;
				data->chance[player]=false;
				data->soll_position[player]+=1.0;
				data->paddle_schlag[player]-=1.0;
			}
			int o1,o2,o3;
			switch (data->the_button)
			{
				case 0: o1=1;o2=2;o3=3; break;
				case 1: o1=0;o2=2;o3=3; break;
				case 2: o1=1;o2=0;o3=3; break;
				case 3: o1=1;o2=2;o3=0; break;
			}
			if (data->chance[player] && (*(data->gameinfo->joystick[player].button[o1]) ||
			                             *(data->gameinfo->joystick[player].button[o2]) ||
																	 *(data->gameinfo->joystick[player].button[o3]) || (data->gameinfo->iscpu[player] && rand()%300==0)))
			{
				*(data->gameinfo->joystick[player].button[o1])=0;
				*(data->gameinfo->joystick[player].button[o2])=0;
				*(data->gameinfo->joystick[player].button[o3])=0;
				data->chance[player]=false;
			}
			if (data->ist_position[player]>=24.95)
			{
				data->step=2;
				data->fade=-1.0;
				Mix_PlayChannel(-1,data->snd_win,0);
				return 0;
			}
			if (data->paddle_schlag[player]<0.0)
			{
				data->paddle_schlag[player]+=0.0015;
				if (data->paddle_schlag[player]>=0.0)
				  data->paddle_schlag[player]=0.0;
			}
			
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

	
	glRotatef(45,1,0,0);
	
	glTranslatef(0,-15,-5);

	glEnable(GL_LIGHTING);


	int player;
	for (player=0;player<data->gameinfo->playernum;player++)
	{
    glPushMatrix();
			glTranslatef(((float)player-(float)(data->gameinfo->playernum-1)/2.0)*3.5,-0.8,-30.0+data->ist_position[player]);
			glPushMatrix();
				glScalef(0.7,0.7,0.7);
				glTranslatef(0,1.0,-1.2);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				glColor4f(data->gameinfo->color[player].r,data->gameinfo->color[player].g,data->gameinfo->color[player].b,1.0);
				ZWusetexture(data->gameinfo->facetex);
				glCallList(data->gameinfo->ball[player]);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);			
				glColor4f(1,1,1,1);
			glPopMatrix();
		  glScalef(2.3,2.3,2.3);
			ZWdrawobject(data->boat);
			glPushMatrix();
			  glTranslatef(0.2,0.2,0);
				float position=data->paddle_schlag[player]*360.0+80.0;
				glScalef(0.8,0.8,0.8);
				glRotatef(position,1,0,0);
				glRotatef(45,0,0,1);
				ZWdrawobject(data->paddle);
			glPopMatrix();
			glPushMatrix();
			  glTranslatef(-0.2,0.2,0);
				glScalef(0.8,0.8,0.8);
				glRotatef(position,1,0,0);
				glRotatef(135,0,0,1);
				ZWdrawobject(data->paddle);
			glPopMatrix();

    glPopMatrix();
	}

	glDisable(GL_LIGHTING);

	
	
	glPushMatrix();
	glRotatef(-90,1,0,0);
	glColor4f(1,1,1,1);
	ZWdrawsprite(data->texture,0, 3,-1.05,15.0,0.2,4,12);
	ZWdrawsprite(data->texture,0,45,-0.9,45.0,10.0,2,3);
	ZWdrawsprite(data->water  ,0,18,-1.2,32.0,19.0,1,0);
	glColor4f(1,1,1,0.6);
	ZWdrawsprite(data->water,sin((float)(SDL_GetTicks())/500.0)*1.3,cos((float)(SDL_GetTicks())/500.0)*0.7+18.0,-1.1,32.0,20.0,1,0);
	glColor4f(1,1,1,0.4);
	ZWdrawsprite(data->water,sin((float)(SDL_GetTicks())/700.0)*0.7,cos((float)(SDL_GetTicks())/700.0)*1.3+18.0,-1,32.0,20.0,1,0);
	glPopMatrix();	

	glEnable(GL_LIGHTING);
	
	glColor4f(1,1,1,1);
	
	
	int a;
	for (a=0;a<8;a++)
	{
		glPushMatrix();
		glTranslatef(((float)a-3.5)*7.5,-0.5,-37.0);
		glPushMatrix();
		  glTranslatef(-1.5,2.1,-1);
			if (data->schlaeger_step>=WAITTIME && data->schlaeger_step<200+WAITTIME)
			  glRotatef((float)(data->schlaeger_step-WAITTIME)/2.0,1,0,-1);
			if (data->schlaeger_step>=200+WAITTIME && data->schlaeger_step<200+2*WAITTIME)
			  glRotatef(100.0,1,0,-1);
			if (data->schlaeger_step>=200+2*WAITTIME)
			  glRotatef((float)(400+2*WAITTIME-data->schlaeger_step)/2.0,1,0,-1);
			glScalef(2,2,2);
			ZWdrawobject(data->schlaeger);
		glPopMatrix();
		glPushMatrix();
		  glTranslatef(1.5,2.1,-1);
			if (data->schlaeger_step<WAITTIME)
			  glRotatef(100.0,1,0,1);
			if (data->schlaeger_step>=WAITTIME && data->schlaeger_step<200+WAITTIME)
			  glRotatef((float)(200+WAITTIME-data->schlaeger_step)/2.0,1,0,1);
			if (data->schlaeger_step>=200+2*WAITTIME)
			  glRotatef((float)(data->schlaeger_step-(200+2*WAITTIME))/2.0,1,0,1);
			glScalef(2,2,2);
			ZWdrawobject(data->schlaeger);
		glPopMatrix();
		glScalef(2,2,2);
		glColor4f(1,1,1,1);
		ZWdrawobject(data->drum);
		if (data->schlaeger_step>=0 && data->schlaeger_step<100)
		{
			glColor4f(1,1,1,0.7);
			glPushMatrix();
				glTranslatef(sin((float)(data->schlaeger_step)/100.0*M_PI)*0.05,0,0);
				ZWdrawobject(data->drum);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-sin((float)(data->schlaeger_step)/100.0*M_PI)*0.05,0,0);
				ZWdrawobject(data->drum);
			glPopMatrix();
			glColor4f(1,1,1,0.4);
			glPushMatrix();
				glTranslatef(sin((float)(data->schlaeger_step)/100.0*M_PI)*0.1,0,0);
				ZWdrawobject(data->drum);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-sin((float)(data->schlaeger_step)/100.0*M_PI)*0.1,0,0);
				ZWdrawobject(data->drum);
			glPopMatrix();
			glColor4f(1,1,1,1);
		}
		if (data->schlaeger_step>=200+WAITTIME && data->schlaeger_step<300+WAITTIME)
		{
			glColor4f(1,1,1,0.7);
			glPushMatrix();
				glTranslatef(sin((float)(data->schlaeger_step-200-WAITTIME)/100.0*M_PI)*0.05,0,0);
				ZWdrawobject(data->drum);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-sin((float)(data->schlaeger_step-200-WAITTIME)/100.0*M_PI)*0.05,0,0);
				ZWdrawobject(data->drum);
			glPopMatrix();
			glColor4f(1,1,1,0.4);
			glPushMatrix();
				glTranslatef(sin((float)(data->schlaeger_step-200-WAITTIME)/100.0*M_PI)*0.1,0,0);
				ZWdrawobject(data->drum);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-sin((float)(data->schlaeger_step-200-WAITTIME)/100.0*M_PI)*0.1,0,0);
				ZWdrawobject(data->drum);
			glPopMatrix();
		}
		if (data->the_button>=0)
		{
			sprintf(buffer,"[%c]",data->the_button+'A');
			glNormal3f(0,0,1);      
			float alpha=1.0-0.4*(float)(data->schlaeger_step%(200+WAITTIME))/(float)(200+WAITTIME);
      glColor4f(1,1,1,alpha);
      ZWdrawsprite(data->gameinfo->maintex,0.1,0.4,1.5,0.6,0.6,2,2);
      
			switch (data->the_button)
			{
				case 0: glColor4f(0.4,0.1,0.1,alpha); break;
				case 1: glColor4f(0,0,0.7,alpha); break;
				case 2: glColor4f(0,0.4,0.0,alpha); break;
				default: glColor4f(0.4,0.3,0.1,alpha); break;		
			}
			glDepthFunc(GL_ALWAYS);
			ZWdrawtextmiddle((ZWtext)(data->text),0.1,0.2,1.5,buffer,1.5);
			glDepthFunc(GL_LEQUAL);
		}
		glPopMatrix();
		glColor4f(1,1,1,1);
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
    gameresult->winner[b]=data->ist_position[b]>=24.95;

		

  //Texturen und Objekte freigeben
  ZWdeleteobject(data->drum);    
  ZWdeleteobject(data->schlaeger);    
  ZWdeleteobject(data->boat);    
  ZWdeleteobject(data->paddle);    
  ZWdeletetexture(data->texture);
  ZWdeletetexture(data->water);

    
  //Musik zurücksetzen:
  Mix_VolumeMusic(data->old_music_volume);
  Mix_FreeMusic(data->backgroundmusic);
  Mix_FreeChunk(data->snd_win);  
  Mix_FreeChunk(data->snd_drum);  
}
