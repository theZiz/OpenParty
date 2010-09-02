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

void draw_newgame_team_4(pmenudata data)
{
  draw_ballon_background(&menudata);
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);

  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);

  glColor4f(0.7,0.7,0.7,0.7);        
  ZWdrawtextmiddle(text,-0.71,-2.14,-4,(char*)"Teilt euch in die Teams ein! Die CPU wird zufällig aufgefüllt.",0.18);
  glColor4f(COLOR_BASE+sin((data->rotation-40.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-0.7 ,-2.15 ,-4,(char*)"Teilt euch in die Teams ein! Die CPU wird zufällig aufgefüllt.",0.18);


  glColor4f(1,0,0,0.5);
  ZWdrawsprite(maintex,-2.1, 0.3,-4,0.7,0.7,2,2);
  glColor4f(0,1,0,0.5);
  ZWdrawsprite(maintex,-0.6, 0.3,-4,0.7,0.7,2,2);
  glColor4f(0,0,1,0.5);
  ZWdrawsprite(maintex,-1.5,-1.2,-4,0.7,0.7,2,2);
  glColor4f(1,1,0,0.5);
  ZWdrawsprite(maintex,   0,-1.2,-4,0.7,0.7,2,2);

  glColor4f(1,1,1,0.8);
  ZWdrawtextmiddle(text,-2.1, 0.5,-4,(char*)"Team",0.4);
  ZWdrawtextmiddle(text,-2.1, 0.1,-4,(char*)"Rot",0.6);

  ZWdrawtextmiddle(text,-0.6, 0.5,-4,(char*)"Team",0.4);
  ZWdrawtextmiddle(text,-0.6, 0.1,-4,(char*)"Grün",0.6);

  ZWdrawtextmiddle(text,-1.5,-1.0,-4,(char*)"Team",0.4);
  ZWdrawtextmiddle(text,-1.5,-1.4,-4,(char*)"Blau",0.6);
  
  glColor4f(0,0,0,0.8);
  ZWdrawtextmiddle(text,   0,-1.0,-4,(char*)"Team",0.4);
  ZWdrawtextmiddle(text,   0,-1.4,-4,(char*)"Gelb",0.6);


  glColor4f(0.3,0.3,0.3,0.5);
  ZWdrawsprite(maintex,   -2.65,-1.2,-4,0.35,0.35,2,2);
  glColor4f(0.8,0,0,0.8);
  ZWdrawtextmiddle(text,  -2.65,-1.35,-4,(char*)"X",0.8);
  ZWdrawtextmiddle(text,  -2.65,-1.65,-4,(char*)"Komplett",0.21);
  ZWdrawtextmiddle(text,  -2.65,-1.77,-4,(char*)"abbrechen",0.17);

  //Cursor anzeigen
  float temp=ZWgettexturepercent();
  ZWsettexturepercent(0.005);
  int a;
  for (a=0;a<playernum+cpuplayer;a++)
  {
    glColor4f(maindata.player[a].color.r,maindata.player[a].color.g,maindata.player[a].color.b,0.8);
    if (data->choose_step[a]==0)
    {
      if (a<playernum)
        ZWdrawsprite(maintex,data->x[a],data->y[a],-4,0.2,0.2,2,2);
      else
        ZWdrawsprite(maintex,data->x[a],data->y[a],-4,0.14,0.14,2,2);
    }
    if (data->choose_step[a]==1)
    {
      glDisable(GL_TEXTURE_2D);
      glBegin(GL_QUADS);
        glVertex3f(data->x[a]-0.2,data->y[a]+0.2,-4);
        glVertex3f(data->x[a]-0.2,data->y[a]-0.2,-4);
        glVertex3f(data->x[a]+0.2,data->y[a]-0.2,-4);
        glVertex3f(data->x[a]+0.2,data->y[a]+0.2,-4);
      glEnd();
      glEnable(GL_TEXTURE_2D);
    }

    glColor4f(1,1,1,1);
    if (a<playernum)
      ZWdrawsprite(facetex,data->x[a],data->y[a],-4,0.2/sqrt(2),0.2/sqrt(2),4,maindata.player[a].facenr);
    else
    {
      char buffer[6];
      sprintf(buffer,"CPU %i",a-playernum+1);
      ZWdrawsprite(facetex,data->x[a],data->y[a],-4,0.14/sqrt(2),0.14/sqrt(2),4,maindata.player[a].facenr);
      glColor4f(0.7,0.7,0.7,0.7);
      ZWdrawtextmiddle(text,data->x[a]-0.015,data->y[a]-0.215,-4,buffer,0.15);
      glColor4f(COLOR_BASE+sin((data->rotation-40.0*((float)(a))/7.0)*M_PI/180.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*((float)(a))/7.0)*M_PI/90.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*((float)(a))/7.0)*M_PI/45.0)*COLOR_MULT,1);
      ZWdrawtextmiddle(text,data->x[a],data->y[a]-0.23,-4,buffer,0.15);
    }
  }  
  
  ZWsettexturepercent(temp);
  glDepthFunc(GL_LEQUAL);   
}

int calc_newgame_team_4(pmenudata data)
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
    for (int a=0;a<ZWattribute->steps;a++)
      data->fade-=(0.2+(1.001-data->fade)*0.8)/700.0;
    if (data->fade<=0.0)
    {
      data->fade=0.0;
      data->fade_dir=1;
      return 1;      
    }
    return 0;
  }

  //Cursorbewegung:
  for (a=0;a<playernum;a++)
  {
    if (data->choose_step[a]==0)
    {
      if (*(ZWattribute->joystick[a].axis[0])<0)
        data->x[a]-=((float)(ZWattribute->steps))/500.0;
      if (*(ZWattribute->joystick[a].axis[0])>0)
        data->x[a]+=((float)(ZWattribute->steps))/500.0;
      if (*(ZWattribute->joystick[a].axis[1])<0)
        data->y[a]+=((float)(ZWattribute->steps))/500.0;
      if (*(ZWattribute->joystick[a].axis[1])>0)
        data->y[a]-=((float)(ZWattribute->steps))/500.0;
      if (data->y[a]>1.0)
        data->y[a]=1.0;
      if (data->y[a]<-1.8)
        data->y[a]=-1.8;
      if (data->x[a]>0.5+(1.0-data->y[a])*0.25)
        data->x[a]=0.5+(1.0-data->y[a])*0.25;
      if (data->x[a]<-2.8/*+(1.0-data->y[a])*0.25*/)
        data->x[a]=-2.8/*+(1.0-data->y[a])*0.25*/;
        
      if (*(ZWattribute->joystick[a].button[0]))
      {
         *(ZWattribute->joystick[a].button[0])=0;
         if (distance(-2.1, 0.3,0,data->x[a],data->y[a],0)<=0.7)
         {
           data->choose_step[a]=1;
           maindata.player[a].team=0;
         }
         if (distance(-0.6, 0.3,0,data->x[a],data->y[a],0)<=0.7)
         {
           data->choose_step[a]=1;
           maindata.player[a].team=1;         
         }
         if (distance(-1.5,-1.2,0,data->x[a],data->y[a],0)<=0.7)
         {
           data->choose_step[a]=1;
           maindata.player[a].team=2;           
         }
         if (distance(   0,-1.2,0,data->x[a],data->y[a],0)<=0.7)
         {
           data->choose_step[a]=1;
           maindata.player[a].team=3;
         }
         if (distance(-2.65, -1.2,0,data->x[a],data->y[a],0)<=0.35)
         {
           data->choose_step[a]=1;
           data->cancel_all++;
         }
      }
    }
    if (data->choose_step[a]==1)
    {
      if (*(ZWattribute->joystick[a].button[2]))
      {
        *(ZWattribute->joystick[a].button[2])=0;
        data->choose_step[a]=0;
        maindata.player[a].team=-1;
        if (distance(-2.65, -1.2,0,data->x[a],data->y[a],0)<=0.35)
          data->cancel_all--;
      }    
    }
  }
  //Abbruchbedingungen prüfen: Alle Teams müssen != -1 sein UND pro Team darf es max. 2 Spieler geben
  int teamsum[4]={0,0,0,0};
  for (a=0;a<playernum;a++)
  {
    if (maindata.player[a].team>-1)
      teamsum[maindata.player[a].team]++;
    else
      break;
  }
  if (a==playernum || data->cancel_all>playernum/2) //Schleife lief komplett durch oder cancel_all!
  {
    if (data->cancel_all>playernum/2)
    {
      data->cancel_all=1;
      data->fade_dir=2;
    }
    else
    {
      //Schauen, ob irgendeine Summe größer als teamsize ist:
      for (a=0;a<4;a++)
        if (teamsum[a]>teamsize) break;
      if (a==4) //Schleife lief durch
      {
        data->fade_dir=2;
        //CPU Player auffüllen und Cursorpositionen setzen 
        for (a=0;a<cpuplayer;a++)
        {
          int destteam;
          do
          {
            destteam=rand()%4;
          }
          while (teamsum[destteam]>=teamsize);
          teamsum[destteam]++;
          maindata.player[playernum+a].team=destteam;
        }
        float teamcount[4]={0,0,0,0};
        for (a=0;a<playernum+cpuplayer;a++)
        {
          switch (maindata.player[a].team)
          {
            case 0: data->x[a]=-2.1+cos(teamcount[maindata.player[a].team]*M_PI)*0.3;
                    data->y[a]= 0.3+sin(teamcount[maindata.player[a].team]*M_PI)*0.3;
            break;
            case 1: data->x[a]=-0.6+cos(teamcount[maindata.player[a].team]*M_PI)*0.3;
                    data->y[a]= 0.3+sin(teamcount[maindata.player[a].team]*M_PI)*0.3;
            break;
            case 2: data->x[a]=-1.5+cos(teamcount[maindata.player[a].team]*M_PI)*0.3;
                    data->y[a]=-1.2+sin(teamcount[maindata.player[a].team]*M_PI)*0.3;
            break;
            case 3: data->x[a]=   0+cos(teamcount[maindata.player[a].team]*M_PI)*0.3;
                    data->y[a]=-1.2+sin(teamcount[maindata.player[a].team]*M_PI)*0.3;
            break;
          }
          teamcount[maindata.player[a].team]++;
          data->choose_step[a]=0;
        }
      }
    }
  }
  return 0;
}

int calc_newgame_team_4_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;
    
  return 0;
}

