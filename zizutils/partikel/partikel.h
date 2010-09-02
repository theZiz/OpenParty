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
#ifndef PARTIKEL_H

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
#include "./../zizwindow/zizwindow.h"
#include "./../3dobjects/3dobjects.h"

typedef union uZWpartikel *pZWpartikel;

typedef union uZWfloatint *pZWfloatint;
typedef union uZWfloatint {
  int i;
  float f;
} tZWfloatint;

typedef struct sZWpoint *pZWpoint;
typedef struct sZWpoint {
  float x,y,z;
} tZWpoint;

typedef struct sZWsprite *pZWsprite;
typedef struct sZWsprite {
  //common
  GLbyte type;
  tZWpoint position;
  GLbyte rotate;
  tZWpoint rotation;
  tZWpoint scale;
  int (*calc_partikel)(pZWpartikel partikel,int n);
  float alpha;
  float live;
  pZWfloatint value;
  float cameradestination;
  int nr;
  //sprite
  GLuint texture;
  GLuint cutcount;
  GLuint spritenr;
  tZWcolor color;
} tZWsprite;

typedef struct sZWmodell *pZWmodell;
typedef struct sZWmodell {
  //common
  GLbyte type;
  tZWpoint position;
  GLbyte rotate;
  tZWpoint rotation;
  tZWpoint scale;
  int (*calc_partikel)(pZWpartikel partikel,int n);
  float alpha;
  float live;
  pZWfloatint value;
  float cameradestination;
  int nr;
  //modell
  void (*draw_partikel)(pZWpartikel partikel);
} tZWmodell;

typedef struct sZWcommon *pZWcommon;
typedef struct sZWcommon {
  GLbyte type;
  tZWpoint position;
  GLbyte rotate;
  tZWpoint rotation;
  tZWpoint scale;
  int (*calc_partikel)(pZWpartikel partikel,int n);
  float alpha;
  float live; //ist für die Lebensdauer gedacht, muss aber nicht
  pZWfloatint value; //Werte, kann beliebig gesetzt werden.
  float cameradestination;
  int nr;
} tZWcommon;

typedef union uZWpartikel {
  GLbyte type;
  tZWcommon common;
  tZWsprite sprite;
  tZWmodell modell;
} tZWpartikel;

PRAEFIX void ZWnewspritepartikel(tZWpoint position,tZWpoint rotation,tZWpoint scale,GLbyte rotate,
                                 int (*calc_partikel)(pZWpartikel partikel,int n),float live,pZWfloatint value,
                                 GLuint texture, GLuint cutcount, GLuint spritenr,tZWcolor color);
PRAEFIX void ZWnewmodellpartikel(tZWpoint position,tZWpoint rotation,tZWpoint scale,GLbyte rotate,
                                 int (*calc_partikel)(pZWpartikel partikel,int n),float live,pZWfloatint value,
                                 void (*draw_partikel)(pZWpartikel partikel));
PRAEFIX void ZWdrawpartikel(tZWpoint cameraposition,tZWpoint camerarotation);
PRAEFIX void ZWcalcpartikel(int n);
PRAEFIX void ZWnewpartikelexplosion(tZWpoint position,int partikelanz,float power,GLuint texture,int cutcount,int partikelnr,float size);
PRAEFIX void ZWresetpartikel(void);

#endif
#define PARTIKEL_H
