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
void draw_newgame_rounds(pmenudata data)
{
  draw_ballon_background(&menudata);
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);

  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);

  glColor4f(0.7,0.7,0.7,0.7);        
  ZWdrawtextmiddle(text,-0.71,-2.19,-4,(char*)"Was soll das Spielziel sein? Die Mehrheit entscheidet!",0.17);
  glColor4f(COLOR_BASE+sin((data->rotation-40.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-0.7 ,-2.2 ,-4,(char*)"Was soll das Spielziel sein? Die Mehrheit entscheidet!",0.17);

  for (float b=0;b<4;b++)
  {
    glColor4f(0.5,0.5,1.0,0.5);
    ZWdrawsprite(maintex,-2.3 +b*0.7, 0.6 ,-4,0.3,0.3,2,2);
    glColor4f(0.5,0.5,1.0,0.5);
    ZWdrawsprite(maintex,-2.13+b*0.7, 0   ,-4,0.3,0.3,2,2);
    glColor4f(1.0,1.0,0.5,0.5);
    ZWdrawsprite(maintex,-1.91+b*0.7,-0.75,-4,0.3,0.3,2,2);
    glColor4f(1.0,0.5,0.5,0.5);
    ZWdrawsprite(maintex,-1.7 +b*0.7,-1.5 ,-4,0.3,0.3,2,2);
  }
  for (float b=0;b<4;b++)
  {
    glColor4f(0,0,0,0.8);
    char buffer[10];
    sprintf(buffer,"%i",((int)(trunc(b+1)))*5);
    ZWdrawtextmiddle(text,-2.3 +b*0.7, 0.65,-4,buffer,0.3);
    ZWdrawtextmiddle(text,-2.3 +b*0.7, 0.5 ,-4,(char*)"Runden",0.2);
    glColor4f(0,0,0,0.8);
    sprintf(buffer,"%i",((int)(trunc(b+5)))*5);
    ZWdrawtextmiddle(text,-2.13+b*0.7, 0.05,-4,buffer,0.3);
    ZWdrawtextmiddle(text,-2.13+b*0.7,-0.1 ,-4,(char*)"Runden",0.2);
    glColor4f(0,0,0,0.8);
    sprintf(buffer,"%i",((int)(trunc(b+1)))*111);
    ZWdrawtextmiddle(text,-1.91+b*0.7,-0.7 ,-4,buffer,0.3);
    ZWdrawtextmiddle(text,-1.91+b*0.7,-0.85,-4,(char*)"Münzen",0.2);
    glColor4f(0,0,0,0.8);
    switch (((int)(trunc(b))))
    {
      case  0: ZWdrawtextmiddle(text,-1.7 +b*0.7,-1.45,-4,(char*)"3",0.3); break;
      case  1: ZWdrawtextmiddle(text,-1.7 +b*0.7,-1.45,-4,(char*)"5",0.3); break;
      case  2: ZWdrawtextmiddle(text,-1.7 +b*0.7,-1.45,-4,(char*)"7",0.3); break;
      default: ZWdrawtextmiddle(text,-1.7 +b*0.7,-1.45,-4,(char*)"10",0.3); break;    
    }
    ZWdrawtextmiddle(text,-1.7 +b*0.7,-1.6 ,-4,(char*)"Rubine",0.2);
  }

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
  for (a=0;a<playernum;a++)
  {
    glColor4f(maindata.player[a].color.r,maindata.player[a].color.g,maindata.player[a].color.b,0.8);
    if (data->choose_step[a]==0)
      ZWdrawsprite(maintex,data->x[a],data->y[a],-4,0.2,0.2,2,2);
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
    ZWdrawsprite(facetex,data->x[a],data->y[a],-4,0.2/sqrt(2),0.2/sqrt(2),4,maindata.player[a].facenr);
  }  
  
  ZWsettexturepercent(temp);
  glDepthFunc(GL_LEQUAL);   
}

int calc_newgame_rounds(pmenudata data)
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
        
      if (*(ZWattribute->joystick[a].button[0]) && 
         (distance(-2.3 +0.0*0.7, 0.6 ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-2.13+0.0*0.7, 0   ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-1.91+0.0*0.7,-0.75,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-1.7 +0.0*0.7,-1.5 ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-2.3 +1.0*0.7, 0.6 ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-2.13+1.0*0.7, 0   ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-1.91+1.0*0.7,-0.75,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-1.7 +1.0*0.7,-1.5 ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-2.3 +2.0*0.7, 0.6 ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-2.13+2.0*0.7, 0   ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-1.91+2.0*0.7,-0.75,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-1.7 +2.0*0.7,-1.5 ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-2.3 +3.0*0.7, 0.6 ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-2.13+3.0*0.7, 0   ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-1.91+3.0*0.7,-0.75,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-1.7 +3.0*0.7,-1.5 ,0,data->x[a],data->y[a],0)<=0.3 ||
          distance(-2.65, -1.2,0,data->x[a],data->y[a],0)<=0.35 ))
      {
        *(ZWattribute->joystick[a].button[0])=0;
        data->choose_step[a]=1;
      }      
    }
    if (data->choose_step[a]==1)
    {
      if (*(ZWattribute->joystick[a].button[2]))
      {
        *(ZWattribute->joystick[a].button[2])=0;
        data->choose_step[a]=0;
      }    
    }    
  }
  //Schauen, ob es weitergehen kann:
  int allefest=1;
  int stimme[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  for (a=0;a<playernum;a++)
  {
    if (data->choose_step[a]==0)
      allefest=0;
    else
    {
      if (distance(-2.3 +0.0*0.7, 0.6 ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[0]++;
      if (distance(-2.13+0.0*0.7, 0   ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[4]++;
      if (distance(-1.91+0.0*0.7,-0.75,0,data->x[a],data->y[a],0)<=0.3)
        stimme[8]++;
      if (distance(-1.7 +0.0*0.7,-1.5 ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[12]++;
      if (distance(-2.3 +1.0*0.7, 0.6 ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[1]++;
      if (distance(-2.13+1.0*0.7, 0   ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[5]++;
      if (distance(-1.91+1.0*0.7,-0.75,0,data->x[a],data->y[a],0)<=0.3)
        stimme[9]++;
      if (distance(-1.7 +1.0*0.7,-1.5 ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[13]++;
      if (distance(-2.3 +2.0*0.7, 0.6 ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[2]++;
      if (distance(-2.13+2.0*0.7, 0   ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[6]++;
      if (distance(-1.91+2.0*0.7,-0.75,0,data->x[a],data->y[a],0)<=0.3)
        stimme[10]++;
      if (distance(-1.7 +2.0*0.7,-1.5 ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[14]++;
      if (distance(-2.3 +3.0*0.7, 0.6 ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[3]++;
      if (distance(-2.13+3.0*0.7, 0   ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[7]++;
      if (distance(-1.91+3.0*0.7,-0.75,0,data->x[a],data->y[a],0)<=0.3)
        stimme[11]++;
      if (distance(-1.7 +3.0*0.7,-1.5 ,0,data->x[a],data->y[a],0)<=0.3)
        stimme[15]++;
      if (distance(-2.65, -1.2,0,data->x[a],data->y[a],0)<=0.35)
        stimme[16]++;
    }
  }
  if (allefest)
  {
    //schauen, ob es eine Mehrheit gibt:
    int max=0;
    int maxstimme=0;
    int maxalone=1;
    for (a=0;a<17;a++)
      if (stimme[a]>max)
      {
        max=stimme[a];
        maxstimme=a;
        maxalone=1;
      }
      else
      if (stimme[a]==max)
      {
        maxalone=0;
      }
    if (maxalone)
    {
      if (maxstimme==16)
        data->cancel_all=1;
      else
        gametype=maxstimme;
      data->fade_dir=2;
    } 
  }
  return 0;
}

int calc_newgame_rounds_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;
    
  return 0;
}
