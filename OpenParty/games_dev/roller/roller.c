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

#ifdef WIN32
#define bool int
#define true 1
#define false 0
#endif

#include "./smart_physics.h"

typedef struct sdata *pdata;
typedef struct sdata {
  int step;
  float fade;
  int countdown;
  ZWtext text;
  pgameinfo gameinfo;
  Mix_Chunk *snd_negative;
  Mix_Chunk *snd_drink;
  Mix_Music *backgroundmusic;
  int old_music_volume;
	GLuint floor,cartex;
	pphysik_object world;
	pphysik_object player[8];
	object3d wall,car[2];
	float street_pos[4][10];
	tZWcolor car_color[4][10];
	int last_car[4];
	bool floor_green[4][64];
	int last_next_step;
	bool back_to_start[8];
	int winner;
	int time;
	//KI:
	int ki_impuls[8],ki_impuls_dir[8];
	int ki_speed[8];
} tdata;

#include "./cpu.h"

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

void one_car_step(pdata data)
{
	int car,street;
	for (street=0;street<4;street++)
	{
		if (((street==1 || street==2) && data->last_car[street]>1500 && rand()%2000==0)
		 || ((street==0 || street==3) && data->last_car[street]>3000 && rand()%4000==0))
		{
			for (car=0;car<10;car++)
				if (data->street_pos[street][car]==INFINITY)
					break;
				if (car!=10)
				{
					if (street>1)
						data->street_pos[street][car]=-32.0;
					else
						data->street_pos[street][car]= 32.0;
					data->car_color[street][car].r=(float)(rand()%256)/255.0;
					data->car_color[street][car].g=(float)(rand()%256)/255.0;
					data->car_color[street][car].b=(float)(rand()%256)/255.0;
				}
			data->last_car[street]=0;
		}
		data->last_car[street]++;
		for (car=0;car<10;car++)
		{
			if (data->street_pos[street][car]!=INFINITY)
			{
				switch (street)
				{
					case 0: data->street_pos[street][car]-=0.003; break;
					case 1: data->street_pos[street][car]-=0.005; break;
					case 2: data->street_pos[street][car]+=0.005; break;
					case 3: data->street_pos[street][car]+=0.003; break;
				}
				if (data->street_pos[street][car]>32.0)
					data->street_pos[street][car]=INFINITY;
				if (data->street_pos[street][car]<-32.0)
					data->street_pos[street][car]=INFINITY;
			}
		}
	}	
	data->last_next_step++;
	if (data->last_next_step>=200)
	{
		data->last_next_step=0;
		//Durchrücken und 1. neu setzen
		int line;
		for (street=0;street<4;street++)
			if (street==0 || street==2)
			{
				for (line=63;line>0;line--)
					data->floor_green[street][line]=data->floor_green[street][line-1];
				data->floor_green[street][0]=data->floor_green[street][1] ^ (rand()%10==0);
			}
			else
			{
				for (line=0;line<63;line++)
					data->floor_green[street][line]=data->floor_green[street][line+1];
				data->floor_green[street][63]=data->floor_green[street][62] ^ (rand()%10==0);
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
    
  //Gameinforeferenzpointer speicher:
  data->gameinfo=gameinfo;
  
	char buffer[1024];
	
  //Ton vorbereiten
  sprintf(buffer,"%ssounds/In a Heartbeat.ogg",gameinfo->datafolder);
  data->backgroundmusic = Mix_LoadMUS(buffer);
  data->old_music_volume=Mix_VolumeMusic((int)(trunc(96.0*gameinfo->volume)));
  Mix_FadeInMusic(data->backgroundmusic,-1,500);

	sprintf(buffer,"%ssounds/negative.ogg",gameinfo->datafolder);
  data->snd_negative=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_negative,(int)(round(64.0*gameinfo->volume)));
  sprintf(buffer,"%ssounds/drink.ogg",gameinfo->datafolder);
  data->snd_drink=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_drink,(int)(round(64.0*gameinfo->volume)));

  //Texturen
  sprintf(buffer,"%stextures/roller.png",gameinfo->datafolder);
  data->floor=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
  sprintf(buffer,"%stextures/mini.png",gameinfo->datafolder);
  data->cartex=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);

  sprintf(buffer,"%smodels/roller_wall.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->wall),buffer,data->floor);
  ZWcreatedrawlist(&(data->wall),gameinfo->lightquali);
  sprintf(buffer,"%smodels/mini.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->car[0]),buffer,data->cartex);
  ZWcreatedrawlist(&(data->car[0]),gameinfo->lightquali);
  sprintf(buffer,"%smodels/truck.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->car[1]),buffer,data->cartex);
  ZWcreatedrawlist(&(data->car[1]),gameinfo->lightquali);

  
  //Spiel ansich vorbereiten
  data->step=0;
  data->fade=1;
  data->countdown=3000;  
	data->world=NULL;
	
	int sort[8]={ 0, 0, 0, 0, 0, 0, 0, 0};
	bool used[8]={false,false,false,false,false,false,false,false};
	for (a=0;a<gameinfo->playernum;a++)
	{
		int number;
		do { number=rand()%gameinfo->playernum; }
		while (used[number]);
		used[number]=true;
		sort[a]=number;
	}
	
	for (a=0;a<gameinfo->playernum;a++)
	{
		data->player[sort[a]]=new_object((-(float)(data->gameinfo->playernum-1)*1.1+(float)(a)*2.2)*3.0,-18,0.0,0,1,0,50,true,&(data->world));
		data->player[sort[a]]->absorption=0.0;
		data->back_to_start[sort[a]]=false;
		data->ki_impuls[a]=0;
		data->ki_impuls_dir[a]=0;
		data->ki_speed[a]=0;
	}
	new_object(-27  ,2.5,0,1,10,1,1,false,&(data->world))->absorption=0.8;
	new_object(-13.5,2.5,0,1,10,1,1,false,&(data->world))->absorption=0.8;
	new_object(  3.5,2.5,0,1,10,1,1,false,&(data->world))->absorption=0.8;
	new_object( 17  ,2.5,0,1,10,1,1,false,&(data->world))->absorption=0.8;
		
	//Rahmen
	float faktor=0.95;
	new_object(-27.0*faktor,-20.25*faktor,0,2,54.0*faktor,40.5*faktor,1,false,&(data->world))->absorption=0.0;
	
	//Autos setzen:
	int b;
	for (a=0;a<4;a++)
	{
		data->last_car[a]=0;
	  for (b=0;b<10;b++)
		  data->street_pos[a][b]=INFINITY;
	}
	data->last_next_step=0;
	for (a=0;a<4;a++)
	  for (b=0;b<64;b++)
		  data->floor_green[a][b]=false;
	for (a=0;a<20000;a++)
	  one_car_step(data);
		
	data->time=90000;
	
	
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

  int steps;
  for (steps=0;steps<*(data->gameinfo->steps);steps++)
  {
    //zeitabhÃ¤ngige Berechnungen
		data->time--;
		if (data->time<=0000)
		{
			data->winner=-1;
			data->step=2;
			data->fade=-0.5;
			Mix_FadeOutMusic(500);
			return 0;
		}
		
		
		//Bewegung der verkackten:
		int player;
		for (player=0;player<data->gameinfo->playernum;player++)
			if (data->back_to_start[player])
			{
				data->player[player]->position.y-=0.005;
				rotate_matrix(data->player[player]->rotation,1,0,0,0.005);
				if (data->player[player]->position.y<=-18.0)
				{
					data->player[player]->move=true;
					data->back_to_start[player]=false;
					data->player[player]->kin_energy=0;
					data->player[player]->old_kin_energy=0;
					data->ki_impuls[player]=0;
					data->ki_impuls_dir[player]=0;
					//Schauen, ob sich nichts überschneidet
					bool kollision=true;
					while (kollision)
					{
						kollision=false;
						int a,b;
						for (a=0;a<data->gameinfo->playernum;a++)
						  for (b=a+1;b<data->gameinfo->playernum;b++)
								if (distance(data->player[a]->position.x,data->player[a]->position.y,data->player[b]->position.x,data->player[b]->position.y)<data->player[a]->dimensions.radius+data->player[b]->dimensions.radius)
								{
									kollision=true;
									pphysik_object left=data->player[a];
									pphysik_object right=data->player[b];
									if (left->position.x>right->position.x)
									{
										left=data->player[b];
										right=data->player[a];
									}
									float dx=left->position.x-right->position.x;
									float dy=left->position.y-right->position.y;
									float l=sqrt(dx*dx+dy*dy);
									dx/=l;
									dy/=l;
									
									if (left->position.x>-24.5)
									  left->position.x-=0.001;//*dx;
									//left->position.y+=0.001*dy;
									if (right->position.x< 24.5)
									  right->position.x+=0.001;//*dx;
									//right->position.y-=0.001*dy;
								}						
					}
				}
			}
		
		//Autobewegung
		one_car_step(data);
		
		for (player=0;player<data->gameinfo->playernum;player++)
		{
			calc_cpu(data,player);
			float energy=0.000001;
			if (*(data->gameinfo->joystick[player].axis[0])==0 && *(data->gameinfo->joystick[player].axis[1])<0)
				add_kin_energy(data->player[player],energy,M_PI/180.0*0.0);
			else
			if (*(data->gameinfo->joystick[player].axis[0])>0 && *(data->gameinfo->joystick[player].axis[1])<0)
				add_kin_energy(data->player[player],energy,M_PI/180.0*45.0);
			else
			if (*(data->gameinfo->joystick[player].axis[0])>0 && *(data->gameinfo->joystick[player].axis[1])==0)
				add_kin_energy(data->player[player],energy,M_PI/180.0*90.0);
			else
			if (*(data->gameinfo->joystick[player].axis[0])>0 && *(data->gameinfo->joystick[player].axis[1])>0)
				add_kin_energy(data->player[player],energy,M_PI/180.0*135.0);
			else
			if (*(data->gameinfo->joystick[player].axis[0])==0 && *(data->gameinfo->joystick[player].axis[1])>0)
				add_kin_energy(data->player[player],energy,M_PI/180.0*180.0);
			else
			if (*(data->gameinfo->joystick[player].axis[0])<0 && *(data->gameinfo->joystick[player].axis[1])>0)
				add_kin_energy(data->player[player],energy,M_PI/180.0*225.0);
			else
			if (*(data->gameinfo->joystick[player].axis[0])<0 && *(data->gameinfo->joystick[player].axis[1])==0)
				add_kin_energy(data->player[player],energy,M_PI/180.0*270.0);
			else
			if (*(data->gameinfo->joystick[player].axis[0])<0 && *(data->gameinfo->joystick[player].axis[1])<0)
				add_kin_energy(data->player[player],energy,M_PI/180.0*315.0);
		}
		calc_world_one_step(data->world,0.1);
		
		//Mal schauen, ob es ein Auto / Floor <-> Kugel Problem gibt...
		int street,car;
		for (player=0;player<data->gameinfo->playernum;player++)
		{
			if (data->back_to_start[player])
			  continue;
		  for (street=0;street<4;street++)
			  for (car=0;car<10;car++)
				{
					if (data->street_pos[street][car]==INFINITY)
					  continue;
					if (street==1 || street==2)
					{
						float angle;
						if (sphere_block_kollision(data->player[player]->position.x,data->player[player]->position.y,data->player[player]->dimensions.radius,
																			 data->street_pos[street][car]-2.5,-2.5-(float)street*4.2-1.3,5.0,2.6,&angle))
						{
						  data->player[player]->move=false;
							data->player[player]->kin_energy=0;
							data->player[player]->old_kin_energy=0;
							data->back_to_start[player]=true;
							Mix_PlayChannel(-1,data->snd_negative,0);
						}
					}
					if (street==0 || street==3)
					{
						float angle;
						if (sphere_block_kollision(data->player[player]->position.x,data->player[player]->position.y,data->player[player]->dimensions.radius,
																			 data->street_pos[street][car]-4.5,-2.5-(float)street*4.2-1.4,9.0,2.8,&angle))
						{
						  data->player[player]->move=false;
							data->player[player]->kin_energy=0;
							data->player[player]->old_kin_energy=0;
							data->back_to_start[player]=true;
							Mix_PlayChannel(-1,data->snd_negative,0);
						}
					}
					//Auto Ausmaße: 2.5x1.3
					//Truck Ausmaße: 4.5x1.4
				}
			int line=(int)(round(32.0*data->player[player]->position.x/27.0+31.5));
			street=(int)(round(32.0*data->player[player]->position.y/27.0-4.5));
			if (street>=0 && street<16 && !(data->floor_green[street/4][line]))
			{
				data->player[player]->move=false;
				data->player[player]->kin_energy=0;
				data->player[player]->old_kin_energy=0;
				data->back_to_start[player]=true;
				Mix_PlayChannel(-1,data->snd_negative,0);
			}
			if (street>=16)
			{
				data->winner=player;
        data->step=2;
				data->fade=-3.0;
        Mix_FadeOutMusic(1000);
				Mix_PlayChannel(-1,data->snd_drink,0);
        return 0;
			}
		}
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
	ZWsetlightdiffuse(0,1.0,1.0,1.0);
	ZWsetlightposition(0,0,1,2);
	ZWenablelight(0);
	
	glTranslatef(0,0,-35);
	ZWusetexture(data->floor);
	float size=27.0;
	glNormal3f(0,0,1);
	glBegin(GL_QUADS);
	float b;
	for (b=0;b<75.0;b++)
	{
    float a;
	for (a=0;a<100.0;a++)
	{
    glTexCoord2f(a      /100.0,0.25+b      /100.0);glVertex3f((-1.0+a      /50.0)*size,( 0.75-(b+0.0)/50.0)*size,0);
    glTexCoord2f(a      /100.0,0.25+(b+1.0)/100.0);glVertex3f((-1.0+a      /50.0)*size,( 0.75-(b+1.0)/50.0)*size,0);
    glTexCoord2f((a+1.0)/100.0,0.25+(b+1.0)/100.0);glVertex3f((-1.0+(a+1.0)/50.0)*size,( 0.75-(b+1.0)/50.0)*size,0);
    glTexCoord2f((a+1.0)/100.0,0.25+b      /100.0);glVertex3f((-1.0+(a+1.0)/50.0)*size,( 0.75-(b+0.0)/50.0)*size,0);  
  }
}
	glEnd();


  //Floor
	glNormal3f(0,0,1);
	glColor4f(1,1,1,1);
	int street,line,a;
	for (street=0;street<4;street++)
	  for (line=0;line<64;line++)
		  if (data->floor_green[street][line])
			  for (a=0;a<4;a++)
			    ZWdrawsprite(data->floor,((float)(line)-31.5)*size/32.0,((float)(street*4+a)+4.5)*size/32.0,0.0,size/64.0,size/64.0,64,0);


	for (a=0;a<data->gameinfo->playernum;a++)
	{
		glPushMatrix();
		  glColor4f(0.1,0.1,0.1,(data->back_to_start[a])?0.21:0.7);
			glTranslatef(data->player[a]->position.x,data->player[a]->position.y,1.0);
			ZWdrawsprite(data->floor,0,0,-0.95,1.2,1.2,32,1);
			ZWusetexture(data->gameinfo->facetex);
/*			//Debug
			  sprintf(buffer,"%.1f:%.1f",data->player[a]->position.x,data->player[a]->position.y);
				glColor4f(1,1,1,1);
				glNormal3f(0,0,1);
			  ZWdrawtextmiddle((ZWtext)(data->text),0,0,1,buffer,2.0); */
			glColor4f(data->gameinfo->color[a].r,data->gameinfo->color[a].g,data->gameinfo->color[a].b,(data->back_to_start[a])?0.3:1.0);
			glMultMatrixf(data->player[a]->rotation);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glCallList(data->gameinfo->ball[a]);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glPopMatrix();
	}
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	int car;
	for (street=0;street<4;street++)
	  for (car=0;car<10;car++)
		  if (data->street_pos[street][car]!=INFINITY)
			{
				glPushMatrix();
					glColor4f(data->car_color[street][car].r,data->car_color[street][car].g,data->car_color[street][car].b,1);
					glTranslatef(data->street_pos[street][car],-2.5-(float)street*4.2,0);
					
					//glNormal3f(0,0,1);
					//ZWdrawsprite(data->floor,0,0,0,4.5,1.4,64,0);
					//Auto Ausmaße: 2.5x1.3
					//Truck Ausmaße: 4.5x1.4
					//glColor4f(1,1,1,0.5);
					glRotatef(90,1,0,0);
					if (street>1)
					  glRotatef(90,0,1,0);
					else
					  glRotatef(-90,0,1,0);
					if (street==1 || street==2)
					{
						glTranslatef(0,0,0.8);
						glScalef(3,3,3);
						ZWdrawobject(data->car[0]);
					}
					else
					{
						glTranslatef(0,0,-0.4);
						glScalef(5,5,5);
						ZWdrawobject(data->car[1]);
					}					
				glPopMatrix();
			}

/*	glPushMatrix();
		glColor4f(0,1,0,1);
		glTranslatef(7,0,1.5);
		glScalef(5,5,5);
		glRotatef(90,1,0,0);
		glRotatef(90,0,1,0);
		ZWdrawobject(data->car[1]);
	glPopMatrix();*/

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	
	//Debugausgabe von den Quadern:
	
	pphysik_object object=data->world;
	while (object!=NULL)
	{
		if (object->type==1)
		{
			glColor4f(1,1,1,0.8);
			glPushMatrix();
			glTranslatef(object->position.x+object->dimensions.dimensions.x/2.0,object->position.y+object->dimensions.dimensions.y/2.0,1.50);
			glScalef(object->dimensions.dimensions.x/2.0,object->dimensions.dimensions.y/2.0,1.5);
			ZWdrawobject(data->wall);
			//ZWdrawsprite(data->floor,object->position.x+object->dimensions.dimensions.x/2.0,object->position.y+object->dimensions.dimensions.y/2.0,1.00,object->dimensions.dimensions.x/2.0,object->dimensions.dimensions.y/2.0,64,0);
			glPopMatrix();
		}
		object=object->next;
	}

  glDepthFunc(GL_ALWAYS); 
	glDisable(GL_LIGHTING);

  //Restzeit
	glColor4f(1,1,1,0.7);
  sprintf(buffer,"%i",data->time/1000);
  ZWdrawtextmiddle((ZWtext)(data->text),-25.5,-19.7,0,buffer,2.0);  
  ZWdrawtextmiddle((ZWtext)(data->text), 25.5,-19.7,0,buffer,2.0);  
  ZWdrawtextmiddle((ZWtext)(data->text),-25.5, 19.1,0,buffer,2.0);  
  ZWdrawtextmiddle((ZWtext)(data->text), 25.5, 19.1,0,buffer,2.0);  

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
    gameresult->winner[b]=(b==data->winner)?1:0;
    
  ZWdeletetexture(data->floor);
  ZWdeletetexture(data->cartex);
  
	ZWdeleteobject(data->wall);    
	ZWdeleteobject(data->car[0]);    
	ZWdeleteobject(data->car[1]);    
	
	  
  //Musik zurÃ¼cksetzen:
  Mix_VolumeMusic(data->old_music_volume);
  Mix_FreeMusic(data->backgroundmusic);
  Mix_FreeChunk(data->snd_negative);  
  Mix_FreeChunk(data->snd_drink);  
  
}
