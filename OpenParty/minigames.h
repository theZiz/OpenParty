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
/*#ifdef FORINSTALL
  #define MINIGAMEDATAFOLDER "/usr/share/openparty/"
#else
  #define MINIGAMEDATAFOLDER "./"
#endif*/

typedef struct sOPjoystick *pOPjoystick;
typedef struct sOPjoystick {
  Uint8* button[4];
  Sint16* axis[2];
} tOPjoystick;

typedef struct sOPcolor *pOPcolor;
typedef struct sOPcolor {
  float r,g,b,a;
} tOPcolor;


typedef struct sgameinfo *pgameinfo;
typedef struct sgameinfo {
//Spiel
  int playernum;
  GLuint ball[8];
  tOPcolor color[8];
  int money[8];
  int rubins[8];
  int facenr[8];
  int teamsize;
  int team[8];
  char iscpu[8];
  Uint32 *steps;
  GLuint maintex;
  GLuint facetex;
  int *shouldend;
//Text
  GLuint texttexture;
  float *textwidth;
//Optionen
  int texquali;
  int fontquali;
  int lightquali;
  int windowx;
  int windowy;
  int resolutionx;
  int resolutiony;
  int fullscreen;
  int stretched;
  float volume;
//Joystick:
  tOPjoystick joystick[8];
//Hinzugekommenes, was auch Kompatibilitätsgründen nicht an der richtigen Stelle kommt:
  GLuint sky_menu_tex;
	char* datafolder;
} tgameinfo;

typedef struct sminigameinfo *pminigameinfo;
typedef struct sminigameinfo {
  char playable[8][4]; //Spieleranzahl und Teams in Konstellation
  
} tminigameinfo;

typedef struct sgameresult *pgameresult;
typedef struct sgameresult {
  char winner[8];
} tgameresult;

#ifdef WIN32
  #include <windows.h>
  #if BUILDING_DLL
    # define DLLIMPORT __declspec (dllexport)
  #else /* Not BUILDING_DLL */
    # define DLLIMPORT __declspec (dllimport)
  #endif /* Not BUILDING_DLL */
#endif //win32

#ifdef WIN32
  #define EXPORT DLLIMPORT
#else
  #define EXPORT extern "C"
#endif

#ifdef WIN32
BOOL APIENTRY DllMain (HINSTANCE hInst,DWORD reason,LPVOID reserved)
{
    return TRUE;
}
#endif

