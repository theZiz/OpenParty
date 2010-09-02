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
void draw_newgame_calib(pmenudata data)
{
  draw_ballon_background(&menudata);
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);
		
  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);
  float a;
	for (a=0;a<5;a++)
	{
		char buffer[64]="Dummy";
		switch ((int)a)
		{
			case 0: sprintf(buffer,"Achtung! Folgende Spieler haben bisher keine"); break;
			case 1: sprintf(buffer,"Gamepadeinstellung und müssen ihr Gamepad im"); break;
			case 2: sprintf(buffer,"Spiel noch konfigurieren. Bis dahin gilt eine"); break;
			case 3: sprintf(buffer,"Standardkonfiguration, deren Belegung durch"); break;
			case 4: sprintf(buffer,"Testen ermittelt werden muss."); break;
						
		}
		glColor4f(0.7,0.7,0.7,0.7);        
		ZWdrawtextmiddle(text,-1.105+a*0.035,0.72-a*0.135,-4,buffer,0.2);
		glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/21.0)*M_PI/180.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-40.0*a/21.0)*M_PI/90.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-40.0*a/21.0)*M_PI/45.0)*COLOR_MULT,1);
		ZWdrawtextmiddle(text,-1.1+a*0.035,0.7-a*0.135,-4,buffer,0.2);		
	}
  for (a=3;a<3+playernum;a++)
  {
    char buffer[36]="-";
		if (!(maindata.player[(int)a-3].has_calib))
		{
			if (strlen(ZWattribute->realjoystick[data->play_with[(int)(trunc(a-3))]].name)>30)
			{
				char temp=ZWattribute->realjoystick[data->play_with[(int)(trunc(a-3))]].name[28];
				ZWattribute->realjoystick[data->play_with[(int)(trunc(a-3))]].name[28]=0;
				sprintf(buffer,"%s...",ZWattribute->realjoystick[data->play_with[(int)(trunc(a-3))]].name);
				ZWattribute->realjoystick[data->play_with[(int)(trunc(a-3))]].name[28]=temp;
			}
			else
				sprintf(buffer,"%s",ZWattribute->realjoystick[data->play_with[(int)(trunc(a-3))]].name);  
			if (data->calib_step[(int)a-3])
			  sprintf(buffer,"%s [A]",buffer);
		}
		glColor4f(0.7,0.7,0.7,0.7);        
		ZWdrawtextmiddle(text,-1.12+a*0.07,0.72-a*0.27,-4,buffer,0.36);
		glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/10.0)*M_PI/180.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-40.0*a/10.0)*M_PI/90.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-40.0*a/10.0)*M_PI/45.0)*COLOR_MULT,1);
		ZWdrawtextmiddle(text,-1.1+a*0.07,0.7-a*0.27,-4,buffer,0.36);
  }
	
  glColor4f(0.7,0.7,0.7,0.7);        
	ZWdrawtextmiddle(text,0.0,-2.19,-4,(char*)"Alle betreffenden Spieler müssen [A] finden und drücken.",0.18);
  glColor4f(COLOR_BASE+sin((data->rotation-40.0)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-40.0)*M_PI/45.0)*COLOR_MULT,1);
	ZWdrawtextmiddle(text,0.01 ,-2.2 ,-4,(char*)"Alle betreffenden Spieler müssen [A] finden und drücken.",0.18);
	
	
  glDepthFunc(GL_LEQUAL);   
}

int calc_newgame_calib(pmenudata data)
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

  return 0;
}

int calc_newgame_calib_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;
	
	//Alle bestätigt?
	int a;
	int result=1;
	for (a=0;a<playernum;a++)
  {
		if (*(ZWattribute->joystick[a].button[0]))
		{
			*(ZWattribute->joystick[a].button[0])=0;
			data->calib_step[a]=1;
		}
	  if (data->calib_step[a]==0)
		  result=0;
	}
	if (result)
	  data->fade_dir=2;
	  
  return 0;
}

