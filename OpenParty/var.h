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
#ifdef FORINSTALL
  #define DATAFOLDER "/usr/share/openparty/"
#else
  #define DATAFOLDER "./"
#endif

#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#define DIR_MODUS 0711
#define FADE_OUT_TIME 700
#define FADE_IN_TIME 400
#define DICE_SPEED 88.8 /*133.0*/
#define MAX_FACES 15
//defines
#define walkspeed 1.0
char texterror[20]="error: unknown text";
//Typendefinitionen
typedef char *pchar;

typedef struct sobject *pobject;
typedef struct sobject {
  char name[256];
  char filename[256];
  char texture[256];
  object3d realobject;
  float maxy;
  pobject next;
} tobject;

typedef struct sset *pset;
typedef struct sset {
  char* object;
  pobject realobject;
  char* type;
  char *triggerfunction_in,*triggerfunction_out;
  int triggerjump_in,triggerjump_out;
  int triggertrap;
  int trapowner;
  char* endfunction;
  int endjump;
  char* name; 
  int nr;
  int gamenextnr;
  float abstand;
  float maxy;
  float realalpha;
  int alwaysvisible;
  tZWpoint position,rotation,scale,cameraposition,camerarotation;
  tZWcolor color;
  pset before,next;
  pset choicenext,choicebefore;
} tset;

typedef struct sitem *pitem;
typedef struct sitem {
	char* name;
	char* script;
	int script_pos;
	int kind; //0 nothing, 1 user, 2 team, 3 set, 4 user_on_set
	int radius;
	char friendlyfire;
	char* object_name;
	pobject object;
	tZWpoint object_scale;
	char* description;
	int description_pos;
	int number;
	int price;
	pitem next;
} titem;

typedef struct splayer *pplayer;
typedef struct splayer {
  int money;
  int rubins;
  pset momset,lastset;
  tZWpoint momposition;
  tZWpoint destposition;
  tZWcolor color;
  int facenr;
  object3d ball;
  float destface;
	float destsize;
  float momface;
	float momsize;
  float time;  
  float savedcamera_y;
  int team;
  char iscpu;
  int joystick_nr;
	char has_calib;
	pitem item[3]; 
} tplayer;

typedef struct smaindata *pmaindata;
typedef struct smaindata{
  tZWpoint momcamerarotation;
  tZWpoint destcamerarotation;
  tZWpoint momcameraposition;
  tZWpoint destcameraposition;
  float time;
  int activplayer;
  int mainloop_pos;
  float fade_counter;
  int choose_choose;
  int choose_move;
  float choose_angle;
  int item_choose_choose;
  int item_choose_move;
	char item_in_script;
	float* item_script_stack;
	int item_script_pos;
	int turn_dice_value[10];
  float turn_dice_next;
	int turn_dice_num;
	int turn_dice_dest;
  int turn_pos;
  float turn_jump_high;
  float *turn_script_stack;
  int turn_script_pos;
  int round;
	float map_zoom;
	float map_x;
	float map_y;
	char map_show_env;
  tplayer player[8];  
	int ingame_kalib_step;
	int ingame_kalib_joystick;
	int ingame_kalib_map[6];
	char gamestartinfo;
	int gameinfochoice[8];
	int use_item_nr;
	int camera_looking_at; //-1 active player, 0..7 Player 0 til 7, 8 angegebene Position
	tZWpoint manuell_camera_point;
	pitem item_shop_item[8];
	int item_shop_price[8];
	int item_shop_item_count;
	int special_step;
	float dice_speed;
} tmaindata;

typedef struct ssky *psky;
typedef struct ssky {
  char texture[256];
  object3d sky;
  tZWpoint position;
  tZWpoint size;
} tsky;

typedef struct slight *plight;
typedef struct slight {
  tZWpoint position;
  tZWcolor ambient;
  tZWcolor diffuse;
  GLuint lightnr;
  plight next;
} tlight;

typedef struct slanguage *planguage;
typedef struct slanguage {
  char* language;
  char* content;
  planguage next;
} tlanguage;

typedef struct stext *ptext;
typedef struct stext {
  char* name;
  planguage language;  
  ptext next;
} ttext;

typedef struct sborder *pborder;
typedef struct sborder {
  tZWcolor left,right,up,down;
} tborder;

typedef struct stextsite *ptextsite;
typedef struct stextsite {
  pchar line[5];
  ptextsite next;
} ttextsite;

typedef struct sstringchain *pstringchain;
typedef struct sstringchain {
  char* string;
  pstringchain before,next;
} tstringchain;

typedef struct ssample *psample;
typedef struct ssample {
  Mix_Chunk *chunk;
  int next_play;
  int real_next_play;
  int plusminus;
  psample next;
} tsample;

typedef struct sminigame *pminigame;
typedef struct sminigame {
  char *name;
	char *filename;
	int played;
  char ingame;
  int revision;
  char installed;
  GLuint preview;
	pminigame next;
  pminigame prechoicenext;
} tminigame;

typedef struct scontroller_db *pcontroller_db;
typedef struct scontroller_db {
	char *name;
	int button[4];
	int axis[2];
	pcontroller_db next;
} tcontroller_db;

//globale Variablen
//Debugvariablen
  char all_are_cpu=0;
//allgemein
  pZWattribute ZWattribute;
  tmaindata maindata;
  int shouldend=0;
  int playernum=8;
  float diceangle=0;
  GLuint maintex;
	GLuint facetex;
	GLuint dicetex;
  int teamsize=1;
  int cpuplayer=0;
  pstringchain possiblelanguages=NULL;
  int gametype=0; //Spieltyp: 0..7 (5..40 Runden) 8..11 (111...444 Münzen) 12..15 (3,5,7 oder 10 Rubine)
	int gamechoice=0;
	pcontroller_db controller_db=NULL;
	pitem firstitem=NULL;
  pset firstchoiceset,momchoiceset;
//Optionen
  int texquali=2;
  int fontquali=2;
  int lightquali=1;
  int windowx=600;
  int windowy=450;
  int resolutionx=640;
  int resolutiony=480;
  int fullscreen=0;
  int stretched=0;
  char language[32];
	char *gameinfotext=NULL;
  char firsttimestarted=1;
//3dobjects
  object3d ball,coin,rubin,dice,arrow,sphere;
  object3d item[10];
//Licht
  GLuint maxlightnr=0;
  plight firstlight=NULL;
//Skybox
  int skyboxart=0;
  tsky sky;
//Objects
  pobject firstobject=NULL;
//Texturen
  GLuint sky_menu_tex;
//Sets
  pset *setlist=NULL;
  pset firstset=NULL;
  pset lastset=NULL;
  pset setbynr[2048];
  int maxsetlist=0;
  int minalphasetlist=0;
//Text
  ZWtext text;
  tborder border;
  ptextsite firsttextsite;
  int maxtextsite=0;
  int momtextsite=0;
  ptext firsttext=NULL;
  ptext *textlist=NULL;
  int textcount=0;
//Sound
  float global_volume=1.0;
  psample first_sample=NULL;
  Mix_Chunk *snd_dice=NULL;
  int dice_channel=-1;
  Mix_Music *backgroundmusic=NULL;
  Mix_Chunk *snd_plop;
  Mix_Chunk *snd_dice_plop;
  Mix_Chunk *snd_jump;
  Mix_Music *snd_menumusic;
  Mix_Chunk *snd_positive;
  Mix_Chunk *snd_negative;
//Choosecircle
  char choosecircle_content[8][32];
	bool choosecircle_on;
	bool choosecircle_face;
	float choosecircle_position;
	float choosecircle_speed;
	int choosecircle_direction;
	int choosecircle_time_moved;
//Minispieleliste
  pminigame firstminigame=NULL;
  pminigame choosecircle_gamelist[5];
	int choosecircle_gamelistnum;
	int minigamenum=0;
	pminigame chosen_minigame;
	tminigame dummyminigame;
	 
void load_options(void)
{
  //possiblelanguages stringchain leeren
  pstringchain anfang=possiblelanguages;
  if (possiblelanguages!=NULL)
  do
  {
    pstringchain temp=possiblelanguages->next;
    free(possiblelanguages);
    possiblelanguages=temp;
  }
  while (possiblelanguages!=anfang);
  
  //possiblelanguages stringchain laden
  SDL_RWops *file;
  file = SDL_RWFromFile(DATAFOLDER"data/languages.ini", "rb");
  int pos=0;
  char buffer[256];
  char buf;
  pstringchain last=NULL;
  while (SDL_RWread(file,&buf,1,1)>0)
  {
    if (buf=='\n' && pos>0)
    {
      buffer[pos]=0;
      pstringchain newstring=(pstringchain)malloc(sizeof(tstringchain));
      newstring->string=(char*)malloc(pos+1);
      sprintf(newstring->string,"%s",buffer);
      newstring->next=NULL;
      newstring->before=last;
      if (possiblelanguages==NULL)
        possiblelanguages=newstring;
      else
        last->next=newstring;
      last=newstring;
      pos=0;
    }
    else
    if (pos<31)
    {
      buffer[pos]=buf;
      pos++;
    }
  }
  last->next=possiblelanguages;
  possiblelanguages->before=last;  
  
  
  SDL_RWclose(file);

  texquali=2;
  fontquali=2;
  lightquali=1;
  windowx=600;
  windowy=450;
  resolutionx=640;
  resolutiony=480;
  fullscreen=0;
  stretched=0;
  sprintf(language,"german");
	#ifndef WIN32
		const char *homenv = getenv("HOME");
		sprintf(buffer,"%s/.config/openparty/settings.xml",homenv);
	#else
		sprintf(buffer,"./settings.xml");	
	#endif
  if (not(ZWfileexists(buffer))) 
    return;
  pxmlpoint xmlpoint=ZWloadxmlfile(buffer);
  pxmltag maintag=ZWgetnexttag(xmlpoint);
  pxmltag first=ZWgetnexttag(maintag->inhalt);
  while (first!=NULL)
  {
    if (strcmp(first->name,"first_time")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"value")==0)
        {
          if (strcmp(attribute->wert,"true")==0)
            firsttimestarted=1;
          if (strcmp(attribute->wert,"false")==0)
            firsttimestarted=0;
        }
        attribute=attribute->next;
      }
    }  
    if (strcmp(first->name,"texture_quality")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"value")==0)
        {
          if (strcmp(attribute->wert,"high")==0)
            texquali=1;
          if (strcmp(attribute->wert,"middle")==0)
            texquali=2;
          if (strcmp(attribute->wert,"low")==0)
            texquali=4;
        }
        attribute=attribute->next;
      }
    }  
    if (strcmp(first->name,"font_quality")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"value")==0)
        {
          if (strcmp(attribute->wert,"high")==0)
            fontquali=1;
          if (strcmp(attribute->wert,"middle")==0)
            fontquali=2;
          if (strcmp(attribute->wert,"low")==0)
            fontquali=3;
        }
        attribute=attribute->next;
      }
    }  
    if (strcmp(first->name,"better_light")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"value")==0)
        {
          if (strcmp(attribute->wert,"True")==0)
            lightquali=1;
          else
            lightquali=0;
        }
        attribute=attribute->next;
      }
    }  
    if (strcmp(first->name,"fullscreen")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"value")==0)
        {
          if (strcmp(attribute->wert,"True")==0)
            fullscreen=1;
          else
            fullscreen=0;
        }
        attribute=attribute->next;
      }
    }
    if (strcmp(first->name,"stretched")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"value")==0)
        {
          if (strcmp(attribute->wert,"True")==0)
            stretched=1;
          else
            stretched=0;
        }
        attribute=attribute->next;
      }
    }
    if (strcmp(first->name,"resolution")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"x")==0)
          resolutionx=atoi(attribute->wert);
        if (strcmp(attribute->name,"y")==0)
          resolutiony=atoi(attribute->wert);
        attribute=attribute->next;
      }
    }  
    if (strcmp(first->name,"window")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"x")==0)
          windowx=atoi(attribute->wert);
        if (strcmp(attribute->name,"y")==0)
          windowy=atoi(attribute->wert);
        attribute=attribute->next;
      }
    }  
    if (strcmp(first->name,"language")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"value")==0)
        {
          int a;
          for (a=0;attribute->wert[a]!=0 && a<31;a++)
            language[a]=attribute->wert[a];
          language[a]=0;
        }
        attribute=attribute->next;
      }
    }  
    if (strcmp(first->name,"volume")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"value")==0)
        {
          global_volume=atof(attribute->wert);
          if (global_volume>1.0)
            global_volume=1.0;
          if (global_volume<0.0)
            global_volume=0.0;
        }
        attribute=attribute->next;
      }
    }  

    ZWfreetag(first);
    first=ZWgetnexttag(maintag->inhalt);
  }
  ZWfreetag(maintag);
  ZWfinishloading(xmlpoint);
}


void load_objects(void)
{
  //Objekte laden
  ZWload3dm(&ball,(char*)DATAFOLDER"models/ball.3dm",facetex);
  ZWcreatedrawlist(&ball,lightquali);
  ZWload3dm(&coin,(char*)DATAFOLDER"models/coin.3dm",maintex);
  ZWcreatedrawlist(&coin,0);
  ZWload3dm(&rubin,(char*)DATAFOLDER"models/rubin.3dm",maintex);
  ZWcreatedrawlist(&rubin,0);
  ZWload3dm(&arrow,(char*)DATAFOLDER"models/arrow.3dm",maintex);
  ZWcreatedrawlist(&arrow,lightquali);
  ZWload3dm(&sphere,(char*)DATAFOLDER"models/sphere.3dm",maintex);
  ZWcreatedrawlist(&sphere,lightquali);
  ZWload3dm(&dice,(char*)DATAFOLDER"models/dice.3dm",dicetex);
  ZWcreatedrawlist(&dice,lightquali);  
  ZWload3dm(&(item[0]),(char*)DATAFOLDER"models/bomb.3dm",maintex);
  ZWcreatedrawlist(&(item[0]),lightquali);  
}

void save_options(void)
{
	char buffer[256];
	#ifndef WIN32
		const char *homenv = getenv("HOME");
		sprintf(buffer,"%s/.config",homenv);
		mkdir(buffer,DIR_MODUS);
		sprintf(buffer,"%s/.config/openparty",homenv);
		mkdir(buffer,DIR_MODUS);
		sprintf(buffer,"%s/.config/openparty/settings.xml",homenv);
	#else
		sprintf(buffer,"./settings.xml");	
	#endif
	

  pxmlfile xmlfile=ZWnewxmlfile(buffer,(char*)"settings");
  ZWbegintag(xmlfile,(char*)"first_time");
    switch (firsttimestarted)
    {
      case 0: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"false"); break;
      case 1: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"true"); break;
    }
  ZWendstandalonetag(xmlfile);
  ZWbegintag(xmlfile,(char*)"texture_quality");
    switch (texquali)
    {
      case 1: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"high"); break;
      case 2: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"middle"); break;
      default: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"low");
    }
  ZWendstandalonetag(xmlfile);
  ZWbegintag(xmlfile,(char*)"font_quality");
    switch (fontquali)
    {
      case 1: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"high"); break;
      case 2: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"middle"); break;
      default: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"low");
    }
  ZWendstandalonetag(xmlfile);
  ZWbegintag(xmlfile,(char*)"better_light");
    switch (lightquali)
    {
      case 1: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"True"); break;
      default: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"False");
    }
  ZWendstandalonetag(xmlfile);
  ZWbegintag(xmlfile,(char*)"resolution");
    ZWaddattribute_number(xmlfile,(char*)"x",resolutionx);
    ZWaddattribute_number(xmlfile,(char*)"y",resolutiony);
  ZWendstandalonetag(xmlfile);
  ZWbegintag(xmlfile,(char*)"window");
    ZWaddattribute_number(xmlfile,(char*)"x",windowx);
    ZWaddattribute_number(xmlfile,(char*)"y",windowy);
  ZWendstandalonetag(xmlfile);
  ZWbegintag(xmlfile,(char*)"fullscreen");
    switch (fullscreen)
    {
      case 1: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"True"); break;
      default: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"False");
    }
  ZWendstandalonetag(xmlfile);
  ZWbegintag(xmlfile,(char*)"stretched");
    switch (stretched)
    {
      case 1: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"True"); break;
      default: ZWaddattribute_string(xmlfile,(char*)"value",(char*)"False");
    }
  ZWendstandalonetag(xmlfile);
  ZWbegintag(xmlfile,(char*)"language");
    ZWaddattribute_string(xmlfile,(char*)"value",language);
  ZWendstandalonetag(xmlfile);
  ZWbegintag(xmlfile,(char*)"volume");
    ZWaddattribute_float(xmlfile,(char*)"value",global_volume);
  ZWendstandalonetag(xmlfile);
  ZWfinishxmlfile(xmlfile);
}

void on_resize(void)
{
  if (fullscreen==0)
  {
    windowx=ZWattribute->width;
    windowy=ZWattribute->height;
    save_options();
  }
}

void new_controller_db_entry(char* name,int button0,int button1,int button2,int button3,int axis0,int axis1)
{
	//schon vorhanden?
	pcontroller_db entry=controller_db;
	while (entry!=NULL)
	{
		if (strcmp(entry->name,name)==0)
		  break;
		entry=entry->next;
	}
	if (entry==NULL)
	{
		entry=(pcontroller_db)malloc(sizeof(tcontroller_db));
		entry->name=(char*)malloc(strlen(name)+1);
		sprintf(entry->name,"%s",name);
		entry->next=controller_db;
		controller_db=entry;
	}
	entry->button[0]=button0;
	entry->button[1]=button1;
	entry->button[2]=button2;
	entry->button[3]=button3;
	entry->axis[0]=axis0;
	entry->axis[1]=axis1;
}

void save_controller_db(void)
{
	char buffer[256];
	#ifndef WIN32
		const char *homenv = getenv("HOME");
		sprintf(buffer,"%s/.config",homenv);
		mkdir(buffer,DIR_MODUS);
		sprintf(buffer,"%s/.config/openparty",homenv);
		mkdir(buffer,DIR_MODUS);
		sprintf(buffer,"%s/.config/openparty/controller_db.xml",homenv);
	#else
		sprintf(buffer,"./settings.xml");	
	#endif

  pxmlfile xmlfile=ZWnewxmlfile(buffer,(char*)"database");
	pcontroller_db entry=controller_db;
	while (entry!=NULL)
	{
		ZWbegintag(xmlfile,(char*)"controller");
			ZWaddattribute_string(xmlfile,(char*)"name",entry->name);
		ZWendnormaltag(xmlfile);
		int a;
		for (a=0;a<4;a++)
		{
			ZWbegintag(xmlfile,(char*)"button");
				ZWaddattribute_number(xmlfile,(char*)"nr",a);
				ZWaddattribute_number(xmlfile,(char*)"mapping",entry->button[a]);
			ZWendstandalonetag(xmlfile);
		}
		for (a=0;a<2;a++)
		{
			ZWbegintag(xmlfile,(char*)"axis");
				ZWaddattribute_number(xmlfile,(char*)"nr",a);
				ZWaddattribute_number(xmlfile,(char*)"mapping",entry->axis[a]);
			ZWendstandalonetag(xmlfile);
		}
		ZWclosetag(xmlfile,(char*)"controller");
		entry=entry->next;
	}
  ZWfinishxmlfile(xmlfile);

}

void load_controller_db(void)
{
	char buffer[256];
	#ifndef WIN32
		const char *homenv = getenv("HOME");
		sprintf(buffer,"%s/.config/openparty/controller_db.xml",homenv);
	#else
		sprintf(buffer,"./controller_db.xml");	
	#endif
  if (not(ZWfileexists(buffer))) 
	{
//		new_controller_db_entry(ZWattribute->realjoystick[0].name,0,1,2,3,0,1);
//		new_controller_db_entry(ZWattribute->realjoystick[1].name,0,1,2,3,0,1);
//		save_controller_db();
	}
	else
	{
		pxmlpoint xmlpoint=ZWloadxmlfile(buffer);
		pxmltag maintag=ZWgetnexttag(xmlpoint);
		pxmltag first=ZWgetnexttag(maintag->inhalt);
		while (first!=NULL)
		{
			if (strcmp(first->name,"controller")==0)
			{
				char* name;
				int button[4];
				int axis[2];
				pxmlattribute attribute=first->attribute;
				while (attribute!=NULL)
				{
					if (strcmp(attribute->name,"name")==0)
					  name=attribute->wert;
					attribute=attribute->next;
				}
				pxmltag second=ZWgetnexttag(first->inhalt);
				while (second!=NULL)
				{
					if (strcmp(second->name,"button")==0)
					{
						int nr=0;
						int mapping=0;
						pxmlattribute attribute=second->attribute;
						while (attribute!=NULL)
						{
							if (strcmp(attribute->name,"nr")==0)
								nr=atoi(attribute->wert)%4;
							if (strcmp(attribute->name,"mapping")==0)
								mapping=atoi(attribute->wert);
							attribute=attribute->next;
						}
						button[nr]=mapping;
					}
					if (strcmp(second->name,"axis")==0)
					{
						int nr=0;
						int mapping=0;
						pxmlattribute attribute=second->attribute;
						while (attribute!=NULL)
						{
							if (strcmp(attribute->name,"nr")==0)
								nr=atoi(attribute->wert)%2;
							if (strcmp(attribute->name,"mapping")==0)
								mapping=atoi(attribute->wert);
							attribute=attribute->next;
						}
						axis[nr]=mapping;
					}
					ZWfreetag(second);
					second=ZWgetnexttag(first->inhalt);
				}
				new_controller_db_entry(name,button[0],button[1],button[2],button[3],axis[0],axis[1]);
			}  
			ZWfreetag(first);
			first=ZWgetnexttag(maintag->inhalt);
		}
		ZWfreetag(maintag);
		ZWfinishloading(xmlpoint);
	}
}

void calib_all_controllers()
{
	int a,b;
	for (a=0;a<playernum;a++)
	{
		if (maindata.player[a].joystick_nr<0) //cpu
		{
			for (b=0;b<4;b++)
				ZWattribute->joystick[a].button[b]=&(ZWattribute->joystick[a].nullbutton[b]);
			for (b=0;b<2;b++)
				ZWattribute->joystick[a].axis[b]=&(ZWattribute->joystick[a].nullaxis[b]);			
			maindata.player[a].has_calib=1;
		}
		else
		{
			//Controller suchen:
			pcontroller_db entry=controller_db;
			while (entry!=NULL)
			{
				if (strcmp(entry->name,ZWattribute->realjoystick[maindata.player[a].joystick_nr].name)==0)
					break;
				entry=entry->next;
			}
			int joysticknr=maindata.player[a].joystick_nr;
			/*for (joysticknr=0;joysticknr<ZWattribute->realjoystickcount-1;joysticknr++)
				if (strcmp(ZWattribute->realjoystick[joysticknr].name,maindata.player[a].joystick_name)==0)
					break;*/
			if (entry==NULL) //nicht vorhanden: Default
			{
				for (b=0;b<4;b++)
					ZWattribute->joystick[a].button[b]=&(ZWattribute->realjoystick[joysticknr].button[b]);
				for (b=0;b<2;b++)
					ZWattribute->joystick[a].axis[b]=&(ZWattribute->realjoystick[joysticknr].axis[b]);
				maindata.player[a].has_calib=(joysticknr<ZWattribute->keyboard_joystick_num)?1:0;
			}
			else //Daten aus DB laden
			{
				for (b=0;b<4;b++)
					ZWattribute->joystick[a].button[b]=&(ZWattribute->realjoystick[joysticknr].button[entry->button[b]]);
				for (b=0;b<2;b++)
					ZWattribute->joystick[a].axis[b]=&(ZWattribute->realjoystick[joysticknr].axis[entry->axis[b]]);				
				maindata.player[a].has_calib=1;
			}
		}
	}
}

//Initialiasierung einiger Variablen
void init_var(void)
{
  //Texturen
  sky_menu_tex=ZWloadtexturefromfileA((char*)DATAFOLDER"textures/sky_menu.png",0,texquali);
  dicetex=ZWloadtexturefromfileA((char*)DATAFOLDER"textures/dice.png",0,texquali);    
  maintex=ZWloadtexturefromfileA((char*)DATAFOLDER"textures/openparty.png",0,texquali);  
  facetex=ZWloadtexturefromfileA((char*)DATAFOLDER"textures/faces.png",0,texquali);  
		
  ZWattribute->on_resize=on_resize;
  srand( time(0));
  sprintf(sky.texture,"");
  maindata.player[0].momset=NULL; maindata.player[1].momset=NULL; maindata.player[2].momset=NULL; maindata.player[3].momset=NULL;
  maindata.player[4].momset=NULL; maindata.player[5].momset=NULL; maindata.player[6].momset=NULL; maindata.player[7].momset=NULL;
  
  maindata.activplayer=0;
  maindata.mainloop_pos=1;
  maindata.fade_counter=1.0;

  load_objects();
  
  //Sounds
  snd_dice=Mix_LoadWAV((char*)DATAFOLDER"sounds/dice.ogg");
  snd_dice_plop=Mix_LoadWAV((char*)DATAFOLDER"sounds/dice_plop.ogg");
  snd_plop=Mix_LoadWAV((char*)DATAFOLDER"sounds/plop.ogg");
  snd_jump=Mix_LoadWAV((char*)DATAFOLDER"sounds/jump.ogg");
  snd_positive=Mix_LoadWAV((char*)DATAFOLDER"sounds/positive.ogg");
  snd_negative=Mix_LoadWAV((char*)DATAFOLDER"sounds/negative.ogg");

  snd_menumusic=Mix_LoadMUS((char*)DATAFOLDER"sounds/Bumbly March.ogg");

  Mix_VolumeChunk(snd_dice,(int)(round(32.0*global_volume)));
  Mix_VolumeChunk(snd_dice_plop,(int)(round(64.0*global_volume)));
  Mix_VolumeChunk(snd_plop,(int)(round(128.0*global_volume)));
  Mix_VolumeChunk(snd_jump,(int)(round(128.0*global_volume)));
  Mix_VolumeChunk(snd_positive,(int)(round(8.0*global_volume)));
  Mix_VolumeChunk(snd_negative,(int)(round(8.0*global_volume)));
  Mix_VolumeMusic((int)(round(128.0*global_volume)));

  dummyminigame.name=NULL;
	dummyminigame.filename=NULL;
	
	load_controller_db();
}

void quit_function(void)
{
  shouldend=1;
}

void setup_opengl(void)
{
  ZWsetmaterial(lnone);
  glEnable(GL_TEXTURE_2D);
  ZWinitfont();
  switch (fontquali)
  {
    case 1: text=ZWcreatefonttexture((char*)DATAFOLDER"data/galapogo.ttf",2048,255,255,255); break;
    case 2: text=ZWcreatefonttexture((char*)DATAFOLDER"data/galapogo.ttf",1024,255,255,255); break;
    default: text=ZWcreatefonttexture((char*)DATAFOLDER"data/galapogo.ttf",512,255,255,255); break;          
  }
}

void del_objects(void)
{
  //Objekte löschen
  ZWdeleteobject(ball);
  ZWdeleteobject(coin);
  ZWdeleteobject(rubin);
  ZWdeleteobject(arrow);
  ZWdeleteobject(sphere);
  ZWdeleteobject(dice);
  ZWdeleteobject(item[0]);
}

void del_sounds(void)
{
  //Sounds
  Mix_FreeChunk(snd_dice);
  Mix_FreeChunk(snd_dice_plop);
  Mix_FreeChunk(snd_plop);
  Mix_FreeChunk(snd_jump);
  Mix_FreeMusic(snd_menumusic);
  Mix_FreeChunk(snd_positive);
  Mix_FreeChunk(snd_negative);
}

void load_mapping(void)
{
	char buffer[256];
	#ifndef WIN32
		const char *homenv = getenv("HOME");
		sprintf(buffer,"%s/.config/openparty/mapping.xml",homenv);
	#else
		sprintf(buffer,"./mapping.xml");	
	#endif
  if (not(ZWfileexists(buffer))) 
	{
		char buffer[256];
		#ifndef WIN32
			const char *homenv = getenv("HOME");
			sprintf(buffer,"%s/.config",homenv);
			mkdir(buffer,DIR_MODUS);
			sprintf(buffer,"%s/.config/openparty",homenv);
			mkdir(buffer,DIR_MODUS);
			sprintf(buffer,"%s/.config/openparty/mapping.xml",homenv);
		#else
			sprintf(buffer,"./mapping.xml");	
		#endif
		pxmlfile xmlfile=ZWnewxmlfile(buffer,(char*)"mapping");
		ZWbegintag(xmlfile,(char*)"keyboard");
		ZWaddattribute_string(xmlfile,(char*)"name",(char*)"Keyboard WASD");
		ZWendnormaltag(xmlfile); 
		  ZWbegintag(xmlfile,(char*)"button");
			  ZWaddattribute_string(xmlfile,(char*)"key",(char*)"h");
			ZWendstandalonetag(xmlfile);
		  ZWbegintag(xmlfile,(char*)"button");
			  ZWaddattribute_string(xmlfile,(char*)"key",(char*)"u");
			ZWendstandalonetag(xmlfile);
		  ZWbegintag(xmlfile,(char*)"button");
			  ZWaddattribute_string(xmlfile,(char*)"key",(char*)"k");
			ZWendstandalonetag(xmlfile);
		  ZWbegintag(xmlfile,(char*)"button");
			  ZWaddattribute_string(xmlfile,(char*)"key",(char*)"j");
			ZWendstandalonetag(xmlfile);
			ZWbegintag(xmlfile,(char*)"axis");
			ZWendnormaltag(xmlfile);
				ZWbegintag(xmlfile,(char*)"left");
					ZWaddattribute_string(xmlfile,(char*)"key",(char*)"a");
				ZWendstandalonetag(xmlfile);
				ZWbegintag(xmlfile,(char*)"right");
					ZWaddattribute_string(xmlfile,(char*)"key",(char*)"d");
				ZWendstandalonetag(xmlfile);
			ZWclosetag(xmlfile,(char*)"axis");
			ZWbegintag(xmlfile,(char*)"axis");
			ZWendnormaltag(xmlfile);
				ZWbegintag(xmlfile,(char*)"left");
					ZWaddattribute_string(xmlfile,(char*)"key",(char*)"w");
				ZWendstandalonetag(xmlfile);
				ZWbegintag(xmlfile,(char*)"right");
					ZWaddattribute_string(xmlfile,(char*)"key",(char*)"s");
				ZWendstandalonetag(xmlfile);
			ZWclosetag(xmlfile,(char*)"axis");
		ZWclosetag(xmlfile,(char*)"keyboard");
		ZWbegintag(xmlfile,(char*)"keyboard");
		ZWaddattribute_string(xmlfile,(char*)"name",(char*)"Keyboard [<][^][>][v]");
		ZWendnormaltag(xmlfile); 
		  ZWbegintag(xmlfile,(char*)"button");
			  ZWaddattribute_string(xmlfile,(char*)"key",(char*)"num4");
			ZWendstandalonetag(xmlfile);
		  ZWbegintag(xmlfile,(char*)"button");
			  ZWaddattribute_string(xmlfile,(char*)"key",(char*)"num8");
			ZWendstandalonetag(xmlfile);
		  ZWbegintag(xmlfile,(char*)"button");
			  ZWaddattribute_string(xmlfile,(char*)"key",(char*)"num6");
			ZWendstandalonetag(xmlfile);
		  ZWbegintag(xmlfile,(char*)"button");
			  ZWaddattribute_string(xmlfile,(char*)"key",(char*)"num5");
			ZWendstandalonetag(xmlfile);
			ZWbegintag(xmlfile,(char*)"axis");
			ZWendnormaltag(xmlfile);
				ZWbegintag(xmlfile,(char*)"left");
					ZWaddattribute_string(xmlfile,(char*)"key",(char*)"left");
				ZWendstandalonetag(xmlfile);
				ZWbegintag(xmlfile,(char*)"right");
					ZWaddattribute_string(xmlfile,(char*)"key",(char*)"right");
				ZWendstandalonetag(xmlfile);
			ZWclosetag(xmlfile,(char*)"axis");
			ZWbegintag(xmlfile,(char*)"axis");
			ZWendnormaltag(xmlfile);
				ZWbegintag(xmlfile,(char*)"left");
					ZWaddattribute_string(xmlfile,(char*)"key",(char*)"up");
				ZWendstandalonetag(xmlfile);
				ZWbegintag(xmlfile,(char*)"right");
					ZWaddattribute_string(xmlfile,(char*)"key",(char*)"down");
				ZWendstandalonetag(xmlfile);
			ZWclosetag(xmlfile,(char*)"axis");
		ZWclosetag(xmlfile,(char*)"keyboard");
		ZWbegintag(xmlfile,(char*)"gamepad");
		ZWaddattribute_string(xmlfile,(char*)"name",(char*)"example joystick");
		ZWendnormaltag(xmlfile); 
			ZWbegintag(xmlfile,(char*)"axis");
			ZWendnormaltag(xmlfile);
				ZWbegintag(xmlfile,(char*)"left");
					ZWaddattribute_number(xmlfile,(char*)"button",4);
				ZWendstandalonetag(xmlfile);
				ZWbegintag(xmlfile,(char*)"right");
					ZWaddattribute_number(xmlfile,(char*)"button",5);
				ZWendstandalonetag(xmlfile);
			ZWclosetag(xmlfile,(char*)"axis");
			ZWbegintag(xmlfile,(char*)"axis");
			ZWendnormaltag(xmlfile);
				ZWbegintag(xmlfile,(char*)"left");
					ZWaddattribute_number(xmlfile,(char*)"button",6);
				ZWendstandalonetag(xmlfile);
				ZWbegintag(xmlfile,(char*)"right");
					ZWaddattribute_number(xmlfile,(char*)"button",7);
				ZWendstandalonetag(xmlfile);
			ZWclosetag(xmlfile,(char*)"axis");
		ZWclosetag(xmlfile,(char*)"gamepad");
		ZWfinishxmlfile(xmlfile);
	}
	//else
	{
		pxmlpoint xmlpoint=ZWloadxmlfile(buffer);
		pxmltag maintag=ZWgetnexttag(xmlpoint);
		pxmltag first=ZWgetnexttag(maintag->inhalt);
		while (first!=NULL)
		{
			if (strcmp(first->name,"gamepad")==0)
			{
				char* name;
				pxmlattribute attribute=first->attribute;
				while (attribute!=NULL)
				{
					if (strcmp(attribute->name,"name")==0)
					  name=attribute->wert;
					attribute=attribute->next;
				}
				pxmltag second=ZWgetnexttag(first->inhalt);
				while (second!=NULL)
				{
					if (strcmp(second->name,"axis")==0)
					{
						int left=0;
						int right=0;
						pxmltag third=ZWgetnexttag(second->inhalt);
						while (third!=NULL)
						{
							if (strcmp(third->name,"left")==0)
							{
								pxmlattribute attribute=third->attribute;
								while (attribute!=NULL)
								{
									if (strcmp(attribute->name,"button")==0)
										left=atoi(attribute->wert);
									attribute=attribute->next;
								}
							}
							if (strcmp(third->name,"right")==0)
							{
								pxmlattribute attribute=third->attribute;
								while (attribute!=NULL)
								{
									if (strcmp(attribute->name,"button")==0)
										right=atoi(attribute->wert);
									attribute=attribute->next;
								}
							}
							ZWfreetag(third);
							third=ZWgetnexttag(second->inhalt);
						}
						ZWnewaxis(name,left,right);
					}
					ZWfreetag(second);
					second=ZWgetnexttag(first->inhalt);
				}
			}  
			if (strcmp(first->name,"keyboard")==0)
			{
				char* name;
				char* button[4]={NULL,NULL,NULL,NULL};
				char* axisl[2]={NULL,NULL};
				char* axisr[2]={NULL,NULL};
				int bcount=0;
				int acount=0;
				pxmlattribute attribute=first->attribute;
				while (attribute!=NULL)
				{
					if (strcmp(attribute->name,"name")==0)
					  name=attribute->wert;
					attribute=attribute->next;
				}
				
				pxmltag second=ZWgetnexttag(first->inhalt);
				while (second!=NULL)
				{
					if (strcmp(second->name,"button")==0)
					{
						pxmlattribute attribute=second->attribute;
						while (attribute!=NULL)
						{
							if (strcmp(attribute->name,"key")==0)
							{
								free(button[bcount]);
								button[bcount]=(char*)malloc(strlen(attribute->wert)+1);
								sprintf(button[bcount],"%s",attribute->wert);
							}
							attribute=attribute->next;
						}
						bcount++;
					}
					if (strcmp(second->name,"axis")==0)
					{
						pxmltag third=ZWgetnexttag(second->inhalt);
						while (third!=NULL)
						{
							if (strcmp(third->name,"left")==0)
							{
								pxmlattribute attribute=third->attribute;
								while (attribute!=NULL)
								{
									if (strcmp(attribute->name,"key")==0)
									{
										free(axisl[acount]);
										axisl[acount]=(char*)malloc(strlen(attribute->wert)+1);
										sprintf(axisl[acount],"%s",attribute->wert);
									}
									attribute=attribute->next;
								}
							}
							if (strcmp(third->name,"right")==0)
							{
								pxmlattribute attribute=third->attribute;
								while (attribute!=NULL)
								{
									if (strcmp(attribute->name,"key")==0)
									{
										free(axisr[acount]);
										axisr[acount]=(char*)malloc(strlen(attribute->wert)+1);
										sprintf(axisr[acount],"%s",attribute->wert);
									}
									attribute=attribute->next;
								}
							}
							ZWfreetag(third);
							third=ZWgetnexttag(second->inhalt);
						}
						acount++;
					}
					ZWfreetag(second);
					second=ZWgetnexttag(first->inhalt);
				}
				ZWnewkeyboard(name,button[0],button[1],button[2],button[3],axisl[0],axisr[0],axisl[1],axisr[1]);
				free(button[0]); free(button[1]); free(button[2]); free(button[3]);
				free(axisl[0]);  free(axisr[0]);  free(axisl[1]);  free(axisr[1]);				
			}  
			ZWfreetag(first);
			first=ZWgetnexttag(maintag->inhalt);
		}
		ZWfreetag(maintag);
		ZWfinishloading(xmlpoint);
	}
}

float distance(float x1,float y1,float z1,float x2,float y2,float z2)
{
  float dx=x1-x2;
  float dy=y1-y2;
  float dz=z1-z2;
  return sqrt(dx*dx+dy*dy+dz*dz);
}
