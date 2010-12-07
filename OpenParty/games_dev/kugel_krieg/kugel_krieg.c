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

#include "./../../minigames.h"

#ifdef WIN32
#define bool char
#define true 1
#define false 0
#endif


#define MAX_BULLET 2

typedef struct sdrawlist *pdrawlist;
typedef struct sdrawlist {
  tZWcolor color;
  bool draw;
  int art; //0 Boden, 1 Würfel
  tZWpoint position;
  int distance;
  int side;
} tdrawlist;

typedef struct sbullet *pbullet;
typedef struct sbullet {
  float direction;
  float power;
  tZWpoint position;
  tZWcolor color;
	char activ;
	float distance;
	int age;
	float load;
} tbullet;

typedef struct sdata *pdata;
typedef struct sdata {
  int step;
  float fade;
  int countdown;
  ZWtext text;
  pgameinfo gameinfo;
  Mix_Music *backgroundmusic;
  Mix_Chunk *snd_explosion;
  object3d tank_basis;
  //object3d tank_top[8];
  object3d dice;
  GLuint texture;
  GLuint dicetex;
  int old_music_volume;
  float x[8];
  float y[8];
  float a[8];
	int live[8];
  int block[20][20];
  pdrawlist drawlist;
  int drawlistcount;
  int momplayer;
	tbullet bullet[8][3];
	pbullet sorted_bullet[8*MAX_BULLET];
	bool totalkollision[8];
	int lastchoice[8];
	int lastshot[8];
	int left[8];
	int right[8];
	int counter;
  int color[20][20];
  int prex[20][20];
  int prey[20][20];
  int path_cost[20][20];
  char refresh;
  int oldboxx[8];
  int oldboxy[8];
  int goalx[8];
  int goaly[8];
  char goal_is_enemy[8];
  int enemy[8];
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

float point_in_quad_get_r(float px,float py,float qx,float qy,float vx1,float vy1,float vx2,float vy2)
{
  if (vx2==0.0)
    return (px-qx-(vx2/vy2)*(py-qy))/(vx1-(vy1*vx2)/vy2);
  else
    return (py-qy-(vy2/vx2)*(px-qx))/(vy1-(vx1*vy2)/vx2);
}

float point_in_quad_get_s(float px,float py,float qx,float qy,float vx1,float vy1,float vx2,float vy2)
{
  if (vx1==0.0)
   return (px-qx-(vx1/vy1)*(py-qy))/(vx2-(vy2*vx1)/vy1);
  else
   return (py-qy-(vy1/vx1)*(px-qx))/(vy2-(vx2*vy1)/vx1);
}

bool point_in_quad(float px,float py,float qx,float qy,float vx1,float vy1,float vx2,float vy2)
{
  float s=point_in_quad_get_s(px,py,qx,qy,vx1,vy1,vx2,vy2);
  float r=point_in_quad_get_r(px,py,qx,qy,vx1,vy1,vx2,vy2);
  return (s>=0.0 && s<=1.0 && r>=0.0 && r<=1.0);
}

bool kollision_tank_wand(float tx,float ty,float angle)
{
  float tank_radius=sqrt(1.0*1.0+0.5*0.5);
  //Erster Punkt:
  angle=M_PI-angle*M_PI/180.0;
  angle+=M_PI/2.0-atan(2.0);
  if (tx+sin(angle)*tank_radius<0.0 || tx+sin(angle)*tank_radius>40.0 ||
      ty+cos(angle)*tank_radius<0.0 || ty+cos(angle)*tank_radius>40.0)
    return true;
  //Zweiter Punkt
  angle+=2.0*atan(2.0);
  if (tx+sin(angle)*tank_radius<0.0 || tx+sin(angle)*tank_radius>40.0 ||
      ty+cos(angle)*tank_radius<0.0 || ty+cos(angle)*tank_radius>40.0)
    return true;
  //Dritter Punkt:
  angle+=2.0*(M_PI/2.0-atan(2.0));
  if (tx+sin(angle)*tank_radius<0.0 || tx+sin(angle)*tank_radius>40.0 ||
      ty+cos(angle)*tank_radius<0.0 || ty+cos(angle)*tank_radius>40.0)
    return true;
  //Vierter Punkt
  angle+=2.0*atan(2.0);
  if (tx+sin(angle)*tank_radius<0.0 || tx+sin(angle)*tank_radius>40.0 ||
      ty+cos(angle)*tank_radius<0.0 || ty+cos(angle)*tank_radius>40.0)
    return true;

return false;
}

bool kollision_tank_quad(float tx,float ty,float angle,float qx,float qy,float vx1,float vy1,float vx2,float vy2)
{
  float tank_radius=sqrt(1.0*1.0+0.5*0.5);
  //Panzer im Quad:
  //Erster Punkt:
  angle=M_PI-angle*M_PI/180.0;
  angle+=M_PI/2.0-atan(2.0);
  if (point_in_quad(tx+sin(angle)*tank_radius,ty+cos(angle)*tank_radius,qx,qy,vx1,vy1,vx2,vy2))
    return true;
  float tqx=tx+sin(angle)*tank_radius;
  float tqy=ty+cos(angle)*tank_radius;
  //Zweiter Punkt
  angle+=2.0*atan(2.0);
  if (point_in_quad(tx+sin(angle)*tank_radius,ty+cos(angle)*tank_radius,qx,qy,vx1,vy1,vx2,vy2))
    return true;
  float tvx1=tx+sin(angle)*tank_radius-tqx;
  float tvy1=ty+cos(angle)*tank_radius-tqy;
  //Dritter Punkt:
  angle+=2.0*(M_PI/2.0-atan(2.0));
  if (point_in_quad(tx+sin(angle)*tank_radius,ty+cos(angle)*tank_radius,qx,qy,vx1,vy1,vx2,vy2))
    return true;
  //Vierter Punkt
  angle+=2.0*atan(2.0);
  if (point_in_quad(tx+sin(angle)*tank_radius,ty+cos(angle)*tank_radius,qx,qy,vx1,vy1,vx2,vy2))
    return true;
  float tvx2=tx+sin(angle)*tank_radius-tqx;
  float tvy2=ty+cos(angle)*tank_radius-tqy;
  
  //Quad im Panzer:
  //Erster Punkt:
  if (point_in_quad(qx,qy,tqx,tqy,tvx1,tvy1,tvx2,tvy2))
    return true;
  //Zweiter Punkt:
  if (point_in_quad(qx+vx1,qy+vy1,tqx,tqy,tvx1,tvy1,tvx2,tvy2))
    return true;
  //Dritter Punkt:
  if (point_in_quad(qx+vx2,qy+vy2,tqx,tqy,tvx1,tvy1,tvx2,tvy2))
    return true;
  //Vierter Punkt:
  if (point_in_quad(qx+vx1+vx2,qy+vy1+vy2,tqx,tqy,tvx1,tvy1,tvx2,tvy2))
    return true;
   
  return false;
}

bool kollision_tank_point(float tx,float ty,float angle,float qx,float qy)
{
  float tank_radius=sqrt(1.0*1.0+0.5*0.5);
  //Panzer im Quad:
  //Erster Punkt:
  angle=M_PI-angle*M_PI/180.0;
  angle+=M_PI/2.0-atan(2.0);
  float tqx=tx+sin(angle)*tank_radius;
  float tqy=ty+cos(angle)*tank_radius;
  //Zweiter Punkt
  angle+=2.0*atan(2.0);
  float tvx1=tx+sin(angle)*tank_radius-tqx;
  float tvy1=ty+cos(angle)*tank_radius-tqy;
  //Vierter Punkt:
  angle+=M_PI;
  float tvx2=tx+sin(angle)*tank_radius-tqx;
  float tvy2=ty+cos(angle)*tank_radius-tqy;
  
  //Punkt im Panzer:
  if (point_in_quad(qx,qy,tqx,tqy,tvx1,tvy1,tvx2,tvy2))
    return true;
  return false;
}


bool kollision_tank_tank(float tx1,float ty1,float angle1,float tx2,float ty2,float angle2)
{
  float tank_radius=sqrt(1.0*1.0+0.5*0.5);
  
  //Abstand tank<->tank überprüfen
  if ((tx1-tx2)*(tx1-tx2)+(ty1-ty2)*(ty1-ty2)>(tank_radius+tank_radius)*(tank_radius+tank_radius)*1.1025) //5% Toleranz
    return false;
  
  //Panzer 1 Vektoren:
  float angle=M_PI-angle1*M_PI/180.0;
  angle+=M_PI/2.0-atan(2.0);
  float tqx=tx1+sin(angle)*tank_radius;
  float tqy=ty1+cos(angle)*tank_radius;
  angle+=2.0*atan(2.0);
  float tvx1=tx1+sin(angle)*tank_radius-tqx;
  float tvy1=ty1+cos(angle)*tank_radius-tqy;
  angle+=M_PI;
  float tvx2=tx1+sin(angle)*tank_radius-tqx;
  float tvy2=ty1+cos(angle)*tank_radius-tqy;
  if (kollision_tank_quad(tx2,ty2,angle2,tqx,tqy,tvx1,tvy1,tvx2,tvy2))
    return true;
  //Panzer 2 Vektoren:
  angle=M_PI-angle2*M_PI/180.0;
  angle+=M_PI/2.0-atan(2.0);
  tqx=tx2+sin(angle)*tank_radius;
  tqy=ty2+cos(angle)*tank_radius;
  angle+=2.0*atan(2.0);
  tvx1=tx2+sin(angle)*tank_radius-tqx;
  tvy1=ty2+cos(angle)*tank_radius-tqy;
  angle+=M_PI;
  tvx2=tx2+sin(angle)*tank_radius-tqx;
  tvy2=ty2+cos(angle)*tank_radius-tqy;
  if (kollision_tank_quad(tx1,ty1,angle1,tqx,tqy,tvx1,tvy1,tvx2,tvy2))
    return true;

   
  return false;
}

bool kollision_tank_tank_all(pdata data,float tx,float ty,float angle,int self)
{
  int player;
  for (player=0;player<data->gameinfo->playernum;player++)
  {
    if (player==self || data->live[player]<=0)
      continue;
    if (kollision_tank_tank(tx,ty,angle,data->x[player],data->y[player],data->a[player]))
      return true;
  }
  return false;
}


int bullet_comparator (const pbullet *elem1,const pbullet *elem2)
{
  return (int)(trunc(((*elem1)->distance-(*elem2)->distance)*10000.0));
}

void newbullet(pdata data,int player,float x,float y,float z,float r,float g,float b,float direction,float power)
{
	if (player<0 || player>7)
	  return;
	int freebullet;
	for (freebullet=0;freebullet<MAX_BULLET;freebullet++)
	{
		if (!(data->bullet[player][freebullet].activ) && data->bullet[player][freebullet].load>=1.0)
		  break;
	}
	if (freebullet==MAX_BULLET)
	  return;
  data->bullet[player][freebullet].activ=true;
	data->bullet[player][freebullet].position.x=x+sin(direction*M_PI/180.0)*0.4;
	data->bullet[player][freebullet].position.y=y;
	data->bullet[player][freebullet].position.z=z+cos(direction*M_PI/180.0)*0.4;
	data->bullet[player][freebullet].color.r=0.5+r*0.5;
	data->bullet[player][freebullet].color.g=0.5+g*0.5;
	data->bullet[player][freebullet].color.b=0.5+b*0.5;
	data->bullet[player][freebullet].direction=direction;
	data->bullet[player][freebullet].power=power;
	data->bullet[player][freebullet].age=1337;
}

void calcbullets(pdata data)
{
//	for (int steps=0;steps<(int)*(data->gameinfo->steps);steps++)
        int a,b,c,d;
		for (a=0;a<data->gameinfo->playernum;a++)
			for (b=0;b<MAX_BULLET;b++)
			{
				if (data->bullet[a][b].load<=1.0)
				  data->bullet[a][b].load+=0.0001;
				if (data->bullet[a][b].activ)
				{
					data->bullet[a][b].position.x+=sin(data->bullet[a][b].direction*M_PI/180.0)*data->bullet[a][b].power;
					data->bullet[a][b].position.z+=cos(data->bullet[a][b].direction*M_PI/180.0)*data->bullet[a][b].power;
					bool kollision=false;
					int mx=(int)(trunc((data->bullet[a][b].position.x)/2.0));
					int my=(int)(trunc((data->bullet[a][b].position.z)/2.0));
					for (c=-1;c<=1;c++)
						for (d=-1;d<=1;d++)
							if (mx+c>=0 && mx+c<20 && my+d>=0 && my+d<20 && data->block[mx+c][my+d] &&
									point_in_quad(data->bullet[a][b].position.x,data->bullet[a][b].position.z,
																		 (float)(mx+c)*2.0,(float)(my+d)*2.0,2,0,0,2))
							{
								kollision=true;
								break;
							}
					kollision=kollision || (data->bullet[a][b].position.x<0.0) || (data->bullet[a][b].position.z<0.0) || (data->bullet[a][b].position.x>40.0) || (data->bullet[a][b].position.z>40.0);
					bool tank_kollision=false;
					int hit;
					for (c=0;c<data->gameinfo->playernum;c++)
					{
						if (data->live[c]<=0)
						  continue;
						if (c==a) //man kann sich NICHT selbst abschie�en
						  continue;
					  if (kollision_tank_point(data->x[c],data->y[c],data->a[c],data->bullet[a][b].position.x,data->bullet[a][b].position.z))
						{
							tank_kollision=true;
							hit=c;
						}
					}
					data->bullet[a][b].age--;
					if (tank_kollision)
					{
						data->live[hit]--;
						if (data->live[hit]<=0)
						{
							tZWpoint position;
							position.x=data->x[hit];
							position.y=0.5;
							position.z=data->y[hit];
							ZWnewpartikelexplosion(position,64,1.0,data->gameinfo->maintex,16,201,0.5);
							Mix_PlayChannel(-1,data->snd_explosion,1);
						}
					}
          if (kollision || tank_kollision || data->bullet[a][b].age<=0)
					{
					  data->bullet[a][b].activ=false;
						data->bullet[a][b].load=0.0;
						ZWnewpartikelexplosion(data->bullet[a][b].position,20,0.5,data->gameinfo->maintex,16,201,0.5);
						Mix_PlayChannel(-1,data->snd_explosion,0);
					}
				}
		  }
}

void drawbullets(tZWpoint cameraposition,float angle,tZWpoint camerarotation,pdata data)
{
	//bullets sortieren:
    int a;
	for (a=0;a<8*MAX_BULLET;a++)
	  if (data->sorted_bullet[a]->activ)
			data->sorted_bullet[a]->distance=point_in_quad_get_r(data->sorted_bullet[a]->position.x,data->sorted_bullet[a]->position.z,
																														cameraposition.x,cameraposition.z,
																														sin(- angle      *M_PI/180.0),cos(- angle      *M_PI/180.0),
																														sin(-(angle+90.0)*M_PI/180.0),cos(-(angle+90.0)*M_PI/180.0));
    else
		  data->sorted_bullet[a]->distance=0;
	qsort(data->sorted_bullet,8*MAX_BULLET,sizeof(pbullet), (int(*)(const void*,const void*))bullet_comparator);										
	glDisable(GL_LIGHTING);
  //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	for (a=0;a<8*MAX_BULLET;a++)
		if (data->sorted_bullet[a]->activ)
		{
			glPushMatrix();
			glColor4f(data->sorted_bullet[a]->color.r,data->sorted_bullet[a]->color.g,data->sorted_bullet[a]->color.b,1);
			glTranslatef(data->sorted_bullet[a]->position.x,data->sorted_bullet[a]->position.y,data->sorted_bullet[a]->position.z);
			glRotatef(camerarotation.x,1,0,0);
			glRotatef(camerarotation.y,0,1,0);
			glRotatef(camerarotation.z,0,0,1);
			ZWdrawsprite(data->texture,0,0,0,0.2,0.2,4,4);
			glPopMatrix();
		}
  //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_LIGHTING);
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
	sprintf(buffer,"%ssounds/In a Heartbeat.ogg",gameinfo->datafolder);
  data->backgroundmusic = Mix_LoadMUS(buffer);
  data->old_music_volume=Mix_VolumeMusic((int)(trunc(96.0*gameinfo->volume)));
  Mix_FadeInMusic(data->backgroundmusic,-1,500);
	sprintf(buffer,"%ssounds/explosion.ogg",gameinfo->datafolder);
  data->snd_explosion=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_explosion,(int)(trunc(16.0*gameinfo->volume)));


  //Objects laden
	sprintf(buffer,"%stextures/kugel_krieg.png",gameinfo->datafolder);
  data->texture=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
	sprintf(buffer,"%stextures/dice_no_color.png",gameinfo->datafolder);
  data->dicetex=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
	sprintf(buffer,"%smodels/tank_basis.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->tank_basis),buffer,data->texture);
  ZWcreatedrawlist(&(data->tank_basis),0);
  /*for (a=0;a<(data->gameinfo->playernum);a++)
  {
    ZWload3dm(&(data->tank_top[a]),(char*)MINIGAMEDATAFOLDER"models/tank_top.3dm",gameinfo->maintex);
    int b;
    for (b=0;b<data->tank_top[a].pointnum;b++)
    {
      float dazu=((float)(gameinfo->facenr[a]%4))*0.25;
      data->tank_top[a].p[b].u+=dazu;
      dazu=((float)(gameinfo->facenr[a]/4))*0.25;
      data->tank_top[a].p[b].v+=dazu;    
    }
    ZWcreatedrawlist(&(data->tank_top[a]),0);
  }*/
	sprintf(buffer,"%smodels/dice.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->dice),buffer,data->dicetex);
  ZWcreatedrawlist(&(data->dice),0);
  //temporäres Licht!
  ZWenablelight(0);
  ZWsetlightambient(0,0.1,0.1,0.1);
  ZWsetlightdiffuse(0,1.5,1.5,1.5);
  ZWsetlightposition(0,20.0,5.0,20.0);
  ZWsetmaterial(lnone);

  
  //Spiel ansich vorbereiten
  switch (gameinfo->playernum)
  {
    case 1: data->x[0]=20; data->y[0]=20; data->a[0]=0;
            break;
    case 2: data->x[0]=2;  data->y[0]=2;  data->a[0]=135;
            data->x[1]=38; data->y[1]=38; data->a[1]=315;
            break;
    case 3:
    case 4: data->x[0]=2;  data->y[0]=2;  data->a[0]=135;
            data->x[1]=2;  data->y[1]=38; data->a[1]=45;
            data->x[2]=38; data->y[2]=2;  data->a[2]=225;
            data->x[3]=38; data->y[3]=38; data->a[3]=315;
            break;
    case 5:
    case 6:
    case 7:
    case 8: data->x[0]=2;  data->y[0]=2;  data->a[0]=135;
            data->x[1]=2;  data->y[1]=20; data->a[1]=90;
            data->x[2]=2;  data->y[2]=38; data->a[2]=45;
            data->x[3]=20; data->y[3]=2;  data->a[3]=180;
            data->x[4]=20; data->y[4]=38; data->a[4]=0; 
            data->x[5]=38; data->y[5]=2;  data->a[5]=225;
            data->x[6]=38; data->y[6]=20; data->a[6]=270;
            data->x[7]=38; data->y[7]=38; data->a[7]=315;            
            break;
  }
  memset(data->block,0,20*20*sizeof(int));
  int b;
  for (a=2;a<18;a++)
    for (b=2;b<18;b++)
    {
      if (rand()%4==0 && !(a>7 && a<12 && b>7 && b<12))
        data->block[a][b]=1;
    }
  
	for (a=0;a<8;a++)
	{
	  for (b=0;b<MAX_BULLET;b++)
		{
		  data->bullet[a][b].activ=false;
			data->bullet[a][b].load=1.0;
			data->sorted_bullet[a+b*8]=&(data->bullet[a][b]);
		}
		data->live[a]=3;
		data->totalkollision[a]=true;
		data->lastshot[a]=0;
	}
	
  data->drawlist=(pdrawlist)malloc(sizeof(tdrawlist)*(400+80));
  float af,bf;
  for (af=0;af<40;af=af+2)
    for (bf=0;bf<40;bf=bf+2)
    {
      int pos=(int)(trunc(af/2.0))+(int)(trunc(bf/2.0))*20;
      data->drawlist[pos].color.a=1.0;
      if (af<20)
      {
        if (bf<20)
        {
          data->drawlist[pos].color.r=1;
          data->drawlist[pos].color.g=0.5;
          data->drawlist[pos].color.b=0.5;
        }
        else
        {
          data->drawlist[pos].color.r=0.5;
          data->drawlist[pos].color.g=1;
          data->drawlist[pos].color.b=0.5;
        }
      }
      else
      {
        if (bf<20)
        {
          data->drawlist[pos].color.r=1;
          data->drawlist[pos].color.g=1;
          data->drawlist[pos].color.b=0.5;
        }
        else
        {
          data->drawlist[pos].color.r=0.5;
          data->drawlist[pos].color.g=0.5;
          data->drawlist[pos].color.b=1;
        }
      }
      if (data->block[(int)(trunc(af/2.0))][(int)(trunc(bf/2.0))]==0)
      {
        data->drawlist[pos].position.x=af+1.0;
        data->drawlist[pos].position.y=-0.4;
        data->drawlist[pos].position.z=bf+1.0;
        data->drawlist[pos].art=0;
      }
      else
      {
        data->drawlist[pos].position.x=af+1.0;
        data->drawlist[pos].position.y=0.6;
        data->drawlist[pos].position.z=bf+1.0;
        data->drawlist[pos].art=1;
      }
      data->drawlist[pos].draw=true;
      data->drawlist[pos].side=abs(((pos%20-111)*(pos/20+123))%6);
      if (pos/20==0)
      {
        data->drawlist[400+pos%20].position.x=af+1.0;
        data->drawlist[400+pos%20].position.y=0.6;
        data->drawlist[400+pos%20].position.z=-1.0;
        data->drawlist[400+pos%20].art=1;
        data->drawlist[400+pos%20].color=data->drawlist[pos].color;
        data->drawlist[400+pos%20].draw=true;
        data->drawlist[400+pos%20].side=rand()%6;
      }
      if (pos/20==19)
      {
        data->drawlist[400+pos%20+20].position.x=af+1.0;
        data->drawlist[400+pos%20+20].position.y=0.6;
        data->drawlist[400+pos%20+20].position.z=41.0;
        data->drawlist[400+pos%20+20].art=1;
        data->drawlist[400+pos%20+20].color=data->drawlist[pos].color;
        data->drawlist[400+pos%20+20].draw=true;
        data->drawlist[400+pos%20+20].side=rand()%6;
      }
      if (pos%20==0)
      {
        data->drawlist[400+pos/20+40].position.x=-1.0;
        data->drawlist[400+pos/20+40].position.y=0.6;
        data->drawlist[400+pos/20+40].position.z=bf+1.0;
        data->drawlist[400+pos/20+40].art=1;
        data->drawlist[400+pos/20+40].color=data->drawlist[pos].color;
        data->drawlist[400+pos/20+40].draw=true;
        data->drawlist[400+pos/20+40].side=rand()%6;
      }
      if (pos%20==19)
      {
        data->drawlist[400+pos/20+60].position.x=41.0;
        data->drawlist[400+pos/20+60].position.y=0.6;
        data->drawlist[400+pos/20+60].position.z=bf+1.0;
        data->drawlist[400+pos/20+60].art=1;
        data->drawlist[400+pos/20+60].color=data->drawlist[pos].color;
        data->drawlist[400+pos/20+60].draw=true;
        data->drawlist[400+pos/20+60].side=rand()%6;
      }
    }
  data->drawlistcount=400+80;
  ZWsetfogcolor(0.8,0.8,0.8);
  ZWenablefog_near(2.0,15.0);
  glClearColor(0.8,0.8,0.8,0);
    
  data->step=0;
  data->fade=1;
  data->countdown=2500;  
	data->counter=120999;
  data->refresh=1;
  for (a=0;a<data->gameinfo->playernum;a++)
  {
    data->oldboxx[a]=(int)(data->x[a])/2;
    data->oldboxy[a]=(int)(data->y[a])/2;
  }
}

#include "cpu.h"

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
    calc_cpu(data);
		calcbullets(data);
	int a;
    for (a=0;a<data->gameinfo->playernum;a++)
    {
			if (data->live[a]<=0)
				continue;

      float dx=0;
      float dy=0;
      float da=0;
      if ((*(data->gameinfo->joystick[a].axis[1])<0))
        da+=1.0/40.0;
      if ((*(data->gameinfo->joystick[a].axis[1])>0))
        da-=1.0/40.0;
      if (*(data->gameinfo->joystick[a].button[1]))
        da-=1.0/40.0;
      if (*(data->gameinfo->joystick[a].button[3]))
        da+=1.0/40.0;
      if ((*(data->gameinfo->joystick[a].axis[1])<0))
      {
        dx+=sin((data->a[a])*M_PI/180.0)/750.0;
        dy-=cos((data->a[a])*M_PI/180.0)/750.0;
      }
      if ((*(data->gameinfo->joystick[a].axis[1])>0))
      {
        dx-=sin((data->a[a])*M_PI/180.0)/750.0;
        dy+=cos((data->a[a])*M_PI/180.0)/750.0;
      }
      if (*(data->gameinfo->joystick[a].button[1]))
      {
        dx+=sin((data->a[a])*M_PI/180.0)/750.0;
        dy-=cos((data->a[a])*M_PI/180.0)/750.0;
      }
      if (*(data->gameinfo->joystick[a].button[3]))
      {
        dx-=sin((data->a[a])*M_PI/180.0)/750.0;
        dy+=cos((data->a[a])*M_PI/180.0)/750.0;
      }

			data->totalkollision[a]=false;
      //Fall 0: Alles ok?
      //Alle 9 Möglichkeiten durchgehen:
      bool kollision=false;
      int mx=(int)(trunc((data->x[a]+dx)/2.0));
      int my=(int)(trunc((data->y[a]+dy)/2.0));
      int b,c;
      for (b=-1;b<=1;b++)
        for (c=-1;c<=1;c++)
          if (mx+b>=0 && mx+b<20 && my+c>=0 && my+c<20 && data->block[mx+b][my+c] &&
              kollision_tank_quad(data->x[a]+dx,data->y[a]+dy,data->a[a]+da,
                                 (float)(mx+b)*2.0,(float)(my+c)*2.0,2,0,0,2))
          {
            kollision=true;
            break;
          }
      kollision=kollision || kollision_tank_wand(data->x[a]+dx,data->y[a]+dy,data->a[a]+da)
                          || kollision_tank_tank_all(data,data->x[a]+dx,data->y[a]+dy,data->a[a]+da,a); 
      if (kollision)
      {
        //Fall 1: X nein, aber Winkel+Y?
        kollision=false;
        for (b=-1;b<=1;b++)
          for (c=-1;c<=1;c++)
            if (mx+b>=0 && mx+b<20 && my+c>=0 && my+c<20 && data->block[mx+b][my+c] &&
                kollision_tank_quad(data->x[a],data->y[a]+dy,data->a[a]+da,
                                   (float)(mx+b)*2.0,(float)(my+c)*2.0,2,0,0,2))
            {
              kollision=true;
              break;
            }
        kollision=kollision || kollision_tank_wand(data->x[a],data->y[a]+dy,data->a[a]+da)
                            || kollision_tank_tank_all(data,data->x[a],data->y[a]+dy,data->a[a]+da,a); 
        if (kollision)
        {
          //Fall 2: Y nein, aber Winkel+X?
          kollision=false;
          for (b=-1;b<=1;b++)
            for (c=-1;c<=1;c++)
              if (mx+b>=0 && mx+b<20 && my+c>=0 && my+c<20 && data->block[mx+b][my+c] &&
                  kollision_tank_quad(data->x[a]+dx,data->y[a],data->a[a]+da,
                                     (float)(mx+b)*2.0,(float)(my+c)*2.0,2,0,0,2))
              {
                kollision=true;
                break;
              }
          kollision=kollision || kollision_tank_wand(data->x[a]+dx,data->y[a],data->a[a]+da)
                              || kollision_tank_tank_all(data,data->x[a]+dx,data->y[a],data->a[a]+da,a); 
          if (kollision)
          {
            //Fall 3: Winkel nein, aber X+Y?
            kollision=false;
            for (b=-1;b<=1;b++)
              for (c=-1;c<=1;c++)
                if (mx+b>=0 && mx+b<20 && my+c>=0 && my+c<20 && data->block[mx+b][my+c] &&
                    kollision_tank_quad(data->x[a]+dx,data->y[a]+dy,data->a[a],
                                       (float)(mx+b)*2.0,(float)(my+c)*2.0,2,0,0,2))
                {
                  kollision=true;
                  break;
                }
            kollision=kollision || kollision_tank_wand(data->x[a]+dx,data->y[a]+dy,data->a[a])
                                || kollision_tank_tank_all(data,data->x[a]+dx,data->y[a]+dy,data->a[a],a);
            if (kollision)
            {
              //Fall 4: Y+Winkel nein, aber X
              kollision=false;
              for (b=-1;b<=1;b++)
                for (c=-1;c<=1;c++)
                  if (mx+b>=0 && mx+b<20 && my+c>=0 && my+c<20 && data->block[mx+b][my+c] &&
                      kollision_tank_quad(data->x[a]+dx,data->y[a],data->a[a],
                                         (float)(mx+b)*2.0,(float)(my+c)*2.0,2,0,0,2))
                  {
                    kollision=true;
                    break;
                  }
              kollision=kollision || kollision_tank_wand(data->x[a]+dx,data->y[a],data->a[a])
                                  || kollision_tank_tank_all(data,data->x[a]+dx,data->y[a],data->a[a],a);
              if (kollision)
              {
                //Fall 5: X+Winkel nein, aber Y
                kollision=false;
                for (b=-1;b<=1;b++)
                  for (c=-1;c<=1;c++)
                    if (mx+b>=0 && mx+b<20 && my+c>=0 && my+c<20 && data->block[mx+b][my+c] &&
                        kollision_tank_quad(data->x[a],data->y[a]+dy,data->a[a],
                                           (float)(mx+b)*2.0,(float)(my+c)*2.0,2,0,0,2))
                    {
                      kollision=true;
                      break;
                    }
                kollision=kollision || kollision_tank_wand(data->x[a],data->y[a]+dy,data->a[a])
                                    || kollision_tank_tank_all(data,data->x[a],data->y[a]+dy,data->a[a],a);
                if (kollision)
                {
                  //Fall 6: X+Y nein, aber Winkel
                  kollision=false;
                  for (b=-1;b<=1;b++)
                    for (c=-1;c<=1;c++)
                      if (mx+b>=0 && mx+b<20 && my+c>=0 && my+c<20 && data->block[mx+b][my+c] &&
                          kollision_tank_quad(data->x[a],data->y[a],data->a[a]+da,
                                             (float)(mx+b)*2.0,(float)(my+c)*2.0,2,0,0,2))
                      {
                        kollision=true;
                        break;
                      }
                  kollision=kollision || kollision_tank_wand(data->x[a],data->y[a],data->a[a]+da)
                                      || kollision_tank_tank_all(data,data->x[a],data->y[a],data->a[a]+da,a);
                  if (kollision)
                  {
                    //Fall 7: Geht halt nicht...
										data->totalkollision[a]=true;
                  }
                  else
									{
                    data->a[a]+=da;
										data->totalkollision[a]=true;
									}
                }
                else
								{
                  data->y[a]+=dy;
									data->totalkollision[a]=true;
								}
              }
              else
							{
                data->x[a]+=dx;
								data->totalkollision[a]=true;
							}
            }
            else
            {
              data->x[a]+=dx;
              data->y[a]+=dy;
							//if (rand()%100==0)
								data->totalkollision[a]=true;
            }
          }
          else
          {
            data->x[a]+=dx;
            data->a[a]+=da;
						//if (rand()%100==0)
							data->totalkollision[a]=true;
          }
        }
        else
        {
          data->y[a]+=dy;
          data->a[a]+=da;
					//if (rand()%100==0)
					  data->totalkollision[a]=true;
        }      
      }
      else
      {
        data->x[a]+=dx;
        data->y[a]+=dy;          
        data->a[a]+=da;
      }      
      

      if (data->a[a]<0)
        data->a[a]+=360.0;
      if (data->a[a]>=360.0)
        data->a[a]-=360.0;
				
			//Schießen
			if (*(data->gameinfo->joystick[a].button[0]) || *(data->gameinfo->joystick[a].button[2]))
			{
				*(data->gameinfo->joystick[a].button[0])=0;
				*(data->gameinfo->joystick[a].button[2])=0;
				newbullet(data,a,data->x[a],0.1,data->y[a],data->gameinfo->color[a].r,data->gameinfo->color[a].g,data->gameinfo->color[a].b,180.0-data->a[a],0.01);
			}
    }
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
	
	//Nachschauen, ob nurnoch ein Team lebt:
	int team_alive=-1;
	int a;
	for (a=0;a<data->gameinfo->playernum;a++)
	{
		if (data->live[a]>0 && team_alive<0)
			team_alive=data->gameinfo->team[a];
		else
		if (data->live[a]>0 && team_alive!=data->gameinfo->team[a])
			break;
	}

  data->counter-=*(data->gameinfo->steps);
  if (data->counter<=999 || a==data->gameinfo->playernum) //Lief durch ohne "break")
	{
				data->counter=0;
        data->step=2;
        Mix_FadeOutMusic(500);
	}

  return 0;
}

EXPORT int op_calc_game_thread(pdata data)
{ 
  if (*(data->gameinfo->shouldend))
    return 1;
    
  return 0;
}

void draw_dice_field(pdata data,int nr)
{
  ZWusetexture(data->dicetex);
  float u=(float)(nr%3)*1.0/3.0;
  float v=(float)(nr/3)*1.0/3.0;
  glNormal3f(0,0,1);
  glBegin(GL_QUADS);
    glTexCoord2f(0.000+u,0.000+v);  glVertex3f(-1, 1, 0);
    glTexCoord2f(0.000+u,0.333+v);  glVertex3f(-1,-1, 0);
    glTexCoord2f(0.333+u,0.333+v);  glVertex3f( 1,-1, 0);
    glTexCoord2f(0.333+u,0.000+v);  glVertex3f( 1, 1, 0);
  glEnd();
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

int drawlist_comparator (const pdrawlist elem1,const pdrawlist elem2)
{
  return elem1->distance-elem2->distance;
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
	
	  
  //Kamera einstellen I
    //glTranslatef(0,0,-1);
    glRotatef(-(-10),1,0,0);
    glRotatef(-(-data->a[player]),0,1,0);

  //Kamera einstellen II
    glTranslatef(-data->x[player]+sin(data->a[player]*M_PI/180)*0.3,-1.0,-data->y[player]-cos(data->a[player]*M_PI/180)*0.3);
    
    ZWsetlightposition(0,20.0,5.0,20.0);

		
    glEnable(GL_LIGHTING);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    int a;
    for (a=0;a<data->gameinfo->playernum;a++)
    {
			if (data->live[a]<=0)
				continue;
      glPushMatrix();
      glTranslatef(data->x[a],0,data->y[a]);
      glColor4f(0.2+data->gameinfo->color[a].r*0.8,0.2+data->gameinfo->color[a].g*0.8,0.2+data->gameinfo->color[a].b*0.8,1.0);
      glRotatef(180-data->a[a],0,1,0);
      ZWdrawobject(data->tank_basis);
      glTranslatef(0,0.45,0);
      glScalef(0.3,0.3,0.3);
      //ZWdrawobject(data->tank_top[a]);
			ZWusetexture(data->gameinfo->facetex);
			glCallList(data->gameinfo->ball[a]);
      glPopMatrix();
    }
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //Frustum berechnen
    tZWfrustum frustum;
    ZWfrustum_calculate(&frustum);    
    
    //Abstand & Frustumtest berechnen:
    for (a=0;a<data->drawlistcount;a++)
    {
      //float distance=    ((data->x[player]-data->drawlist[a].position.x)*(data->x[player]-data->drawlist[a].position.x)+
      //                    (data->y[player]-data->drawlist[a].position.z)*(data->y[player]-data->drawlist[a].position.z))*100.0;
      float distance=-point_in_quad_get_r(data->drawlist[a].position.x,data->drawlist[a].position.z,
                                         data->x[player],data->y[player],
                                         sin(-data->a[player]*M_PI/180.0),cos(-data->a[player]*M_PI/180.0),
                                         sin(-(data->a[player]+90.0)*M_PI/180.0),cos(-(data->a[player]+90.0)*M_PI/180.0))*100.0;
      data->drawlist[a].distance=(int)(trunc(distance));
      data->drawlist[a].draw=(distance<1600.0) &&
                             ZWfrustum_isboxwithin(&frustum,data->drawlist[a].position.x,data->drawlist[a].position.y,
                                                            data->drawlist[a].position.z,2,0,2);
    }

    //Sortieren der Daten, Achtung! Schlechter Codestil ^^
    qsort(data->drawlist,data->drawlistcount,sizeof(tdrawlist), (int(*)(const void*,const void*))drawlist_comparator);
    for (a=0;a<data->drawlistcount;a++)
    {
      if (!(data->drawlist[a].draw))
        continue;
      glPushMatrix();
      glColor4f(data->drawlist[a].color.r,data->drawlist[a].color.g,data->drawlist[a].color.b,data->drawlist[a].color.a);
      glTranslatef(data->drawlist[a].position.x,data->drawlist[a].position.y,data->drawlist[a].position.z);
      if (data->drawlist[a].art==0)
      {
        glRotatef(-90,1,0,0);      
        draw_dice_field(data,data->drawlist[a].side);
      }
      else
        ZWdrawobject(data->dice);
      glPopMatrix();
    }
		
    tZWpoint camerarotation;
		camerarotation.x=-10;
		camerarotation.y=-data->a[player];
		camerarotation.z=0;		
		tZWpoint cameraposition;
		cameraposition.x=data->x[player]-sin(data->a[player]*M_PI/180)*0.3;
		cameraposition.y=1.0;
		cameraposition.z=data->y[player]+cos(data->a[player]*M_PI/180)*0.3;
  	drawbullets(cameraposition,data->a[player],camerarotation,data);		
		
    glDisable(GL_LIGHTING);
		glLoadIdentity();
		ZWdrawpartikel(cameraposition,camerarotation);
    glDepthFunc(GL_ALWAYS);  

    //HUD:
		glLoadIdentity();		
		float pos=0;
		for (a=0;a<MAX_BULLET;a++)
		{
			if (!(data->bullet[player][a].activ || data->live[player]<=0))
			{
				if (data->bullet[player][a].load<1.0)
				  glColor4f(0.5+data->gameinfo->color[player].r*0.5,0.5+data->gameinfo->color[player].g*0.5,0.5+data->gameinfo->color[player].b*0.5,0.6);
				else
				  glColor4f(0.5+data->gameinfo->color[player].r*0.5,0.5+data->gameinfo->color[player].g*0.5,0.5+data->gameinfo->color[player].b*0.5,1.0);
				ZWdrawsprite(data->texture,0.32+pos,-0.48,-1.0,0.08,0.08,4,4);
				if (data->bullet[player][a].load<1.0)
				{
				  glColor4f(1,1,1,0.6);
					sprintf(buffer,"%i%%",(int)(trunc(data->bullet[player][a].load*100)));
					ZWdrawtextmiddle((ZWtext)(data->text),0.32+pos,-0.494,-1.0,buffer,0.1);  
				}
		  }
			pos+=0.18;
	  }

		pos=0;
		for (a=0;a<data->live[player];a++)
		{
  		glColor4f(data->gameinfo->color[player].r,data->gameinfo->color[player].g,data->gameinfo->color[player].b,1);
		  ZWdrawsprite(data->gameinfo->maintex,-0.32-pos,-0.5,-1.0,0.08,0.08,2,2);
      glColor4f(1,1,1,1);
      ZWdrawsprite(data->gameinfo->facetex,-0.32-pos,-0.5,-1.0,0.08/sqrt(2),0.08/sqrt(2),4,data->gameinfo->facenr[player]);
			pos+=0.18;
	  }
		
		//Counter:
    sprintf(buffer,"%i",data->counter/1000);
		glColor4f(0,0,0,0.5);
    ZWdrawtextmiddle((ZWtext)(data->text),0.68,-0.51,-1.0,buffer,0.16);  
		glColor4f(1,1,1,1);
    ZWdrawtextmiddle((ZWtext)(data->text),0.68,-0.51,-1.0,buffer,0.15);  


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
	ZWresetpartikel();
	int b;
  for (b=0;b<data->gameinfo->playernum;b++)
    gameresult->winner[b]=data->live[b]>0; 
  ZWdeleteobject(data->tank_basis);
  //int a;
  //for (a=0;a<(data->gameinfo->playernum);a++)
    //ZWdeleteobject(data->tank_top[a]);    
  ZWdeletetexture(data->texture);
  ZWdeleteobject(data->dice);
  ZWdisablelight(0);
  
  ZWdeletetexture(data->texture);
  ZWdeletetexture(data->dicetex);
 
  glClearColor(0,0,0,0); 
    
  free(data->drawlist);
  ZWdisablefog();   
  //Musik zurücksetzen:
  Mix_VolumeMusic(data->old_music_volume);
  Mix_FreeMusic(data->backgroundmusic);
  Mix_FreeChunk(data->snd_explosion);
}
