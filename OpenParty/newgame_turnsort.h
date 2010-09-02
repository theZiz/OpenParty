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
void draw_newgame_turnsort(pmenudata data)
{
  draw_ballon_background(&menudata);
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);

  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);

  glColor4f(0.7,0.7,0.7,0.7);        
  ZWdrawtextmiddle(text,-0.71,-2.19,-4,(char*)"Jeder zieht ein Los für die Festsetzung der Spielreihenfolge!",0.17);
  glColor4f(COLOR_BASE+sin((data->rotation-40.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-0.7 ,-2.2 ,-4,(char*)"Jeder zieht ein Los für die Festsetzung der Spielreihenfolge!",0.17);

  float max=playernum+cpuplayer;
  for (float b=0;b<max;b++)
  {
    float r=0.43/sin(M_PI/max);
    float x=-1.0+sin(b/max*M_PI*2.0)*r;
    float y=-0.5+cos(b/max*M_PI*2.0)*r;
    float faktor=1.0-(y+0.5+r)/(2*r);
    glColor4f(COLOR_BASE+sin((data->rotation-40.0*faktor)*M_PI/180.0)*COLOR_MULT,
              COLOR_BASE+sin((data->rotation-40.0*faktor)*M_PI/90.0)*COLOR_MULT,
              COLOR_BASE+sin((data->rotation-40.0*faktor)*M_PI/45.0)*COLOR_MULT,0.5);
    ZWdrawsprite(maintex,x,y,-4,0.43,0.43,2,2);
  }
  for (float b=0;b<max;b++)
  {
    float r=0.43/sin(M_PI/max);
    float x=-1.0+sin(b/max*M_PI*2.0)*r;
    float y=-0.5+cos(b/max*M_PI*2.0)*r;
    float faktor=1.0-(y+0.5+r)/(2*r);
    glColor4f(0.9+sin((data->rotation-40.0*faktor)*M_PI/180.0)/10.0,
              0.9+sin((data->rotation-40.0*faktor)*M_PI/90.0)/10.0,
              0.9+sin((data->rotation-40.0*faktor)*M_PI/45.0)/10.0,0.8);
    if (data->blocked_field[(int)(trunc(b))])
    {
      char buffer[10];
      sprintf(buffer,"%i.",data->color_choose[(int)(trunc(b))]+1);
      ZWdrawtextmiddle(text,x,y-0.3,-4,buffer,0.5);
    }
    else
      ZWdrawtextmiddle(text,x,y-0.2,-4,(char*)"?",1.0);
  }
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
  if (data->fade_dir==2)
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
  ZWsettexturepercent(temp);
  glDepthFunc(GL_LEQUAL);   
}

int calc_newgame_turnsort(pmenudata data)
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
      //data->fade_dir=1;
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
      if (data->x[a]<-2.8+(1.0-data->y[a])*0.25)
        data->x[a]=-2.8+(1.0-data->y[a])*0.25;
        
      if (*(ZWattribute->joystick[a].button[0]))
      {
        float max=playernum+cpuplayer;
        for (float b=0;b<max;b++)
        {
          float r=0.43/sin(M_PI/max);
          float x=-1.0+sin(b/max*M_PI*2.0)*r;
          float y=-0.5+cos(b/max*M_PI*2.0)*r;
          if (distance(x,y,0,data->x[a],data->y[a],0)<=0.43 && !(data->blocked_field[(int)(trunc(b))]))
          {
            *(ZWattribute->joystick[a].button[0])=0;
            data->choose_step[a]=1;
            data->blocked_field[(int)(trunc(b))]=1;
            data->x[a]=x;
            data->y[a]=y+0.2;
            data->calib_step[a]=(int)(trunc(b));
            break;
          }
        }
      }      
    }
  }
  //Wenn Fest, CPU-Spieler zuweisen und Fade_Out.
  int fest=1;
  for (int a=0;a<playernum;a++)
    if (data->choose_step[a]==0)
      fest=0;
  if (fest)
  {
    float max=playernum+cpuplayer;
    for (int a=playernum;a<playernum+cpuplayer;a++)
    {
      int los;
      do
      {
        los=rand()%(playernum+cpuplayer);
      }
      while (data->blocked_field[los]);
      data->blocked_field[los]=1;
      data->calib_step[a]=los;
      float r=0.43/sin(M_PI/max);
      float x=-1.0+sin(((float)los)/max*M_PI*2.0)*r;
      float y=-0.5+cos(((float)los)/max*M_PI*2.0)*r;
      data->x[a]=x;
      data->y[a]=y+0.3;
    }
    Mix_FadeOutMusic(500);    
    data->fade_dir=2;
  }
  return 0;
}

int calc_newgame_turnsort_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;
    
  return 0;
}

