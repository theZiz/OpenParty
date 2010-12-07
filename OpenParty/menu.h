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
void draw_menu(pmenudata data)
{
  draw_ballon_background(&menudata);
  
  glLoadIdentity();
  
  //Menubeschriftung:
  float zoom[6];
  int a;
  for (a=0;a<6;a++)
    zoom[a]=1.0;  
  for (a=0;a<6;a++)
    if (data->menu_choose>((float)a)-1.0 && data->menu_choose<((float)a)+1.0)
      zoom[a]=1.0+cos((data->menu_choose-((float)a))*M_PI/2.0)/2.5f;
  
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);
  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);

  glColor4f(0.7,0.7,0.7,0.7);        
	ZWdrawtextmiddle(text,0.0,-2.19,-4,(char*)"Tipp: Drücke F1 für Hilfe zum Keyboardmapping!",0.18);
  glColor4f(COLOR_BASE+sin((data->rotation-40.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/45.0)*COLOR_MULT,1);
	ZWdrawtextmiddle(text,0.01 ,-2.2 ,-4,(char*)"Tipp: Drücke F1 für Hilfe zum Keyboardmapping!",0.18);

  glColor4f(0.7,0.7,0.7,0.7);
  ZWdrawtextmiddle(text,-1.13, 0.53,-4,(char*)"Partytime",0.5*zoom[0]);
  ZWdrawtextmiddle(text,-0.88, 0.08,-4,(char*)"Minispiele",0.5*zoom[1]);
  ZWdrawtextmiddle(text,-0.63,-0.37,-4,(char*)"Spielfelder",0.5*zoom[2]);
  ZWdrawtextmiddle(text,-0.38,-0.82,-4,(char*)"Optionen",0.5*zoom[3]);
  ZWdrawtextmiddle(text,-0.13,-1.27,-4,(char*)"Über",0.5*zoom[4]);
  ZWdrawtextmiddle(text, 0.12,-1.72,-4,(char*)"Ende",0.5*zoom[5]);
  glColor4f(COLOR_BASE+sin((data->rotation-00.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-00.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-00.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.1 , 0.5 ,-4,(char*)"Partytime",0.5*zoom[0]);
  glColor4f(COLOR_BASE+sin((data->rotation-09.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-09.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-09.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-0.85, 0.05,-4,(char*)"Minispiele",0.5*zoom[1]);
/*  glColor4f(COLOR_BASE+sin((data->rotation-18.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-18.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-18.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-0.6 ,-0.4,-4,(char*)"Spielfelder",0.5*zoom[2]);*/
  glColor4f(COLOR_BASE+sin((data->rotation-27.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-27.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-27.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-0.35,-0.85,-4,(char*)"Optionen",0.5*zoom[3]);
  glColor4f(COLOR_BASE+sin((data->rotation-36.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-36.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-36.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-0.1 ,-1.3 ,-4,(char*)"Über",0.5*zoom[4]);
  glColor4f(COLOR_BASE+sin((data->rotation-45.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-45.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-45.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text, 0.15,-1.75,-4,(char*)"Ende",0.5*zoom[5]);

  if (firsttimestarted)
    draw_gamestartinfo(&maindata,0,1.03);
  
  glDepthFunc(GL_LEQUAL);
}

int calc_menu(pmenudata data)
{
  //Rotation
  data->rotation+=(float)(ZWattribute->steps)/42.0;
  while (data->rotation>=360.0)
    data->rotation-=360.0;

  //Hüpfeball
  int a;
  for (a=0;a<ZWattribute->steps;a++)
  {
    data->ballmov+=0.0005;
    if (data->ballmov>1.0)
    {
      data->ballmov=0;
      //anderes Gesicht rausrechnen:
      int b;
      for (b=0;b<data->jumpball.pointnum;b++)
      {
        float dazu=((float)(data->facenr%4))*0.25;
        data->jumpball.p[b].u-=dazu;
        dazu=((float)(data->facenr/4))*0.25;
        data->jumpball.p[b].v-=dazu;    
      }
      data->facenr=rand()%MAX_FACES;
      data->facecolor=rand()%27;
      //neues Gesicht reinrechnen:
      for (b=0;b<data->jumpball.pointnum;b++)
      {
        float dazu=((float)(data->facenr%4))*0.25;
        data->jumpball.p[b].u+=dazu;
        dazu=((float)(data->facenr/4))*0.25;
        data->jumpball.p[b].v+=dazu;    
      }
      ZWrefreshdrawlist(&(data->jumpball),1);
    }
  }

  if (shouldend==1)
  {
    //später kann an dieser Stelle ein Menü kommen
    return 1;
  }

  //Fade  
  if (data->fade_dir==1)
  {
    data->fade+=(float)(ZWattribute->steps)/700.0;
    if (data->fade>=1.0)
    {
      data->fade=1.0;
      data->fade_dir=0;
    }
    return 0;
  }
  if (data->fade_dir==2)
  {
    data->fade-=(float)(ZWattribute->steps)/700.0;
    if (data->fade<=0.0)
    {
      float menu_delta=data->menu_delta;
      data->menu_delta=0;
      float menu_choose=data->menu_choose;
      data->menu_choose=0;
      data->fade=0;
      data->fade_dir=1;
      pstringchain temp;
      //tplayer tempplayer[8];
      switch (data->dest)
      {
        case 0:  ZWreset_virtuell_joytick();
                 //Login
                 memset(&(data->play_with[0]),255,8);
                 data->counter=OPENPARTY_WAITTIME;
                 ZWschleifeex((void(*)(void*))draw_newgame_login,(int(*)(void*))calc_newgame_login_thread,
                 (int(*)(void*))calc_newgame_login,(void*)(data),sizeof(tmenudata));
                 playernum=0;
                 for (a=0;a<8;a++)
                   if (data->play_with[a]!=-1)
                     playernum++;
                 //Kalibrieren
                 ZWreset_virtuell_joytick();
								 for (a=playernum;a<8;a++)
								   maindata.player[a].joystick_nr=-1;
								 //ohne CPU kalibrieren:
								 calib_all_controllers();
                 memset(&(data->calib_step[0]),0,8);
								 data->counter=0;
								 for (a=0;a<playernum;a++)
								 {
								   data->calib_step[a]=maindata.player[a].has_calib;
									 data->counter+=maindata.player[a].has_calib;
								 }
								 if (data->counter!=playernum) //nicht alle kalibriert
								 {
                   //data->counter=OPENPARTY_WAITTIME2;
                   ZWschleifeex((void(*)(void*))draw_newgame_calib,(int(*)(void*))calc_newgame_calib_thread,
								   (int(*)(void*))calc_newgame_calib,(void*)(data),sizeof(tmenudata));
								 }
                 //Farb- und Gesichtswahl
                 memset(&(data->color_choose[0]),0,8);
                 memset(&(data->blocked_color[0]),0,27);
                 memset(&(data->choose_step[0]),0,8);
                 for (int a=0;a<8;a++)
                 {
                   maindata.player[a].facenr=0;
                   ZWload3dm(&(maindata.player[a].ball),(char*)DATAFOLDER"models/ball.3dm",facetex);
                   ZWcreatedrawlist(&(maindata.player[a].ball),lightquali);
                 }
                 ZWschleifeex((void(*)(void*))draw_newgame_choose,(int(*)(void*))calc_newgame_choose_thread,
                 (int(*)(void*))calc_newgame_choose,(void*)(data),sizeof(tmenudata));
                 //Spielartauswahl
								 data->cancel_all=0;
                 for (int a=0;a<8;a++)
                 {
                   data->x[a]=0.5+((float)a)*0.1;
                   data->y[a]=1.0-((float)a)*0.4;
                 }
                 memset(&(data->choose_step[0]),0,8);
                 ZWschleifeex((void(*)(void*))draw_newgame_gamekind,(int(*)(void*))calc_newgame_gamekind_thread,
                 (int(*)(void*))calc_newgame_gamekind,(void*)(data),sizeof(tmenudata));
								 if (data->cancel_all)
								   break;
                 /*//Sprachauswahl
                 //Standardsprache suchen und auswählen:
                 temp=possiblelanguages;
                 while (temp!=NULL)
                 {
                   if (strcmp(temp->string,standardlanguage)==0)
                     break;
                   temp=temp->next;
                 }
                 for (int a=0;a<3;a++)
                   for (int b=0;b<8;b++)
                     data->lan_choose[a][b]=temp;
                 memset(&(data->choose_step[0]),0,8);
                 ZWschleifeex((void(*)(void*))draw_newgame_language,(int(*)(void*))calc_newgame_language_thread,
                 (int(*)(void*))calc_newgame_language,(void*)(data),sizeof(tmenudata));*/
                 //CPU-Gegner-Auswahl
                 for (int a=0;a<8;a++)
                 {
                   data->x[a]=0.5+((float)a)*0.1;
                   data->y[a]=1.0-((float)a)*0.4;
                 }
                 memset(&(data->choose_step[0]),0,8);
                 for (int a=0;a<8;a++)
                 {
                   if (playernum+a>8 || (a+playernum)%teamsize!=0 || (a+playernum)/teamsize<2)
                   {
                     data->blocked_field[a]=1;
                     continue;                   
                   }
                   data->blocked_field[a]=0;
                 }
                 //Farben der Spieler blockieren:
                 memset(&(data->blocked_color[0]),0,27);
                 for (int a=0;a<playernum;a++)
                 {
                   int c;
                   int tr=(int)(maindata.player[a].color.r*2.0);
                   int tg=(int)(maindata.player[a].color.g*2.0);
                   int tb=(int)(maindata.player[a].color.b*2.0);
                   for (c=0;c<27;c++)
                   {
                     int br=c%3;
                     int bg=(c/3)%3;
                     int bb=(c/9)%3;
                     if (tr==br && tg==bg && (bb-tb)*(bb-tb)<=1 && (bb+tb>1 || (tr==2 && tg==2) || bb==tb))
                       data->blocked_color[c]=1;      
                     if (tg==bg && tb==bb && (br-tr)*(br-tr)<=1 && (br+tr>1 || (tb==2 && tg==2) || br==tr))
                       data->blocked_color[c]=1;      
                     if (tb==bb && tr==br && (bg-tg)*(bg-tg)<=1 && (bg+tg>1 || (tr==2 && tb==2) || bg==tg))
                       data->blocked_color[c]=1;
                   }
                 }
                 //Computerspieler auswählen und blokieren
                 for (int a=playernum;a<8;a++)
                 {
                   int tr,tg,tb;
                   do
                   {
                     tr=rand()%3;
                     tg=rand()%3;
                     tb=rand()%3;
                   }
                   while (data->blocked_color[tr+tg*3+tb*9]==1);
                   for (int c=0;c<27;c++)
                   {
                     int br=c%3;
                     int bg=(c/3)%3;
                     int bb=(c/9)%3;
                     if (tr==br && tg==bg && (bb-tb)*(bb-tb)<=1)
                       data->blocked_color[c]=1;      
                     if (tg==bg && tb==bb && (br-tr)*(br-tr)<=1)
                       data->blocked_color[c]=1;      
                     if (tb==bb && tr==br && (bg-tg)*(bg-tg)<=1)
                       data->blocked_color[c]=1;
                   }
                   maindata.player[a].color.r=float(tr)*0.5;
                   maindata.player[a].color.g=float(tg)*0.5;
                   maindata.player[a].color.b=float(tb)*0.5;
                 }
                 //Gesicht zufällig setzen:
                 for (int a=playernum;a<8;a++)
                 {
                   int passt=0;
                   int facenr;
                   while (passt==0)
                   {
                     passt=1;
                     facenr=rand()%MAX_FACES;
                     for (int b=0;b<a;b++)
                       if (maindata.player[b].facenr==facenr)
                         passt=0;
                   }
                   maindata.player[a].facenr=facenr;
                 }
                 ZWschleifeex((void(*)(void*))draw_newgame_cpu,(int(*)(void*))calc_newgame_cpu_thread,
                 (int(*)(void*))calc_newgame_cpu,(void*)(data),sizeof(tmenudata));
								 if (data->cancel_all)
								   break;
                 //Teamauswahl:
                 if (teamsize==1)
                   for (int a=0;a<playernum+cpuplayer;a++)
                     maindata.player[a].team=a;
                 else
                 if (teamsize==2 && playernum+cpuplayer==8)
                 {
                    for (int a=0;a<playernum+cpuplayer;a++)
                    {
                      data->x[a]=0.5+((float)a)*0.1;
                      data->y[a]=1.0-((float)a)*0.4;
                    }                   
                    for (int a=0;a<playernum+cpuplayer;a++)
                      maindata.player[a].team=-1;
                    memset(&(data->choose_step[0]),0,8);
                    ZWschleifeex((void(*)(void*))draw_newgame_team_4,(int(*)(void*))calc_newgame_team_4_thread,
                    (int(*)(void*))calc_newgame_team_4,(void*)(data),sizeof(tmenudata));
                    if (data->cancel_all)
                      break;
                 }
                 else
                 if (teamsize==2 && playernum+cpuplayer==6)
                 {
                    for (int a=0;a<playernum;a++)
                    {
                      data->x[a]=0.5+((float)a)*0.1;
                      data->y[a]=1.0-((float)a)*0.4;
                    }                   
                    for (int a=0;a<playernum;a++)
                      maindata.player[a].team=-1;
                    memset(&(data->choose_step[0]),0,8);
                    ZWschleifeex((void(*)(void*))draw_newgame_team_3,(int(*)(void*))calc_newgame_team_3_thread,
                    (int(*)(void*))calc_newgame_team_3,(void*)(data),sizeof(tmenudata));
                    if (data->cancel_all)
                      break;
                 }
                 else
                 {
                   for (int a=0;a<playernum;a++)
                   {
                     data->x[a]=0.5+((float)a)*0.1;
                     data->y[a]=1.0-((float)a)*0.4;
                   }                   
                   for (int a=0;a<playernum;a++)
                     maindata.player[a].team=-1;
                   memset(&(data->choose_step[0]),0,8);
                   ZWschleifeex((void(*)(void*))draw_newgame_team_2,(int(*)(void*))calc_newgame_team_2_thread,
                   (int(*)(void*))calc_newgame_team_2,(void*)(data),sizeof(tmenudata));
                    if (data->cancel_all)
                      break;
                 }
                 //Spielziel/Runden-auswahl
                 for (int a=0;a<playernum;a++)
                 {
                   data->x[a]=0.5+((float)a)*0.1;
                   data->y[a]=1.0-((float)a)*0.4;
                 }
                 memset(&(data->choose_step[0]),0,8);
                 ZWschleifeex((void(*)(void*))draw_newgame_rounds,(int(*)(void*))calc_newgame_rounds_thread,
                 (int(*)(void*))calc_newgame_rounds,(void*)(data),sizeof(tmenudata));
                  if (data->cancel_all)
                    break;                 
                 //Ziehreihenfolge:
                 memset(&(data->choose_step[0]),0,8);
                 memset(&(data->blocked_field[0]),0,8);
                 memset(&(data->color_choose[0]),-1,8);
                 memset(&(data->calib_step[0]),-1,8); //jetzt mal für das Los der Spieler
                 for (int a=0;a<playernum+cpuplayer;a++)
                 {
                   int los;
                   do
                   {
                     los=rand()%(playernum+cpuplayer);
                   }
                   while (data->color_choose[los]!=-1);
                   data->color_choose[los]=a; //hier drin werden nun die Zufallszahlen gespeichert
                 }
                 for (int a=0;a<playernum+cpuplayer;a++)
                 {
                   data->x[a]=0.5+((float)a)*0.1;
                   data->y[a]=1.0-((float)a)*0.4;
                 }
                 ZWschleifeex((void(*)(void*))draw_newgame_turnsort,(int(*)(void*))calc_newgame_turnsort_thread,
                 (int(*)(void*))calc_newgame_turnsort,(void*)(data),sizeof(tmenudata));
                 //Umsortieren, wie das Ergebnis des Losverfahrens ist:
                 for (int a=0;a<playernum;a++)
                   maindata.player[a].iscpu=0;
                 for (int a=playernum;a<playernum+cpuplayer;a++)
                 {
                   maindata.player[a].iscpu=1;
                   ZWload3dm(&(maindata.player[a].ball),(char*)DATAFOLDER"models/ball.3dm",facetex);
                   int b;
                   for (b=0;b<maindata.player[a].ball.pointnum;b++)
                   {
                     float dazu=((float)(maindata.player[a].facenr%4))*0.25;
                     maindata.player[a].ball.p[b].u+=dazu;
                     dazu=((float)(maindata.player[a].facenr/4))*0.25;
                     maindata.player[a].ball.p[b].v+=dazu;    
                   }
                   ZWcreatedrawlist(&(maindata.player[a].ball),lightquali);
                 }
                 playernum+=cpuplayer;
                 for (int a=0;a<playernum;a++)
                   for (int b=a+1;b<playernum;b++)
                     if (data->color_choose[data->calib_step[a]]>data->color_choose[data->calib_step[b]])
                     {
                       //Sortierung tauschen
                       char c=data->calib_step[a];
                       data->calib_step[a]=data->calib_step[b];
                       data->calib_step[b]=c;
                       
                       //Spielerdaten tauschen
                       tplayer temp=maindata.player[a];
                       maindata.player[a]=maindata.player[b];
                       maindata.player[b]=temp;
                       
                       //Joystick tauschen
                       tZWjoystick joy=ZWattribute->joystick[a];
                       ZWattribute->joystick[a]=ZWattribute->joystick[b];
                       ZWattribute->joystick[b]=joy;
                     }
								 //mit CPU kalibrieren:
								 calib_all_controllers();
                     
                 loadlevel((char*)"forest.xml");
                 refreshsetlist();
                 prepare_game(getsetbyname((char*)"start"),0);
                 Mix_FadeInMusic(backgroundmusic, -1,FADE_IN_TIME);
                 ZWattribute->newticks=SDL_GetTicks();
                 ZWschleifeex((void(*)(void*))draw_main,(int(*)(void*))calc_main_thread,(int(*)(void*))calc_main,
                 (void*)(&maindata),sizeof(tmaindata));
                 Mix_FadeInMusic(snd_menumusic, -1,FADE_IN_TIME); 
                 //vorläufiges Endergebnis
                 memset(&(data->choose_step[0]),0,8);
								 for (int a=0;a<playernum;a++)
								   if (maindata.player[a].iscpu)
									   data->choose_step[a]=1;
								 data->fade=0;
								 data->fade_dir=1;
                 reset_after_game();
                 ZWschleifeex((void(*)(void*))draw_aftergame,(int(*)(void*))calc_aftergame_thread,
                 (int(*)(void*))calc_aftergame,(void*)(data),sizeof(tmenudata));
        break;
        case 1:  choosen_mg=0;
                 playernum=0;
                 before_minigames_choose();
                 ZWschleifeex((void(*)(void*))draw_minigames,(int(*)(void*))calc_minigames_thread,(int(*)(void*))calc_minigames,
                 (void*)(data),sizeof(tmenudata));
                 after_minigames_choose();
        break;
        case 2:
        break;
        case 3:  data->need_reboot=0;
                 ZWschleifeex((void(*)(void*))draw_options,(int(*)(void*))calc_options_thread,(int(*)(void*))calc_options,
                 (void*)(data),sizeof(tmenudata));
        break;
        case 4:  ZWschleifeex((void(*)(void*))draw_about,(int(*)(void*))calc_about_thread,(int(*)(void*))calc_about,
                 (void*)(data),sizeof(tmenudata));
        break;
        case 5: shouldend=1; break;
      }
      data->menu_delta=menu_delta;
      data->menu_choose=menu_choose;
      data->fade=0.0;
      data->fade_dir=1;
    }
    return 0;
  }
  return 0;
}

int calc_menu_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;

  if (firsttimestarted)
  {
    if (is_any_button_down(1))
    {
      firsttimestarted=0;
      save_options();
    }
    return 0;
  }

  //Menuauswahl
  if (is_any_axis_down(0) && data->menu_delta==0)
  {
    data->old_choose=data->menu_choose;
    data->menu_delta=0.001;
  }
  if (is_any_axis_up(0) && data->menu_delta==0)
  {
    data->old_choose=data->menu_choose;
    data->menu_delta=-0.001;
  }
  if (data->menu_delta!=0.0)
  {
    if (data->menu_delta<0.0)
    {
      //Nach oben werden Bewegungen gleich im Keim erstickt
      if (data->menu_choose<=0.0)
      {
        data->menu_choose=0.0;
        data->menu_delta=0.0;
      }
      else
      {
        data->menu_delta-=(float)(ZWattribute->steps)/150.0;
        data->menu_choose=data->old_choose+data->menu_delta;
        if (data->menu_delta<=-1.0)
        {
          data->menu_choose=trunc(data->old_choose)-1.0;
          data->menu_delta=0.0;
        }
      }
    }
    else
    {
      //Nach oben werden Bewegungen gleich im Keim erstickt
      if (data->menu_choose>=5.0)
      {
        data->menu_choose=5.0;
        data->menu_delta=0.0;
      }
      else
      {
        data->menu_delta+=(float)(ZWattribute->steps)/150.0;
        data->menu_choose=data->old_choose+data->menu_delta;
        if (data->menu_delta>=1.0)
        {
          data->menu_choose=trunc(data->old_choose)+1.0;
          data->menu_delta=0.0;
        }
      }
    }
  }
  //Menuauswahl die zweite
  if (is_any_button_down(1) && data->menu_delta==0)
  {
    Mix_PlayChannel(-1, snd_plop, 0);
    data->dest=(int)(trunc(data->menu_choose));
    data->fade_dir=2;
  }
  return 0;
}

