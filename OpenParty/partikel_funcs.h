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
int ZWcalc_color_explosion(pZWpartikel partikel,int n)
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

  if (partikel->common.live<2.0)
  {
    partikel->sprite.color.a=partikel->common.live/2.0;
  }
  if (partikel->common.live<0) return 1;
  return 0;
}

void ZWnewcolorexplosion(tZWpoint position,int partikelanz,float power,GLuint texture,int cutcount,int partikelnr,float size,float r,float g,float b)
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
    tZWpoint dummy;
    tZWpoint scale;
    scale.x=0;
    scale.y=0;
    scale.z=0;
    tZWcolor color;
    color.r=r;
    color.g=g;
    color.b=b;
    color.a=1.0;    
    float live=(float)(1800+rand()%400)/1000.0;
    ZWnewspritepartikel(position,dummy,scale,0,ZWcalc_color_explosion,live,value,texture,cutcount,partikelnr,color);
  }
}

int ZWcalc_color_circle(pZWpartikel partikel,int n)
{
  int a;
  for (a=0;a<n;a++)
  {
    partikel->common.position.x+=partikel->common.value[0].f;
    partikel->common.position.y+=partikel->common.value[1].f;
    partikel->common.position.z+=partikel->common.value[2].f;
    partikel->common.value[1].f-=partikel->common.value[3].f;
  }
  partikel->common.live-=(float)n/1000.0;

  partikel->sprite.color.a=partikel->common.live*2.0;
  if (partikel->common.live<0) return 1;
  return 0;
}

void ZWnewcolorcircle(tZWpoint position,int partikelanz,float power,GLuint texture,int cutcount,int partikelnr,float size,float r,float g,float b,float angle,float gravitation,float radius)
{
  int a;
  for (a=0;a<partikelanz;a++)
  {
    pZWfloatint value=(pZWfloatint)malloc(sizeof(tZWfloatint)*4);
    float xz_angle=(float)(rand()%2000)*M_PI/1000.0;
    value[0].f=sin(xz_angle)*power;
    value[1].f=sin(angle+(float)(rand()%1000)/2000.0-0.25)*power;
    value[2].f=cos(xz_angle)*power;
    value[3].f=gravitation;
    tZWpoint dummy;
    tZWpoint scale;
    scale.x=size;
    scale.y=size;
    scale.z=size;
    tZWcolor color;
    color.r=r;
    color.g=g;
    color.b=b;
    color.a=1.0;
    float live=0.5;//(float)(1800+rand()%400)/1000.0;
    tZWpoint newposition=position;
    newposition.x+=sin(xz_angle)*radius;
    newposition.z+=cos(xz_angle)*radius;
    ZWnewspritepartikel(newposition,dummy,scale,0,ZWcalc_color_circle,live,value,texture,cutcount,partikelnr,color);
  }
}


