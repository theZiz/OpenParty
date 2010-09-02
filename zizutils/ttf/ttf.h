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
#ifndef TTF_H

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

#include <SDL/SDL_ttf.h>
//#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glext.h>
#include "./../texture/texture.h"

typedef struct sZWtext *pZWtext;
typedef struct sZWtext 
{
  GLuint texture;
  float breite[256];
  //GLuint vbo;
}ZWtext;


PRAEFIX void ZWinitfont(void);
PRAEFIX ZWtext ZWcreatefonttexture(char* fontname,int res,GLbyte r,GLbyte g,GLbyte b); //Bsp: ...("font.ttf",256) <- Ein Buchstabe 16x16
PRAEFIX void ZWdrawtext(ZWtext t,float x,float y,float z,char* text,float scale); // (scale==1 => Nutzen der maxmialen Buchstabengröße
PRAEFIX void ZWdrawtextmiddle(ZWtext t,float x,float y,float z,char* text,float scale); // (scale==1 => Nutzen der maxmialen Buchstabengröße
PRAEFIX void ZWdrawtextright(ZWtext t,float x,float y,float z,char* text,float scale);
PRAEFIX void ZWdrawtext_fac(ZWtext t,float x,float y,float z,char* text,float scale,float faktor);
PRAEFIX void ZWdrawtextmiddle_fac(ZWtext t,float x,float y,float z,char* text,float scale,float faktor);
PRAEFIX void ZWdrawtextright_fac(ZWtext t,float x,float y,float z,char* text,float scale,float faktor);
PRAEFIX void ZWdrawtext_shadow(ZWtext t,float x,float y,float z,char* text,float scale,float faktor,float r,float g,float b,
                               float sr,float sg,float sb,float sa);
PRAEFIX void ZWdrawtextright_shadow(ZWtext t,float x,float y,float z,char* text,float scale,float faktor,float r,float g,float b,
                                    float sr,float sg,float sb,float sa);
PRAEFIX void ZWdrawtextmiddle_shadow(ZWtext t,float x,float y,float z,char* text,float scale,float faktor,float r,float g,float b,
                                     float sr,float sg,float sb,float sa);
PRAEFIX void ZWdeletefonttexture(ZWtext texture);
PRAEFIX void ZWquitfont(void);
#endif
#define TTF_H
