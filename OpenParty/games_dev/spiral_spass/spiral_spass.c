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
  Mix_Music *backgroundmusic;
  Mix_Chunk *snd_win;
  int old_music_volume;
  object3d spiral_zylinder;
  object3d spiral_basis;
  object3d spiral_box;
  object3d ballballon[8];
  GLuint texture;
  float height[8];
  int nextbutton[8];
} tdata;

int calc_konfetti(pZWpartikel partikel,int steps)
{
  partikel->common.live-=steps;
  int a;
  for (a=0;a<steps;a++)
  if (partikel->common.position.y>-1.1)
    {
      partikel->common.value[3].f*=0.93;
      partikel->common.value[4].f-=0.00000015;
      partikel->common.value[5].f*=0.93;
      partikel->common.position.x+=partikel->common.value[3].f;
      partikel->common.position.y+=partikel->common.value[4].f;
      partikel->common.position.z+=partikel->common.value[5].f;
      partikel->common.rotation.y+=0.4;
      partikel->common.rotation.x+=0.2;
      partikel->common.rotation.z+=0.1;
    }
  return 0;
}

void draw_konfetti(pZWpartikel partikel)
{
  glRotatef(partikel->common.rotation.y,0,1,0);
  glRotatef(partikel->common.rotation.x,1,0,0);
  glRotatef(partikel->common.rotation.z,0,0,1);
  glColor4f(partikel->common.value[0].f,partikel->common.value[1].f,partikel->common.value[2].f,1.0);
  glBegin(GL_QUADS);
    glVertex3f(-1.0*partikel->common.scale.x, 1.0*partikel->common.scale.y,0);
    glVertex3f(-1.0*partikel->common.scale.x,-1.0*partikel->common.scale.y,0);
    glVertex3f( 1.0*partikel->common.scale.x,-1.0*partikel->common.scale.y,0);
    glVertex3f( 1.0*partikel->common.scale.x, 1.0*partikel->common.scale.y,0);
    //und nochmal rückwärts, so muss man Culling nicht ausschalten
    glVertex3f( 1.0*partikel->common.scale.x, 1.0*partikel->common.scale.y,0);
    glVertex3f( 1.0*partikel->common.scale.x,-1.0*partikel->common.scale.y,0);
    glVertex3f(-1.0*partikel->common.scale.x,-1.0*partikel->common.scale.y,0);
    glVertex3f(-1.0*partikel->common.scale.x, 1.0*partikel->common.scale.y,0);
  glEnd();
  glRotatef(-partikel->common.rotation.z,0,0,1);
  glRotatef(-partikel->common.rotation.x,1,0,0);
  glRotatef(-partikel->common.rotation.y,0,1,0);
}

void throw_konfetti(float x,float y,float z,int num)
{
  int a;
  for (a=0;a<num;a++)
  {
    tZWpoint position;
    float r=(float)(rand()%1000)*M_PI/500.0;
    position.x=x+sin(r)*0.22;//(float)(rand()%1000)/2000.0-0.25;
    position.y=y;//+(float)(rand()%1000)/2000.0-0.25;
    position.z=z+cos(r)*0.22;//(float)(rand()%1000)/2000.0-0.25;
    tZWpoint rotation;
    rotation.x=(float)(rand()%3600)/10.0;
    rotation.y=(float)(rand()%3600)/10.0;
    rotation.z=(float)(rand()%3600)/10.0;
    tZWpoint scale;
    scale.x=0.08;
    scale.y=0.08;
    scale.z=0.08;
    pZWfloatint value=(pZWfloatint)malloc(sizeof(tZWfloatint)*6);
    value[0].f=(float)(rand()%256)/255.0;
    value[1].f=(float)(rand()%256)/255.0;
    value[2].f=(float)(rand()%256)/255.0;
    value[3].f=(float)(rand()%1000-500)/20000.0;
    value[4].f=-(float)(rand()%1000)/2000000.0;
    value[5].f=(float)(rand()%1000-500)/20000.0;
    ZWnewmodellpartikel(position,rotation,scale,0,calc_konfetti,1000.0,value,draw_konfetti);
  }
}


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
  sprintf(buffer,"%ssounds/explosion.ogg",gameinfo->datafolder);
  data->snd_win=Mix_LoadWAV(buffer);
  Mix_VolumeChunk(data->snd_win,(int)(round(128.0*gameinfo->volume)));
  Mix_FadeInMusic(data->backgroundmusic,-1,500);

  //Objekte laden  
  sprintf(buffer,"%stextures/spiral_spass.png",gameinfo->datafolder);
  data->texture=ZWloadtexturefromfileA(buffer,0,gameinfo->texquali);
  sprintf(buffer,"%smodels/spiral_zylinder.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->spiral_zylinder),buffer,data->texture);
  ZWcreatedrawlist(&(data->spiral_zylinder),gameinfo->lightquali);
  sprintf(buffer,"%smodels/spiral_basis.3dm",gameinfo->datafolder);
  ZWload3dm(&(data->spiral_basis),buffer,data->texture);
  ZWcreatedrawlist(&(data->spiral_basis),0);
  sprintf(buffer,"%smodels/spiral_box.3dm",gameinfo->datafolder);
	ZWload3dm(&(data->spiral_box),buffer,data->texture);
  ZWcreatedrawlist(&(data->spiral_box),0);
  for (a=0;a<(data->gameinfo->playernum);a++)
  {
		sprintf(buffer,"%smodels/ballballon.3dm",gameinfo->datafolder);		
    ZWload3dm(&(data->ballballon[a]),buffer,gameinfo->facetex);
    int b;
    for (b=0;b<data->ballballon[a].pointnum;b++)
    {
      float dazu=((float)(gameinfo->facenr[a]%4))*0.25;
      data->ballballon[a].p[b].u+=dazu;
      dazu=((float)(gameinfo->facenr[a]/4))*0.25;
      data->ballballon[a].p[b].v+=dazu;    
    }
    ZWcreatedrawlist(&(data->ballballon[a]),gameinfo->lightquali);
    data->height[a]=0.0;
    data->nextbutton[a]=0;
  }
  
  //Licht!
  ZWenablelight(0);
  ZWsetlightambient(0,0.0,0.0,0.0);
  ZWsetlightdiffuse(0,1.0,1.0,1.0);
  ZWsetlightposition(0,0.0,0.0,0.0);
  ZWsetmaterial(lnone);
  

  
  //Spiel ansich vorbereiten
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

  //zeitunabhängige Bewegung:

  //Gewinner überprüfen:
  int ende=0;
  int a;
  for (a=0;a<data->gameinfo->playernum;a++)
    if (data->height[a]>=1.0)
    {
      throw_konfetti((float)a*0.85-2.975+(float)(8-data->gameinfo->playernum)*0.425,1.2,-5,50);
      ende=1;
    }
  if (ende)
  {
    data->step=2;
    data->fade=-10.0;
    Mix_PlayChannel(-1,data->snd_win,0);
    return 0;  
  }


  int steps;
  for (steps=0;steps<*(data->gameinfo->steps);steps++)
  {
    //zeitabhängige Berechnungen
  }
  
  for (a=0;a<data->gameinfo->playernum;a++)
    if (data->gameinfo->iscpu[a])
    {
      int b;
      for (b=0;b<*(data->gameinfo->steps);b++)
        if (rand()%180==0)
        {
          data->nextbutton[a]=(data->nextbutton[a]+1)%4;
          data->height[a]+=0.01;
        }
    }
    else
    {
      //Erst schauen, ob eine falsche Taste gedrückt wird:
      int button1,button2,button3;
      switch (data->nextbutton[a])
      {
        case 0: button1=1;button2=2;/*button3=3;*/ break;
        case 1: button1=2;button2=3;/*button3=0;*/ break;
        case 2: button1=3;button2=0;/*button3=1;*/ break;
        default: button1=0;button2=1;/*button3=2;*/
      }
      if (*(data->gameinfo->joystick[a].button[button1]) || 
          *(data->gameinfo->joystick[a].button[button2])/* ||
          *(data->gameinfo->joystick[a].button[button3])*/)
      {
       /* data->height[a]-=(float)(*(data->gameinfo->steps))/15000.0;
        if (data->height[a]<0.0)
          data->height[a]=0.0;*/
      }
      else
      if (*(data->gameinfo->joystick[a].button[data->nextbutton[a]])/* ||
         (*(data->gameinfo->joystick[a].axis[0])<0 && data->nextbutton[a]==0) ||
         (*(data->gameinfo->joystick[a].axis[1])<0 && data->nextbutton[a]==1) ||
         (*(data->gameinfo->joystick[a].axis[0])>0 && data->nextbutton[a]==2) ||
         (*(data->gameinfo->joystick[a].axis[1])>0 && data->nextbutton[a]==3)*/)
      {
        *(data->gameinfo->joystick[a].button[data->nextbutton[a]])=0;
        data->nextbutton[a]=(data->nextbutton[a]+1)%4;
        data->height[a]+=0.01;
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

  glTranslatef(0,0.6,-3.0);
  glScalef(3.5,2.0,3.5);
  ZWdrawobject(data->spiral_box);
  
  glLoadIdentity();



  glColor4f(1,1,1,1);
  int a;
  for (a=0;a<data->gameinfo->playernum;a++)
  {
    glTranslatef((float)a*0.85-2.975+(float)(8-data->gameinfo->playernum)*0.425,data->height[a]*1.4-1.6,-5);
    glRotatef(data->height[a]*360.0*8.0,0,-1,0);
    ZWdrawobject(data->spiral_zylinder);
    glLoadIdentity();
  }

  for (a=0;a<data->gameinfo->playernum;a++)
  {
    glTranslatef((float)a*0.85-2.975+(float)(8-data->gameinfo->playernum)*0.425,-1.1,-5);
    glScalef(0.4,0.4,0.4);
    ZWdrawobject(data->spiral_basis);
    glLoadIdentity();
  }

  for (a=0;a<data->gameinfo->playernum;a++)
  {
    if (data->height[a]>=1.0)
      continue;
    glTranslatef((float)a*0.85-2.975+(float)(8-data->gameinfo->playernum)*0.425,1.2,-5);
    glScalef(0.5,0.5,0.5);
    glColor4f(data->gameinfo->color[a].r,data->gameinfo->color[a].g,data->gameinfo->color[a].b,0.7);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    ZWdrawobject(data->ballballon[a]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glLoadIdentity();
  }

  glDisable(GL_LIGHTING);


  glDisable(GL_TEXTURE_2D);
  glTranslatef(0,0,0.0);
  tZWpoint cameraposition;
  cameraposition.x=0;
  cameraposition.y=0;
  cameraposition.z=0.0;
  tZWpoint camerarotation;
  camerarotation.x=0;
  camerarotation.y=0;
  camerarotation.z=0;
  ZWdrawpartikel(cameraposition,camerarotation);
  glLoadIdentity();
  glEnable(GL_TEXTURE_2D);

  glDepthFunc(GL_ALWAYS);  

  glColor4f(1,1,1,1);
  for (a=0;a<data->gameinfo->playernum;a++)
  {
    glTranslatef((float)a*0.85-2.975+(float)(8-data->gameinfo->playernum)*0.425,-1.8,-4.5);
    glScalef(0.4,0.4,0.4);
    ZWusetexture(data->texture);
    float ldazu=0;
    float odazu=0;
    float rdazu=0;
    float udazu=0;
    switch (data->nextbutton[a])
    {
      case 0: ldazu=0.25; break;
      case 1: odazu=0.25; break;
      case 2: rdazu=0.25; break;
      default: udazu=0.25;    
    }
    glBegin(GL_TRIANGLES); //links
      glTexCoord2f(ldazu+0.0  ,0.5      );  glVertex3f(-1, 0.8125,0);
      glTexCoord2f(ldazu+0.0  ,0.703125 );  glVertex3f(-1,-0.8125,0);
      glTexCoord2f(ldazu+0.125,0.6015625);  glVertex3f( 0, 0     ,0);
    glEnd();
    glBegin(GL_TRIANGLES); //oben
      glTexCoord2f(odazu+0.25 ,0.5      );  glVertex3f( 1, 0.8125,0);
      glTexCoord2f(odazu+0.0  ,0.5      );  glVertex3f(-1, 0.8125,0);
      glTexCoord2f(odazu+0.125,0.6015625);  glVertex3f( 0, 0     ,0);
    glEnd();
    glBegin(GL_TRIANGLES); //rechts
      glTexCoord2f(rdazu+0.25 ,0.703125 );  glVertex3f( 1,-0.8125,0);
      glTexCoord2f(rdazu+0.25 ,0.5      );  glVertex3f( 1, 0.8125,0);
      glTexCoord2f(rdazu+0.125,0.6015625);  glVertex3f( 0, 0     ,0);
    glEnd();
    glBegin(GL_TRIANGLES); //unten
      glTexCoord2f(udazu+0.0  ,0.703125 );  glVertex3f(-1,-0.8125,0);
      glTexCoord2f(udazu+0.25 ,0.703125 );  glVertex3f( 1,-0.8125,0);
      glTexCoord2f(udazu+0.125,0.6015625);  glVertex3f( 0, 0     ,0);
    glEnd();
    glLoadIdentity();
  }
   
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
  if (data->step!=1 && data->fade>=0.0 && data->fade<=1.0)
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
    gameresult->winner[b]=(data->height[b]>=1.0);
 
  ZWdisablelight(0);
 
  ZWresetpartikel();
 
  //Texturen und Objekte freigeben
  ZWdeleteobject(data->spiral_zylinder);
  ZWdeleteobject(data->spiral_basis);
  ZWdeleteobject(data->spiral_box);
  int a;
  for (a=0;a<(data->gameinfo->playernum);a++)
    ZWdeleteobject(data->ballballon[a]);    
  ZWdeletetexture(data->texture);
    
  //Musik zurücksetzen:
  Mix_VolumeMusic(data->old_music_volume);
  Mix_FreeMusic(data->backgroundmusic);
  Mix_FreeChunk(data->snd_win);
  
}
