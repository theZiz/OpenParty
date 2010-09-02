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

#include "./../../../zizutils/ttf/ttf.h"
#include "./../../../zizutils/texture/texture.h"
#include "./../../../zizutils/partikel/partikel.h"

#include "./../../testgame/minigames.h"

#ifdef WIN32
#define bool char
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
  Mix_Music *backgroundmusic;
  Mix_Chunk *snd_next;
	Mix_Chunk *snd_negative;
  object3d mini;
  GLuint texture;
	GLuint street;
  int old_music_volume;
  int gang[8];
  float progress[8];
  float progress_is[8];
  float pointer[8];
	float position[8];
	float timeout[8];
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
	sprintf(buffer,"%ssounds/Feelin Good.ogg",gameinfo->datafolder);
  data->backgroundmusic = Mix_LoadMUS(buffer);
  data->old_music_volume=Mix_VolumeMusic((int)(trunc(96.0*gameinfo->volume)));
  Mix_FadeInMusic(data->backgroundmusic,-1,500);
	sprintf(buffer,"%ssounds/brumm.ogg",gameinfo->datafolder);
  data->snd_next=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_next,(int)(trunc(128.0*gameinfo->volume)));
	sprintf(buffer,"%ssounds/negative.ogg",gameinfo->datafolder);
  data->snd_negative=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_negative,(int)(trunc(16.0*gameinfo->volume)));

  //Objects laden
	sprintf(buffer,"%stextures/mini.png",gameinfo->datafolder);
  data->texture=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
	sprintf(buffer,"%stextures/street.png",gameinfo->datafolder);
  data->street=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
	sprintf(buffer,"%smodels/mini.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->mini),buffer,data->texture);
  ZWcreatedrawlist(&(data->mini),data->gameinfo->lightquali);

  //Licht
  ZWenablelight(0);
  ZWsetlightambient(0,0.1,0.1,0.1);
  ZWsetlightdiffuse(0,1.5,1.5,1.5);
  ZWsetlightposition(0,20.0,5.0,20.0);
  ZWsetmaterial(lnone);

  
	for (a=0;a<gameinfo->playernum;a++)
	{
		data->gang[a]=0;
		data->progress[a]=0.0;
		data->progress_is[a]=0.0;
		data->pointer[a]=0.0;
		data->position[a]=0.0;
		data->timeout[a]=0.0;
	}
	
  data->step=0;
  data->fade=1;
  data->countdown=2500;  
}


EXPORT int op_calc_game(pdata data)
{ 
  ZWcalcpartikel(*(data->gameinfo->steps));
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
  if (data->countdown>0) //Countdown
  {
    data->countdown-=*(data->gameinfo->steps);
    return 0;
  }
	
  //zeitabhängige Bewegung:
  int time;
  for (time=0;time<(int)(*(data->gameinfo->steps));time++)
  {

  }


  if (data->step==2) //Fade Out
  {
    data->fade+=(float)(*(data->gameinfo->steps))/1500.0;
    if (data->fade>=1.0)
    {
      data->fade=1.0;
      return 1;
    }
    return 0;
  }
	
  if (false) //Ende
	{
		data->step=2;
		Mix_FadeOutMusic(500);
}

  return 0;
}

EXPORT int op_calc_game_thread(pdata data)
{ 
  srand(SDL_GetTicks()*time(NULL));
  if (*(data->gameinfo->shouldend))
    return 1;
		
	int player;
	for (player=0;player<data->gameinfo->playernum;player++)
	{
		data->position[player]+=(float)(*(data->gameinfo->steps))*0.003*(float)(data->gang[player])/5.0;
		if (data->position[player]>=1.0)
		  data->position[player]-=1.0;
			
		data->pointer[player]+=(float)(*(data->gameinfo->steps))*0.0015*sqrt((float)(data->gang[player]+1));
		if (data->pointer[player]>=2.0*M_PI)
		  data->pointer[player]-=2.0*M_PI;
        int step;
		for (step=0;step<(int)(*(data->gameinfo->steps));step++)
		{
			if (data->progress[player]-data->progress_is[player]>0.001)
				data->progress_is[player]+=0.001;
			else
			if (data->progress[player]-data->progress_is[player]<-0.001)
				data->progress_is[player]-=0.001;
				
			if (data->timeout[player]>0.0)
			  data->timeout[player]-=0.001;
		}
	}

		
	if (data->countdown>0)
	  return 0;
	
	for (player=0;player<data->gameinfo->playernum;player++)
	{
        int step;
		for (step=0;step<(int)(*(data->gameinfo->steps));step++)
			if ((*(data->gameinfo->joystick[player].button[0]) || (data->gameinfo->iscpu[player] &&
					rand()%700<(-0.8+cos(fabs(cos(data->pointer[player]))*M_PI))*20.0))
					&& (data->timeout[player]<=0.0) && (data->gang[player]<5))
			{
				data->timeout[player]=0.5;
				*(data->gameinfo->joystick[player].button[0])=0;
				float faktor=cos(fabs(cos(data->pointer[player]))*M_PI);
				data->progress[player]+=fabs(faktor)*faktor*0.25;
				if (data->progress[player]<0.0)
				{
					if (data->gang[player]==0)
						data->progress[player]=0.0;
					else
					{
						data->progress[player]+=1.0;
						data->gang[player]--;
						Mix_PlayChannel(-1,data->snd_negative,0);
					}
				}
				if (data->progress[player]>=1.0)
				{
					data->progress[player]-=1.0;
					data->gang[player]++;
					Mix_PlayChannel(-1,data->snd_next,0);
					if (data->gang[player]==5)
					{
						data->step=2;
						//data->fade=-5.0;
					}
				}
			}
	}
  return 0;
}


void set_viewport(pdata data,float x,float y,float w,float h)
{
  //Viewport neu einstellen:
  int width=data->gameinfo->windowx;
  int height=data->gameinfo->windowy;
  if (data->gameinfo->fullscreen)
  {
    width=data->gameinfo->resolutionx;
    height=data->gameinfo->resolutiony;
  }
  float faktor=((float)width/(float)height)/(4.0f/3.0f);
  if (data->gameinfo->stretched==0)
  {
    if (faktor<1)
      glViewport((float)width*x , ((float)height-(float)height*faktor)/2.0+(float)height*y ,
                 (float)width*w , (float)height*faktor*h                                   );
    else
      glViewport(((float)width-(float)width/faktor)/2.0+(float)width*x , (float)height*y ,
                 (float)width*w/faktor                                 , (float)height*h );
  }
  else
    glViewport((float)width*x,(float)height*y,(float)width*w,(float)height*h);
    
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 60.0, (float)width*w/(faktor*(float)height*h), 0.1,100);
  glMatrixMode( GL_MODELVIEW );
    
}

EXPORT void op_draw_game(pdata data)
{
  char buffer[256];
  int player;
  for (player=0;player<data->gameinfo->playernum;player++)
  {
    switch (data->gameinfo->playernum)
    {
      case 1: set_viewport(data,0,0,1,1); break;
      case 2: set_viewport(data,0,0.5-(float)player*0.5,1,0.5); break;
      case 3: //set_viewport(data,0,0.666-(float)player*0.333,1,0.333); break;
      case 4: set_viewport(data,(float)(player%2)*0.5,(float)(1-player/2)*0.5,0.5,0.5); break;
      case 5: 
      case 6: set_viewport(data,(float)(player%2)*0.5,(float)(2-player/2)*0.333,0.5,0.333); break;
      case 7: 
      case 8: set_viewport(data,(float)(player%2)*0.5,(float)(3-player/2)*0.25,0.5,0.25); break;
    }
		glEnable(GL_LIGHTING);
		glLoadIdentity();
		glTranslatef(0,0.4,-2.5);
		float delta=1.0;
		int a,b,c;
		glNormal3f(0,0,1);
		glColor4f(1,1,1,1);
		glTranslatef(0,-delta*3.0*data->position[player],0);
		for (c=0;c<2;c++)
			for (a=0;a<8;a++)
				for (b=0;b<6;b++)
					ZWdrawsprite(data->street,((float)a-3.5)*delta,((float)(-b+6*c)+2.5)*delta/2.0-0.4,0,delta/2.0,delta/4.0,8,a+b*8);
		glTranslatef(0,delta*3.0*data->position[player],0);
		glRotatef(90,1,0,0);
		glRotatef(180,0,1,0);
		glRotatef(sin((float)(SDL_GetTicks())*0.013*(float)(data->gang[player])/5.0)*5.0*(float)(data->gang[player])/5.0,1,0,0);
		glRotatef(cos((float)(SDL_GetTicks())*0.009*(float)(data->gang[player])/5.0)*3.0*(float)(data->gang[player])/5.0,0,0,1);
		glColor4f(data->gameinfo->color[player].r,data->gameinfo->color[player].g,data->gameinfo->color[player].b,1);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		ZWdrawobject(data->mini);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glRotatef(180,0,-1,0);
		glRotatef(90,-1,0,0);
		glNormal3f(0,0,1);
		glColor4f(1,1,1,1);
	  ZWdrawsprite(data->gameinfo->facetex,0,-0.4,1,0.3,0.3,4,data->gameinfo->facenr[player]);
    glDisable(GL_LIGHTING);
		glLoadIdentity();
    glDepthFunc(GL_ALWAYS);  

    //HUD
		glTranslatef(-0.65,-0.95,0);
		ZWusetexture(data->street);
		glBegin(GL_QUADS);
		  glTexCoord2f(0.625*data->progress_is[player],0.750); glVertex3f(-0.85+1.7*data->progress_is[player], 0.1,-2);
		  glTexCoord2f(0.625*data->progress_is[player],0.875); glVertex3f(-0.85+1.7*data->progress_is[player],-0.1,-2);
		  glTexCoord2f(0.625                       ,0.875); glVertex3f( 0.85,-0.1,-2);
		  glTexCoord2f(0.625                       ,0.750); glVertex3f( 0.85, 0.1,-2);		
		glEnd();
		glBegin(GL_QUADS);
		  glTexCoord2f(0.000                       ,0.875); glVertex3f(-0.85, 0.1,-2);
		  glTexCoord2f(0.000                       ,1.000); glVertex3f(-0.85,-0.1,-2);
		  glTexCoord2f(0.625*data->progress_is[player],1.000); glVertex3f(-0.85+1.7*data->progress_is[player],-0.1,-2);
		  glTexCoord2f(0.625*data->progress_is[player],0.875); glVertex3f(-0.85+1.7*data->progress_is[player], 0.1,-2);		
		glEnd();
		
		sprintf(buffer,"Gang: %i",data->gang[player]);
	  glColor4f(0,0,0,0.8);
		ZWdrawtext((ZWtext)(data->text),-0.74,0.21,-2,buffer,0.3);  
		switch(data->gang[player])
		{
		  case 0: glColor4f(0.5,0.5,0.5,1); break;
		  case 1: glColor4f(0,0,1,1); break;
		  case 2: glColor4f(0.5,0.5,0.25,1); break;
		  case 3: glColor4f(1,1,0,1); break;
		  case 4: glColor4f(1,0.5,0,1); break;
		  case 5: glColor4f(1,0,0,1); break;
	  }
		ZWdrawtext((ZWtext)(data->text),-0.75,0.2,-2,buffer,0.3);  
		glColor4f(1,1,1,1);
		glTranslatef(1.55,0.05,0);
		ZWusetexture(data->street);
		glBegin(GL_QUADS);
		  glTexCoord2f(0.625,0.75); glVertex3f(-0.6, 0.2,-2);
		  glTexCoord2f(0.625,1.00); glVertex3f(-0.6,-0.2,-2);
		  glTexCoord2f(1.000,1.00); glVertex3f( 0.6,-0.2,-2);
		  glTexCoord2f(1.000,0.75); glVertex3f( 0.6, 0.2,-2);		
		glEnd();
		
		if (data->timeout[player]<=0.0)
		  glColor4f(1,1,1,0.9);
		else
		  glColor4f(1,1,1,0.2);
		ZWdrawsprite(data->texture,cos(data->pointer[player])*0.55,0,-2,0.1,0.2,16,0);		  

		glLoadIdentity();
		//Countdown
		if (data->countdown>0)
		{
			sprintf(buffer,"%i",data->countdown/1000+1);
			glColor4f(0,0,0,0.5);
			ZWdrawtextmiddle((ZWtext)(data->text),0,-0.42,-4,buffer,2.15);  
			ZWdrawtextmiddle((ZWtext)(data->text),0,-0.44,-4,buffer,2.3);
			switch (data->countdown/1000+1)
			{
				case  1: glColor4f(0.5,1.0,0.5,0.9); break;
				case  2: glColor4f(1.0,1.0,0.5,0.9); break;
				default: glColor4f(1.0,0.5,0.5,0.9);
			}
			ZWdrawtextmiddle((ZWtext)(data->text),0,-0.4,-4,buffer,2.0);  
		}
		
    //Fade
    glLoadIdentity();
    if (data->step!=1)
    {
      glDisable(GL_TEXTURE_2D);
      glColor4f(0,0,0,data->fade);
      glBegin(GL_QUADS);
        glVertex3f(-2,2,-1);
        glVertex3f(-2,-2,-1);
        glVertex3f(2,-2,-1);
        glVertex3f(2,2,-1);      
      glEnd();
      glEnable(GL_TEXTURE_2D);
    }

    glDepthFunc(GL_LEQUAL);
  }
    
  set_viewport(data,0,0,1,1);
}

EXPORT void op_reset_game(pdata data,pgameresult gameresult)
{
	int b;
  for (b=0;b<data->gameinfo->playernum;b++)
    gameresult->winner[b]=data->gang[b]>=5; 
  ZWdeleteobject(data->mini);
  ZWdeletetexture(data->texture);
  ZWdeletetexture(data->street);
  ZWdisablelight(0);
     
  //Musik zurücksetzen:
  Mix_VolumeMusic(data->old_music_volume);
  Mix_FreeMusic(data->backgroundmusic);
  Mix_FreeChunk(data->snd_next);
  Mix_FreeChunk(data->snd_negative);
}
