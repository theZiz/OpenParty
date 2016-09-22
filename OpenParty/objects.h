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
float getobjectalpha(tZWpoint ballpos, tZWpoint campos, tZWpoint objpos,float f,float min)
{
  tZWpoint m=ballpos; //OB
  tZWpoint n; //BK/|BK|
  n.x=campos.x-ballpos.x;
  n.y=campos.y-ballpos.y;
  n.z=campos.z-ballpos.z;
  float betrag=sqrt(n.x*n.x+n.y*n.y+n.z*n.z);
  if (betrag!=0)
  {
    n.x/=betrag;
    n.y/=betrag;
    n.z/=betrag;
  }
  else
    n.z=1;
    
  float p=objpos.x*n.x+objpos.y*n.y+objpos.z*n.z; //OG*n
  
  float r=(p-n.x*m.x-n.y*m.y-n.z*m.z)/(n.x*n.x+n.y*n.y+n.z*n.z); //p*((n.x*m.x+n.y*m.y+n.z*m.z)/(n.x²+n.y²+n.z²))
  if (r<=0) return 1.0;
  
  tZWpoint sp;
  sp.x=m.x+r*n.x;
  sp.y=m.y+r*n.y;
  sp.z=m.z+r*n.z;
  float a=sqrt((sp.x-objpos.x)*(sp.x-objpos.x)+(sp.y-objpos.y)*(sp.y-objpos.y)+(sp.z-objpos.z)*(sp.z-objpos.z));
  a+=min;
  float v=a/r;
  v/=f;
  if (v>=1.0) return 1.0;
  if (v>=0.5) return (v-0.5)*10.0/5.0;
  return 0.0;
}

/*float newgetobjectalpha(tZWpoint campos,float *vector, tZWpoint objpos,float f,float min)
{
  tZWpoint m;
	m.x=campos.x+vector[0]*10.0;
	m.y=campos.y+vector[1]*10.0;
	m.z=campos.z+vector[2]*10.0;
  tZWpoint n; //BK/|BK|
  n.x=campos.x-m.x;
  n.y=campos.y-m.y;
  n.z=campos.z-m.z;
  float betrag=sqrt(n.x*n.x+n.y*n.y+n.z*n.z);
  if (betrag!=0)
  {
    n.x/=betrag;
    n.y/=betrag;
    n.z/=betrag;
  }
  else
    n.z=1;
    
  float p=objpos.x*n.x+objpos.y*n.y+objpos.z*n.z; //OG*n
  
  float r=(p-n.x*m.x-n.y*m.y-n.z*m.z)/(n.x*n.x+n.y*n.y+n.z*n.z); //p*((n.x*m.x+n.y*m.y+n.z*m.z)/(n.x²+n.y²+n.z²))
  if (r<=0) return 1.0;
  
  tZWpoint sp;
  sp.x=m.x+r*n.x;
  sp.y=m.y+r*n.y;
  sp.z=m.z+r*n.z;
  float a=sqrt((sp.x-objpos.x)*(sp.x-objpos.x)+(sp.y-objpos.y)*(sp.y-objpos.y)+(sp.z-objpos.z)*(sp.z-objpos.z));
  a+=min;
  float v=a/r;
  v/=f;
  if (v>=1.0) return 1.0;
  if (v>=0.5) return (v-0.5)*10.0/5.0;
  return 0.0;
}*/

void newobject(char* filename,char* name,char* texture,bool simple)
{
  pobject object=(pobject)malloc(sizeof(tobject));
  GLuint tex=ZWloadtexturefromfileA(texture,1,texquali);
  ZWload3dm(&(object->realobject),filename,tex);
  if (simple)
    ZWcreatedrawlist(&(object->realobject),0);
  else
    ZWcreatedrawlist(&(object->realobject),1);

  sprintf(object->name,"%s",name);
  sprintf(object->filename,"%s",&(filename[9]));
  sprintf(object->texture,"%s",&(texture[11]));

  object->next=firstobject;
  firstobject=object;
  //maxy bestimmen
  object->maxy=0;
  int a;
  for (a=0;a<object->realobject.pointnum;a++)
    if (object->realobject.p[a].y>object->maxy) object->maxy=object->realobject.p[a].y;
}

void deleteallobjects(void)
{
  pobject now=firstobject;
  while (now!=NULL)
  {
    pobject temp=now;
    now=now->next;
    ZWdeletedrawlist(temp->realobject);
    ZWdeleteobject(temp->realobject);
    free(temp);
  }
  firstobject=NULL;
  if (sky.texture[0]!=0)
  {
    ZWdeletedrawlist(sky.sky);
    ZWdeleteobject(sky.sky);
  }
  sky.texture[0] = 0;
}

