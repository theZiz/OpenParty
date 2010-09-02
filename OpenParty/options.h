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
void draw_options(pmenudata data)
{

  draw_ballon_background(data);
  
  //Menubeschriftung:
  float zoom[8];
  int a;
  for (a=0;a<8;a++)
    zoom[a]=1.0;  
  for (a=0;a<8;a++)
    if (data->menu_choose>((float)a)-1.0 && data->menu_choose<((float)a)+1.0)
      zoom[a]=1.0+cos((data->menu_choose-((float)a))*M_PI/2.0)/3.5;
  
  glLoadIdentity();
  glDepthFunc(GL_ALWAYS);
  glTranslatef((0.08-pow(1.0-data->fade*(1.0+sqrt(0.08)),2.0))*6.0,0,0);
  glTranslatef(-0.25,0,0);
  glColor4f(0.7,0.7,0.7,0.7);
  char texturtext[32],fonttext[32],lighttext[32],restext[32],strechtext[32],fullscreentext[32],volumetext[32];
  switch (texquali)
  {
    case 1: sprintf(texturtext,"Textur: hoch"); break;
    case 2: sprintf(texturtext,"Textur: mittel"); break;
    default: sprintf(texturtext,"Textur: niedrig");
  }
  switch (fontquali)
  {
    case 1: sprintf(fonttext,"Schrift: hoch"); break;
    case 2: sprintf(fonttext,"Schrift: mittel"); break;
    default: sprintf(fonttext,"Schrift: niedrig");  
  }
  
  sprintf(lighttext,"Sprache: %s",language);

  sprintf(restext,"Auflösung: %ix%i",resolutionx,resolutiony);
  switch (stretched)
  {
    case 0: sprintf(strechtext,"Stauchen: Nein"); break;
    default: sprintf(strechtext,"Stauchen: Ja");
  }
  switch (fullscreen)
  {
    case 0: sprintf(fullscreentext,"Fullscreen: Nein"); break;
    default: sprintf(fullscreentext,"Fullscreen: Ja");
  }
 
  sprintf(volumetext,"Volume: %i%%",(int)(round(global_volume*100.0)));

  float dist_x=1.2/7.0;
  float dist_y=2.4/7.0;
  float dist_a=40.0/7.0;
  
  ZWdrawtextmiddle(text,-1.13+0.0*dist_x,0.53-0.0*dist_y,-4,texturtext,0.35*zoom[0]);
  ZWdrawtextmiddle(text,-1.13+1.0*dist_x,0.53-1.0*dist_y,-4,fonttext,0.35*zoom[1]);
  ZWdrawtextmiddle(text,-1.13+2.0*dist_x,0.53-2.0*dist_y,-4,lighttext,0.35*zoom[2]);
  ZWdrawtextmiddle(text,-1.13+3.0*dist_x,0.53-3.0*dist_y,-4,restext,0.35*zoom[3]);
  ZWdrawtextmiddle(text,-1.13+4.0*dist_x,0.53-4.0*dist_y,-4,strechtext,0.35*zoom[4]);
  ZWdrawtextmiddle(text,-1.13+5.0*dist_x,0.53-5.0*dist_y,-4,fullscreentext,0.35*zoom[5]);
  ZWdrawtextmiddle(text,-1.13+6.0*dist_x,0.53-6.0*dist_y,-4,volumetext,0.35*zoom[6]);
  ZWdrawtextmiddle(text,-1.13+7.0*dist_x,0.53-7.0*dist_y,-4,(char*)"Zurück",0.35*zoom[7]);
  glColor4f(COLOR_BASE+sin((data->rotation-0.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-0.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-0.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+0.0*dist_x,0.5-0.0*dist_y,-4,texturtext,0.35*zoom[0]);
  glColor4f(COLOR_BASE+sin((data->rotation-1.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-1.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-1.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+1.0*dist_x,0.5-1.0*dist_y,-4,fonttext,0.35*zoom[1]);
  glColor4f(COLOR_BASE+sin((data->rotation-2.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-2.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-2.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+2.0*dist_x,0.5-2.0*dist_y,-4,lighttext,0.35*zoom[2]);
  glColor4f(COLOR_BASE+sin((data->rotation-3.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-3.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-3.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+3.0*dist_x,0.5-3.0*dist_y,-4,restext,0.35*zoom[3]);
  glColor4f(COLOR_BASE+sin((data->rotation-4.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-4.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-4.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+4.0*dist_x,0.5-4.0*dist_y,-4,strechtext,0.35*zoom[4]);
  glColor4f(COLOR_BASE+sin((data->rotation-5.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-5.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-5.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+5.0*dist_x,0.5-5.0*dist_y,-4,fullscreentext,0.35*zoom[5]);
  glColor4f(COLOR_BASE+sin((data->rotation-6.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-6.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-6.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+6.0*dist_x,0.5-6.0*dist_y,-4,volumetext,0.35*zoom[6]);
  glColor4f(COLOR_BASE+sin((data->rotation-7.0*dist_a)*M_PI/180.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-7.0*dist_a)*M_PI/90.0)*COLOR_MULT,
            COLOR_BASE+sin((data->rotation-7.0*dist_a)*M_PI/45.0)*COLOR_MULT,1);
  ZWdrawtextmiddle(text,-1.11+7.0*dist_x,0.5-7.0*dist_y,-4,(char*)"Zurück",0.35*zoom[7]);

  //Neustart:
  if (data->need_reboot)
  {
    char buffer[30];
    sprintf(buffer,"Neustart erforderlich");
    glColor4f(0,0,0,0.7);
    ZWdrawtext(text,-2.76,-2.24,-4,buffer,0.3);
    glColor4f(0.9+sin((data->rotation-40.0)*M_PI/180.0)/10.0,
              0.9+sin((data->rotation-40.0)*M_PI/90.0)/10.0,
              0.9+sin((data->rotation-40.0)*M_PI/45.0)/10.0,1);
    ZWdrawtext(text,-2.75,-2.25,-4,buffer,0.3);
  }

  glDepthFunc(GL_LEQUAL);   
}

int calc_options(pmenudata data)
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
      switch (data->dest)
      {
      
        case 7: return 1;
      }
    }
    return 0;
  }

  //Menuauswahl
  if ((is_any_axis_left(0) || is_any_axis_right(0)) && data->menu_delta==0)
  {
    int dazu=0;
    if (is_any_axis_left(1) )
      dazu=+1;
    else
    if (is_any_axis_right(1)) 
      dazu=-1;
    int modus;
    switch ((int)(trunc(data->menu_choose)))
    {
      case 0: if (texquali==1 && dazu>0)
                texquali=2;
              else 
              if (texquali==2 && dazu<0)
                texquali=1;
              else 
              if (texquali==2 && dazu>0)
                texquali=4;
              else
              if (texquali==4 && dazu<0)
                texquali=2;
              else 
                break;
              del_op_objects();
              del_objects();
              ZWdeleteobject(data->jumpball);
              ZWreloadalltextures(texquali);
              maintex=ZWloadtexturefromfileA((char*)DATAFOLDER"textures/openparty.png",0,texquali);  
							facetex=ZWloadtexturefromfileA((char*)DATAFOLDER"textures/faces.png",0,texquali);  
              sky_menu_tex=ZWloadtexturefromfileA((char*)DATAFOLDER"textures/sky_menu.png",0,texquali);              
              load_objects();
              load_op_objects();
              ZWload3dm(&(data->jumpball),(char*)DATAFOLDER"models/ball.3dm",facetex);
              for (int b=0;b<data->jumpball.pointnum;b++)
              {
                float dazu=((float)(data->facenr%4))*0.25;
                data->jumpball.p[b].u+=dazu;
                dazu=((float)(data->facenr/4))*0.25;
                data->jumpball.p[b].v+=dazu;    
              }
              ZWcreatedrawlist(&(data->jumpball),lightquali);
      break;
      case 1: fontquali+=dazu;
              if (fontquali<1)
              {
                fontquali=1;
                break;
              }
              if (fontquali>3)
              {
                fontquali=3;
                break;
              }
              ZWdeletefonttexture(text);
              switch (fontquali)
              {
                case 1: text=ZWcreatefonttexture((char*)DATAFOLDER"data/galapogo.ttf",2048,255,255,255); break;
                case 2: text=ZWcreatefonttexture((char*)DATAFOLDER"data/galapogo.ttf",1024,255,255,255); break;
                default: text=ZWcreatefonttexture((char*)DATAFOLDER"data/galapogo.ttf",512,255,255,255); break;          
              }
      break;
      case 2: //momentane Sprache suchen:
              {
                pstringchain momlan=possiblelanguages;
                pstringchain before=NULL;
                do
                {
                  before=momlan;
                  momlan=momlan->next;
                  if (strcmp(momlan->string,language)==0)
                    break;
                }
                while (momlan!=possiblelanguages);
                if (dazu>0)
                {
                  momlan=momlan->next;
                  sprintf(language,"%s",momlan->string);
                }
                else
                  sprintf(language,"%s",before->string);
              }
      break;
/*      case 2: lightquali-=dazu;
              if (lightquali<0)
              {
                lightquali=0;
                break;
              }
              if (lightquali>1)
              {
                lightquali=1;
                break;
              }
              del_op_objects();
              del_objects();
              ZWdeleteobject(data->jumpball);
              load_objects();
              load_op_objects();
              ZWload3dm(&(data->jumpball),(char*)DATAFOLDER"models/ball.3dm",maintex);
              for (int b=0;b<data->jumpball.pointnum;b++)
              {
                float dazu=((float)(data->facenr%4))*0.25;
                data->jumpball.p[b].u+=dazu;
                dazu=((float)(data->facenr/4))*0.25;
                data->jumpball.p[b].v+=dazu;    
              }
              ZWcreatedrawlist(&(data->jumpball),lightquali);
      break;*/
      case 3: //momentane Auflösung suchen
              for (modus=0;modus<ZWattribute->resolutioncount;modus++)
                if (resolutionx==ZWattribute->resolution[modus].x && resolutiony==ZWattribute->resolution[modus].y) break;
              modus+=dazu;
              if (modus<0)
                modus=ZWattribute->resolutioncount-1;
              if (modus>=ZWattribute->resolutioncount)
                modus=0;
              resolutionx=ZWattribute->resolution[modus].x;
              resolutiony=ZWattribute->resolution[modus].y;
              data->need_reboot=1;
      break;
      case 4: stretched-=dazu;
              if (stretched<0)
              {
                stretched=0;
                break;
              }
              if (stretched>1)
              {
                stretched=1;
                break;
              }
              ZWattribute->strech=stretched;
              ZWclearall();
              ZWflipbuffer();
              ZWglResizeWindow();
      break;
      case 5: fullscreen-=dazu;
              if (fullscreen<0)
              {
                fullscreen=0;
                break;
              }
              if (fullscreen>1)
              {
                fullscreen=1;
                break;
              }
              data->need_reboot=1;
      break;
      case 6: //global_volume-=(float)(dazu)/10.0;
              global_volume=(float)((int)(round(global_volume*10.0))-dazu)/10.0;
              if (global_volume<0.0)
              {
                global_volume=0.0;
                break;
              }
              if (global_volume>1.0)
              {
                global_volume=1.0;
                break;
              }
              Mix_VolumeChunk(snd_dice,(int)(round(32.0*global_volume)));
              Mix_VolumeChunk(snd_dice_plop,(int)(round(64.0*global_volume)));
              Mix_VolumeChunk(snd_plop,(int)(round(128.0*global_volume)));
              Mix_VolumeChunk(snd_jump,(int)(round(128.0*global_volume)));
              Mix_VolumeChunk(snd_positive,(int)(round(8.0*global_volume)));
              Mix_VolumeChunk(snd_negative,(int)(round(8.0*global_volume)));
              Mix_VolumeMusic((int)(round(128.0*global_volume)));
      break;
    }
  }
  return 0;
}

int calc_options_thread(pmenudata data)
{
  //Rotation
  data->rotation+=(float)(ZWattribute->steps)/42.0;
  while (data->rotation>=360.0)
    data->rotation-=360.0;

  if (data->fade_dir!=0) return 0;

  //Menuauswahl
  if (is_any_axis_down(0) && data->menu_delta==0)
  {
    data->old_choose=data->menu_choose;
    data->menu_delta=0.001;
  }
  if (is_any_axis_up(0) && data->menu_delta==0)
  {
    data->old_choose=data->menu_choose;
    data->menu_delta=-0.001;
  }
  if (data->menu_delta!=0.0)
  {
    if (data->menu_delta<0.0)
    {
      //Nach oben werden Bewegungen gleich im Keim erstickt
      if (data->menu_choose<=0.0)
      {
        data->menu_choose=0.0;
        data->menu_delta=0.0;
      }
      else
      {
        data->menu_delta-=(float)(ZWattribute->steps)/150.0;
        data->menu_choose=data->old_choose+data->menu_delta;
        if (data->menu_delta<=-1.0)
        {
          data->menu_choose=trunc(data->old_choose)-1.0;
          data->menu_delta=0.0;
        }
      }
    }
    else
    {
      //Nach oben werden Bewegungen gleich im Keim erstickt
      if (data->menu_choose>=7.0)
      {
        data->menu_choose=7.0;
        data->menu_delta=0.0;
      }
      else
      {
        data->menu_delta+=(float)(ZWattribute->steps)/150.0;
        data->menu_choose=data->old_choose+data->menu_delta;
        if (data->menu_delta>=1.0)
        {
          data->menu_choose=trunc(data->old_choose)+1.0;
          data->menu_delta=0.0;
        }
      }
    }
  }
  //Menuauswahl die zweite
  if (is_any_button_down(1) && data->menu_delta==0 && data->menu_choose==7)
  {
    Mix_PlayChannel(-1, snd_plop, 0);
    data->dest=(int)(trunc(data->menu_choose));
    data->fade_dir=2;
    save_options();
  }
  
  return 0;
}

