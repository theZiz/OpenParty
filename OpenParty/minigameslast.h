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
typedef struct safter_data *pafter_data;
typedef struct safter_data {
  int step; //0 Fade In, 1 Anzeigen, 2 Fade Out
  float step_float;
  float rotation;
  Sint32 countdown;
  pmaindata maindata;
  pgameresult gameresult;
} tafter_data;

void draw_after_minigame(pafter_data data)
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

  glColor4f(1,0.8,0,0.7);
  
  if (strcmp(language,(char*)"german")==0)
    ZWdrawtextmiddle(text,0,1.7,-4,(char*)"Ergebnis",1.0);  
  else
    ZWdrawtextmiddle(text,0,1.7,-4,(char*)"Result",1.0);    

  float temp=ZWgettexturepercent();
  ZWsettexturepercent(0.005);
  
  for (int a=0;a<4;a++)
    for (int b=0;b<2;b++)
    {
      if (a+4*b>=playernum)
        break;
      glColor4f(data->maindata->player[a+b*4].color.r,data->maindata->player[a+b*4].color.g,
                data->maindata->player[a+b*4].color.b,data->maindata->player[a+b*4].color.a);
      ZWdrawsprite(maintex,-2.2+(float)a*1.5,0.8-(float)b*1.8,-4,0.5,0.5,2,2);
      glColor4f(1,1,1,1);
   ZWdrawsprite(facetex,-2.2+(float)a*1.5,0.8-(float)b*1.8,-4,0.5/sqrt(2),0.5/sqrt(2),4,data->maindata->player[a+4*b].facenr);
      if (strcmp(language,(char*)"german")==0)
        switch (data->gameresult->winner[a+4*b])
        {
          case 0: glColor4f(0.8,0,0,0.5);
                  ZWdrawtextmiddle(text,-2.2+(float)a*1.5,-0.0-(float)b*1.8,-4,(char*)"Verloren",0.35); 
                  ZWdrawtextmiddle(text,-2.2+(float)a*1.5,-0.3-(float)b*1.8,-4,(char*)"+0",0.35); 
          break;
          default: glColor4f(0,0.6,0,0.5);
                  ZWdrawtextmiddle(text,-2.2+(float)a*1.5,-0.0-(float)b*1.8,-4,(char*)"Gewonnen",0.35); 
                  ZWdrawtextmiddle(text,-2.2+(float)a*1.5,-0.3-(float)b*1.8,-4,(char*)"+7",0.35); 
          break;
        }
      else
        switch (data->gameresult->winner[a+4*b])
        {
          case 0: glColor4f(0.8,0,0,0.5);
                  ZWdrawtextmiddle(text,-2.2+(float)a*1.5,-0.0-(float)b*1.8,-4,(char*)"Lost",0.35); 
                  ZWdrawtextmiddle(text,-2.2+(float)a*1.5,-0.3-(float)b*1.8,-4,(char*)"+0",0.35); 
          break;
          default: glColor4f(0,0.6,0,0.5);
                  ZWdrawtextmiddle(text,-2.2+(float)a*1.5,-0.0-(float)b*1.8,-4,(char*)"Won",0.35); 
                  ZWdrawtextmiddle(text,-2.2+(float)a*1.5,-0.3-(float)b*1.8,-4,(char*)"+7",0.35); 
          break;
        }
      
    }
  ZWsettexturepercent(temp);

  char buffer[10];
  sprintf(buffer,"%i",data->countdown/1000);
  glColor4f(0,0,0,0.5);
  ZWdrawtextmiddle(text,2.85,-2.2,-4,buffer,0.4);  

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

int calc_after_minigame(pafter_data data)
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
      return 1;
    }
    return 0;
  }
  data->countdown-=ZWattribute->steps;
  if (data->countdown<=0)
  {
    data->step=2;
    data->countdown=0;
    Mix_FadeOutMusic(500);
  }
  
  return 0;
}

int calc_after_minigame_thread(pafter_data data)
{
  handle_textcontent(-1);
  //Rotation
  data->rotation+=(float)(ZWattribute->steps)/42.0;
  while (data->rotation>=360.0)
    data->rotation-=360.0;
  return 0;
}

void after_minigame(pmaindata maindata,pgameresult gameresult)
{
  tafter_data data;
  //Init
    data.step=0;
    data.step_float=1;  
    data.rotation=0;
    data.maindata=maindata;
    data.gameresult=gameresult;
    data.countdown=5000;
  //Schleife
    Mix_FadeInMusic(snd_menumusic,-1,500);
    ZWattribute->newticks=SDL_GetTicks();
    int result=ZWschleifeex((void (*)(void*))draw_after_minigame,(int (*)(void*))calc_after_minigame_thread,
                            (int (*)(void*))calc_after_minigame,&data,sizeof(tafter_data));
  //Geld wirklich anpassen:
    for (int a=0;a<playernum;a++)
      if (gameresult->winner[a])
        maindata->player[a].money+=7;
}
