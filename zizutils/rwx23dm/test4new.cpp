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
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "./../texture/texture.h"
#include "./../3dobjects/3dobjects.h"

object3d testobject;

float betrag(float zahl)
{
  if (zahl<0.0) return -zahl;
  return zahl;
}

int main( int argc, char* argv[] )
{
  int arg=1;
  if (argc<2)
  printf("Nicht genug Parameter: test4new input.rwx\n");
  else
  while (arg<argc)
  {
    printf("Teste: %s",argv[arg]);
    for (int a=0;a<20-strlen(argv[arg]);a++)
      printf(" ");
    testobject=ZWnewobject();
    ZWloadfromrwx(&testobject,argv[arg]);
    float maxwert=0;
    for (int a=0;a<testobject.pointnum;a++)
    {
      if (betrag(testobject.p[a].x)>maxwert)
        maxwert=testobject.p[a].x;
      if (betrag(testobject.p[a].y)>maxwert)
        maxwert=testobject.p[a].y;
      if (betrag(testobject.p[a].z)>maxwert)
        maxwert=testobject.p[a].z;
    }
    printf(" alt: %f",maxwert);
    float oldwert=maxwert;
    maxwert=0;
    for (int a=0;a<testobject.pointnum;a++)
    {
      if (betrag(testobject.p[a].x)>maxwert)
        maxwert=betrag(testobject.p[a].x);
      if (betrag(testobject.p[a].y)>maxwert)
        maxwert=betrag(testobject.p[a].y);
      if (betrag(testobject.p[a].z)>maxwert)
        maxwert=betrag(testobject.p[a].z);
    }
    if (maxwert==oldwert)
      printf(" neu: %f\n",maxwert);
    else
      printf(" neu: %f ERR\n",maxwert);
    arg++;
  }
  return 0;
}
