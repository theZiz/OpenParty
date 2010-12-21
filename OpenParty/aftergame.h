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
void draw_aftergame(pmenudata data)
{
  draw_ballon_background(&menudata);
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);

  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);
  float a;
  for (a=0;a<playernum;a++)
  {
		char buffer1[64]="";
    char buffer2[32]="";
    if (strcmp(language,(char*)"german")==0)
    {
      if (teamsize>1)
        switch (maindata.player[(int)(trunc(a))].team)
        {
          case 0: sprintf(buffer1,"Münzen: %i Rubine: %i Team: Rot",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins); break;
          case 1: sprintf(buffer1,"Münzen: %i Rubine: %i Team: Grün",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins); break;
          case 2: sprintf(buffer1,"Münzen: %i Rubine: %i Team: Blau",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins); break;
          case 3: sprintf(buffer1,"Münzen: %i Rubine: %i Team: Gelb",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins); break;
        }
      else
        sprintf(buffer1,"Münzen: %i Rubine: %i Team: gabs nich'",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins);
      if (data->choose_step[(int)(trunc(a))]==0)
        sprintf(buffer2,"Drücke [A]");
      else
        sprintf(buffer2,"Ok");				
    }
    else
    {
      if (teamsize>1)
        switch (maindata.player[(int)(trunc(a))].team)
        {
          case 0: sprintf(buffer1,"Coins: %i Rubins: %i Team: Red",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins); break;
          case 1: sprintf(buffer1,"Coins: %i Rubins: %i Team: Green",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins); break;
          case 2: sprintf(buffer1,"Coins: %i Rubins: %i Team: Blue",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins); break;
          case 3: sprintf(buffer1,"Coins: %i Rubins: %i Team: Yellow",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins); break;
        }
      else
        sprintf(buffer1,"Count: %i Rubins: %i Team: doesn't exist'",maindata.player[(int)(trunc(a))].money,maindata.player[(int)(trunc(a))].rubins);
      if (data->choose_step[(int)(trunc(a))]==0)
        sprintf(buffer2,"Push [A]");
      else
        sprintf(buffer2,"Ok");				
    }
    
		glColor4f(0.7,0.7,0.7,0.7);        
		float fein=-1.3;
		float zusammen=0.02;
		ZWdrawtext(text,-1.11+fein+a*0.1,0.71-a*0.4-zusammen,-4,buffer1,0.2);
		glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
		ZWdrawtext(text,-1.1+fein+a*0.1,0.7-a*0.4-zusammen,-4,buffer1,0.2);
		glColor4f(0.7,0.7,0.7,0.7);
		ZWdrawtext(text,-1.06+fein+a*0.1,0.51-a*0.4+zusammen,-4,buffer2,0.2);
		glColor4f(COLOR_BASE+sin((data->rotation-40.0*(a+0.5)/7.0)*M_PI/180.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-40.0*(a+0.5)/7.0)*M_PI/90.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-40.0*(a+0.5)/7.0)*M_PI/45.0)*COLOR_MULT,1);
		ZWdrawtext(text,-1.05+fein+a*0.1,0.5-a*0.4+zusammen,-4,buffer2,0.2);
		glEnable(GL_LIGHTING);
    glPushMatrix();
		glTranslatef(fein+a*0.1-1.3,0.6-a*0.4,-4);
		glScalef(0.15,0.15,0.15);
		glRotatef(sin(data->rotation*M_PI/180.0)*10.0,1,0,0);
		glRotatef(cos(data->rotation*M_PI/180.0)*10.0,0,1,0);
		glColor4f(maindata.player[(int)trunc(a)].color.r,maindata.player[(int)trunc(a)].color.g,maindata.player[(int)trunc(a)].color.b,1);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		ZWdrawobject(maindata.player[(int)trunc(a)].ball);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glDisable(GL_LIGHTING);
		glPopMatrix();
		
  }    

  if (data->fade_dir==1)
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

int calc_aftergame(pmenudata data)
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
	
	int all=0;
	for (int a=0;a<playernum;a++)
	{
		if (*(ZWattribute->joystick[a].button[0]))
	    data->choose_step[a]=1;
	  all+=data->choose_step[a];
	}
	if (all==playernum)
		data->fade_dir=2;

  return 0;
}

int calc_aftergame_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;
  int a;
  for (a=0;a<playernum;a++)
    if (data->choose_step[a]!=1) break;
  if (a==playernum)
    data->fade_dir=2;  
  return 0;
}

