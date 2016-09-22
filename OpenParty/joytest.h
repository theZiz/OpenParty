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
void draw_joytest(pmenudata data)
{

  draw_ballon_background(data);
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);
  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);
  glTranslatef(-0.25,0,0);

  float dist_x=0.0;
  float dist_y=0.22;
  float dist_a=40.0/9.0;
  float zoom=0.28;
  
	int zeile=0;
	int spalte=0;
	int joy=0;
	
	while (joy<ZWattribute->realjoystickcount && zeile<10)
	{
		glColor4f(0.7,0.7,0.7,0.7);
		ZWdrawtext(text,-2.73+(float)zeile*dist_x,0.53-(float)zeile*dist_y,-4,ZWattribute->realjoystick[joy].name,zoom*0.8);
		glColor4f(COLOR_BASE+sin((data->rotation-(float)zeile*dist_a)*M_PI/180.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-(float)zeile*dist_a)*M_PI/90.0)*COLOR_MULT,
							COLOR_BASE+sin((data->rotation-(float)zeile*dist_a)*M_PI/45.0)*COLOR_MULT,1);
		ZWdrawtext(text,-2.71+(float)zeile*dist_x,0.5-(float)zeile*dist_y,-4,ZWattribute->realjoystick[joy].name,zoom*0.8);
		zeile++;
		spalte=0;
		if (joy>=ZWattribute->realjoystickcount || zeile>=10)
		  break;
		int a;
		for (a=0;a<ZWattribute->realjoystick[joy].buttoncount;a++)
		{
			char buffer[8];
			sprintf(buffer,"%i:[ ]",a);
			if (ZWattribute->realjoystick[joy].button[a])
			  glColor4f(1.0,0.5,0.5,0.8);
			else
			  glColor4f(0,0,0,0.8);
			ZWdrawtext(text,-2.7+(float)zeile*dist_x+0.65*(float)spalte,0.5-(float)zeile*dist_y,-4,buffer,zoom*0.8);
			spalte++;
			if (spalte>5 && a!=ZWattribute->realjoystick[joy].buttoncount-1)
			{
			  zeile++;
				spalte=0;
				if (joy>=ZWattribute->realjoystickcount || zeile>=10)
					break;
			}
		}
		for (a=0;a<ZWattribute->realjoystick[joy].axiscount;a++)
		{
			char buffer[8];
			if (a%2==0)
			  sprintf(buffer,"%i:[<]",a);
			else
			  sprintf(buffer,"%i:[^]",a);
			if (ZWattribute->realjoystick[joy].axis[a]<0)
			  glColor4f(1.0,0.5,0.5,0.8);
			else
			  glColor4f(0,0,0,0.8);			
			ZWdrawtext(text,-2.7+(float)zeile*dist_x+0.65*(float)spalte,0.5-(float)zeile*dist_y,-4,buffer,zoom*0.8);
			if (a%2==0)
			  sprintf(buffer,"[>]");
			else
			  sprintf(buffer,"[v]");
			if (ZWattribute->realjoystick[joy].axis[a]>0)
			  glColor4f(1.0,0.5,0.5,0.8);
			else
			  glColor4f(0,0,0,0.8);
			ZWdrawtext(text,-2.7+(float)zeile*dist_x+0.65*(float)spalte+0.4,0.5-(float)zeile*dist_y,-4,buffer,zoom*0.8);
			spalte++;
			if (spalte>5 && a!=ZWattribute->realjoystick[joy].axiscount-1)
			{
			  zeile++;
				spalte=0;
				if (joy>=ZWattribute->realjoystickcount || zeile>=10)
					break;
			}
		}		zeile++;
		if (joy>=ZWattribute->realjoystickcount || zeile>=10)
		  break;
		joy++;
	}

  glDepthFunc(GL_LEQUAL);   
}

int calc_joytest(pmenudata data)
{
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
      ZWrefreshdrawlist(&(data->jumpball),lightquali);
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

int calc_joytest_thread(pmenudata data)
{
  //Rotation
  data->rotation+=(float)(ZWattribute->steps)/42.0;
  while (data->rotation>=360.0)
    data->rotation-=360.0;

  if (data->fade_dir!=0) return 0;

/*  //Menuauswahl die zweite
  if (is_any_button_down(1))
  {
    Mix_PlayChannel(-1, snd_plop, 0);
    data->fade_dir=2;
  }*/
  
  return 0;
}
