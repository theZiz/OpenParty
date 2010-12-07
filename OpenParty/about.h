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
void draw_about(pmenudata data)
{

  draw_ballon_background(data);
  
  //Menubeschriftung:
  
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);
  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);
  glTranslatef(-0.25,0,0);
  glColor4f(0.7,0.7,0.7,0.7);

  float dist_x=1.2/7.0*0.8;
  float dist_y=2.4/7.0;
  float dist_a=40.0/7.0;
  float zoom=0.28;
  
  ZWdrawtextmiddle(text,-1.12+0.0*dist_x,0.52-0.0*dist_y,-4,(char*)"Open Party Version 0.8.0",zoom*1.2);
  ZWdrawtextmiddle(text,-1.12+1.0*dist_x,0.52-0.8*dist_y,-4,(char*)"licensed under MPL and LGPL",zoom*0.8);
  ZWdrawtextmiddle(text,-1.12+2.0*dist_x,0.52-1.6*dist_y,-4,(char*)"mainly developed by Ziz",zoom*1.1);
  ZWdrawtextmiddle(text,-1.12+3.0*dist_x,0.52-2.45*dist_y,-4,(char*)"Main menu music from Kevin MacLeod",zoom*0.85);
  ZWdrawtextmiddle(text,-1.12+4.0*dist_x,0.52-3.3*dist_y,-4,(char*)"Big Thanks to www.DelphiGL.com, especially",zoom*0.85);
  ZWdrawtextmiddle(text,-1.12+5.0*dist_x,0.52-4.2*dist_y,-4,(char*)"Horazont (e.g. as translation monkey),",zoom*0.9);
  ZWdrawtextmiddle(text,-1.12+6.0*dist_x,0.52-5.0*dist_y,-4,(char*)"Sellman, i0n0s, Flash and other...",zoom*0.9);
  ZWdrawtextmiddle(text,-1.12+7.0*dist_x,0.52-7.0*dist_y,-4,(char*)"Back with any button",zoom*0.7);
  glColor4f(COLOR_BASE+sin((data->rotation-0.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-0.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-0.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+0.0*dist_x,0.5-0.0*dist_y,-4,(char*)"Open Party Version 0.8.0",zoom*1.2);
  glColor4f(COLOR_BASE+sin((data->rotation-1.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-1.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-1.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+1.0*dist_x,0.5-0.8*dist_y,-4,(char*)"licensed under MPL and LGPL",zoom*0.8);
  glColor4f(COLOR_BASE+sin((data->rotation-2.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-2.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-2.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+2.0*dist_x,0.5-1.6*dist_y,-4,(char*)"mainly developed by Ziz",zoom*1.1);
  glColor4f(COLOR_BASE+sin((data->rotation-3.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-3.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-3.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+3.0*dist_x,0.5-2.45*dist_y,-4,(char*)"Main menu music from Kevin MacLeod",zoom*0.85);
  glColor4f(COLOR_BASE+sin((data->rotation-4.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-4.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-4.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+4.0*dist_x,0.5-3.3*dist_y,-4,(char*)"Big Thanks to www.DelphiGL.com, especially",zoom*0.85);
  glColor4f(COLOR_BASE+sin((data->rotation-5.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-5.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-5.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+5.0*dist_x,0.5-4.2*dist_y,-4,(char*)"Horazont (e.g. as translation monkey),",zoom*0.9);
  glColor4f(COLOR_BASE+sin((data->rotation-6.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-6.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-6.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+6.0*dist_x,0.5-5.0*dist_y,-4,(char*)"Sellman, i0n0s, Flash and other...",zoom*0.9);
  glColor4f(COLOR_BASE+sin((data->rotation-7.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-7.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-7.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+7.0*dist_x,0.5-7.0*dist_y,-4,(char*)"Back with any button",zoom*0.7);

  glDepthFunc(GL_LEQUAL);   
}

int calc_about(pmenudata data)
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

int calc_about_thread(pmenudata data)
{
  //Rotation
  data->rotation+=(float)(ZWattribute->steps)/42.0;
  while (data->rotation>=360.0)
    data->rotation-=360.0;

  if (data->fade_dir!=0) return 0;

  //Menuauswahl die zweite
  if (is_any_button_down(1))
  {
    Mix_PlayChannel(-1, snd_plop, 0);
    data->fade_dir=2;
  }
  
  return 0;
}

