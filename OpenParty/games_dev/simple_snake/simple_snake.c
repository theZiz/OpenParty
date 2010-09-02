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
#define SNAKESIZE 2
//typedef char unsigned bool;

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
  float circle;
  Sint32 countdown;
  Sint32 zeit;
  Uint32 *steps;
  ZWtext text;
  float richtung[8];
  float x[8];
  float y[8];
  int maxx,maxy;
  char *feld;
  char *draw;
  GLuint texture;
  int playernum;
  int texquali;
  tOPcolor color[8];
  tOPjoystick joystick[8];
  char alive[8];
  char iscpu[8];
  int cpudir[8];
  GLuint maintex;
	GLuint facetex;
  int facenr[8];
  int *shouldend;
  Mix_Music *backgroundmusic;
  int old_music_volume;
  Mix_Chunk *snd_explosion;
	float speed_up;
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

void draw_pixel(pdata data)
{
  int a;
  for (a=0;a<data->playernum;a++)
  {
    int x=(int)(trunc(data->x[a]*(float)data->maxx));
    if (x>data->maxx-1) 
      x=data->maxx-1;
    int y=(int)(trunc(data->y[a]*(float)data->maxy));
    if (y>data->maxy-1) 
      y=data->maxy-1;
    int b,c;
    for (b=-(4/data->texquali)*SNAKESIZE;b<(4/data->texquali)*SNAKESIZE;b++)
      for (c=-(4/data->texquali)*SNAKESIZE;c<(4/data->texquali)*SNAKESIZE;c++)
        if (b*b+c*c<(4/data->texquali)*SNAKESIZE*(4/data->texquali)*SNAKESIZE)
        {
          float rfaktor=0.5-data->color[a].r;
          float gfaktor=0.5-data->color[a].g;
          float bfaktor=0.5-data->color[a].b;
          float abstand=sqrt((float)(b*b+c*c))/(float)((4/data->texquali)*SNAKESIZE);

          float red=0.5-rfaktor*(1.0-abstand*0.7);
          float green=0.5-gfaktor*(1.0-abstand*0.7);
          float blue=0.5-bfaktor*(1.0-abstand*0.7);
					int dest=(x+b+(y+c)*data->maxy);
					if (dest<0 || dest>=data->maxx*data->maxy)
					  continue;
          data->draw[dest*3+0]=(int)(red*255.0);
          data->draw[dest*3+1]=(int)(green*255.0);
          data->draw[dest*3+2]=(int)(blue*255.0);
        }
    x=(int)(trunc(data->x[a]*256.0));
    if (x>255) 
      x=255;
    y=(int)(trunc(data->y[a]*256.0));
    if (y>255) 
      y=255;
    for (b=-SNAKESIZE;b<SNAKESIZE;b++)
      for (c=-SNAKESIZE;c<SNAKESIZE;c++)
        if (b*b+c*c<SNAKESIZE*SNAKESIZE)
				{
					int dest=x+b+(y+c)*256;
					if (dest<0 || dest>=256*256)
					  continue;
          data->feld[dest]=255;
				}
  }
}

EXPORT void op_init_game(pdata data,pgameinfo gameinfo)
{
  //Text initialisieren
  data->text.texture=gameinfo->texttexture;
  int a;
  for (a=0;a<256;a++)
    data->text.breite[a]=gameinfo->textwidth[a];
  
  //Gezeichnetes und Feld resetten
  data->maxx=1024/gameinfo->texquali;
  data->maxy=1024/gameinfo->texquali;
  data->feld=(char*)malloc(256*256);
  data->draw=(char*)malloc(data->maxx*data->maxy*3);  
  memset(data->draw,0,data->maxx*data->maxy*3);
  memset(data->feld,0,256*256);
  int b;
  for (a=0;a<data->maxx;a++)
    for (b=0;b<data->maxy;b++)
    {
      if (a<(4/gameinfo->texquali)*SNAKESIZE || a>data->maxx-1-(4/gameinfo->texquali)*SNAKESIZE ||
          b<(4/gameinfo->texquali)*SNAKESIZE || b>data->maxy-1-(4/gameinfo->texquali)*SNAKESIZE)
      {
        data->draw[(a+b*data->maxy)*3+0]=255;
        data->draw[(a+b*data->maxy)*3+1]=255;
        data->draw[(a+b*data->maxy)*3+2]=255;
      }
    }
  for (a=0;a<256;a++)
    for (b=0;b<256;b++)
    {
      if (a<SNAKESIZE || a>255-SNAKESIZE ||
          b<SNAKESIZE || b>255-SNAKESIZE)
        data->feld[a+b*256]=255;
    }
  for (a=0;a<8;a++)
  {
    data->color[a]=gameinfo->color[a];
    data->joystick[a]=gameinfo->joystick[a];
    data->iscpu[a]=gameinfo->iscpu[a];
    data->facenr[a]=gameinfo->facenr[a];
  }
  data->texquali=gameinfo->texquali;
  data->playernum=gameinfo->playernum;
  float dazu=(float)(rand()%36000)*M_PI/18000.0;
  for (a=0;a<data->playernum;a++)
  {
    data->x[a]=0.5+sin((float)a/(float)(data->playernum)*2.0*M_PI+dazu)*0.25;
    data->y[a]=0.5+cos((float)a/(float)(data->playernum)*2.0*M_PI+dazu)*0.25;
    data->richtung[a]=(float)a/(float)(data->playernum)*2.0*M_PI+dazu;
    if (data->richtung[a]>2.0*M_PI)
      data->richtung[a]-=2.0*M_PI;
    data->alive[a]=1;
    data->cpudir[a]=0;
  }
  draw_pixel(data);

  glGenTextures(1, &(data->texture));
  ZWusetexture(data->texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, data->maxx, data->maxy,0, GL_RGB, GL_UNSIGNED_BYTE, data->draw);

  char buffer[1024];

  sprintf(buffer,"%ssounds/Vegas Glitz.ogg",gameinfo->datafolder);
  data->backgroundmusic = Mix_LoadMUS(buffer);
  data->old_music_volume=Mix_VolumeMusic((int)(trunc(96.0*gameinfo->volume)));
  Mix_FadeInMusic(data->backgroundmusic,-1,500);
	sprintf(buffer,"%ssounds/explosion.ogg",gameinfo->datafolder);
  data->snd_explosion=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_explosion,(int)(trunc(16.0*gameinfo->volume)));

  data->step=0;
  data->fade=1;
  data->shouldend=gameinfo->shouldend;
  data->maintex=gameinfo->maintex;
  data->facetex=gameinfo->facetex;
  data->steps=gameinfo->steps;
  data->zeit=30000;
  data->circle=0;
  data->countdown=2500;  
	
	data->speed_up=0.1;
}

int get_hindernis_distance(pdata data,float fx,float fy,float richtung,int max)
{
  int a;
  int x=(int)(trunc(fx*256.0));
  if (x>255)
    x=255;
  int y=(int)(trunc(fy*256.0));
  if (y>255)
    y=255;
  
  for (a=5;a<=max;a++)
  {
    int rx=x+(int)(trunc(sin(richtung)*(float)a));
    int ry=y+(int)(trunc(cos(richtung)*(float)a));
    if ((Uint8)(data->feld[rx+ry*256])==255)
      return a;
  }
  return max+1;
}

EXPORT int op_calc_game(pdata data)
{ 
  ZWcalcpartikel(*(data->steps));
glClearColor(sin(data->circle*M_PI/180.0)*0.5+0.5,sin(data->circle*M_PI/90.0)*0.5+0.5,sin(data->circle*M_PI/45.0)*0.5+0.5,0);

  if (data->step==0) //Fade In
  {
    data->fade-=(float)(*(data->steps))/500.0;
    if (data->fade<=0.0)
    {
      data->fade=0.0;
      data->step=1;
    }
    return 0;
  }
  if (data->step==2) //Fade Out
  {
    data->fade+=(float)(*(data->steps))/500.0;
    if (data->fade>=1.0)
    {
      data->fade=1.0;
      return 1;
    }
    return 0;
  }
  data->circle+=(float)(*(data->steps))/20.0;
  while (data->circle>=360.0)
    data->circle-=360.0;
  if (data->countdown>0)
  {
    data->countdown-=*(data->steps);
    return 0;
  }
  int steps;
  for (steps=0;steps<*(data->steps);steps++)
  {
		if (data->speed_up<1.0)
		{
			data->speed_up+=0.0004;
			if (data->speed_up>1.0)
			  data->speed_up=1.0;
		}
		
    int a;
    for (a=0;a<data->playernum;a++)
    if (data->alive[a])
    {
      if (data->iscpu[a])
      {
        int links=get_hindernis_distance(data,data->x[a],data->y[a],data->richtung[a]+0.8,20);
        int vorne=get_hindernis_distance(data,data->x[a],data->y[a],data->richtung[a],20);
        int rechts=get_hindernis_distance(data,data->x[a],data->y[a],data->richtung[a]-0.8,20);
        if (vorne<21 && links>rechts && data->cpudir[a]==0)
          data->cpudir[a]=rand()%256;
        if (vorne<21 && rechts>=links && data->cpudir[a]==0)
          data->cpudir[a]=-rand()%256;
          
        if (data->cpudir[a]>0)
          data->richtung[a]+=0.004*data->speed_up;
        if (data->cpudir[a]<0)          
          data->richtung[a]-=0.004*data->speed_up;
          
        if (data->cpudir[a]>0)
          data->cpudir[a]--;
        if (data->cpudir[a]<0)
          data->cpudir[a]++;
      }
      else
      {
        if (*(data->joystick[a].axis[0])>0 && *(data->joystick[a].axis[1])==0) //Rechts
        {
          if (data->richtung[a]<M_PI/2.0 || data->richtung[a]>3.0*M_PI/2.0)
            data->richtung[a]+=0.004*data->speed_up;
          if (data->richtung[a]>M_PI/2.0 && data->richtung[a]<3.0*M_PI/2.0)
            data->richtung[a]-=0.004*data->speed_up;
        }
        else
        if (*(data->joystick[a].axis[0])>0 && *(data->joystick[a].axis[1])>0) //Rechts-Unten
        {
          if (data->richtung[a]<M_PI/4.0 || data->richtung[a]>5.0*M_PI/4.0)
            data->richtung[a]+=0.004*data->speed_up;
          if (data->richtung[a]>M_PI/4.0 && data->richtung[a]<5.0*M_PI/4.0)
            data->richtung[a]-=0.004*data->speed_up;
        }
        else
        if (*(data->joystick[a].axis[0])==0 && *(data->joystick[a].axis[1])>0) //Unten
        {
          if (data->richtung[a]>M_PI)
            data->richtung[a]+=0.004*data->speed_up;
          if (data->richtung[a]<M_PI)
            data->richtung[a]-=0.004*data->speed_up;
        }
        else
        if (*(data->joystick[a].axis[0])<0 && *(data->joystick[a].axis[1])>0) //Links-Unten
        {
          if (data->richtung[a]>3.0*M_PI/4.0 && data->richtung[a]<7.0*M_PI/4.0)
            data->richtung[a]+=0.004*data->speed_up;
          if (data->richtung[a]<3.0*M_PI/4.0 || data->richtung[a]>7.0*M_PI/4.0)
            data->richtung[a]-=0.004*data->speed_up;
        }
        else
        if (*(data->joystick[a].axis[0])<0 && *(data->joystick[a].axis[1])==0) //Links
        {
          if (data->richtung[a]>M_PI/2.0 && data->richtung[a]<3.0*M_PI/2.0)
            data->richtung[a]+=0.004*data->speed_up;
          if (data->richtung[a]<M_PI/2.0 || data->richtung[a]>3.0*M_PI/2.0)
            data->richtung[a]-=0.004*data->speed_up;
        }
        else
        if (*(data->joystick[a].axis[0])<0 && *(data->joystick[a].axis[1])<0) //Links-Oben
        {
          if (data->richtung[a]>M_PI/4.0 && data->richtung[a]<5.0*M_PI/4.0)
            data->richtung[a]+=0.004*data->speed_up;
          if (data->richtung[a]<M_PI/4.0 || data->richtung[a]>5.0*M_PI/4.0)
            data->richtung[a]-=0.004*data->speed_up;
        }
        else
        if (*(data->joystick[a].axis[0])==0 && *(data->joystick[a].axis[1])<0) //Oben
        {
          if (data->richtung[a]<M_PI)
            data->richtung[a]+=0.004*data->speed_up;
          if (data->richtung[a]>M_PI)
            data->richtung[a]-=0.004*data->speed_up;
        }
        else
        if (*(data->joystick[a].axis[0])>0 && *(data->joystick[a].axis[1])<0) //Rechts-Oben
        {
          if (data->richtung[a]<3.0*M_PI/4.0 || data->richtung[a]>7.0*M_PI/4.0)
            data->richtung[a]+=0.004*data->speed_up;
          if (data->richtung[a]>3.0*M_PI/4.0 && data->richtung[a]<7.0*M_PI/4.0)
            data->richtung[a]-=0.004*data->speed_up;
        }
        if (*(data->joystick[a].button[0]))
            data->richtung[a]+=0.004*data->speed_up;
        if (*(data->joystick[a].button[2]))
            data->richtung[a]-=0.004*data->speed_up;
      }      
      if (data->richtung[a]>2.0*M_PI)
        data->richtung[a]-=2.0*M_PI;
      if (data->richtung[a]<0.0)
        data->richtung[a]+=2.0*M_PI;
      data->x[a]+=sin(data->richtung[a])*0.00015*data->speed_up;
      data->y[a]+=cos(data->richtung[a])*0.00015*data->speed_up;

      int x=(int)(trunc((data->x[a]+sin(data->richtung[a])*0.011)*256.0));
      if (x>255) 
        x=255;
      int y=(int)(trunc((data->y[a]+cos(data->richtung[a])*0.011)*256.0));
      if (y>255) 
        y=255;
      if ((Uint8)(data->feld[x+y*256])==255)
      {
        data->alive[a]=0;
        tZWpoint position;
        position.x= data->x[a]*2.0-1.0;
        position.y=-data->y[a]*2.0+1.0;
        position.z=0;//-2.0+sin(data->circle*M_PI/90.0)*0.15;
        ZWnewpartikelexplosion(position,20,0.12,data->maintex,16,201,0.12);
        Mix_PlayChannel(-1,data->snd_explosion,0);
      }
      
      //schauen, wieviele noch leben:
      int alivecount=0;
      int b;
      for (b=0;b<data->playernum;b++)
        if (data->alive[b])
          alivecount++;
      if (alivecount<=1)
      {
        data->step=2;
        Mix_FadeOutMusic(500);
        return 0;
      }
      
    }
    if (steps%(20/data->texquali)==0)
      draw_pixel(data);

    data->zeit-=1;
    if (data->zeit<=0)
    {
      data->step=2;
      Mix_FadeOutMusic(500);
      return 0;
    }
  }

  ZWusetexture(data->texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, data->maxx, data->maxy, GL_RGB, GL_UNSIGNED_BYTE, data->draw);
  return 0;
}

EXPORT int op_calc_game_thread(pdata data)
{ 
  if (*(data->shouldend))
    return 1;  
  
  return 0;
}

EXPORT void op_draw_game(pdata data)
{
  glColor4f(-sin(data->circle*M_PI/180.0)*0.5+0.5,-sin(data->circle*M_PI/90.0)*0.5+0.5,-sin(data->circle*M_PI/45.0)*0.5+0.5,1);
  ZWdrawtextmiddle((ZWtext)(data->text), 2.65,2.05,-4,(char*)"Dead",0.4);
  ZWdrawtextmiddle((ZWtext)(data->text),-2.65,2.05,-4,(char*)"Alive",0.4);

  int a;
  for (a=0;a<data->playernum;a++)
  {
    glColor4f(data->color[a].r,data->color[a].g,data->color[a].b,1);
    if (data->alive[a])
    {
      ZWdrawsprite(data->maintex,-2.65+cos(a*M_PI)*0.1, 2.0-0.25-a*0.5,-4,0.2,0.2,2,2);
      glColor4f(1,1,1,1);
      ZWdrawsprite(data->facetex,-2.65+cos(a*M_PI)*0.1, 2.0-0.25-a*0.5,-4,0.2/sqrt(2),0.2/sqrt(2),4,data->facenr[a]);
    }
    else
    {
      ZWdrawsprite(data->maintex, 2.65+cos(a*M_PI)*0.1, 2.0-0.25-a*0.5,-4,0.2,0.2,2,2);
      glColor4f(1,1,1,1);
      ZWdrawsprite(data->facetex, 2.65+cos(a*M_PI)*0.1, 2.0-0.25-a*0.5,-4,0.2/sqrt(2),0.2/sqrt(2),4,data->facenr[a]);
    }
  }

  char buffer[64];
  glTranslatef(0,0,sin(data->circle*M_PI/90.0)*0.15);
  glRotatef(sin(data->circle*M_PI/45.0)*5.0,0,0,1);  
  ZWusetexture(data->texture);
  glColor4f(1,1,1,1);
  glBegin(GL_QUADS);
    glTexCoord2f(0,0);  glVertex3f(-1, 1,-2);
    glTexCoord2f(0,1);  glVertex3f(-1,-1,-2);
    glTexCoord2f(1,1);  glVertex3f( 1,-1,-2);
    glTexCoord2f(1,0);  glVertex3f( 1, 1,-2);
  glEnd();

  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);  
  if (data->countdown>0)
  {
    sprintf(buffer,"%i",data->countdown/1000+1);
    ZWdrawtextmiddle((ZWtext)(data->text),0,-0.4,-4,buffer,2.0);  
  }
  sprintf(buffer,"%i",data->zeit/1000);
  ZWdrawtextmiddle((ZWtext)(data->text),2.85,-2.2,-4,buffer,0.4);


  glDepthFunc(GL_LEQUAL);
  glTranslatef(0,0,sin(data->circle*M_PI/90.0)*0.15);
  glRotatef(sin(data->circle*M_PI/45.0)*5.0,0,0,1);  
  tZWpoint cameraposition;
  cameraposition.x=0;
  cameraposition.y=0;
  cameraposition.z=2.0;
  tZWpoint camerarotation;
  camerarotation.x=0;
  camerarotation.y=0;
  camerarotation.z=0;
  ZWdrawpartikel(cameraposition,camerarotation);
  glDepthFunc(GL_ALWAYS);  

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
  ZWresetpartikel();
  glDeleteTextures(1, &(data->texture));  
  glClearColor( 0, 0, 0, 0 );
  
  int b;
  for (b=0;b<data->playernum;b++)
    gameresult->winner[b]=data->alive[b];
  
  free(data->draw);
  free(data->feld);
  Mix_VolumeMusic(data->old_music_volume);
  Mix_FreeMusic(data->backgroundmusic);
  Mix_FreeChunk(data->snd_explosion);
}
