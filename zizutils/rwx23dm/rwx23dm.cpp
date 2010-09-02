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
    if (argc<3)
    printf("Nicht genug Parameter: rwx23dm input.rwx output.3dm [ni]\nn - Normalisieren\ni - Punktreihenfolger invertieren\n");
    else
    {
    printf("Erstelle Objekt\n");
    testobject=ZWnewobject();
    printf("Lade %s\n",argv[1]);
    ZWloadfromrwx(&testobject,argv[1]);
    if (argc>3)
    {
      if (argv[3][0]=='n' || argv[3][1]=='n')
      {
        printf("Normalisiere...\n  Suche größten Wert... ");
        float maxwert=0;
        for (int a=0;a<testobject.pointnum;a++)
        {
          if (betrag(testobject.p[a].x)>maxwert)
            maxwert=betrag(testobject.p[a].x);
          if (betrag(testobject.p[a].y)>maxwert)
            maxwert=betrag(testobject.p[a].y);
          if (betrag(testobject.p[a].z)>maxwert)
            maxwert=betrag(testobject.p[a].z);
        }
        printf("%f\n",maxwert);
        printf("  Normalisiere\n");
        float faktor=1.0f/maxwert;
        for (int a=0;a<testobject.pointnum;a++)
        {
          testobject.p[a].x*=faktor;
          testobject.p[a].y*=faktor;
          testobject.p[a].z*=faktor;
        }
      }
      if (argv[3][0]=='i' || argv[3][1]=='i')
      {
        printf("Invertiere...\n");
        for (int a=0;a<testobject.polygonnum;a++)
        {
          for (int b=0;b<testobject.polohne[a].panz/2;b++)
          {
            int temp=testobject.polohne[a].p[b];
            testobject.polohne[a].p[b]=testobject.polohne[a].p[testobject.polohne[a].panz-b-1];
            testobject.polohne[a].p[testobject.polohne[a].panz-b-1]=temp;
          }
        }
      }
    }
    printf("Speichere %s\n",argv[2]);
    ZWsave3dm(testobject,argv[2]);
    }
    printf("rwx23dm - Version 1.1\nKonvertiert mit Wings3d erstellte RWX-Dateien in 3dm-(Version 01) Dateien\n");
  return 0;
}
