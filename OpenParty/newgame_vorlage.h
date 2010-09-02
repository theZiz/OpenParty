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
      char buffer1[32]="";
      char buffer2[32]="";
      glColor4f(0.7,0.7,0.7,0.7);        
      ZWdrawtextmiddle(text,-1.12+a*0.1,0.72-a*0.4,-4,buffer1,0.3);
      glColor4f(COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/180.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/90.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*a/7.0)*M_PI/45.0)*COLOR_MULT,1);
      ZWdrawtextmiddle(text,-1.1+a*0.1,0.7-a*0.4,-4,buffer1,0.3);
      glColor4f(0.7,0.7,0.7,0.7);
      ZWdrawtextmiddle(text,-1.07+a*0.1,0.52-a*0.4,-4,buffer2,0.3);
      glColor4f(COLOR_BASE+sin((data->rotation-40.0*(a+1.0)/14.0)*M_PI/180.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*(a+1.0)/14.0)*M_PI/90.0)*COLOR_MULT,
                COLOR_BASE+sin((data->rotation-40.0*(a+1.0)/14.0)*M_PI/45.0)*COLOR_MULT,1);
      ZWdrawtextmiddle(text,-1.05+a*0.1,0.5-a*0.4,-4,buffer2,0.3);
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
  
  return 0;
}

int calc_newgame_choose_thread(pmenudata data)
{
  if (data->fade_dir!=0) return 0;
  
  return 0;
}

