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

#include "partikel.h"

#ifdef WIN32
  #define EXPORT DLLIMPORT
#else
  #define EXPORT
#endif

#include <math.h>
#include "./../texture/texture.h"

pZWpartikel ___ZWpartikellist=NULL;
pZWpartikel ___ZWpartikelbuffer=NULL;
int ___ZWpartikellistcount=0;
int ___ZWpartikelbuffercount=0;
int ___ZWpartikelbuffersize=256;

void ZWsyncpartikelbuffer(void)
{
  if (___ZWpartikelbuffercount==0) return;
  pZWpartikel partikellist=(pZWpartikel)malloc(sizeof(tZWpartikel)*(___ZWpartikellistcount+___ZWpartikelbuffercount));
  memcpy(partikellist,___ZWpartikellist,sizeof(tZWpartikel)*___ZWpartikellistcount);
  free(___ZWpartikellist);
  ___ZWpartikellist=partikellist;
  memcpy(&(partikellist[___ZWpartikellistcount]),___ZWpartikelbuffer,sizeof(tZWpartikel)*___ZWpartikelbuffercount);
  ___ZWpartikellistcount+=___ZWpartikelbuffercount;
  ___ZWpartikelbuffercount=0;
}

EXPORT void ZWnewspritepartikel(tZWpoint position,tZWpoint rotation,tZWpoint scale,GLbyte rotate,
                                int (*calc_partikel)(pZWpartikel partikel,int n),float live,pZWfloatint value,
                                GLuint texture, GLuint cutcount, GLuint spritenr,tZWcolor color)
{
  if (___ZWpartikelbuffer==NULL)
    ___ZWpartikelbuffer=(pZWpartikel)malloc(sizeof(tZWpartikel)*___ZWpartikelbuffersize);
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].type=0;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.alpha=1;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.position=position;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.rotate=rotate;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.rotation=rotation;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.scale=scale;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.calc_partikel=calc_partikel;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.live=live;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.value=value;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].sprite.texture=texture;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].sprite.cutcount=cutcount;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].sprite.spritenr=spritenr;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].sprite.color=color;
  ___ZWpartikelbuffercount++;
  if (___ZWpartikelbuffercount>=___ZWpartikelbuffersize) ZWsyncpartikelbuffer();
}

EXPORT void ZWnewmodellpartikel(tZWpoint position,tZWpoint rotation,tZWpoint scale,GLbyte rotate,
                                int (*calc_partikel)(pZWpartikel partikel,int n),float live,pZWfloatint value,
                                void (*draw_partikel)(pZWpartikel partikel))
{
  if (___ZWpartikelbuffer==NULL)
    ___ZWpartikelbuffer=(pZWpartikel)malloc(sizeof(tZWpartikel)*___ZWpartikelbuffersize);
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].type=1;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.alpha=1;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.position=position;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.rotate=rotate;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.rotation=rotation;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.scale=scale;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.calc_partikel=calc_partikel;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.live=live;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].common.value=value;
  ___ZWpartikelbuffer[___ZWpartikelbuffercount].modell.draw_partikel=draw_partikel;
  ___ZWpartikelbuffercount++;
  if (___ZWpartikelbuffercount>=___ZWpartikelbuffersize) ZWsyncpartikelbuffer();
}

int ZWteilepartikel(int links,int rechts)
{
  int i=links;
  int j=rechts-1;
  float pivot=___ZWpartikellist[rechts].common.cameradestination;
  do
  {
    while (___ZWpartikellist[i].common.cameradestination<=pivot && i<rechts)
      i++;
    while (___ZWpartikellist[j].common.cameradestination>=pivot && j>links)
      j--;
    if (i<j)
    {
      tZWpartikel temp=___ZWpartikellist[i];
      ___ZWpartikellist[i]=___ZWpartikellist[j];
      ___ZWpartikellist[j]=temp;
    }
  }
  while (i<j);
  if (___ZWpartikellist[i].common.cameradestination>pivot)
  {
    tZWpartikel temp=___ZWpartikellist[i];
    ___ZWpartikellist[i]=___ZWpartikellist[rechts];
    ___ZWpartikellist[rechts]=temp;
  }     
  return i;
}

void ZWquicksortpartikel(int links,int rechts)
{
  if (links<rechts)
  {
    int teiler=ZWteilepartikel(links,rechts);
    ZWquicksortpartikel(links,teiler-1);
    ZWquicksortpartikel(teiler+1,rechts);
  }
}

EXPORT void ZWdrawpartikel(tZWpoint cameraposition,tZWpoint camerarotation)
{
  //Ausrichten der "Kamera"
  glRotatef(-camerarotation.x,1,0,0);
  glRotatef(-camerarotation.y,0,1,0);
  glRotatef(-camerarotation.z,0,0,1);
  float matrix[4][4];
  glGetFloatv(GL_MODELVIEW_MATRIX,(void*)matrix);
  float vektor[3];
  vektor[0]=-matrix[0][2];
  vektor[1]=-matrix[1][2];
  vektor[2]=-matrix[2][2];
  float betrag=sqrt(vektor[0]*vektor[0]+vektor[1]*vektor[1]+vektor[2]*vektor[2]);
  vektor[0]/=betrag;
  vektor[1]/=betrag;
  vektor[2]/=betrag;
  
  int a;
  //Abstände zur Cameraposition berechnen
  for (a=0;a<___ZWpartikellistcount;a++)
  {
    ___ZWpartikellist[a].common.cameradestination=-fabs((___ZWpartikellist[a].common.position.x-cameraposition.x)*vektor[0]+
                                                        (___ZWpartikellist[a].common.position.y-cameraposition.y)*vektor[1]+
                                                        (___ZWpartikellist[a].common.position.z-cameraposition.z)*vektor[2]);
  }
/*    ___ZWpartikellist[a].common.cameradestination=-sqrt((___ZWpartikellist[a].common.position.x-cameraposition.x)*
                                                  (___ZWpartikellist[a].common.position.x-cameraposition.x)+
                                                  (___ZWpartikellist[a].common.position.y-cameraposition.y)*
                                                  (___ZWpartikellist[a].common.position.y-cameraposition.y)+
                                                  (___ZWpartikellist[a].common.position.z-cameraposition.z)*
                                                  (___ZWpartikellist[a].common.position.z-cameraposition.z));*/
  //Sortieren
  ZWquicksortpartikel(0,___ZWpartikellistcount-1);
  //Zeichnen
  
  glTranslatef(-cameraposition.x,-cameraposition.y,-cameraposition.z);
  
  for (a=0;a<___ZWpartikellistcount;a++)
  {
    ___ZWpartikellist[a].common.nr=a;
    glPushMatrix();
    glTranslatef(___ZWpartikellist[a].common.position.x,___ZWpartikellist[a].common.position.y,
                 ___ZWpartikellist[a].common.position.z);
    if (___ZWpartikellist[a].common.rotate==0)
    {
      glRotatef(camerarotation.z,0,0,1);
      glRotatef(camerarotation.y,0,1,0);    
      glRotatef(camerarotation.x,1,0,0);
    }
    else
    {
      glRotatef(___ZWpartikellist[a].common.rotation.x,1,0,0);
      glRotatef(___ZWpartikellist[a].common.rotation.y,0,1,0);
      glRotatef(___ZWpartikellist[a].common.rotation.z,0,0,1);
    }    
    if (___ZWpartikellist[a].common.type==0) //Sprite
    {
      glColor4f(___ZWpartikellist[a].sprite.color.r,___ZWpartikellist[a].sprite.color.g,___ZWpartikellist[a].sprite.color.b,
                ___ZWpartikellist[a].sprite.color.a);
      ZWdrawsprite(___ZWpartikellist[a].sprite.texture   ,0,0,0,
                   ___ZWpartikellist[a].sprite.scale.x   ,___ZWpartikellist[a].sprite.scale.y,
                   ___ZWpartikellist[a].sprite.cutcount  ,___ZWpartikellist[a].sprite.spritenr);
    }
    else
    {
      if (___ZWpartikellist[a].modell.draw_partikel!=NULL) 
        ___ZWpartikellist[a].modell.draw_partikel(&(___ZWpartikellist[a]));
    }
    glPopMatrix();
  }

  glTranslatef(cameraposition.x,cameraposition.y,cameraposition.z); 
  glRotatef(camerarotation.z,0,0,1);
  glRotatef(camerarotation.y,0,1,0);    
  glRotatef(camerarotation.x,1,0,0);
}

void ZWdeletepartikel(int nr)
{
  if (___ZWpartikellist[nr].common.value!=NULL)
    free(___ZWpartikellist[nr].common.value);
  if (nr<___ZWpartikellistcount-1)
  {
    //memcpy(&(___ZWpartikellist[nr]),&(___ZWpartikellist[nr+1]),(___ZWpartikellistcount-nr-1)*sizeof(tZWpartikel));
    ___ZWpartikellist[nr]=___ZWpartikellist[___ZWpartikellistcount-1];
  }
  ___ZWpartikellistcount--;
}

EXPORT void ZWcalcpartikel(int n)
{
  ZWsyncpartikelbuffer();
  int a;
  for (a=0;a<___ZWpartikellistcount;a++)
    if (___ZWpartikellist[a].common.calc_partikel!=NULL)
    {
      if (___ZWpartikellist[a].common.calc_partikel(&(___ZWpartikellist[a]),n)!=0)
      {
        ZWdeletepartikel(a);
        a--;
      }
    }
    else
    {
      ___ZWpartikellist[a].common.live-=n;
      if (___ZWpartikellist[a].common.live<0)
      {
        ZWdeletepartikel(a);
        a--;
      }
    }
}

/*void ZWdraw_partikel_explosion(pZWpartikel partikel)
{
//  glDisable(GL_LIGHTING);
  if (partikel->common.live>1.8)
    glColor4f(1,1,0,0.8);
  else
  if (partikel->common.live>1.3)
    glColor4f(1,2.0*(partikel->common.live-1.3),0,0.8);
  else
  if (partikel->common.live>0.8)
    glColor4f(0.5+(partikel->common.live-0.8),0.5-(partikel->common.live-0.8),0.5-(partikel->common.live-0.8),
              0.8-(partikel->common.live-1.3)*0.8);
  else
    glColor4f(0.5,0.5,0.5,partikel->common.live/2.0);
      
  ZWdrawsprite(partikel->common.value[4].i,0,0,0,partikel->common.scale.x,partikel->common.scale.y,
               partikel->common.value[5].i,partikel->common.value[6].i);
// glEnable(GL_LIGHTING);
}*/

int ZWcalc_partikel_explosion(pZWpartikel partikel,int n)
{
  partikel->common.scale.x=pow(4.4-partikel->common.live*2.0,0.25)/2.0*partikel->common.value[4].f;
  partikel->common.scale.y=pow(4.4-partikel->common.live*2.0,0.25)/2.0*partikel->common.value[4].f;
  int a;
  for (a=0;a<n;a++)
  {
    partikel->common.position.x+=partikel->common.value[0].f;
    partikel->common.position.y+=partikel->common.value[1].f;
    partikel->common.position.z+=partikel->common.value[2].f;
    partikel->common.value[0].f*=0.999;
    partikel->common.value[1].f*=0.999;
    partikel->common.value[2].f*=0.999;    
  }
  partikel->common.live-=(float)n/1000.0;

  if (partikel->common.live>1.8)
  {
    //glColor4f(1,1,0,0.8);
    partikel->sprite.color.r=1;
    partikel->sprite.color.g=1;
    partikel->sprite.color.b=0;
    partikel->sprite.color.a=0.8;
  }
  else
  if (partikel->common.live>1.3)
  {
    //glColor4f(1,2.0*(partikel->common.live-1.3),0,0.8);
    partikel->sprite.color.r=1;
    partikel->sprite.color.g=2.0*(partikel->common.live-1.3);
    partikel->sprite.color.b=0;
    partikel->sprite.color.a=0.8;
  }
  else
  if (partikel->common.live>0.8)
  {
    //glColor4f(0.5+(partikel->common.live-0.8),0.5-(partikel->common.live-0.8),0.5-(partikel->common.live-0.8),
    //          0.8-(partikel->common.live-1.3)*0.8);
    partikel->sprite.color.r=0.5+(partikel->common.live-0.8);
    partikel->sprite.color.g=0.5-(partikel->common.live-0.8);
    partikel->sprite.color.b=0.5-(partikel->common.live-0.8);
    partikel->sprite.color.a=0.8-(partikel->common.live-1.3)*0.8;
  }          
  else
  {
    //glColor4f(0.5,0.5,0.5,partikel->common.live/2.0);
    partikel->sprite.color.r=0.5;
    partikel->sprite.color.g=0.5;
    partikel->sprite.color.b=0.5;
    partikel->sprite.color.a=partikel->common.live/2.0;
  }

  if (partikel->common.live<0) return 1;
  return 0;
}

EXPORT void ZWnewpartikelexplosion(tZWpoint position,int partikelanz,float power,GLuint texture,int cutcount,int partikelnr,float size)
{
  int a;
  for (a=0;a<partikelanz;a++)
  {
    pZWfloatint value=(pZWfloatint)malloc(sizeof(tZWfloatint)*5/*7*/);
    value[0].f=((float)(rand()%2000-1000)/1000000.0)*power;
    value[1].f=((float)(rand()%2000-1000)/1000000.0)*power;
    value[2].f=((float)(rand()%2000-1000)/1000000.0)*power;
    value[3].f=power;
    value[4].f=size;
    /*value[4].i=texture;
    value[5].i=cutcount;
    value[6].i=partikelnr;*/
    tZWpoint dummy;
    tZWpoint scale;
    scale.x=0;
    scale.y=0;
    scale.z=0;
    tZWcolor color;
    color.r=1;
    color.g=1;
    color.b=0;
    color.a=0.8;    
    float live=(float)(1800+rand()%400)/1000.0;
    ZWnewspritepartikel(position,dummy,scale,0,ZWcalc_partikel_explosion,live,value,texture,cutcount,partikelnr,color
                        /*ZWdraw_partikel_explosion*/);
  }
}

void ZWresetpartikel(void)
{
  int a;
  for (a=0;a<___ZWpartikellistcount;a++)
    if (___ZWpartikellist[a].common.value!=NULL)
      free(___ZWpartikellist[a].common.value);
  free(___ZWpartikellist);
  free(___ZWpartikelbuffer);
  ___ZWpartikelbuffer=NULL;
  ___ZWpartikellist=NULL;
  ___ZWpartikellistcount=0;
  ___ZWpartikelbuffercount=0;
  ___ZWpartikelbuffersize=256;
}
