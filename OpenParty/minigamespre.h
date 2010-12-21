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
typedef struct sprepare_data *pprepare_data;
typedef struct sprepare_data {
  int step; //0 Fade In, 1 Anzeigen, 2 Fade Out
  float step_float;
  float rotation;
  GLuint thumb;
  pmaindata maindata;
  char player_status[8];
  int result;
} tprepare_data;

void draw_prepare_minigame(pprepare_data data)
{
  //Hintergrund
  glColor4f(1,1,1,1);
  glTranslatef(0,0,-13);
  float dazu=(data->rotation)/180.0;
  dazu=dazu-trunc(dazu);
  ZWusetexture(sky_menu_tex);
  glBegin(GL_QUADS);
    glTexCoord2f(0+dazu,0);glVertex3f(-10,10,0);
    glTexCoord2f(0+dazu,1);glVertex3f(-10,-10,0);
    glTexCoord2f(1+dazu,1);glVertex3f(10,-10,0);
    glTexCoord2f(1+dazu,0);glVertex3f(10,10,0);  
  glEnd();

  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);
  
  glTranslatef( 1.2,0.6,1);
  ZWusetexture(data->thumb);
  glBegin(GL_QUADS);
    glTexCoord2f(0.01,0.01);glVertex3f(-1, 1,-4);
    glTexCoord2f(0.01,0.99);glVertex3f(-1,-1,-4);
    glTexCoord2f(0.99,0.99);glVertex3f( 1,-1,-4);
    glTexCoord2f(0.99,0.01);glVertex3f( 1, 1,-4);  
  glEnd();

  glLoadIdentity();
  glColor4f(0,0,0,0.5);
  if (strcmp(language,(char*)"german")==0)
  {
    ZWdrawtext(text,-2.9,1.9,-4,(char*)"[A] drücken fürs Spielen",0.25);
    ZWdrawtext(text,-2.9,1.7,-4,(char*)"[B] drücken fürs Üben",0.25);
  }
  else
  {
    ZWdrawtext(text,-2.9,1.9,-4,(char*)"Press [A] for Playing",0.25);
    ZWdrawtext(text,-2.9,1.7,-4,(char*)"Press [B] for Training",0.25);
  }
  
  
  float temp=ZWgettexturepercent();
  ZWsettexturepercent(0.005);
  for (int a=0;a<4;a++)
    for (int b=0;b<2;b++)
    {
      if (a+4*b>=playernum)
        break;
      glColor4f(data->maindata->player[a+b*4].color.r,data->maindata->player[a+b*4].color.g,
                data->maindata->player[a+b*4].color.b,data->maindata->player[a+b*4].color.a);
      ZWdrawsprite(maintex,-2.6+(float)a*0.8,1.2-(float)b*1.1,-4,0.35,0.35,2,2);
      glColor4f(1,1,1,1);
      ZWdrawsprite(facetex,-2.6+(float)a*0.8,1.2-(float)b*1.1,-4,0.35/sqrt(2),0.35/sqrt(2),4,data->maindata->player[a+4*b].facenr);
      glColor4f(0,0,0,0.5);
      if (strcmp(language,(char*)"german")==0)
        switch (data->player_status[a+4*b])
        {
          case 1: ZWdrawtextmiddle(text,-2.6+(float)a*0.8,0.65-(float)b*1.1,-4,(char*)"Üben",0.25); break;
          case 2: ZWdrawtextmiddle(text,-2.6+(float)a*0.8,0.65-(float)b*1.1,-4,(char*)"Spielen",0.25); break;
        }
      else
        switch (data->player_status[a+4*b])
        {
          case 1: ZWdrawtextmiddle(text,-2.6+(float)a*0.8,0.65-(float)b*1.1,-4,(char*)"Train",0.25); break;
          case 2: ZWdrawtextmiddle(text,-2.6+(float)a*0.8,0.65-(float)b*1.1,-4,(char*)"Play",0.25); break;
        }
      
    }
  ZWsettexturepercent(temp);  
  
  glLoadIdentity();
  drawtextsite();
  if (data->step!=1)
  {
    glDisable(GL_TEXTURE_2D);
    glColor4f(0,0,0,data->step_float);
    glBegin(GL_QUADS);
      glVertex3f(-1,1,-1);
      glVertex3f(-1,-1,-1);
      glVertex3f(1,-1,-1);
      glVertex3f(1,1,-1);      
    glEnd();
    glEnable(GL_TEXTURE_2D);
  }
  glDepthFunc(GL_LEQUAL);
}

int calc_prepare_minigame(pprepare_data data)
{
  if (shouldend)
    return 1;
  if (data->step==0) //Fade In
  {
    data->step_float-=(float)(ZWattribute->steps)/500.0;
    if (data->step_float<=0.0)
    {
      data->step_float=0.0;
      data->step=1;
    }
    return 0;
  }
  if (data->step==2) //Fade Out
  {
    data->step_float+=(float)(ZWattribute->steps)/500.0;
    if (data->step_float>=1.0)
    {
      data->step_float=1.0;
      return data->result;
    }
    return 0;
  }
  
  int meinung=0;
  int practice=0;
  for (int a=0;a<playernum;a++)
  {
    if (*(ZWattribute->joystick[a].button[0]))
      data->player_status[a]=2;
    if (*(ZWattribute->joystick[a].button[1]))
      data->player_status[a]=1;
    if (*(ZWattribute->joystick[a].button[2]) || *(ZWattribute->joystick[a].button[3]))
      data->player_status[a]=0;
    if (data->player_status[a]==1)
    {
      meinung++;
      practice=1;
    }
    if (data->player_status[a]==2)
      meinung++;
  }
  if (meinung==playernum)
  {
    if (practice)
      data->result=2;
    else
      data->result=1;
    data->step=2;
    Mix_FadeOutMusic(500);
  }
  
  
  return 0;
}

int calc_prepare_minigame_thread(pprepare_data data)
{
  handle_textcontent(-1);
  //Rotation
  data->rotation+=(float)(ZWattribute->steps)/42.0;
  while (data->rotation>=360.0)
    data->rotation-=360.0;
  return 0;
}

int prepare_minigame(char* minigame,pmaindata maindata)
{
  tprepare_data data;
  //Init
    data.step=0;
    data.step_float=1;  
    data.rotation=0;
    char buffer[256];
    //Text einlesen:
      char content[65536]="no text";
      sprintf(buffer,DATAFOLDER"games/description/%s.txt",minigame);
      SDL_RWops *rw=SDL_RWFromFile(buffer,"rb");
      planguage description=NULL;
      if(rw!=NULL)
      {
        int anz=1;
        while (anz>0)
        {
          char c;
          anz=SDL_RWread(rw,&c,1,1);
          int pos=0;
          while (anz>0 && pos<65535 && c!='\n')
          {
            content[pos]=c;
            pos++;
            anz=SDL_RWread(rw,&c,1,1);
          }
          if (pos>0)
            content[pos]=0;
          planguage newlan=(planguage)malloc(sizeof(tlanguage));
          //Doppelpunkt suchen:
          int a;
          for (a=0;content[a]!=0;a++)
            if (content[a]==':')
              break;
          content[a]=0;
          newlan->language=(char*)malloc(strlen(content)+1);
          sprintf(newlan->language,"%s",content);
          newlan->content=(char*)malloc(strlen(&(content[a+1]))+1);
          sprintf(newlan->content,"%s",&(content[a+1]));
          newlan->next=description;
          description=newlan;
        }
        SDL_RWclose(rw);
      }
    //nach language suchen
    planguage momlan=description;
    planguage last=NULL;
    while (momlan!=NULL)
    {
      if (strcmp(momlan->language,language)==0) break;
      last=momlan;
      momlan=momlan->next;
    }
    if (momlan!=NULL)
      createtextcontent(momlan->content);
    else
      createtextcontent(last->content); 
    
    //Wieder freigeben:
    while (description!=NULL)
    {
      free(description->content);
      free(description->language);
      planguage next=description->next;
      free(description);
      description=next;
    }

    sprintf(buffer,DATAFOLDER"games/thumb/%s.png",minigame);
    data.thumb=ZWloadtexturefromfileA(buffer,0,texquali);
    data.maindata=maindata;
    for (int a=0;a<playernum;a++)
      if (maindata->player[a].iscpu)
        data.player_status[a]=2;
      else
        data.player_status[a]=0;
    
    Mix_FadeInMusic(snd_menumusic,-1,500);
    
  //Schleife
    ZWattribute->newticks=SDL_GetTicks();
    int result=ZWschleifeex((void (*)(void*))draw_prepare_minigame,(int (*)(void*))calc_prepare_minigame_thread,
                            (int (*)(void*))calc_prepare_minigame,&data,sizeof(tprepare_data));
  //Rückgabe
    ZWdeletetexture(data.thumb);
    deletetextcontent();
    return result;
}
