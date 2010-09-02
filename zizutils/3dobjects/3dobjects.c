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

#include "3dobjects.h"

#ifdef WIN32
  #define EXPORT DLLIMPORT
#else
  #define EXPORT
#endif

#include "./../texture/texture.h"

EXPORT void ZWnormaleberechnen(point p1,point p2,point p3,GLfloat* normale)
{
point v1,v2;
v1.x=p1.x-p2.x;
v1.y=p1.y-p2.y;
v1.z=p1.z-p2.z;
v2.x=p2.x-p3.x;
v2.y=p2.y-p3.y;
v2.z=p2.z-p3.z;
normale[0]=v1.y*v2.z-v1.z*v2.y;
normale[1]=v1.z*v2.x-v1.x*v2.z;
normale[2]=v1.x*v2.y-v1.y*v2.x;
}

EXPORT object3d ZWnewobject()              // der Konstruktor
{
  object3d object;
  object.pointnum=0;
  object.polygonnumohne=0;
  object.polygonnummit=0;
  object.p=NULL;
  object.polohne=NULL;
  object.polmit=NULL;
  object.text=0;
  object.listnr=0;
  object.polygonnum=0;
  return object;
}

EXPORT void ZWdeleteobject(object3d object)
{
  if (object.p!=NULL) free(object.p);
  if (object.polohne!=NULL) 
  {
  int a;
  for (a=0;a<object.polygonnum;a++)
    free(object.polohne[a].p);
  free(object.polohne);
  }
}

EXPORT void ZWdeletedrawlist(object3d object)
{
  glDeleteLists(object.listnr,1);
}

int ZWgetoneline(SDL_RWops *file,char *buffer)
{
 int a=0;
 char c=' ';
 while (c!=10)
 {
  if (SDL_RWread(file,&c,1,1)!=1) return 0;
  buffer[a]=c;
  a++;
 }
 buffer[a]=0;
 return 1;
}

char ZWgetkoords(char *buffer,float *x,float *y,float *z,float *u,float *v)
{
 (*x)=0;
 float vorzeichen=1; 
 //erstes Leerzeichen nach "Vertex" finden:
 char* position=strstr(buffer,"Vertex");
 position=strstr(position," ");
 // - finden, verarbeiten und überspringen
 if (position[1]=='-')
 {
  vorzeichen=-1;
  position=&position[1];
 } 
 //zur Zahl springen:
 position=&position[1];
 //Schleife für Vorkommazahlen bis zum Komma (.)
 while (position[0]!='.')
 {
  (*x)=(*x)*10+(position[0]-48);
  position=&position[1];
 }
 //zur ersten Nachkommastelle springen:
 position=&position[1];
 //Schleife für Nachkommazahlen bis zum Leerzeichen
 float nachkomma=0;
 float teiler=1;
 while (position[0]!=' ')
 {
  nachkomma=nachkomma*10+(position[0]-48);
  teiler=teiler*10;
  position=&position[1];
 }
 (*x)=vorzeichen*((*x)+nachkomma/teiler);

 //zweite Zahl:
 (*y)=0;
 // - finden, verarbeiten und überspringen
 vorzeichen=1;
 if (position[1]=='-') 
 {
  vorzeichen=-1;
  position=&position[1];
 } 
 //zur Zahl springen:
 position=&position[1];
 //Schleife für Vorkommazahlen bis zum Komma (.)
 while (position[0]!='.')
 {
  (*y)=(*y)*10+(position[0]-48);
  position=&position[1];
 }
 //zur ersten Nachkommastelle springen:
 position=&position[1];
 //Schleife für Nachkommazahlen bis zum Leerzeichen
 nachkomma=0;
 teiler=1;
 while (position[0]!=' ')
 {
  nachkomma=nachkomma*10+(position[0]-48);
  teiler=teiler*10;
  position=&position[1];
 }
 (*y)=vorzeichen*((*y)+nachkomma/teiler);
 
 //dritte Zahl:
 (*z)=0;
 // - finden, verarbeiten und überspringen
 vorzeichen=1;
 if (position[1]=='-') 
 {
  vorzeichen=-1;
  position=&position[1];
 } 
 //zur Zahl springen:
 position=&position[1];
 //Schleife für Vorkommazahlen bis zum Komma (.)
 while (position[0]!='.')
 {
  (*z)=(*z)*10+(position[0]-48);
  position=&position[1];
 }
 //zur ersten Nachkommastelle springen:
 position=&position[1];
 //Schleife für Nachkommazahlen bis zum Leerzeichen
 nachkomma=0;
 teiler=1;
 while (position[0]!=' ' && position[0]!=13 && position[0]!=10)
 {
  nachkomma=nachkomma*10+(position[0]-48);
  teiler=teiler*10;
  position=&position[1];
 }
 (*z)=vorzeichen*((*z)+nachkomma/teiler);
 
 if (position[1]=='U' && position[2]=='V')
 {
 //zur Zahl springen:
 position=&position[4];
 // - finden, verarbeiten und überspringen
 vorzeichen=1;
 if (position[0]=='-') 
 {
  vorzeichen=-1;
  position=&position[1];
 } 
 
 //Schleife für Vorkommazahlen bis zum Komma (.)
 (*u)=0;
 while (position[0]!='.')
 {
  (*u)=(*u)*10+(position[0]-48);
  position=&position[1];
 }
 //zur ersten Nachkommastelle springen:
 position=&position[1];
 //Schleife für Nachkommazahlen bis zum Leerzeichen
 nachkomma=0;
 teiler=1;
 while (position[0]!=' ' && position[0]!=13 && position[0]!=10)
 {
  nachkomma=nachkomma*10+(position[0]-48);
  teiler=teiler*10;
  position=&position[1];
 }
 (*u)=vorzeichen*((*u)+nachkomma/teiler);

 //zur Zahl springen:
 position=&position[1];
 // - finden, verarbeiten und überspringen
 vorzeichen=1;
 if (position[0]=='-') 
 {
  vorzeichen=-1;
  position=&position[1];
 } 
  
 //Schleife für Vorkommazahlen bis zum Komma (.)
 (*v)=0;
 while (position[0]!='.')
 {
  (*v)=(*v)*10+(position[0]-48);
  position=&position[1];
 }
 //zur ersten Nachkommastelle springen:
 position=&position[1];
 //Schleife für Nachkommazahlen bis zum Leerzeichen
 nachkomma=0;
 teiler=1;
 while (position[0]!=' ' && position[0]!=13 && position[0]!=10)
 {
  nachkomma=nachkomma*10+(position[0]-48);
  teiler=teiler*10;
  position=&position[1];
 }
 (*v)=vorzeichen*((*v)+nachkomma/teiler);
 return 1;
 }
 else return 0;
}

multigon ZWgetmultigon(char *buffer,int dazu)
{
 multigon m;
 m.p=NULL; 
 int position=0;

if (strstr(buffer,"Triangle")==buffer)
{
m.panz=3;
m.p=(unsigned short*)malloc(sizeof(unsigned short)*m.panz);
position=9;
}

if (strstr(buffer,"Quad")==buffer)
{
m.panz=4;
m.p=(unsigned short*)malloc(sizeof(unsigned short)*m.panz);
position=5;
}

if (strstr(buffer,"Polygon")==buffer)
{
position=8;
m.panz=0;
while (buffer[position]!=' ')
{
m.panz=m.panz*10+buffer[position]-48;
position++;
}

m.p=(unsigned short*)malloc(sizeof(unsigned short)*m.panz);
position+=2;
}

int a;

for (a=0;a<m.panz;a++)
{
m.p[a]=0;
while (buffer[position]!=' ' && buffer[position]!=10 && buffer[position]!=13)
{
m.p[a]=m.p[a]*10+buffer[position]-48;
position++;
}
m.p[a]-=1;
m.p[a]+=dazu;
position++;
}

return m;
}


EXPORT void ZWloadfromrwx(object3d *object,char* name) //Läd ein Object aus einer rwx-Datei
{
  char buffer[256];
  SDL_RWops *file = SDL_RWFromFile(name, "rb");
  //Anzahl an Vertex ermitteln
  object->pointnum=0;
  while (ZWgetoneline(file,(char*)&buffer)==1)
  {
   if (strstr((char*)&buffer,"Vertex")==(char*)&buffer)
   object->pointnum++;
  } 
  //point allokieren und auslesen:
  object->p=(point*)malloc(sizeof(point) * object->pointnum);
  SDL_RWseek(file, 0, SEEK_SET);
  
  //Punkte auslesen
  int a=0;
  while (ZWgetoneline(file,(char*)&buffer)==1)
  {
   if (strstr((char*)&buffer,"Vertex")==(char*)&buffer)
   {
    float x;
    float y;
    float z;
    float u;
    float v;
    if (ZWgetkoords((char*)&buffer,&x,&y,&z,&u,&v)==1)
    {
    object->p[a].uv=1;
    object->p[a].u=u;
    object->p[a].v=v;
    }
    else object->p[a].uv=0;
    object->p[a].x=x;
    object->p[a].y=y;
    object->p[a].z=z;
    a++;
   } 
  }  
 //Multigonanzahl ermitteln:
  SDL_RWseek(file, 0, SEEK_SET);
  unsigned short polygonnum=0;
  while (ZWgetoneline(file,(char*)&buffer)==1)
  {
   if (strstr((char*)&buffer,"Triangle")==(char*)&buffer ||
       strstr((char*)&buffer,"Quad")==(char*)&buffer ||
       strstr((char*)&buffer,"Polygon")==(char*)&buffer)
   polygonnum++;
  }
  //Multigon allokieren und auslesen:

  pmultigon pol=(pmultigon)malloc(sizeof(multigon) * polygonnum);
  SDL_RWseek(file, 0, SEEK_SET);
  a=0;
  int dazu=0;
  int countvertex=0;
  while (ZWgetoneline(file,(char*)&buffer)==1)
  {
   if (strstr((char*)&buffer,"Vertex")==(char*)&buffer)
     countvertex++;
   if (strstr((char*)&buffer,"clumpbegin")==(char*)&buffer)
     dazu=countvertex;

   if (strstr((char*)&buffer,"Triangle")==(char*)&buffer ||
       strstr((char*)&buffer,"Quad")==(char*)&buffer ||
       strstr((char*)&buffer,"Polygon")==(char*)&buffer)
   {
    pol[a]=ZWgetmultigon((char*)&buffer,dazu);
    a++;
   }
  }
  
  //Multigon "von links und rechts" einordnen:
  
  object->polohne=(pmultigon)malloc(sizeof(multigon) * polygonnum);
  object->polygonnumohne=0;
  object->polygonnummit=0;
  for (a=0;a<polygonnum;a++)
  {
    if (object->p[pol[a].p[0]].uv==0)
    {
      object->polohne[object->polygonnumohne]=pol[a];
      object->polygonnumohne++;
    }
    else
    {
      object->polygonnummit++;    
      object->polohne[polygonnum-object->polygonnummit]=pol[a];
    } 
  }
  free(pol);
  object->polmit=&(object->polohne[object->polygonnumohne]);
  
  //Multigon ordnen nach Eckenanzahl
  int b;
  if (object->polygonnumohne>1)
  for (a=0;a<object->polygonnumohne;a++)
    for (b=1;b<object->polygonnumohne-a;b++)
    if (object->polohne[b-1].panz>object->polohne[b].panz)
    {
      multigon temp=object->polohne[b-1];
      object->polohne[b-1]=object->polohne[b];
      object->polohne[b]=temp;
    }        

  if (object->polygonnummit>1)
  for (a=0;a<object->polygonnummit;a++)
    for (b=1;b<object->polygonnummit-a;b++)
    if (object->polmit[b-1].panz>object->polmit[b].panz)
    {
      multigon temp=object->polmit[b-1];
      object->polmit[b-1]=object->polmit[b];
      object->polmit[b]=temp;
    }        

 
 object->polygonnum=polygonnum; 
 
 
}

EXPORT void ZWrefreshdrawlist(object3d *object,char betterlight)
{
GLint oldtexture;
glGetIntegerv(GL_TEXTURE_BINDING_2D,&oldtexture);
glNewList(object->listnr,GL_COMPILE);
GLfloat allenormalen[object->polygonnum][3];
GLfloat allepunktnormalen[object->pointnum][3];

int a;
for (a=0;a<object->polygonnum;a++) //von allen Flächen die Normalen berechnen
  ZWnormaleberechnen(object->p[object->polohne[a].p[0]],object->p[object->polohne[a].p[1]],object->p[object->polohne[a].p[2]],&(allenormalen[a][0]));

if (betterlight==1)
{
  for (a=0;a<object->pointnum;a++) //von allen Punkten die Normalen berechnen
  {
    //absuchen, ob ein Polygon den Punkt enthällt und dann die Normale draufaddieren und Counter erhöhen
    int b;
    int counter=0;
    allepunktnormalen[a][0]=0;
    allepunktnormalen[a][1]=0;
    allepunktnormalen[a][2]=0;        
    //Alle Polygone durchgehen
    for (b=0;b<object->polygonnum;b++)
    {
      //Alle Punkte des Polygons durchgehen
      int c;
      for (c=0;c<object->polohne[b].panz;c++)
        if (object->p[object->polohne[b].p[c]].x==object->p[a].x &&
            object->p[object->polohne[b].p[c]].y==object->p[a].y &&
            object->p[object->polohne[b].p[c]].z==object->p[a].z)
        {
          allepunktnormalen[a][0]+=allenormalen[b][0];
          allepunktnormalen[a][1]+=allenormalen[b][1];
          allepunktnormalen[a][2]+=allenormalen[b][2];
          counter++;
          break;
        }                   
    }
    if (counter>0)
    {
      allepunktnormalen[a][0]=allepunktnormalen[a][0]/(float)counter;
      allepunktnormalen[a][1]=allepunktnormalen[a][1]/(float)counter;
      allepunktnormalen[a][2]=allepunktnormalen[a][2]/(float)counter;
    }
  }
}

//Betrachtung aller Flächen ohne Texturen
if (object->polygonnumohne>0)
{
  glDisable(GL_TEXTURE_2D);
  int pos=0;
  if (pos<object->polygonnumohne && object->polohne[pos].panz==3)
  {
    glBegin(GL_TRIANGLES);
    while (pos<object->polygonnumohne && object->polohne[pos].panz==3)
    {
      if (betterlight==0) glNormal3fv(&(allenormalen[pos][0]));
//      glColor4ubv(object->polohne[pos].color);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[pos].p[0]][0]));
      glVertex3f(object->p[object->polohne[pos].p[0]].x,object->p[object->polohne[pos].p[0]].y,object->p[object->polohne[pos].p[0]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[pos].p[1]][0]));
      glVertex3f(object->p[object->polohne[pos].p[1]].x,object->p[object->polohne[pos].p[1]].y,object->p[object->polohne[pos].p[1]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[pos].p[2]][0]));
      glVertex3f(object->p[object->polohne[pos].p[2]].x,object->p[object->polohne[pos].p[2]].y,object->p[object->polohne[pos].p[2]].z);
      pos++;
    }
    glEnd();
  }
  if (pos<object->polygonnumohne && object->polohne[pos].panz==4)
  {
    glBegin(GL_QUADS);
    while (pos<object->polygonnumohne && object->polohne[pos].panz==4)
    {
      if (betterlight==0) glNormal3fv(&(allenormalen[pos][0]));
//      glColor4ubv(object->polohne[pos].color);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[pos].p[0]][0]));
      glVertex3f(object->p[object->polohne[pos].p[0]].x,object->p[object->polohne[pos].p[0]].y,object->p[object->polohne[pos].p[0]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[pos].p[1]][0]));
      glVertex3f(object->p[object->polohne[pos].p[1]].x,object->p[object->polohne[pos].p[1]].y,object->p[object->polohne[pos].p[1]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[pos].p[2]][0]));
      glVertex3f(object->p[object->polohne[pos].p[2]].x,object->p[object->polohne[pos].p[2]].y,object->p[object->polohne[pos].p[2]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[pos].p[3]][0]));
      glVertex3f(object->p[object->polohne[pos].p[3]].x,object->p[object->polohne[pos].p[3]].y,object->p[object->polohne[pos].p[3]].z);
      pos++;
    }
    glEnd();
  }
  if (pos<object->polygonnumohne && object->polohne[pos].panz>4)
  {
    while (pos<object->polygonnumohne && object->polohne[pos].panz>4)
    {
      glBegin(GL_TRIANGLE_FAN);
      if (betterlight==0) glNormal3fv(&(allenormalen[pos][0]));
//      glColor4ubv(object->polohne[pos].color);
      int a;
      for (a=0;a<object->polohne[pos].panz;a++)
      {
        if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[pos].p[a]][0]));
        glVertex3f(object->p[object->polohne[pos].p[a]].x,object->p[object->polohne[pos].p[a]].y,object->p[object->polohne[pos].p[a]].z);
      }
      glEnd();
      pos++;
    }
  }
  glEnable(GL_TEXTURE_2D);
}
//Betrachtung aller Flächen mit Texturen
if (object->polygonnummit>0)
{
  glBindTexture(GL_TEXTURE_2D, object->text);
  int pos=0;
  if (pos<object->polygonnummit && object->polmit[pos].panz==3)
  {
    glBegin(GL_TRIANGLES);
    while (pos<object->polygonnummit && object->polmit[pos].panz==3)
    {
      if (betterlight==0) glNormal3fv(&(allenormalen[object->polygonnumohne+pos][0]));
//      glColor4ubv(object->polmit[pos].color);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[object->polygonnumohne+pos].p[0]][0]));
      glTexCoord2f(object->p[object->polmit[pos].p[0]].u,object->p[object->polmit[pos].p[0]].v);
      glVertex3f(object->p[object->polmit[pos].p[0]].x,object->p[object->polmit[pos].p[0]].y,object->p[object->polmit[pos].p[0]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[object->polygonnumohne+pos].p[1]][0]));
      glTexCoord2f(object->p[object->polmit[pos].p[1]].u,object->p[object->polmit[pos].p[1]].v);
      glVertex3f(object->p[object->polmit[pos].p[1]].x,object->p[object->polmit[pos].p[1]].y,object->p[object->polmit[pos].p[1]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[object->polygonnumohne+pos].p[2]][0]));
      glTexCoord2f(object->p[object->polmit[pos].p[2]].u,object->p[object->polmit[pos].p[2]].v);
      glVertex3f(object->p[object->polmit[pos].p[2]].x,object->p[object->polmit[pos].p[2]].y,object->p[object->polmit[pos].p[2]].z);
      pos++;
    }
    glEnd();
  }
  if (pos<object->polygonnummit && object->polmit[pos].panz==4)
  {
    glBegin(GL_QUADS);
    while (pos<object->polygonnummit && object->polmit[pos].panz==4)
    {
      if (betterlight==0) glNormal3fv(&(allenormalen[object->polygonnumohne+pos][0]));
//      glColor4ubv(object->polmit[pos].color);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[object->polygonnumohne+pos].p[0]][0]));
      glTexCoord2f(object->p[object->polmit[pos].p[0]].u,object->p[object->polmit[pos].p[0]].v);
      glVertex3f(object->p[object->polmit[pos].p[0]].x,object->p[object->polmit[pos].p[0]].y,object->p[object->polmit[pos].p[0]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[object->polygonnumohne+pos].p[1]][0]));
      glTexCoord2f(object->p[object->polmit[pos].p[1]].u,object->p[object->polmit[pos].p[1]].v);
      glVertex3f(object->p[object->polmit[pos].p[1]].x,object->p[object->polmit[pos].p[1]].y,object->p[object->polmit[pos].p[1]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[object->polygonnumohne+pos].p[2]][0]));
      glTexCoord2f(object->p[object->polmit[pos].p[2]].u,object->p[object->polmit[pos].p[2]].v);
      glVertex3f(object->p[object->polmit[pos].p[2]].x,object->p[object->polmit[pos].p[2]].y,object->p[object->polmit[pos].p[2]].z);
      if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[object->polygonnumohne+pos].p[3]][0]));
      glTexCoord2f(object->p[object->polmit[pos].p[3]].u,object->p[object->polmit[pos].p[3]].v);
      glVertex3f(object->p[object->polmit[pos].p[3]].x,object->p[object->polmit[pos].p[3]].y,object->p[object->polmit[pos].p[3]].z);
      pos++;
    }
    glEnd();
  }
  if (pos<object->polygonnummit && object->polmit[pos].panz>4)
  {
    while (pos<object->polygonnummit && object->polmit[pos].panz>4)
    {
      glBegin(GL_TRIANGLE_FAN);
      if (betterlight==0) glNormal3fv(&(allenormalen[object->polygonnumohne+pos][0]));
//      glColor4ubv(object->polmit[pos].color);
      int a;
      for (a=0;a<object->polmit[pos].panz;a++)
      {
        if (betterlight==1) glNormal3fv(&(allepunktnormalen[object->polohne[object->polygonnumohne+pos].p[a]][0]));
        glTexCoord2f(object->p[object->polmit[pos].p[a]].u,object->p[object->polmit[pos].p[a]].v);
        glVertex3f(object->p[object->polmit[pos].p[a]].x,object->p[object->polmit[pos].p[a]].y,object->p[object->polmit[pos].p[a]].z);
      }
      glEnd();
      pos++;
    }
  }  
}
glEndList();
glBindTexture(GL_TEXTURE_2D, oldtexture);
}

EXPORT void ZWcreatedrawlist(object3d *object,char betterlight)
{
  object->listnr=glGenLists(1);
  ZWrefreshdrawlist(object,betterlight);
}

EXPORT void ZWdrawobject(object3d object)
{
  ZWusetexture(object.text);
  glCallList(object.listnr);
}

EXPORT void ZWsave3dm(object3d object,char* name)
{
  if (object.p==NULL) return;
  if (object.polohne==NULL) return;
  SDL_RWops *file = SDL_RWFromFile(name, "wb");
  char index[]="ZIZ3DM01";
  SDL_RWwrite(file,&index,8,1);
  SDL_RWwrite(file,&(object.pointnum),sizeof(object.pointnum),1);
  SDL_RWwrite(file,object.p,sizeof(point)*object.pointnum,1);
  SDL_RWwrite(file,&(object.polygonnumohne),sizeof(object.polygonnumohne),1);
  SDL_RWwrite(file,&(object.polygonnummit),sizeof(object.polygonnummit),1);
  int a;
  for (a=0;a<object.polygonnum;a++)
  {
    SDL_RWwrite(file,&(object.polohne[a].panz),sizeof(object.polohne[a].panz),1);  
    //SDL_RWwrite(file,object.polohne[a].color,sizeof(GLubyte)*4,1);  
    SDL_RWwrite(file,object.polohne[a].p,sizeof(unsigned short)*object.polohne[a].panz,1);          
  }

  SDL_RWclose(file);
}

EXPORT void ZWload3dm(object3d *object,char* name,GLuint text)
{
  SDL_RWops *file = SDL_RWFromFile(name, "rb");
  if (file==NULL) {printf("fail loading %s\n",name);return;}
  char index[9]="ZIZ3DM01";
  SDL_RWread(file,&index,8,1);
  SDL_RWread(file,&(object->pointnum),sizeof(object->pointnum),1);
  object->p=(point*)malloc(sizeof(point)*object->pointnum);
  SDL_RWread(file,object->p,sizeof(point)*object->pointnum,1);
  SDL_RWread(file,&(object->polygonnumohne),sizeof(object->polygonnumohne),1);
  SDL_RWread(file,&(object->polygonnummit),sizeof(object->polygonnummit),1);
  object->polygonnum=object->polygonnumohne+object->polygonnummit;
  object->polohne=(pmultigon)malloc(sizeof(multigon)*object->polygonnum);
  int a;
  for (a=0;a<object->polygonnum;a++)
  {
    SDL_RWread(file,&(object->polohne[a].panz),sizeof(object->polohne[a].panz),1);  
    //SDL_RWread(file,object->polohne[a].color,sizeof(GLubyte)*4,1);  
    object->polohne[a].p=(unsigned short*)malloc(sizeof(unsigned short)*object->polohne[a].panz);
    SDL_RWread(file,object->polohne[a].p,sizeof(unsigned short)*object->polohne[a].panz,1);          
  }

  object->polmit=&(object->polohne[object->polygonnumohne]);
  object->text=text;
  SDL_RWclose(file);
}
