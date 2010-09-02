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

int mg_installed;
int mg_updateable;
int mg_not_installed;
float choosen_mg;
pminigame *minigamelist=NULL;

void prepare_minigames(void)
{
  mg_installed=0;
  mg_updateable=0;
  mg_not_installed=0;
  free(minigamelist);
  pminigame momgame=firstminigame;
  while (momgame!=NULL)
  {
    if (momgame->installed)
    {
      mg_installed++;
      //TODO: Oder Updatebar!
    }
    else
      mg_not_installed++;
    momgame=momgame->next; 
  }
  minigamelist=(pminigame*)(malloc(sizeof(pminigame)*(mg_installed+mg_not_installed+mg_updateable)));
  momgame=firstminigame;
  int a=0;
  while (momgame!=NULL)
  {
    minigamelist[a]=momgame;
    char buffer[256];
    sprintf(buffer,DATAFOLDER"games/thumb/%s.png",minigamelist[a]->filename);
    minigamelist[a]->preview=ZWloadtexturefromfileA(buffer,0,texquali*2);
    a++;
    momgame=momgame->next;
  }
  //Sortieren
  char sorted=0;
  while (!sorted)
  {
    sorted=1;
    for (a=0;a<mg_installed+mg_not_installed+mg_updateable-1;a++)
      if (strcmp(minigamelist[a]->name,minigamelist[a+1]->name)>0)
      {
        pminigame temp=minigamelist[a];
        minigamelist[a]=minigamelist[a+1];
        minigamelist[a+1]=temp;
        sorted=0;
        break;
      }
    
  }
}

void before_minigames_choose(void)
{
  Mix_VolumeChunk(snd_negative,(int)(round(24.0*global_volume)));
  hint_counter=0;
}

void after_minigames(void)
{
  int a;  
  for (a=0;a<mg_installed+mg_not_installed+mg_updateable;a++)
    ZWdeletetexture(minigamelist[a]->preview);
  free(minigamelist);
  minigamelist=NULL;  
}

void after_minigames_choose(void)
{
	Mix_VolumeChunk(snd_negative,(int)(round(8.0*global_volume)));	
}

void draw_minigames(pmenudata data)
{

  draw_ballon_background(data);
  
  //Menubeschriftung:
  float zoom[9];
  int a;
  for (a=0;a<9;a++)
    zoom[a]=1.0;  
  for (a=0;a<9;a++)
    if (data->menu_choose>((float)a)-1.0 && data->menu_choose<((float)a)+1.0)
      zoom[a]=1.0+cos((data->menu_choose-((float)a))*M_PI/2.0)/2.0;
  
  float sum=mg_installed+mg_not_installed+mg_updateable;
  for (a=0;a<mg_installed+mg_not_installed+mg_updateable;a++)
  {
    glLoadIdentity();
    glTranslatef(3.2+ballonx,0+ballony+(sqrt(1.0-(1.0-data->fade)*(1.0-data->fade))-1.0)*3.5+cos(data->rotation*M_PI/180.0)/2.0*(1.0-data->fade),-7+ballonz);
    glRotatef(25,1,0,0);
    float z=cos((float)(a-choosen_mg)*2.0*M_PI/sum);
    glTranslatef(sin(((float)a-choosen_mg)*2.0*M_PI/sum)*2.5*data->fade,0,z*2.5*data->fade);
    float zoom=(0.35+((z+1.0)/2.0)*((z+1.0)/2.0)*((z+1.0)/2.0)*((z+1.0)/2.0)*0.35)*data->fade;
    ZWusetexture(minigamelist[a]->preview);
    glRotatef(-25,1,0,0);
    glColor4f(0.4+(z+1.0)*0.3,0.5+(z+1.0)*0.25,(0.4+(z+1.0)*0.3)*0.7,1);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
      glVertex3f(-1.6*zoom,-2.35*zoom,0);
      glVertex3f( 1.6*zoom,-2.35*zoom,0);
      glVertex3f( 1.6*zoom, 1.6 *zoom,0);
      glVertex3f(-1.6*zoom, 1.6 *zoom,0);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor4f(0.4+(z+1.0)*0.3,0.4+(z+1.0)*0.3,0.4+(z+1.0)*0.3,1);
    glBegin(GL_QUADS);
      glTexCoord2f(0.01,0.99); glVertex3f(-1.5*zoom,-1.5*zoom,0);
      glTexCoord2f(0.99,0.99); glVertex3f( 1.5*zoom,-1.5*zoom,0);
      glTexCoord2f(0.99,0.01); glVertex3f( 1.5*zoom, 1.5*zoom,0);
      glTexCoord2f(0.01,0.01); glVertex3f(-1.5*zoom, 1.5*zoom,0);
    glEnd();
    //bisher nur installiert
    glColor4f(0.2,0.8,0.3,0.8);
    ZWdrawtext(text,-1.5*zoom,-1.77*zoom,0.01,(char*)"[ ]",0.35*zoom);
    glColor4f(0.2,0.2,0.2,0.8);
    ZWdrawtext(text,-1.1*zoom,-1.77*zoom,0.01,(char*)"Installiert",0.35*zoom);

    if (minigamelist[a]->ingame)
    {
      glColor4f(0.2,0.8,0.3,0.8);
      ZWdrawtext(text,-1.5*zoom,-2.15*zoom,0.01,(char*)"[ ]",0.35*zoom);
      glColor4f(0.2,0.2,0.2,0.8);
      ZWdrawtext(text,-1.1*zoom,-2.15*zoom,0.01,(char*)"wird gespielt",0.35*zoom);
    }
    else
    {
      glColor4f(0.8,0.2,0.3,0.8);
      ZWdrawtext(text,-1.5*zoom,-2.15*zoom,0.01,(char*)"[ ]",0.35*zoom);
      glColor4f(0.2,0.2,0.2,0.8);
      ZWdrawtext(text,-1.1*zoom,-2.15*zoom,0.01,(char*)"wird nicht gespielt",0.35*zoom);
    }
  }
    
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);
  
  glColor4f(0.2,0.2,0.2,0.9);
  ZWdrawtext(text,1.17-2.5,-3.67,-7,(char*)"[<]",0.8*data->fade*((left_axis_timeout>0)?1.0:0.8));
  ZWdrawtext(text,1.17+2.5,-3.67,-7,(char*)"[>]",0.8*data->fade*((right_axis_timeout>0)?1.0:0.8));
  glColor4f(1,1,1,0.9);
  ZWdrawtext(text,1.2 -2.5,-3.70,-7,(char*)"[<]",0.8*data->fade*((left_axis_timeout>0)?1.0:0.8));
  ZWdrawtext(text,1.2 +2.5,-3.70,-7,(char*)"[>]",0.8*data->fade*((right_axis_timeout>0)?1.0:0.8));
  
  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);
  
  
  glTranslatef(-0.25,0,0);
  glColor4f(0.7,0.7,0.7,0.7);
  char content[9][64];
  
  sprintf(content[0],"Spielen");
  sprintf(content[1],"[<]Alle Spiele[>]");
  sprintf(content[2],"Markieren");
  sprintf(content[3],"Alle markieren");
  sprintf(content[4],"Installieren/Aktualisieren");
  sprintf(content[5],"In OpenParty spielen");
  sprintf(content[6],"Löschen");
  sprintf(content[7],"Auf Updates prüfen");
  sprintf(content[8],"Zurück   ");

  float dist_x=0;//1.2/7.0;
  float dist_y=0.25;//2.4/7.0;
  float dist_a=40.0/7.0;
  float basex=-2.8;
  float basey=-0.2;
  float mainzoom=0.2;
  
  for (a=0;a<9;a++)
    ZWdrawtext(text,basex+(float)a*dist_x,basey-(float)a*dist_y,-4,content[a],mainzoom*zoom[a]);
  for (a=0;a<9;a++)
  {
    switch (a) {case 1: case 2: case 3: case 4: case 6: case 7: continue;}
    glColor4f(COLOR_BASE+sin((data->rotation-0.0*dist_a)*M_PI/180.0)*COLOR_MULT,
              COLOR_BASE+sin((data->rotation-0.0*dist_a)*M_PI/90.0)*COLOR_MULT,
              COLOR_BASE+sin((data->rotation-0.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
    ZWdrawtext(text,basex+0.01+(float)a*dist_x,basey-0.01-(float)a*dist_y,-4,content[a],mainzoom*zoom[a]);
  }
  
  glLoadIdentity();
  char buffer[256];
  
  glDisable(GL_TEXTURE_2D);
    glColor4f(1,1,1,0.7);
    glTranslatef(3.2+ballonx,1.25,-7);
    glBegin(GL_QUADS);
      glVertex3f(-1.7*data->fade,-0.9*data->fade,0);
      glVertex3f( 1.7*data->fade,-0.9*data->fade,0);
      glVertex3f( 1.7*data->fade, 0.9*data->fade,0);
      glVertex3f(-1.7*data->fade, 0.9*data->fade,0);
    glEnd();
  glEnable(GL_TEXTURE_2D);

  glLoadIdentity();
  
  glColor4f(0.2,0.2,0.2,0.9);
  sprintf(buffer,"Installierte Spiele: %i",mg_installed+mg_updateable);
  ZWdrawtextmiddle_fac(text,3.2+ballonx,1.25+0.55*data->fade,-7,buffer,0.4*data->fade,0.95);
  sprintf(buffer,"Verfügbare Spiele: %i",mg_installed+mg_updateable+mg_not_installed);
  ZWdrawtextmiddle_fac(text,3.2+ballonx,1.25+0.15*data->fade,-7,buffer,0.4*data->fade,0.95);
  sprintf(buffer,"Updates verfügbar: %i",mg_updateable);
  ZWdrawtextmiddle_fac(text,3.2+ballonx,1.25-0.25*data->fade,-7,buffer,0.4*data->fade,0.95);
  int count=0;
  for (a=0;a<mg_installed+mg_not_installed+mg_updateable;a++)
    if (minigamelist[a]->ingame)
      count++;
  sprintf(buffer,"Ingame verwendet: %i",count);
  ZWdrawtextmiddle_fac(text,3.2+ballonx,1.25-0.65*data->fade,-7,buffer,0.4*data->fade,0.95);
  
  if (data->fade_dir==1 && playernum!=0)
  {
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glColor4f(0,0,0,1.0-data->fade);
    glBegin(GL_QUADS);
      glVertex3f(-1,1,-1);
      glVertex3f(-1,-1,-1);
      glVertex3f(1,-1,-1);
      glVertex3f(1,1,-1);      
    glEnd();
    glEnable(GL_TEXTURE_2D);  
  }

  if (hint_counter>0)
  {  
		glColor4f(0.7,0.1,0.1,0.8);
		ZWdrawtextmiddle_fac(text,-3.5,1.5,-7,hint_text1,0.6*data->fade,0.95);
		ZWdrawtextmiddle_fac(text,-3.5,1.0,-7,hint_text2,0.6*data->fade,0.95);
		ZWdrawtextmiddle_fac(text,-3.5,0.5,-7,hint_text3,0.6*data->fade,0.95);
	}
  
  
  
  glDepthFunc(GL_LEQUAL);   
}

int calc_minigames(pmenudata data)
{
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
      ZWrefreshdrawlist(&(data->jumpball),lightquali);
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
      playernum=0;
    }
    ballonx=cos(data->fade*M_PI)-1.0;
    return 0;
  }
  if (data->fade_dir==2)
  {
    data->fade-=(float)(ZWattribute->steps)/700.0;
    if (data->fade<=0.0)
    {
      data->fade=0.0;
      data->fade_dir=1;
      ballonx=0;
      int result;
      switch (data->dest)
      {
        case 0:  ZWreset_virtuell_joytick();
                 //Login
                 memset(&(data->play_with[0]),255,8);
                 memset(&(data->choose_step[0]),0,8);
                 data->counter=OPENPARTY_WAITTIME;
                 playernum=0;
                 ZWschleifeex((void(*)(void*))draw_newgame_login_new,(int(*)(void*))calc_newgame_login_thread_new,
                 (int(*)(void*))calc_newgame_login_new,(void*)(data),sizeof(tmenudata)); 
                 data->fade_dir=1;
                 SDL_Delay(2000);
								 for (int a=playernum;a<8;a++)
								   maindata.player[a].joystick_nr=-1;
                 calib_all_controllers();
                 for (int a=0;a<8;a++)
                 {
                   maindata.player[a].color.r=a%2;
                   maindata.player[a].color.g=(a/2)%2;
                   maindata.player[a].color.b=(a/4)%2;
                   maindata.player[a].color.a=1;
                   maindata.player[a].facenr=rand()%15;
                   maindata.player[a].team=a;
                   maindata.player[a].iscpu=(a<playernum)?0:1;
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
                 if (playernum!=8)
                   playernum=playernum+rand()%(8-playernum)+1;
                 result=0;
                 tgameresult gameresult;
                 while (result!=1 && (shouldend==0))
                 {
                   for (int player=0;player<playernum;player++)
                   {
                     if (!(maindata.player[player].iscpu))
                       continue;
                     for (int a=0;a<4;a++)
                       *(ZWattribute->joystick[player].button[a])=0;
                     for (int a=0;a<2;a++)
                      *(ZWattribute->joystick[player].axis[a])=0;
                   }
                   //Minispielvorauswahl
                     result=prepare_minigame(minigamelist[(int)(trunc(choosen_mg))]->filename,&maindata);
                   //Minispiel starten
                     play_minigame(minigamelist[(int)(trunc(choosen_mg))]->filename,0,&gameresult,result-1);
                 }   
                 //Minispielergebnis
                 after_minigame(&maindata,&gameresult);
                 Mix_FadeInMusic(backgroundmusic,-1,FADE_IN_TIME);
                 
                 for (int a=0;a<playernum;a++)
                 {
                   ZWdeletedrawlist(maindata.player[a].ball); 
                   ZWdeleteobject(maindata.player[a].ball);
                 }
                 
                 break;
        case 8:return 1;
      }
    }
    ballonx=cos(data->fade*M_PI)-1.0;
    return 0;
  }

  //Menuauswahl
  for (a=0;a<ZWattribute->steps;a++)
  {
		if (hint_counter>0)
		  hint_counter--;
    is_any_axis_left_wait();
    is_any_axis_right_wait();
    if (left_axis_timeout>0)
    {
      float delta=choosen_mg-trunc(choosen_mg);
      if (delta<=0.0)
        delta=1.0;
      choosen_mg=choosen_mg-delta/(float)(left_axis_timeout);
      if (choosen_mg<0.0)
        choosen_mg+=(float)(mg_installed+mg_not_installed+mg_updateable);
    }
    if (right_axis_timeout>0)
    {
      float delta=1.0-choosen_mg+trunc(choosen_mg);
      choosen_mg=choosen_mg+delta/(float)(right_axis_timeout);
      if (choosen_mg>=(float)(mg_installed+mg_not_installed+mg_updateable))
        choosen_mg-=(float)(mg_installed+mg_not_installed+mg_updateable);
    }
  }
  return 0;
}

int calc_minigames_thread(pmenudata data)
{
  //Rotation
  data->rotation+=(float)(ZWattribute->steps)/42.0;
  while (data->rotation>=360.0)
    data->rotation-=360.0;

  if (data->fade_dir!=0) return 0;

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
      if (data->menu_choose>=8.0)
      {
        data->menu_choose=8.0;
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
    data->dest=8;
    int count;
    int a;
    switch ((int)(trunc(data->menu_choose)))
    {
      case 0: Mix_PlayChannel(-1, snd_plop, 0);
              data->dest=(int)(trunc(data->menu_choose));
              data->fade_dir=2;
      case 2: break;
      case 3: break;
      case 4: break;
      case 5: count=0;
							for (a=0;a<mg_installed+mg_not_installed+mg_updateable;a++)
								if (minigamelist[a]->ingame)
									count++;
              if (count>3 || minigamelist[(int)(trunc(choosen_mg))]->ingame==0)
              {
								minigamelist[(int)(trunc(choosen_mg))]->ingame=1-minigamelist[(int)(trunc(choosen_mg))]->ingame;
								Mix_PlayChannel(-1, snd_plop, 0);
								save_minigames(); 
							}
							else
							{
								Mix_PlayChannel(-1, snd_negative, 0);
								hint_counter=3000;
								if (strcmp(language,"german")==0)
								{
									sprintf(hint_text1,"Mindestens 3");
									sprintf(hint_text2,"Minispiele");
									sprintf(hint_text3,"notwendig!");
								}
								else
								{
									sprintf(hint_text1,"At leat 3");
									sprintf(hint_text2,"mini games");
									sprintf(hint_text3,"necessary!");
								}
							}
              break;
      case 6: break;
      case 7: break;
      case 8: Mix_PlayChannel(-1, snd_plop, 0);
              data->dest=(int)(trunc(data->menu_choose));
              data->fade_dir=2;
              break;
    }
  }
  
  return 0;
}

