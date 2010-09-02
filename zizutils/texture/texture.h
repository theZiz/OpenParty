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
#ifndef TEXTURE_H

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

#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>


PRAEFIX GLuint ZWloadtexturefromfile(char* name,int better,int reduce); //Läd eine PNG/JPG/BMP/etc als Textur in das Object
PRAEFIX GLuint ZWloadtexturefromfileA(char* name,int better,int reduce); //Läd eine PNG/JPG/BMP/etc als Textur in das Object mit Beachtung von Alpha
PRAEFIX void ZWdeletetexture(GLuint texnr);
PRAEFIX void ZWusetexture(GLuint texnr); //Lässt die State-Machine OpenGL diese Texture nutzen
PRAEFIX void ZWdrawsprite(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr);
PRAEFIX void ZWreloadalltextures(int reduce);
PRAEFIX void ZWdrawspritevmirror(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr);
PRAEFIX void ZWdrawspritehmirror(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr);
PRAEFIX void ZWdrawspritevhmirror(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr);
PRAEFIX void ZWdrawspritealpha(GLuint texnr,float x,float y,float z,float w,float h,int anz,int nr,float alpha);
PRAEFIX void ZWsettexturepercent(float p);
PRAEFIX float ZWgettexturepercent(void);
PRAEFIX void ZWsettextureresolution(GLuint texnr,int w,int h);
PRAEFIX void ZWsetstdreduce(int i);
PRAEFIX int ZWgetstdreduce(void);
PRAEFIX GLuint ZWgetmomtexture(void);
PRAEFIX void ZWsetmomtexture(GLuint momtexnr);

#endif
#define TEXTURE_H
