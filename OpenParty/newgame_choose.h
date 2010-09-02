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
void draw_newgame_choose(pmenudata data)
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
      ZWdrawtextmiddle(text,-1.12+a*0.1,0.72-a*0.4,-4,(char*)"Slot nicht besetzt",0.3);
      glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
      ZWdrawtextmiddle(text,-1.1+a*0.1,0.7-a*0.4,-4,(char*)"Slot nicht besetzt",0.3);
    }
    else
    {
      if (data->choose_step[(int)(trunc(a))]==0)
      {
        char buffer1[64]="Wähle eine Farbe mit [<] und [>]";
        char buffer2[32]="";
        glColor4f(0.7,0.7,0.7,0.7);        
        ZWdrawtextmiddle(text,-1.12+a*0.1,0.72-a*0.4,-4,buffer1,0.3);
        glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
        ZWdrawtextmiddle(text,-1.1+a*0.1,0.7-a*0.4,-4,buffer1,0.3);
        glColor4f(0.7,0.7,0.7,0.7);
        ZWdrawtextmiddle(text,-1.07+a*0.1,0.52-a*0.4,-4,buffer2,0.3);
        glColor4f(COLOR_BASE+sin((data->rotation-40.0*(a+0.5)/7.0)*M_PI/180.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*(a+0.5)/7.0)*M_PI/90.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*(a+0.5)/7.0)*M_PI/45.0)*COLOR_MULT,1);
        ZWdrawtextmiddle(text,-1.05+a*0.1,0.5-a*0.4,-4,buffer2,0.3);
        float b;
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        float beginx=-2.85;
        float beginy=0.57;
        float size=0.135;
        for (b=0;b<27;b++)
        {
          float fr=((float)(((int)trunc(b    ))%3))/2.0;
          float fg=((float)(((int)trunc(b/3.0))%3))/2.0;
          float fb=((float)(((int)trunc(b/9.0))%3))/2.0;
          float fa=1.0-((float)(data->blocked_color[(int)trunc(b)]))*0.5;
          glColor4f(fr,fg,fb,fa);
          glVertex3f(beginx+a*0.1+b*size-size*fa/2.0,beginy-a*0.4+size*fa/2.0,-4);
          glVertex3f(beginx+a*0.1+b*size-size*fa/2.0,beginy-a*0.4-size*fa/2.0,-4);
          glVertex3f(beginx+a*0.1+b*size+size*fa/2.0,beginy-a*0.4-size*fa/2.0,-4);
          glVertex3f(beginx+a*0.1+b*size+size*fa/2.0,beginy-a*0.4+size*fa/2.0,-4);
        }
        glEnd();
        glEnable(GL_TEXTURE_2D);
        //ausgwähltes hervorheben:
        b=((float)(data->color_choose[((int)trunc(a))]));
        float fr=((float)(((int)trunc(b    ))%3))/2.0;
        float fg=((float)(((int)trunc(b/3.0))%3))/2.0;
        float fb=((float)(((int)trunc(b/9.0))%3))/2.0;
        glColor4f(fr,fg,fb,1.0-((float)(data->blocked_color[(int)trunc(b)]))*0.5);
        ZWdrawsprite(maintex,beginx+a*0.1+b*size,beginy-a*0.4,-4,0.1,0.1,2,2);
      }
      if (data->choose_step[(int)(trunc(a))]==1)
      {
        char buffer1[64]="Wähle ein Gesicht [<][>]";
        char buffer2[32]="[C]: Zurück [A]: Weiter";
        glColor4f(0.7,0.7,0.7,0.7);        
        ZWdrawtext(text,-2.87+a*0.1,0.72-a*0.4,-4,buffer1,0.3);
        glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
        ZWdrawtext(text,-2.85+a*0.1,0.7-a*0.4,-4,buffer1,0.3);
        glColor4f(0.7,0.7,0.7,0.7);
        ZWdrawtext(text,-2.82+a*0.1,0.52-a*0.4,-4,buffer2,0.3);
        glColor4f(COLOR_BASE+sin((data->rotation-40.0*(a+1.0)/14.0)*M_PI/180.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*(a+1.0)/14.0)*M_PI/90.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*(a+1.0)/14.0)*M_PI/45.0)*COLOR_MULT,1);
        ZWdrawtext(text,-2.8+a*0.1,0.5-a*0.4,-4,buffer2,0.3);
        glColor4f(maindata.player[(int)trunc(a)].color.r,
                  maindata.player[(int)trunc(a)].color.g,
                  maindata.player[(int)trunc(a)].color.b,1.0);
        glPushMatrix();
        glEnable(GL_LIGHTING);
        ZWenablelight(0);
        glTranslatef(a*0.1+(a/2.0-trunc(a/2.0))*0.7+0.25,0.7-a*0.4,-4);
        glScalef(0.25,0.25,0.25);
        glRotatef(sin(data->rotation*M_PI/180.0)*10.0,1,0,0);
        glRotatef(cos(data->rotation*M_PI/180.0)*10.0,0,1,0);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        ZWdrawobject(maindata.player[(int)trunc(a)].ball);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        ZWdisablelight(0);
        glDisable(GL_LIGHTING);
        glPopMatrix();
      }      
      if (data->choose_step[(int)(trunc(a))]==2)
      {
        char buffer1[32]="     Ready!";
        char buffer2[32]="    [C]: Zurück";
        glColor4f(0.7,0.7,0.7,0.7);        
        ZWdrawtext(text,-2.87+a*0.1,0.72-a*0.4,-4,buffer1,0.3);
        glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
        ZWdrawtext(text,-2.85+a*0.1,0.7-a*0.4,-4,buffer1,0.3);
        glColor4f(0.7,0.7,0.7,0.7);
        ZWdrawtext(text,-2.82+a*0.1,0.52-a*0.4,-4,buffer2,0.3);
        glColor4f(COLOR_BASE+sin((data->rotation-40.0*(a+1.0)/14.0)*M_PI/180.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*(a+1.0)/14.0)*M_PI/90.0)*COLOR_MULT,
                  COLOR_BASE+sin((data->rotation-40.0*(a+1.0)/14.0)*M_PI/45.0)*COLOR_MULT,1);
        ZWdrawtext(text,-2.8+a*0.1,0.5-a*0.4,-4,buffer2,0.3);
        glColor4f(maindata.player[(int)trunc(a)].color.r,
                  maindata.player[(int)trunc(a)].color.g,
                  maindata.player[(int)trunc(a)].color.b,1.0);
        glPushMatrix();
        glEnable(GL_LIGHTING);
        ZWenablelight(0);
        glTranslatef(a*0.1+(a/2.0-trunc(a/2.0))*0.7+0.25,0.7-a*0.4,-4);
        glScalef(0.25,0.25,0.25);
        glRotatef(sin(data->rotation*M_PI/180.0)*10.0,1,0,0);
        glRotatef(cos(data->rotation*M_PI/180.0)*10.0,0,1,0);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        ZWdrawobject(maindata.player[(int)trunc(a)].ball);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        ZWdisablelight(0);
        glDisable(GL_LIGHTING);
        glPopMatrix();
      }      
    }
  }    

  glDepthFunc(GL_LEQUAL);   
}

int calc_newgame_choose(pmenudata data)
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

  //Wahl der Gesichter
  for (a=0;a<playernum;a++)
  {
    if (data->choose_step[a]==1)
    {
      if (*(ZWattribute->joystick[a].axis[0])>0)
      {
        maindata.player[a].facenr++;
        if (maindata.player[a].facenr>MAX_FACES-1)
          maindata.player[a].facenr=0;
        *(ZWattribute->joystick[a].axis[0])=0;
        ZWdeleteobject(maindata.player[a].ball);
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
      if (*(ZWattribute->joystick[a].axis[0])<0)
      {
        maindata.player[a].facenr--;
        if (maindata.player[a].facenr<0)
          maindata.player[a].facenr=MAX_FACES-1;
        *(ZWattribute->joystick[a].axis[0])=0;
        ZWdeleteobject(maindata.player[a].ball);
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
    }
    if (data->choose_step[a]==1)
    {
      if (*(ZWattribute->joystick[a].button[0]))
      {
        data->choose_step[a]++;
        *(ZWattribute->joystick[a].button[0])=0;
      }
      if (*(ZWattribute->joystick[a].button[2]))
      {
        data->choose_step[a]=0;
        memset(&(data->blocked_color[0]),0,27);
        int b;
        for (b=0;b<playernum;b++)
        {
          if (data->choose_step[b]==0) continue;
          int c;
          int tr=(data->color_choose[b]  )%3;
          int tg=(data->color_choose[b]/3)%3;
          int tb=(data->color_choose[b]/9)%3;
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
        *(ZWattribute->joystick[a].button[2])=0;
      }
    }
    if (data->choose_step[a]==2)
    {
      if (*(ZWattribute->joystick[a].button[2]))
      {
        data->choose_step[a]--;
        *(ZWattribute->joystick[a].button[2])=0;
      }
    }
  }

  
  return 0;
}

int calc_newgame_choose_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;
  
  //Wahl der Farbe:
  int a;
  for (a=0;a<playernum;a++)
  {
    if (data->choose_step[a]==0)
    {
      if (*(ZWattribute->joystick[a].axis[0])>0)
      {
        data->color_choose[a]++;
        if (data->color_choose[a]>26)
          data->color_choose[a]=0;
        *(ZWattribute->joystick[a].axis[0])=0;
      }
      if (*(ZWattribute->joystick[a].axis[0])<0)
      {
        data->color_choose[a]--;
        if (data->color_choose[a]<0)
          data->color_choose[a]=26;
        *(ZWattribute->joystick[a].axis[0])=0;
      }
      if (*(ZWattribute->joystick[a].button[0]) && data->blocked_color[data->color_choose[a]]==0)
      {
        int b;
        int tr=(data->color_choose[a]  )%3;
        int tg=(data->color_choose[a]/3)%3;
        int tb=(data->color_choose[a]/9)%3;
        for (b=0;b<27;b++)
        {
          int br=b%3;
          int bg=(b/3)%3;
          int bb=(b/9)%3;
          if (tr==br && tg==bg && (bb-tb)*(bb-tb)<=1 && (bb+tb>1 || (tr==2 && tg==2) || bb==tb))
            data->blocked_color[b]=1;      
          if (tg==bg && tb==bb && (br-tr)*(br-tr)<=1 && (br+tr>1 || (tb==2 && tg==2) || br==tr))
            data->blocked_color[b]=1;      
          if (tb==bb && tr==br && (bg-tg)*(bg-tg)<=1 && (bg+tg>1 || (tr==2 && tb==2) || bg==tg))
            data->blocked_color[b]=1;
        }
        maindata.player[a].color.r=((float)((data->color_choose[a]  )%3))/2.0;
        maindata.player[a].color.g=((float)((data->color_choose[a]/3)%3))/2.0;
        maindata.player[a].color.b=((float)((data->color_choose[a]/9)%3))/2.0;
        data->choose_step[a]++;
        *(ZWattribute->joystick[a].button[0])=0;
      }
    }
  }
  
  //Schauen, ob ALLE Spiele auf "Ready" sind:
  for (a=0;a<playernum;a++)
    if (data->choose_step[a]!=2) break;
  if (a==playernum)
    data->fade_dir=2;  
  return 0;
}

