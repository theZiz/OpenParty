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

char fade_login_out=1;

void draw_newgame_login_new(pmenudata data)
{
  draw_ballon_background(&menudata);
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);

  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);
  float a;
  for (a=0;a<8;a++)
  {
    if (data->play_with[(int)(trunc(a))]==-1)
    {
      glColor4f(0.7,0.7,0.7,0.7);
      if (strcmp(language,(char*)"german")==0)
      {
        ZWdrawtextmiddle(text,-1.12+a*0.1,0.72-a*0.4,-4,(char*)"Taste drücken um mitzuspielen",0.3);
        glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
        ZWdrawtextmiddle(text,-1.1+a*0.1,0.7-a*0.4,-4,(char*)"Taste drücken um mitzuspielen",0.3);
      }
      else
      {
        ZWdrawtextmiddle(text,-1.12+a*0.1,0.72-a*0.4,-4,(char*)"Press Button to play!",0.3);
        glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
        ZWdrawtextmiddle(text,-1.1+a*0.1,0.7-a*0.4,-4,(char*)"Press Button to play!",0.3);
      }
      
    }
    else
    {
      glColor4f(0.7,0.7,0.7,0.7);
      char buffer[64];
      if (strlen(ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name)>30)
      {
        char temp=ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name[28];
        ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name[28]=0;
        if (strcmp(language,(char*)"german")==0)
          switch (data->choose_step[(int)(trunc(a))])
          {
            case 1: sprintf(buffer,"%s... [A] Weiter",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
            case 2: sprintf(buffer,"%s... Fertig [C] Zurück",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
            case 3: sprintf(buffer,"%s... [ ] Kalibriere",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
          }
        else
          switch (data->choose_step[(int)(trunc(a))])
          {
            case 1: sprintf(buffer,"%s... [A] Continue",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
            case 2: sprintf(buffer,"%s... Ready [C] Back",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
            case 3: sprintf(buffer,"%s... [ ] Calibrate",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
          }
        
        ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name[28]=temp;
      }
      else
      {
        if (strcmp(language,(char*)"german")==0)
          switch (data->choose_step[(int)(trunc(a))])
          {
            case 1: sprintf(buffer,"%s [A] Weiter",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
            case 2: sprintf(buffer,"%s Fertig [C] Zurück",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
            case 3: sprintf(buffer,"%s [A] Kalibrieren",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
          }
          else
          switch (data->choose_step[(int)(trunc(a))])
          {
            case 1: sprintf(buffer,"%s [A] Continue",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
            case 2: sprintf(buffer,"%s Ready [C] Back",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
            case 3: sprintf(buffer,"%s [A] Calibrate",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name); break;
          }
      }
        
      ZWdrawtextmiddle(text,-1.11+a*0.1,0.71-a*0.4,-4,buffer,0.2);
      glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
      ZWdrawtextmiddle(text,-1.1+a*0.1,0.7-a*0.4,-4,buffer,0.2);
    }
  }    

  //Schauen, ob jemand im Step 4 - der Kalibrierung - ist
  int c;
  for (c=0;c<8;c++)
  if (data->choose_step[c]==4)
  {
    draw_ingame_kalib(&maindata);
  }

  if (fade_login_out && data->fade_dir!=1)
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

  glDepthFunc(GL_LEQUAL);   
}

int calc_newgame_login_new(pmenudata data)
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
      data->fade_dir=0;
      return 1;      
    }
    return 0;
  }
  
  //Schauen, ob jemand im Step 4 - der Kalibrierung - ist
  int c;
  for (c=0;c<8;c++)
  if (data->choose_step[c]==4)
  {
    calc_ingame_kalib(&maindata);
    if (maindata.mainloop_pos==2)
      data->choose_step[c]=1;
    return 0;
  }
  
  //Schauen, wer alles mitspielt.
  int b;
  for (a=0;a<ZWattribute->realjoystickcount;a++)
    for (b=0;b<ZWattribute->realjoystick[a].buttoncount;b++)
      if (ZWattribute->realjoystick[a].button[b]==1) 
      {
        //Schauen, ob der Controller überhaupt genug Achsen und Buttons hat!
        if (ZWattribute->realjoystick[a].buttoncount<4 || ZWattribute->realjoystick[a].axiscount<2)
          continue;
        //Schauen, ob Joystick a schon verwendet wird und Suche nach dem letzten Element:
        int lastelem=8;
        for (c=7;c>=0;c--)
        {
          if (data->play_with[c]==-1)
            lastelem=c;
          if (data->play_with[c]==a)
            break;
        }
        if (c<0 && lastelem<8) //nicht vorhanden
        {
          playernum++;
          ZWattribute->realjoystick[a].button[b]=0;
          data->play_with[lastelem]=a;
          //Controller suchen:
          pcontroller_db entry=controller_db;
          while (entry!=NULL)
          {
            if (strcmp(entry->name,ZWattribute->realjoystick[a].name)==0)
              break;
            entry=entry->next;
          }
          if (entry==NULL && a>=ZWattribute->keyboard_joystick_num) //nicht vorhanden
          {
            data->choose_step[lastelem]=3;
            
          }
          else
          {
            data->choose_step[lastelem]=1;
            maindata.player[lastelem].joystick_nr=a;
            calib_all_controllers();
          }
        }
        else
        {
          if (data->choose_step[c]==3) //noch nicht kalibriert
          {
            ZWattribute->realjoystick[a].button[b]=0;
            maindata.activplayer=c;
            maindata.player[c].joystick_nr=a;
            data->choose_step[c]=4;
            maindata.ingame_kalib_step=0;
            maindata.ingame_kalib_joystick=a;
          }
          else
          {
            if (*(ZWattribute->joystick[c].button[0]) && data->choose_step[c]==1)
            {
              *(ZWattribute->joystick[c].button[0])=0;
              data->choose_step[c]=2;
            }
            if (*(ZWattribute->joystick[c].button[2]))
            {
              *(ZWattribute->joystick[c].button[2])=0;
              switch (data->choose_step[c])
              {
                case 2: data->choose_step[c]=1; break;
                case 1: data->choose_step[c]=0; 
                        for (;c<7;c++)
                        {
                          data->play_with[c]=data->play_with[c+1];
                          data->choose_step[c]=data->choose_step[c+1];
                          ZWattribute->joystick[c]=ZWattribute->joystick[c+1];
                          maindata.player[c]=maindata.player[c+1];
                        }
                        data->play_with[7]=-1;
                        data->choose_step[7]=0;
                        ZWattribute->joystick[7].button[0]=&(ZWattribute->joystick[7].nullbutton[0]);
                        ZWattribute->joystick[7].button[1]=&(ZWattribute->joystick[7].nullbutton[1]);
                        ZWattribute->joystick[7].button[2]=&(ZWattribute->joystick[7].nullbutton[2]);
                        ZWattribute->joystick[7].button[3]=&(ZWattribute->joystick[7].nullbutton[3]);
                        ZWattribute->joystick[7].axis[0]=&(ZWattribute->joystick[7].nullaxis[0]);
                        ZWattribute->joystick[7].axis[1]=&(ZWattribute->joystick[7].nullaxis[1]);
                        playernum--;
                        break;
              }
            }
          }
        }
        a=ZWattribute->realjoystickcount;
        break;
      }  
      
  if (playernum>0)
  {
    for (c=0;c<playernum;c++)
      if (data->choose_step[c]!=2)
        break;
    if (c==playernum)
      data->fade_dir=2;
  }
  
  return 0;
}

int calc_newgame_login_thread_new(pmenudata data)
{
  return 0;
}


/*void draw_newgame_login(pmenudata data)
{
  draw_ballon_background(&menudata);
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);

  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);
  float a;
  for (a=0;a<8;a++)
  {
    if (data->play_with[(int)(trunc(a))]==-1)
    {
      glColor4f(0.7,0.7,0.7,0.7);
      ZWdrawtextmiddle(text,-1.12+a*0.1,0.72-a*0.4,-4,(char*)"Taste drücken um mitzuspielen",0.3);
      glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
      ZWdrawtextmiddle(text,-1.1+a*0.1,0.7-a*0.4,-4,(char*)"Taste drücken um mitzuspielen",0.3);
    }
    else
    {
      glColor4f(0.7,0.7,0.7,0.7);
      char buffer[32];
      if (strlen(ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name)>30)
      {
        char temp=ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name[28];
        ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name[28]=0;
        sprintf(buffer,"%s...",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name);
        ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name[28]=temp;
      }
      else
        sprintf(buffer,"%s",ZWattribute->realjoystick[data->play_with[(int)(trunc(a))]].name);  
        
      ZWdrawtextmiddle(text,-1.12+a*0.1,0.72-a*0.4,-4,buffer,0.3);
      glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
      ZWdrawtextmiddle(text,-1.1+a*0.1,0.7-a*0.4,-4,buffer,0.3);
    }
  }
  //Counter:
  char buffer[10];
  sprintf(buffer,"%i",(int)(trunc(data->counter)));
  glColor4f(0.7,0.7,0.7,0.7);
  ZWdrawtextright(text,2.39,-2.04,-4,(char*)"Weiter in:",0.2);
  ZWdrawtextmiddle(text,2.71,-2.11,-4,buffer,0.8);
  glColor4f(COLOR_BASE+sin((data->rotation-40.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextright(text,2.4,-2.05,-4,(char*)"Weiter in:",0.2);
  ZWdrawtextmiddle(text,2.75,-2.15,-4,buffer,0.8);
    

  glDepthFunc(GL_LEQUAL);   
}

int calc_newgame_login(pmenudata data)
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
  
  //Schauen, wer alles mitspielt.
  int b;
  for (a=0;a<ZWattribute->realjoystickcount;a++)
    for (b=0;b<ZWattribute->realjoystick[a].buttoncount;b++)
      if (ZWattribute->realjoystick[a].button[b]==1) 
      {
        ZWattribute->realjoystick[a].button[b]=0;
        //Schauen, ob der Controller überhaupt genug Achsen und Buttons hat!
        if (ZWattribute->realjoystick[a].buttoncount<4 || ZWattribute->realjoystick[a].axiscount<2)
          continue;
        data->counter=OPENPARTY_WAITTIME;
        //Schauen, ob Joystick a schon verwendet wird und Suche nach dem letzten Element:
        int c;
        int lastelem=8;
        for (c=7;c>=0;c--)
        {
          if (data->play_with[c]==-1)
            lastelem=c;
          if (data->play_with[c]==a)
            break;
        }
        if (c<0 && lastelem<8) //nicht vorhanden
          data->play_with[lastelem]=a;
        else
          for (;c<7;c++)
            data->play_with[c]=data->play_with[c+1];
      }  
  
  return 0;
}

int calc_newgame_login_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;
  
  //Counter runterzählen
  if (data->play_with[0]!=-1)
  {
    data->counter-=((float)(ZWattribute->steps))/1000.0;
    if (data->counter<0.000)
		{
			int a;
			for (a=0;a<8;a++)
			  if (data->play_with[a]>-1)
				  maindata.player[a].joystick_nr=data->play_with[a];
      data->fade_dir=2;
		}
  }
  return 0;
}*/

