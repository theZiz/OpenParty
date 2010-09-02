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
#ifndef XML_H

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

typedef struct sxmlfile *pxmlfile;
typedef struct sxmlfile
{
  SDL_RWops *file;
  char* name;
  int ebene;
  char last;
} txmlfile;

typedef struct sxmlpoint *pxmlpoint;
typedef struct sxmlpoint
{
  char *xml;
  int pos;
  int len;
} txmlpoint;

typedef struct sxmlattribute *pxmlattribute;
typedef struct sxmlattribute
{
  char* name;
  char* wert; //Es erfolgt keine Umwandlung in irgendein Format, es wird gezeigt, was das ist.
  pxmlattribute next;
} txmlattribute;

typedef struct sxmltag *pxmltag;
typedef struct sxmltag
{
  char* name; //Name das Tags
  pxmlpoint inhalt;//Inhalt, wenn NULL ist der Tag leer oder ein Alleinstehnder <leer/>
  pxmlattribute attribute; //Attribute (verkettete Liste, NULL, wenn es keine Attribute gab
} txmltag;

PRAEFIX pxmlfile ZWnewxmlfile(char* filename,char* name);
PRAEFIX void ZWfinishxmlfile(pxmlfile xmlfile);
PRAEFIX void ZWbegintag(pxmlfile xmlfile,char* tag);
PRAEFIX void ZWaddattribute_string(pxmlfile xmlfile,char* name,char* wert);
PRAEFIX void ZWaddattribute_number(pxmlfile xmlfile,char* name,int wert);
PRAEFIX void ZWaddattribute_float(pxmlfile xmlfile,char* name,float wert);
PRAEFIX void ZWendnormaltag(pxmlfile xmlfile);
PRAEFIX void ZWendstandalonetag(pxmlfile xmlfile);
PRAEFIX void ZWclosetag(pxmlfile xmlfile,char* tag);
PRAEFIX pxmlpoint ZWloadxmlfile(char* filename);
PRAEFIX pxmltag ZWgetnexttag(pxmlpoint xmlpoint);
PRAEFIX void ZWfinishloading(pxmlpoint xmlpoint);
PRAEFIX void ZWfreetag(pxmltag xmltag);
PRAEFIX void ZWaddplaintext(pxmlfile xmlfile,char* text);
PRAEFIX void ZWclosetagonsameline(pxmlfile xmlfile,char* tag);
PRAEFIX void ZWendnormaltagwithoutbreak(pxmlfile xmlfile);

#endif
#define XML_H
