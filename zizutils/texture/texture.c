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

#include "texture.h"

#ifdef WIN32
  #define EXPORT DLLIMPORT
#else
  #define EXPORT
#endif

typedef struct sZWtexturename *pZWtexturename;
typedef struct sZWtexturename {
  char filename[1024];
  GLuint texture;
  int alpha;
  int better;
	int refcount;
  pZWtexturename next;
} tZWtexturename;


GLuint ZWmomtexnr=65535;
pZWtexturename ___firsttexture=NULL;
float ZWtexturepercent___=0.01;
int ZWtexture_width___[1024];
int ZWtexture_height___[1024];
int ZWtexture_stdreduce___=1;

EXPORT void ZWsetstdreduce(int i)
{
	ZWtexture_stdreduce___=i;
}

EXPORT int ZWgetstdreduce(void)
{
	return ZWtexture_stdreduce___;
}

EXPORT void ZWsettextureresolution(GLuint texnr,int w,int h)
{
  ZWtexture_width___[texnr]=w;
  ZWtexture_height___[texnr]=h;
}


EXPORT void ZWsettexturepercent(float p)
{
  ZWtexturepercent___=p;
}

EXPORT float ZWgettexturepercent(void)
{
  return ZWtexturepercent___;
}


EXPORT GLuint ZWloadtexturefromfile(char* name,int better,int reduce)
{
  if (reduce<1) return -1;
	ZWtexture_stdreduce___=reduce; 

  pZWtexturename now=___firsttexture;
  while (now!=NULL)
  {
		now->refcount++;
    if (strcmp(now->filename,name)==0) return now->texture;
    now=now->next;
  }

  SDL_Surface* bild= IMG_Load(name);
  
  //Skaliertes Bild erzeugen...
  GLubyte *scale_bild=(GLubyte*)malloc(sizeof(GLubyte)*(bild->w/reduce)*(bild->h/reduce)*3);
  GLubyte *bild_pixels=(GLubyte*)bild->pixels;
  int a,b;
  for (a=0;a<bild->w/reduce;a++)
    for (b=0;b<bild->h/reduce;b++)
    {
/*      scale_bild[a*3  +b*(bild->w/reduce)*3]=bild_pixels[a*3*reduce  +b*reduce*bild->w*3];
      scale_bild[a*3+1+b*(bild->w/reduce)*3]=bild_pixels[a*3*reduce+1+b*reduce*bild->w*3];
      scale_bild[a*3+2+b*(bild->w/reduce)*3]=bild_pixels[a*3*reduce+2+b*reduce*bild->w*3];*/    
      int c,d;
      int summer=0,summeg=0,summeb=0;
      for (c=0;c<reduce;c++)
        for (d=0;d<reduce;d++)
        {
          int x=(a*reduce+c);
          int y=(b*reduce+d);
          summer+=(int)bild_pixels[x*3  +y*bild->w*3];
          summeg+=(int)bild_pixels[x*3+1+y*bild->w*3];
          summeb+=(int)bild_pixels[x*3+2+y*bild->w*3];
        }
      summer/=reduce*reduce;
      summeg/=reduce*reduce;
      summeb/=reduce*reduce;
      scale_bild[a*3  +b*(bild->w/reduce)*3]=summer;
      scale_bild[a*3+1+b*(bild->w/reduce)*3]=summeg;
      scale_bild[a*3+2+b*(bild->w/reduce)*3]=summeb;
    }
  GLuint texnr;
  glGenTextures(1, &texnr);
  //glBindTexture(GL_TEXTURE_2D, texnr);
  ZWusetexture(texnr);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);    
  if (better==1)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  }
  else
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bild->w/reduce, bild->h/reduce, GL_RGB, GL_UNSIGNED_BYTE, scale_bild);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, bild->w/reduce, bild->h/reduce,0, GL_RGB, GL_UNSIGNED_BYTE, scale_bild);
  if (texnr<1024)
  {
    ZWtexture_width___[texnr]=bild->w/reduce;
    ZWtexture_height___[texnr]=bild->h/reduce;
  }
  free(scale_bild);
  SDL_FreeSurface(bild);
  
  pZWtexturename newname=(pZWtexturename)malloc(sizeof(tZWtexturename));
  sprintf(newname->filename,"%s",name);
  newname->texture=texnr;
  newname->next=___firsttexture;
  newname->alpha=1;
  newname->better=better;  
	newname->refcount=1;
  ___firsttexture=newname;    
  
  return texnr;
}


EXPORT GLuint ZWloadtexturefromfileA(char* name,int better,int reduce)
{
  if (reduce<1) return -1; 
	ZWtexture_stdreduce___=reduce; 

  pZWtexturename now=___firsttexture;
  while (now!=NULL)
  {
		now->refcount++;
		if (strcmp(now->filename,name)==0) return now->texture;
		now=now->next;
  }

  SDL_Surface* bild= IMG_Load(name);
  
  //Skaliertes Bild erzeugen...
  GLubyte *scale_bild=(GLubyte*)malloc(sizeof(GLubyte)*(bild->w/reduce)*(bild->h/reduce)*4);
  GLubyte *bild_pixels=(GLubyte*)bild->pixels;
  int a,b;
  for (a=0;a<bild->w/reduce;a++)
    for (b=0;b<bild->h/reduce;b++)
    {
      int c,d;
      int summer=0,summeg=0,summeb=0,summea=0;
      for (c=0;c<reduce;c++)
        for (d=0;d<reduce;d++)
        {
          int x=(a*reduce+c);
          int y=(b*reduce+d);
          summer+=(int)bild_pixels[x*4  +y*bild->w*4];
          summeg+=(int)bild_pixels[x*4+1+y*bild->w*4];
          summeb+=(int)bild_pixels[x*4+2+y*bild->w*4];
          summea+=(int)bild_pixels[x*4+3+y*bild->w*4];
        }
      summer/=reduce*reduce;
      summeg/=reduce*reduce;
      summeb/=reduce*reduce;
      summea/=reduce*reduce;
      scale_bild[a*4  +b*(bild->w/reduce)*4]=summer;
      scale_bild[a*4+1+b*(bild->w/reduce)*4]=summeg;
      scale_bild[a*4+2+b*(bild->w/reduce)*4]=summeb;
      scale_bild[a*4+3+b*(bild->w/reduce)*4]=summea;
    }
  GLuint texnr;
  glGenTextures(1, &texnr);
  //glBindTexture(GL_TEXTURE_2D, texnr);
  ZWusetexture(texnr);
  if (better==1)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  }
  else
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);    
  //gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bild->w/reduce, bild->h/reduce, GL_RGBA, GL_UNSIGNED_BYTE, scale_bild);
  //glTexImage2D(GL_TEXTURE_2D, 0, 4, bild->w, bild->h,0, GL_RGBA, GL_UNSIGNED_BYTE, bild->pixels);    
  glTexImage2D(GL_TEXTURE_2D, 0, 4, bild->w/reduce, bild->h/reduce,0, GL_RGBA, GL_UNSIGNED_BYTE, scale_bild);
  if (texnr<1024)
  {
    ZWtexture_width___[texnr]=bild->w/reduce;
    ZWtexture_height___[texnr]=bild->h/reduce;
  }
  free(scale_bild);
  SDL_FreeSurface(bild);
  
  pZWtexturename newname=(pZWtexturename)malloc(sizeof(tZWtexturename));
  sprintf(newname->filename,"%s",name);
  newname->texture=texnr;
  newname->next=___firsttexture;
  newname->alpha=1;
  newname->better=better;  
	newname->refcount=1;
  ___firsttexture=newname;    
  
  return texnr;
}


EXPORT void ZWusetexture(GLuint texnr)
{
if (texnr==ZWmomtexnr) return;
ZWmomtexnr=texnr;
glBindTexture(GL_TEXTURE_2D, texnr);
}

EXPORT void ZWdeletetexture(GLuint texnr)
{
  //Nr suchen:
  pZWtexturename now=___firsttexture;
  pZWtexturename before=NULL;
  while (now!=NULL)
  {
    if (now->texture==texnr)
      break;
    before=now;
    now=now->next;
  }
  if (now!=NULL)
  {
		now->refcount--;
		if (now->refcount<=0)
		{
			if (before==NULL)
				___firsttexture=now->next;
			else
				before->next=now->next;
			free(now);  
      glDeleteTextures(1, &texnr);
		}
  }
	else
		glDeleteTextures(1, &texnr);
}

EXPORT void ZWdrawsprite(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr)
{
  float u1,v1,u2,v2;
  if (texnr<1024)
  {
    u1=((float)(nr%anz))/(float)anz+1.0/(float)(ZWtexture_width___[texnr]);
    v1=((float)(nr/anz))/(float)anz+1.0/(float)(ZWtexture_height___[texnr]);
    u2=((float)(nr%anz+1))/(float)anz-1.0/(float)(ZWtexture_width___[texnr]);
    v2=((float)(nr/anz+1))/(float)anz-1.0/(float)(ZWtexture_height___[texnr]);
  }
  else
  {
    u1=((float)(nr%anz))/(float)anz;
    v1=((float)(nr/anz))/(float)anz;
    u2=((float)(nr%anz+1)-ZWtexturepercent___)/(float)anz;
    v2=((float)(nr/anz+1)-ZWtexturepercent___)/(float)anz; 
  }
  ZWusetexture(texnr);
  glBegin(GL_QUADS);
    glTexCoord2f(u1,v1);
    glVertex3f(x-w,y+h,z);
    glTexCoord2f(u1,v2);
    glVertex3f(x-w,y-h,z);
    glTexCoord2f(u2,v2);
    glVertex3f(x+w,y-h,z);
    glTexCoord2f(u2,v1);
    glVertex3f(x+w,y+h,z);
  glEnd();  
/*  ZWusetexture(texnr);
  glBegin(GL_QUADS);
    glTexCoord2f(((float)(nr%anz)+ZWtexturepercent___)/(float)anz,((float)(nr/anz)+ZWtexturepercent___)/(float)anz);
    glVertex3f(x-w,y+h,z);
    glTexCoord2f(((float)(nr%anz)+ZWtexturepercent___)/(float)anz,((float)(nr/anz+1)-ZWtexturepercent___)/(float)anz);
    glVertex3f(x-w,y-h,z);
    glTexCoord2f(((float)(nr%anz+1)-ZWtexturepercent___)/(float)anz,((float)(nr/anz+1)-ZWtexturepercent___)/(float)anz);
    glVertex3f(x+w,y-h,z);
    glTexCoord2f(((float)(nr%anz+1)-ZWtexturepercent___)/(float)anz,((float)(nr/anz)+ZWtexturepercent___)/(float)anz);
    glVertex3f(x+w,y+h,z);
  glEnd();*/
}

EXPORT void ZWreloadalltextures(int reduce)
{
	if (reduce<1)
	  reduce=ZWtexture_stdreduce___; 
  pZWtexturename now=___firsttexture;
  while (now!=NULL)
  {
    SDL_Surface* bild= IMG_Load(now->filename);

    //Skaliertes Bild erzeugen...
    int faktor=3;
    if (now->alpha==1) 
      faktor=4;
    GLubyte *scale_bild=(GLubyte*)malloc(sizeof(GLubyte)*(bild->w/reduce)*(bild->h/reduce)*faktor);
    GLubyte *bild_pixels=(GLubyte*)bild->pixels;
    int a,b;
    for (a=0;a<bild->w/reduce;a++)
      for (b=0;b<bild->h/reduce;b++)
      {
        int c,d;
        int summer=0,summeg=0,summeb=0,summea=0;
        for (c=0;c<reduce;c++)
          for (d=0;d<reduce;d++)
          {
            int x=(a*reduce+c);
            int y=(b*reduce+d);
            summer+=(int)bild_pixels[x*faktor  +y*bild->w*faktor];
            summeg+=(int)bild_pixels[x*faktor+1+y*bild->w*faktor];
            summeb+=(int)bild_pixels[x*faktor+2+y*bild->w*faktor];
            if (faktor>3)
              summea+=(int)bild_pixels[x*faktor+3+y*bild->w*faktor];
          }
        summer/=reduce*reduce;
        summeg/=reduce*reduce;
        summeb/=reduce*reduce;
        if (faktor>3)
          summea/=reduce*reduce;
        scale_bild[a*faktor  +b*(bild->w/reduce)*faktor]=summer;
        scale_bild[a*faktor+1+b*(bild->w/reduce)*faktor]=summeg;
        scale_bild[a*faktor+2+b*(bild->w/reduce)*faktor]=summeb;
        if (faktor>3)
          scale_bild[a*faktor+3+b*(bild->w/reduce)*faktor]=summea;
      }
    
    //glDeleteTextures(1, &(now->texture));
    //glGenTextures(1, &(now->texture));
    ZWusetexture(now->texture);
    if (now->better==1)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    if (now->alpha==1)
      gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bild->w/reduce, bild->h/reduce, GL_RGBA, GL_UNSIGNED_BYTE, scale_bild);
    else
      gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bild->w/reduce, bild->h/reduce, GL_RGB, GL_UNSIGNED_BYTE, scale_bild);      
    SDL_FreeSurface(bild);
		if (now->texture<1024)
		{
			ZWtexture_width___[now->texture]=bild->w/reduce;
			ZWtexture_height___[now->texture]=bild->h/reduce;
		}
    free(scale_bild);
    now=now->next;
  }
}


EXPORT void ZWdrawspritevmirror(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr)
{
  ZWusetexture(texnr);
  glBegin(GL_QUADS);
    glTexCoord2f((float)(nr%anz+1)/(float)anz,(float)(nr/anz)/(float)anz);
    glVertex3f(x-w,y+h,z);
    glTexCoord2f((float)(nr%anz+1)/(float)anz,(float)(nr/anz+1)/(float)anz);
    glVertex3f(x-w,y-h,z);
    glTexCoord2f((float)(nr%anz)/(float)anz,(float)(nr/anz+1)/(float)anz);
    glVertex3f(x+w,y-h,z);
    glTexCoord2f((float)(nr%anz)/(float)anz,(float)(nr/anz)/(float)anz);
    glVertex3f(x+w,y+h,z);
  glEnd();  
}

EXPORT void ZWdrawspritehmirror(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr)
{
  ZWusetexture(texnr);
  glBegin(GL_QUADS);
    glTexCoord2f((float)(nr%anz)/(float)anz,(float)(nr/anz)/(float)anz);
    glVertex3f(x-w,y+h,z);
    glTexCoord2f((float)(nr%anz)/(float)anz,(float)(nr/anz+1)/(float)anz);
    glVertex3f(x-w,y-h,z);
    glTexCoord2f((float)(nr%anz+1)/(float)anz,(float)(nr/anz+1)/(float)anz);
    glVertex3f(x+w,y-h,z);
    glTexCoord2f((float)(nr%anz+1)/(float)anz,(float)(nr/anz)/(float)anz);
    glVertex3f(x+w,y+h,z);
  glEnd();  
}

EXPORT void ZWdrawspritevhmirror(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr)
{
  ZWusetexture(texnr);
  glBegin(GL_QUADS);
    glTexCoord2f((float)(nr%anz)/(float)anz,(float)(nr/anz)/(float)anz);
    glVertex3f(x-w,y+h,z);
    glTexCoord2f((float)(nr%anz)/(float)anz,(float)(nr/anz+1)/(float)anz);
    glVertex3f(x-w,y-h,z);
    glTexCoord2f((float)(nr%anz+1)/(float)anz,(float)(nr/anz+1)/(float)anz);
    glVertex3f(x+w,y-h,z);
    glTexCoord2f((float)(nr%anz+1)/(float)anz,(float)(nr/anz)/(float)anz);
    glVertex3f(x+w,y+h,z);
  glEnd();  
}

EXPORT void ZWdrawspritealpha(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr,float alpha)
{
  ZWusetexture(texnr);
  glColor4f(1,1,1,alpha);
  glBegin(GL_QUADS);
    glTexCoord2f((float)(nr%anz)/(float)anz,(float)(nr/anz)/(float)anz);
    glVertex3f(x-w,y+h,z);
    glTexCoord2f((float)(nr%anz)/(float)anz,(float)(nr/anz+1)/(float)anz);
    glVertex3f(x-w,y-h,z);
    glTexCoord2f((float)(nr%anz+1)/(float)anz,(float)(nr/anz+1)/(float)anz);
    glVertex3f(x+w,y-h,z);
    glTexCoord2f((float)(nr%anz+1)/(float)anz,(float)(nr/anz)/(float)anz);
    glVertex3f(x+w,y+h,z);
  glEnd();  
  glColor4f(1,1,1,1);
}

EXPORT GLuint ZWgetmomtexture(void)
{
  return ZWmomtexnr;
}

EXPORT void ZWsetmomtexture(GLuint momtexnr)
{
  ZWmomtexnr=momtexnr;
}
