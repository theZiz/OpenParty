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

//defines
#define OPENPARTY_WAITTIME 5.999
#define OPENPARTY_WAITTIME2 10.999
#define COLOR_BASE 0.3
#define COLOR_MULT 0.2
//typedefs
typedef struct smenudata *pmenudata;
typedef struct smenudata {
  int position;
  float rotation;
  float ballmov;
  int facenr;
  int facecolor;
  object3d jumpball;
  float menu_choose;
  float old_choose;
  float menu_delta;
  float fade;
  int fade_dir;
  int dest;
  char play_with[8];
  float counter;
  int need_reboot;
  char calib_step[8];
  char blocked_color[27];
  char color_choose[8];
  char choose_step[8];
  float x[8],y[8];
  char blocked_field[8];
  //pstringchain lan_choose[3][8];
	char cancel_all;
} tmenudata;

//globale Variablen
  tmenudata menudata;
  GLuint VertexProgram;
  float ballonx=0,ballony=0,ballonz=0;
//Objects
  object3d letter_o,letter_p,letter_e,letter_n,letter_a,letter_r,letter_t,letter_y;
  object3d ballon;
  int hint_counter;
  char hint_text1[32];
  char hint_text2[32];
  char hint_text3[32];
  

void load_op_objects(void)
{
  //Open Party Buchstaben
    ZWload3dm(&letter_o,(char*)DATAFOLDER"models/o.3dm",maintex);
    ZWcreatedrawlist(&letter_o,lightquali);
    ZWload3dm(&letter_p,(char*)DATAFOLDER"models/p.3dm",maintex);
    ZWcreatedrawlist(&letter_p,lightquali);
    ZWload3dm(&letter_e,(char*)DATAFOLDER"models/e.3dm",maintex);
    ZWcreatedrawlist(&letter_e,lightquali);
    ZWload3dm(&letter_n,(char*)DATAFOLDER"models/n.3dm",maintex);
    ZWcreatedrawlist(&letter_n,lightquali);
    ZWload3dm(&letter_a,(char*)DATAFOLDER"models/a.3dm",maintex);
    ZWcreatedrawlist(&letter_a,lightquali);
    ZWload3dm(&letter_r,(char*)DATAFOLDER"models/r.3dm",maintex);
    ZWcreatedrawlist(&letter_r,lightquali);
    ZWload3dm(&letter_t,(char*)DATAFOLDER"models/t.3dm",maintex);
    ZWcreatedrawlist(&letter_t,lightquali);
    ZWload3dm(&letter_y,(char*)DATAFOLDER"models/y.3dm",maintex);
    ZWcreatedrawlist(&letter_y,lightquali);
    
    ZWload3dm(&ballon,(char*)DATAFOLDER"models/ballon.3dm",maintex);
    ZWcreatedrawlist(&ballon,lightquali);
}
  
void init_op_var(void)
{    
 
  load_op_objects();
  //snd_menumusic=Mix_LoadMUS((char*)DATAFOLDER"sounds/plop.ogg");
  //menudata
    menudata.ballmov=0;
    menudata.facenr=rand()%MAX_FACES;
    menudata.facecolor=rand()%27;
    ZWload3dm(&(menudata.jumpball),(char*)DATAFOLDER"models/ball.3dm",facetex);
    menudata.rotation=0;
    int b;
    for (b=0;b<menudata.jumpball.pointnum;b++)
    {
      float dazu=((float)(menudata.facenr%4))*0.25;
      menudata.jumpball.p[b].u+=dazu;
      dazu=((float)(menudata.facenr/4))*0.25;
      menudata.jumpball.p[b].v+=dazu;    
    }
    ZWcreatedrawlist(&(menudata.jumpball),lightquali);
    menudata.menu_delta=0;
    menudata.menu_choose=0.0;
    menudata.fade=0;
    menudata.fade_dir=1;
}

void del_op_objects(void)
{
  ZWdeleteobject(letter_o);
  ZWdeleteobject(letter_p);
  ZWdeleteobject(letter_e);
  ZWdeleteobject(letter_n);
  ZWdeleteobject(letter_a);
  ZWdeleteobject(letter_r);
  ZWdeleteobject(letter_t);
  ZWdeleteobject(letter_y);
  ZWdeleteobject(ballon);
}

//float distance(float x1,float y1,float z1,float x2,float y2,float z2)
//{
//  return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
//}
