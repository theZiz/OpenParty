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
int play_minigame(char* name,char showinfo,pgameresult gameresult,int practice)
{
  //Minispiel binden/laden
    char buffer[256];
    #ifdef WIN32
    sprintf(buffer,DATAFOLDER"games/bin/%s.dll",name);
    HINSTANCE handle=LoadLibrary(buffer);
    #else
    sprintf(buffer,DATAFOLDER"games/bin/%s.so",name);
    void *handle=dlopen(buffer,RTLD_LAZY);    
    if (handle==NULL)
    {
      printf("Fehler beim Laden von %s\n",buffer);
      return 1;
    }
    #endif
    
    void (*get_gameinfos)(pminigameinfo gameinfo);
    int (*get_gamedata_size)(void);
    void (*init_game)(void*,pgameinfo);
    int (*calc_game)(void*);
    int (*calc_game_thread)(void*);
    void (*draw_game)(void*);
    void (*reset_game)(void*,pgameresult);
    
    #ifdef WIN32
    get_gameinfos = (void (*)(pminigameinfo))GetProcAddress(handle, "op_get_gameinfos");    
    get_gamedata_size = (int (*)(void))GetProcAddress(handle, "op_get_gamedata_size");
    init_game = (void (*)(void*,pgameinfo))GetProcAddress(handle, "op_init_game");
    calc_game = (int (*)(void*))GetProcAddress(handle, "op_calc_game");
    calc_game_thread = (int (*)(void*))GetProcAddress(handle, "op_calc_game_thread");
    draw_game = (void (*)(void*))GetProcAddress(handle, "op_draw_game");
    reset_game = (void (*)(void*,pgameresult))GetProcAddress(handle, "op_reset_game");    
    #else
    get_gameinfos = (void (*)(pminigameinfo))dlsym(handle, "op_get_gameinfos");
    get_gamedata_size = (int (*)(void))dlsym(handle, "op_get_gamedata_size");
    init_game = (void (*)(void*,pgameinfo))dlsym(handle, "op_init_game");
    calc_game = (int (*)(void*))dlsym(handle, "op_calc_game");
    calc_game_thread = (int (*)(void*))dlsym(handle, "op_calc_game_thread");
    draw_game = (void (*)(void*))dlsym(handle, "op_draw_game");
    reset_game = (void (*)(void*,pgameresult))dlsym(handle, "op_reset_game");    
    #endif
    void* gamedata=malloc(get_gamedata_size());
    
  //Infos einholen
    tminigameinfo minigameinfo;
    get_gameinfos(&minigameinfo);
    if (showinfo)
    {
      printf("Playernum: | 1er Teams | 2er Teams | 3er Teams | 4er Teams\n");
      printf("-----------+-----------+-----------+-----------+----------\n");
      for (int a=0;a<8;a++)
      {
        char c1,c2,c3,c4;
        if (minigameinfo.playable[a][0]==0) c1=' '; else c1='X';
        if (minigameinfo.playable[a][1]==0) c1=' '; else c2='X';
        if (minigameinfo.playable[a][2]==0) c1=' '; else c3='X';
        if (minigameinfo.playable[a][3]==0) c1=' '; else c4='X';
        printf("    %i      |     %c     |     %c     |     %c     |     %c    \n",a+1,c1,c2,c3,c4);
        if (a<7)
          printf("-----------+-----------+-----------+-----------+----------\n");
      }
    } 
  //Minispiel starten
    tgameinfo gameinfo;
    //Gameinfos setzen:
      //Spiel
        gameinfo.playernum=playernum;
        for (int a=0;a<8;a++)
        {
          gameinfo.ball[a]=maindata.player[a].ball.listnr;
          gameinfo.color[a].r=maindata.player[a].color.r;
          gameinfo.color[a].g=maindata.player[a].color.g;
          gameinfo.color[a].b=maindata.player[a].color.b;
          gameinfo.color[a].a=maindata.player[a].color.a;
          gameinfo.money[a]=maindata.player[a].money;
          gameinfo.rubins[a]=maindata.player[a].rubins;
          gameinfo.facenr[a]=maindata.player[a].facenr;
          gameinfo.team[a]=maindata.player[a].team;
          gameinfo.iscpu[a]=maindata.player[a].iscpu;
        }
        gameinfo.teamsize=teamsize;
        gameinfo.steps=&(ZWattribute->steps);
        gameinfo.maintex=maintex;
        gameinfo.facetex=facetex;
        gameinfo.shouldend=&shouldend;
				gameinfo.sky_menu_tex=sky_menu_tex;
      //Text
        gameinfo.texttexture=text.texture;
        gameinfo.textwidth=text.breite;
      //Optionen
        gameinfo.texquali=texquali;
        gameinfo.fontquali=fontquali;
        gameinfo.lightquali=lightquali;
        gameinfo.windowx=windowx;
        gameinfo.windowy=windowy;
        gameinfo.resolutionx=resolutionx;
        gameinfo.resolutiony=resolutiony;
        gameinfo.fullscreen=fullscreen;
        gameinfo.stretched=stretched;
        gameinfo.volume=global_volume;
				gameinfo.datafolder=(char*)malloc(64);
				sprintf(gameinfo.datafolder,"%s",DATAFOLDER);
      //Joystick
        for (int a=0;a<8;a++)
        {
          gameinfo.joystick[a].button[0]=ZWattribute->joystick[a].button[0];
          gameinfo.joystick[a].button[1]=ZWattribute->joystick[a].button[1];
          gameinfo.joystick[a].button[2]=ZWattribute->joystick[a].button[2];
          gameinfo.joystick[a].button[3]=ZWattribute->joystick[a].button[3];
          gameinfo.joystick[a].axis[0]=ZWattribute->joystick[a].axis[0];
          gameinfo.joystick[a].axis[1]=ZWattribute->joystick[a].axis[1]; 
        }
        
    init_game(gamedata,&gameinfo);
    ZWattribute->newticks=SDL_GetTicks();
    ZWschleifeex(draw_game,calc_game_thread,calc_game,gamedata,get_gamedata_size());
  //Minspiel freigeben
    reset_game(gamedata,gameresult);
    if (showinfo)
    {
      printf("Gewonnen haben:\n");
      char one_vic=0;
      for (int a=0;a<playernum;a++)
        if  (gameresult->winner[a])
        {
          printf("Spieler %i hat gewonnen\n",a+1);
          one_vic=1;
        }
      if (one_vic==0)
        printf("Keiner\n");
    }
		free(gameinfo.datafolder);
    free(gamedata);
    #ifdef WIN32
    FreeLibrary(handle);
    #else
    dlclose(handle);
    #endif
    return 0;
}
