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
/* Diese Datei bietet:
	* einen Datentyp für einfach 3d-Modelle
	* Funktionen zum Laden von verschiedenen Dateien (rwx, 3dm)
	* Funktionen zum Speichern ins 3dm-Format
	* Funktionen zum Zeichnen von einem 3d-Modell */

#ifndef _3DOBJECTS_H

#ifdef WIN32
  #include <windows.h>
  #if BUILDING_DLL
    # define DLLIMPORT __declspec (dllexport)
  #else /* Not BUILDING_DLL */
    # define DLLIMPORT __declspec (dllimport)
  #endif /* Not BUILDING_DLL */
#endif //win32

#ifdef WIN32
  #ifdef __cplusplus
    #define PRAEFIX extern "C" DLLIMPORT
  #else
    #define PRAEFIX DLLIMPORT
  #endif
#else
  #ifdef __cplusplus
    #define PRAEFIX extern "C"
  #else
    #define PRAEFIX  
  #endif
#endif

#include <GL/gl.h>

typedef struct spoint
{
float x;
float y;
float z;
char uv; //boolean, ob dieser Punkt UV-Koordinaten besitzt.
float u;
float v;
}point;

typedef struct stex
{
float x;
float y;
} tex;

typedef struct smultigon *pmultigon;
typedef struct smultigon
{
unsigned short panz;
unsigned short *p;
} multigon;

typedef struct sobject3d
{
    GLuint text; //optional: Gibt eine Texture an, die gezeichnet wird, wenn "texture=true" gilt.
    unsigned short pointnum;
    point *p;
    unsigned short polygonnum,polygonnumohne,polygonnummit;
    pmultigon polohne,polmit; //Es gibt nur EIN Array, aber 2 "Einsprungspunkte". So kann man alle oder aber nur einen Teil betrachten
    //unsigned short listnr;
    GLuint listnr;
}object3d;

//Object3d Version 2: Jetzt neu durchdacht und mit beliebigen Polygonen (>=3)
PRAEFIX object3d ZWnewobject(void);
PRAEFIX void ZWdeleteobject(object3d object);
PRAEFIX void ZWdeletedrawlist(object3d object);
PRAEFIX void ZWloadfromrwx(object3d *object,char* name); //Läd ein Object aus einer rwx-Datei
PRAEFIX void ZWcreatedrawlist(object3d *object,char betterlight); //erzeugt eine Drawlist -> Nötig fürs Zeichnen WICHTIG!
PRAEFIX void ZWdrawobject(object3d object); //zeichnet des Object
PRAEFIX void ZWsave3dm(object3d object,char* name);
PRAEFIX void ZWload3dm(object3d *object,char* name,GLuint text);
PRAEFIX void ZWrefreshdrawlist(object3d *object,char betterlight);
PRAEFIX void ZWnormaleberechnen(point p1,point p2,point p3,GLfloat* normale);

#endif
#define _3DOBJECTS_H
