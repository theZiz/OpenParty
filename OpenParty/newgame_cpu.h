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
void draw_newgame_cpu(pmenudata data)
{
  draw_ballon_background(&menudata);
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);

  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);

  glColor4f(0.7,0.7,0.7,0.7);
  if (strcmp(language,(char*)"german")==0)
  {
    ZWdrawtextmiddle(text,-0.71,-2.19,-4,(char*)"Wie viele CPU Spieler sollen mitspielen? Die Mehrheit entscheidet!",0.17);
    glColor4f(COLOR_BASE+sin((data->rotation-40.0)*M_PI/180.0)*COLOR_MULT,
              COLOR_BASE+sin((data->rotation-40.0)*M_PI/90.0)*COLOR_MULT,
              COLOR_BASE+sin((data->rotation-40.0)*M_PI/45.0)*COLOR_MULT,1);
    ZWdrawtextmiddle(text,-0.7 ,-2.2 ,-4,(char*)"Wie viele CPU Spieler sollen mitspielen? Die Mehrheit entscheidet!",0.17);
  }
  else
  {
    ZWdrawtextmiddle(text,-0.71,-2.19,-4,(char*)"How many AI players play? The Majority decides!",0.17);
    glColor4f(COLOR_BASE+sin((data->rotation-40.0)*M_PI/180.0)*COLOR_MULT,
              COLOR_BASE+sin((data->rotation-40.0)*M_PI/90.0)*COLOR_MULT,
              COLOR_BASE+sin((data->rotation-40.0)*M_PI/45.0)*COLOR_MULT,1);
    ZWdrawtextmiddle(text,-0.7 ,-2.2 ,-4,(char*)"How many AI players play? The Majority decides!",0.17);
  }
  

  glColor4f(0.5,1,0.5,0.5);
  ZWdrawsprite(maintex,-2.4 , 0.6 ,-4,0.5,0.5,2,2);
  glColor4f(0.5,1,0,0.5-((float)(data->blocked_field[0]))*0.3);
  ZWdrawsprite(maintex,-1.35, 0.6 ,-4,0.5-((float)(data->blocked_field[0]))*0.2,0.5-((float)(data->blocked_field[0]))*0.2,2,2);
  glColor4f(0.5,0,1,0.5-((float)(data->blocked_field[1]))*0.3);
  ZWdrawsprite(maintex,-0.3 , 0.6 ,-4,0.5-((float)(data->blocked_field[1]))*0.2,0.5-((float)(data->blocked_field[1]))*0.2,2,2);
  glColor4f(0.5,0,0,0.5-((float)(data->blocked_field[2]))*0.3);
  ZWdrawsprite(maintex,-2.1 ,-0.45,-4,0.5-((float)(data->blocked_field[2]))*0.2,0.5-((float)(data->blocked_field[2]))*0.2,2,2);
  glColor4f(1,0,0,0.5-((float)(data->blocked_field[3]))*0.3);
  ZWdrawsprite(maintex,-1.05,-0.45,-4,0.5-((float)(data->blocked_field[3]))*0.2,0.5-((float)(data->blocked_field[3]))*0.2,2,2);
  glColor4f(1,1,0,0.5-((float)(data->blocked_field[4]))*0.3);
  ZWdrawsprite(maintex, 0   ,-0.45,-4,0.5-((float)(data->blocked_field[4]))*0.2,0.5-((float)(data->blocked_field[4]))*0.2,2,2);
  glColor4f(1,0,1,0.5-((float)(data->blocked_field[5]))*0.3);
  ZWdrawsprite(maintex,-1.8 ,-1.5 ,-4,0.5-((float)(data->blocked_field[5]))*0.2,0.5-((float)(data->blocked_field[5]))*0.2,2,2);
  glColor4f(1,1,1,0.5-((float)(data->blocked_field[6]))*0.3);
  ZWdrawsprite(maintex,-0.75,-1.5 ,-4,0.5-((float)(data->blocked_field[6]))*0.2,0.5-((float)(data->blocked_field[6]))*0.2,2,2);
  glColor4f(0,0,0,0.5-((float)(data->blocked_field[7]))*0.3);
  ZWdrawsprite(maintex, 0.3 ,-1.5 ,-4,0.5-((float)(data->blocked_field[7]))*0.2,0.5-((float)(data->blocked_field[7]))*0.2,2,2);

/*  glColor4f(0.7,0.7,0.7,0.7);
  ZWdrawtextmiddle(text,-2.41 , 0.74,-4,(char*)"Wieviele",0.2);
  ZWdrawtextmiddle(text,-2.41 , 0.54,-4,(char*)"CPU",0.2);
  ZWdrawtextmiddle(text,-2.41 , 0.34,-4,(char*)"Spieler?",0.2);  
  glColor4f(COLOR_BASE+sin((data->rotation-00.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-00.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-00.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-2.4 , 0.75,-4,(char*)"Wieviele",0.2);
  ZWdrawtextmiddle(text,-2.4 , 0.55,-4,(char*)"CPU",0.2);
  ZWdrawtextmiddle(text,-2.4 , 0.35,-4,(char*)"Spieler?",0.2);*/

  glColor4f(0,0,0,0.8);
  if (strcmp(language,(char*)"german")==0)
  {
    ZWdrawtextmiddle(text,-2.4 , 0.75,-4,(char*)"Neues",0.2);
    ZWdrawtextmiddle(text,-2.4 , 0.55,-4,(char*)"CPU",0.2);
    ZWdrawtextmiddle(text,-2.4 , 0.35,-4,(char*)"Aussehen",0.2);
  }
  else
  {
    ZWdrawtextmiddle(text,-2.4 , 0.75,-4,(char*)"New",0.2);
    ZWdrawtextmiddle(text,-2.4 , 0.55,-4,(char*)"CPU",0.2);
    ZWdrawtextmiddle(text,-2.4 , 0.35,-4,(char*)"appearance",0.2);
  }
  
  glColor4f(0,0,0,0.8-((float)(data->blocked_field[0]))*0.5);
  ZWdrawtextmiddle(text,-1.35, 0.55,-4,(char*)"0 CPU",0.4-((float)(data->blocked_field[0]))*0.16);
  glColor4f(1,1,1,0.8-((float)(data->blocked_field[1]))*0.5);
  ZWdrawtextmiddle(text,-0.3 , 0.55,-4,(char*)"1 CPU",0.4-((float)(data->blocked_field[1]))*0.16);
  glColor4f(1,1,1,0.8-((float)(data->blocked_field[2]))*0.5);
  ZWdrawtextmiddle(text,-2.1 ,-0.5 ,-4,(char*)"2 CPU",0.4-((float)(data->blocked_field[2]))*0.16);
  glColor4f(1,1,1,0.8-((float)(data->blocked_field[3]))*0.5);
  ZWdrawtextmiddle(text,-1.05,-0.5 ,-4,(char*)"3 CPU",0.4-((float)(data->blocked_field[3]))*0.16);
  glColor4f(0,0,0,0.8-((float)(data->blocked_field[4]))*0.5);
  ZWdrawtextmiddle(text, 0   ,-0.5 ,-4,(char*)"4 CPU",0.4-((float)(data->blocked_field[4]))*0.16);
  glColor4f(1,1,1,0.8-((float)(data->blocked_field[5]))*0.5);
  ZWdrawtextmiddle(text,-1.8 ,-1.55,-4,(char*)"5 CPU",0.4-((float)(data->blocked_field[5]))*0.16);
  glColor4f(0,0,0,0.8-((float)(data->blocked_field[6]))*0.5);
  ZWdrawtextmiddle(text,-0.75,-1.55,-4,(char*)"6 CPU",0.4-((float)(data->blocked_field[6]))*0.16);
  glColor4f(1,1,1,0.8-((float)(data->blocked_field[7]))*0.5);
  ZWdrawtextmiddle(text, 0.3 ,-1.55,-4,(char*)"7 CPU",0.4-((float)(data->blocked_field[7]))*0.16);

  glColor4f(0.3,0.3,0.3,0.5);
  ZWdrawsprite(maintex,   -2.65,-1.2,-4,0.35,0.35,2,2);
  glColor4f(0.8,0,0,0.8);
  if (strcmp(language,(char*)"german")==0)
  {
    ZWdrawtextmiddle(text,  -2.65,-1.35,-4,(char*)"X",0.8);
    ZWdrawtextmiddle(text,  -2.65,-1.65,-4,(char*)"Komplett",0.21);
    ZWdrawtextmiddle(text,  -2.65,-1.77,-4,(char*)"abbrechen",0.17);
  }
  else
  {
    ZWdrawtextmiddle(text,  -2.65,-1.35,-4,(char*)"X",0.8);
    ZWdrawtextmiddle(text,  -2.65,-1.65,-4,(char*)"Cancel",0.21);
    ZWdrawtextmiddle(text,  -2.65,-1.77,-4,(char*)"complete",0.17);
  }
  

  //Cursor anzeigen
  float temp=ZWgettexturepercent();
  ZWsettexturepercent(0.005);
  int a;
  for (a=0;a<8;a++)
  {
    //Wenn nächsten Spieler mit der Teamgröße nicht möglich sind: Abbruch
    int possible=0;
    int b;
    if (a<playernum)
      possible=1;
    else
    for (b=a-playernum;b<8-playernum;b++)
      if (data->blocked_field[b+1]==0)
        possible=1;
    if (possible==0)
      break;
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

int calc_newgame_cpu(pmenudata data)
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
         ((distance(-1.35, 0.6 ,0,data->x[a],data->y[a],0)<=0.5 && data->blocked_field[0]==0) ||
          (distance(-0.3 , 0.6 ,0,data->x[a],data->y[a],0)<=0.5 && data->blocked_field[1]==0)  ||
          (distance(-2.1 ,-0.45,0,data->x[a],data->y[a],0)<=0.5 && data->blocked_field[2]==0)  ||
          (distance(-1.05,-0.45,0,data->x[a],data->y[a],0)<=0.5 && data->blocked_field[3]==0)  ||
          (distance( 0   ,-0.45,0,data->x[a],data->y[a],0)<=0.5 && data->blocked_field[4]==0)  ||
          (distance(-1.8 ,-1.5 ,0,data->x[a],data->y[a],0)<=0.5 && data->blocked_field[5]==0)  ||
          (distance(-0.75,-1.5 ,0,data->x[a],data->y[a],0)<=0.5 && data->blocked_field[6]==0)  ||
          (distance( 0.3 ,-1.5 ,0,data->x[a],data->y[a],0)<=0.5 && data->blocked_field[7]==0)  ||
          (distance(-2.65, -1.2,0,data->x[a],data->y[a],0)<=0.35) ))
      {
        *(ZWattribute->joystick[a].button[0])=0;
        data->choose_step[a]=1;
      }
      //Neu würfeln:
      if (*(ZWattribute->joystick[a].button[0]) && distance(-2.4 , 0.6 ,0,data->x[a],data->y[a],0)<=0.5)
      {
        *(ZWattribute->joystick[a].button[0])=0;
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
            if (tr==br && tg==bg && (bb-tb)*(bb-tb)<=1)
              data->blocked_color[c]=1;      
            if (tg==bg && tb==bb && (br-tr)*(br-tr)<=1)
              data->blocked_color[c]=1;      
            if (tb==bb && tr==br && (bg-tg)*(bg-tg)<=1)
              data->blocked_color[c]=1;
          }
        }
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
            if (tr==br && tg==bg && (bb-tb)*(bb-tb)<=1 && (bb+tb>1 || (tr==2 && tg==2) || bb==tb))
              data->blocked_color[c]=1;      
            if (tg==bg && tb==bb && (br-tr)*(br-tr)<=1 && (br+tr>1 || (tb==2 && tg==2) || br==tr))
              data->blocked_color[c]=1;      
            if (tb==bb && tr==br && (bg-tg)*(bg-tg)<=1 && (bg+tg>1 || (tr==2 && tb==2) || bg==tg))
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
  //Schauen, ob alle Spieler "fest" sind
  int fest=1;
  int zero=0,one=0,two=0,three=0,four=0,five=0,six=0,seven=0,cancel=0;
  for (a=0;a<playernum;a++)
  {
    if (distance(-1.35, 0.6 ,0,data->x[a],data->y[a],0)<=0.5)
      zero++;
    if (distance(-0.3 , 0.6 ,0,data->x[a],data->y[a],0)<=0.5)
      one++;
    if (distance(-2.1 ,-0.45,0,data->x[a],data->y[a],0)<=0.5)
      two++;
    if (distance(-1.05,-0.45,0,data->x[a],data->y[a],0)<=0.5)
      three++;
    if (distance( 0   ,-0.45,0,data->x[a],data->y[a],0)<=0.5)
      four++;
    if (distance(-1.8 ,-1.5 ,0,data->x[a],data->y[a],0)<=0.5)
      five++;
    if (distance(-0.75,-1.5 ,0,data->x[a],data->y[a],0)<=0.5)
      six++;
    if (distance( 0.3 ,-1.5 ,0,data->x[a],data->y[a],0)<=0.5)
      seven++;
    if (distance(-2.65, -1.2,0,data->x[a],data->y[a],0)<=0.35)
      cancel++;
    if (data->choose_step[a]==0)
      fest=0;
  }
  if (fest==1)
  {
    //Größtes ermitteln:
    cpuplayer=-1;
    if (zero>one && zero>two && zero>three && zero>four && zero>five && zero>six && zero>seven && zero>cancel)
      cpuplayer=0;
    if (one>zero && one>two && one>three && one>four && one>five && one>six && one>seven && one>cancel)
      cpuplayer=1;
    if (two>zero && two>one && two>three && two>four && two>five && two>six && two>seven && two>cancel)
      cpuplayer=2;
    if (three>zero && three>one && three>two && three>four && three>five && three>six && three>seven && three>cancel)
      cpuplayer=3;
    if (four>zero && four>one && four>two && four>three && four>five && four>six && four>seven && four>cancel)
      cpuplayer=4;
    if (five>zero && five>one && five>two && five>three && five>four && five>six && five>seven && five>cancel)
      cpuplayer=5;
    if (six>zero && six>one && six>two && six>three && six>four && six>five && six>seven && six>cancel)
      cpuplayer=6;
    if (seven>zero && seven>one && seven>two && seven>three && seven>four && seven>five && seven>six && seven>cancel)
      cpuplayer=7;
    if (cancel>zero && cancel>one && cancel>two && cancel>three && cancel>four && cancel>five && cancel>six && cancel>seven)
      data->cancel_all=1;
    if (cpuplayer>=0 || data->cancel_all)
      data->fade_dir=2;
  }
  
  return 0;
}

int calc_newgame_cpu_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;
    
  return 0;
}

