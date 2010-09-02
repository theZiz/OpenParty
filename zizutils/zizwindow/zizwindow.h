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

#ifndef ZIZWINDOW_H

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

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define lnone 0
#define lbronze 100
#define lbronzep 101
#define lchrom 102
#define lgold 103
#define lgoldp 104
#define lkupfer 105
#define lkupferp 106
#define lmessing 107
#define lsilber 108
#define lsilberp 109
#define lzinn 110
#define lgummi 111
#define lplastik 112
#define ljade 113
#define lobsidian 114
#define lperle 115
#define lrubin 116
#define lsmaragd 117
#define ltuerkis 118

typedef struct sZWcolor *pZWcolor;
typedef struct sZWcolor {
  float r,g,b,a;
} tZWcolor;

typedef struct sZWrealjoystick *pZWrealjoystick;
typedef struct sZWrealjoystick {
  SDL_Joystick *joy;
  Uint8 buttoncount;
  Uint8 only_axiscount;
  Uint8 hatcount;
  Uint8 axiscount;
  Uint8 *button;
	char  *button_mapped_direction; //0 not mapped, 1 right, -1 left
	Uint8 *button_mapped_axis;
  Sint16 *axis;
  char *name;
  char *axis_free;
} tZWrealjoystick;

typedef struct sZWjoystick *pZWjoystick;
typedef struct sZWjoystick {
  Uint8* button[4];
  Sint16* axis[2];
	Uint8 nullbutton[4];
	Sint16 nullaxis[2];
} tZWjoystick;

typedef struct sZWresolution *pZWresolution;
typedef struct sZWresolution {
  int x,y;
} tZWresolution;

typedef struct sZWnewaxis *pZWnewaxis;
typedef struct sZWnewaxis {
	char* name;
	int joynr;
	int axisnr;
	int left;
	int right;
	pZWnewaxis next;
} tZWnewaxis;

typedef struct sZWnewkeyboard *pZWnewkeyboard;
typedef struct sZWnewkeyboard {
	char* name;
	int joynr;
	int key[8]; //left,up,right,down ABCD
	pZWnewkeyboard next;
} tZWnewkeyboard;


typedef struct sZWattribute *pZWattribute;
typedef struct sZWattribute {
  int strech;
  void (*on_resize)(void);
  SDL_Surface* surface;
  const SDL_VideoInfo* info;
  int width;
  int height;
  int bpp;
  int sdlflags;
  int initflags;
  float glnear;
  float glfar;
  char keypressed[512];
	Sint8 key_mapped[512]; //-1 nein, sonst Joysticknr;
	Sint8 key_dest[512]; //0..3 Buttons, 4..7 Achsen
  short unicode[65536];
  float perspektive;
  Uint32 steps;
  int fps;
  int realjoystickcount;
  pZWrealjoystick realjoystick;
  tZWjoystick joystick[8];
  void (*quit_function)(void);
  int minwait;
  Uint32 newticks;
  Uint32 oldticks;
  pZWresolution resolution;
  int resolutioncount;
	GLuint keyboardtexture;
	char keyboardtexture_exists;
	char keyboardtexture_position;
	int keyboard_joystick_num;
  char pausescreen;
	GLuint pausetexture;
	char pausetexture_exists;
} tZWattribute;

typedef struct sZWfrustum *pZWfrustum;
typedef struct sZWfrustum {
  float frustum[6][4];
} tZWfrustum;

PRAEFIX void ZWinitparameter(void);
PRAEFIX void ZWinitsdl(void);
PRAEFIX void ZWinitopengl(void);
PRAEFIX void ZWquitsdl(void);
PRAEFIX int ZWschleife(void (*draw_scene)(void* data),int (*calc_proc)(void* data),void *data,int size);
PRAEFIX int ZWschleifeex(void (*draw_scene)(void* data),int (*calc_proc)(void* data),int (*calc_procex)(void* data),void *data,int size);
PRAEFIX void ZWprocess_events( void );
PRAEFIX void ZWenablefog_near(float near, float far);
PRAEFIX void ZWenablefog_ext(float d);
PRAEFIX void ZWenablefog_ext2(float d);
PRAEFIX void ZWdisablefog(void);
PRAEFIX void ZWsetfogcolor(float r,float g,float b);
PRAEFIX void ZWsetmaterialambient(float r,float g,float b,float a);
PRAEFIX void ZWsetmaterialspecular(float r,float g,float b,float a);
PRAEFIX void ZWsetmaterialdiffuse(float r,float g,float b,float a);
PRAEFIX void ZWsetmaterialshininess(float s);
PRAEFIX void ZWsetmaterial(int material);
PRAEFIX void ZWsetlightambient(int licht,float r,float g,float b);
PRAEFIX void ZWsetlightdiffuse(int licht,float r,float g,float b);
PRAEFIX void ZWsetlightposition(int licht,float x,float y,float z);
PRAEFIX void ZWenableculling(void);
PRAEFIX void ZWdisableculling(void);
PRAEFIX void ZWdisablelight(int licht);
PRAEFIX void ZWenablelight(int licht);
PRAEFIX void ZWclearall(void);
PRAEFIX void ZWflipbuffer(void);
PRAEFIX void ZWhandle_key_down( SDL_keysym* keysym );
PRAEFIX void ZWhandle_key_up( SDL_keysym* keysym );
PRAEFIX void ZWglResizeWindow(void);
PRAEFIX pZWattribute ZWgetattribute(void);
PRAEFIX void ZWNormalizePlane(pZWfrustum frustum,int pPlane);
PRAEFIX void ZWfrustum_calculate(pZWfrustum frustum);
PRAEFIX int ZWfrustum_isboxwithin(pZWfrustum frustum,float pX,float pY,float pZ,float pB,float pH,float pT);
PRAEFIX int ZWfileexists(char* filename);
PRAEFIX void ZWreset_virtuell_joytick(void);
PRAEFIX void ZWnewaxis(char* name,int left,int right);
PRAEFIX void ZWnewkeyboard(char* name,char* b1,char* b2,char* b3,char* b4,char* a1l,char* a1r,char* a2l,char* a2r);

#endif
#define ZIZWINDOW_H
