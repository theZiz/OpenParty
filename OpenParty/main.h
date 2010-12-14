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
void dummy()
{

}

void draw_main(pmaindata data)
{

  //Kamera einstellen I
	if (data->mainloop_pos==5 || (data->mainloop_pos==4 && data->item_in_script==3)) //Map oder Setselect
	{
    //glRotatef(-data->map_zoom,0,0,1);		
	  glRotatef(90,1,0,0);
	}
	else
	{
    glRotatef(-data->momcamerarotation.x,1,0,0);
    glRotatef(-data->momcamerarotation.y,0,1,0);    
    glRotatef(-data->momcamerarotation.z,0,0,1);
	}
    
  //Sky
    if (sky.texture[0]!=0)
    {
      glColor4f(1,1,1,1);    
      glScalef(sky.size.x,sky.size.y,sky.size.z);
      ZWdrawobject(sky.sky);
      glScalef(1.0/sky.size.x,1.0/sky.size.y,1.0/sky.size.z);
    }  

  //Kamera einstellen II
	if (data->mainloop_pos==5 || (data->mainloop_pos==4 && data->item_in_script==3)) //Map oder Setselect
    glTranslatef(-data->map_x,-data->map_zoom,-data->map_y);
	else
    glTranslatef(-data->momcameraposition.x,-data->momcameraposition.y,-data->momcameraposition.z);

  //Setselect Einstellung I
  if (data->mainloop_pos==4 && data->item_in_script==3)
    glTranslatef(0,0,data->player[data->activplayer].item[data->use_item_nr]->radius*0.1);
  
  //Frustum berechnen
    tZWfrustum frustum;
    ZWfrustum_calculate(&frustum);
  
  //Licht
    if (firstlight!=NULL)
      glEnable(GL_LIGHTING);
    refreshlightposition();

  //alle Spieler
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    int playercount;
    for (playercount=0;playercount<playernum;playercount++)
    {
      if (ZWfrustum_isboxwithin(&frustum,data->player[playercount].momposition.x,
                                         data->player[playercount].momposition.y,
                                         data->player[playercount].momposition.z,
                                         1,1,1)==0) continue;
      
      glPushMatrix();
        glTranslatef(data->player[playercount].momposition.x,
                     data->player[playercount].momposition.y,
                     data->player[playercount].momposition.z);
        float dazu=0;
        if (playercount==data->activplayer)
        {
          if (data->mainloop_pos==3)
          {
            if (data->turn_pos==5)
              dazu=sin(data->turn_dice_next*M_PI)*0.8;
            else
              dazu=sin(data->turn_jump_high*M_PI/2.0)*1.5; 
          }
          draw_dice(data);             
        }
        glTranslatef(0,0.5+dazu,0);
        glScalef(0.5*data->player[playercount].momsize,0.5,0.5*data->player[playercount].momsize);
        glRotatef(data->player[playercount].momface+data->player[playercount].savedcamera_y,0,1,0);
        glColor4f(data->player[playercount].color.r,data->player[playercount].color.g,
                  data->player[playercount].color.b,data->player[playercount].color.a);
        ZWdrawobject(data->player[playercount].ball);
      glPopMatrix();
    }
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
  //die Sets
    int setcount;
    //erstmal nur feste Objekte zeichnen
    for (setcount=0;setcount<maxsetlist && setlist[setcount]->realalpha>=1.0;setcount++)
    {
      if (setlist[setcount]->realalpha<=0.05 || strcmp(setlist[setcount]->type,"ghost")==0) continue;
      if (ZWfrustum_isboxwithin(&frustum,setlist[setcount]->position.x,setlist[setcount]->position.y,setlist[setcount]->position.z,
                                setlist[setcount]->scale.x,setlist[setcount]->scale.y,setlist[setcount]->scale.z)==0) continue;
      glPushMatrix();
        glTranslatef(setlist[setcount]->position.x,setlist[setcount]->position.y,setlist[setcount]->position.z);
        glRotatef(setlist[setcount]->rotation.x,1,0,0);
        glRotatef(setlist[setcount]->rotation.y,0,1,0);    
        glRotatef(setlist[setcount]->rotation.z,0,0,1);    
        glScalef(setlist[setcount]->scale.x,setlist[setcount]->scale.y,setlist[setcount]->scale.z);
        glColor4f(setlist[setcount]->color.r,setlist[setcount]->color.g,
                  setlist[setcount]->color.b,setlist[setcount]->realalpha);
        ZWdrawobject(setlist[setcount]->realobject->realobject);
        if (setlist[setcount]->triggertrap>=0)
        {
          glColor4f(data->player[setlist[setcount]->trapowner].color.r,data->player[setlist[setcount]->trapowner].color.g,data->player[setlist[setcount]->trapowner].color.b,0.7);
          glRotatef(-90,1,0,0);
          ZWdrawsprite(maintex,0,0,setlist[setcount]->maxy*1.1,setlist[setcount]->scale.x,setlist[setcount]->scale.z,4,1);		  
        }
      glPopMatrix();
    }
  
  //die Partikel
  glPushMatrix();
  glLoadIdentity();
  if (firstlight!=NULL)
    glDisable(GL_LIGHTING);
  ZWdrawpartikel(data->momcameraposition,data->momcamerarotation);
  if (firstlight!=NULL)
    glEnable(GL_LIGHTING);
  glPopMatrix();
  
    //durchsichtige Objekte zeichnen
    for (;setcount<maxsetlist;setcount++)
    {
      if (setlist[setcount]->realalpha<=0.05 || strcmp(setlist[setcount]->type,"ghost")==0) continue;
      if (ZWfrustum_isboxwithin(&frustum,setlist[setcount]->position.x,setlist[setcount]->position.y,setlist[setcount]->position.z,
                                setlist[setcount]->scale.x,setlist[setcount]->scale.y,setlist[setcount]->scale.z)==0) continue;
      glPushMatrix();
        glTranslatef(setlist[setcount]->position.x,setlist[setcount]->position.y,setlist[setcount]->position.z);
        glRotatef(setlist[setcount]->rotation.x,1,0,0);
        glRotatef(setlist[setcount]->rotation.y,0,1,0);    
        glRotatef(setlist[setcount]->rotation.z,0,0,1);    
        glScalef(setlist[setcount]->scale.x,setlist[setcount]->scale.y,setlist[setcount]->scale.z);
        glColor4f(setlist[setcount]->color.r,setlist[setcount]->color.g,
                  setlist[setcount]->color.b,setlist[setcount]->realalpha);
        ZWdrawobject(setlist[setcount]->realobject->realobject);
      glPopMatrix();
    }
  
  
   if (firstlight!=NULL)
    glDisable(GL_LIGHTING);
 
  //Nun wird ALLES gezeichnet
//  ZWdisablefog();
  glDepthFunc(GL_ALWAYS);
  
  glLoadIdentity();  
  //Setauswahlkugel
  if (data->mainloop_pos==4 && data->item_in_script==3)
  {
    glTranslatef(0,data->player[data->activplayer].item[data->use_item_nr]->radius*-0.1,-data->map_zoom);
    glScalef(data->player[data->activplayer].item[data->use_item_nr]->radius,data->player[data->activplayer].item[data->use_item_nr]->radius,data->player[data->activplayer].item[data->use_item_nr]->radius);
    glColor4f(1,1,1,0.3);
    ZWdrawobject(sphere);
    glLoadIdentity();
    glNormal3f(0,0,1);
    glColor4f(1,1,1,0.9);
    ZWdrawsprite(maintex, momchoiceset->position.x-data->map_x,data->map_y-momchoiceset->position.z+data->player[data->activplayer].item[data->use_item_nr]->radius*-0.1,momchoiceset->position.y-data->map_zoom,momchoiceset->scale.x,momchoiceset->scale.z,4,0);		  
  }
  glNormal3f(0,0,1);
    

  //Messages
  draw_messages();
    

  //die Anzeigen
  if (!(data->mainloop_pos==4 && data->item_in_script==3))
  for (int a=0;a<playernum;a++)
  {
    float size=1.0;
    float dazu=0.0;
    if (data->player[a].iscpu)
    {
      glColor4f(1,1,1,0.8);
      if (a==data->activplayer)
        ZWdrawtextmiddle(text,((float)a-((float)playernum)*0.5+0.5)*0.95,2.4-0.07,-5,(char*)"CPU",0.5);
      else
        ZWdrawtextmiddle(text,((float)a-((float)playernum)*0.5+0.5)*0.95,2.4-0.07,-5,(char*)"CPU",0.4);
    }
    char buffer[10];
    float r,g,b;
    switch (data->player[a].team)
    {
      case 0: sprintf(buffer,"Rot");   r=1  ;g=0.5;b=0.5; break;
      case 1: sprintf(buffer,"Grün");  r=0.5;g=1  ;b=0.5; break;
      case 2: sprintf(buffer,"Blau");  r=0.5;g=0.5;b=1  ; break;
      default: sprintf(buffer,"Gelb"); r=1  ;g=1  ;b=0.5; break;
    }
    if (a==data->activplayer)
    {
      if (teamsize>1)
      {
        glColor4f(0,0,0,0.8);
        ZWdrawtextmiddle(text,((float)a-((float)playernum)*0.5+0.5)*0.95+0.02,2.4-0.65+0.02,-5,buffer,0.3);
        glColor4f(r,g,b,1);
        ZWdrawtextmiddle(text,((float)a-((float)playernum)*0.5+0.5)*0.95     ,2.4-0.65     ,-5,buffer,0.3);    
      }
      drawplayeratt(((float)a-((float)playernum)*0.5+0.5)*0.95,2.4+dazu*0.45,-5,0.45*size,data->player[a].color.r,
               data->player[a].color.g,data->player[a].color.b,0.7,0.8,a%8,data->player[a].money,data->player[a].rubins,data->player[a].item[0],data->player[a].item[1],data->player[a].item[2]);
    }
    else
    {
      if (teamsize>1)
      {
        glColor4f(0,0,0,0.8);
        ZWdrawtextmiddle(text,((float)a-((float)playernum)*0.5+0.5)*0.95+0.02,2.4-0.5+0.02,-5,buffer,0.22);
        glColor4f(r,g,b,1);
        ZWdrawtextmiddle(text,((float)a-((float)playernum)*0.5+0.5)*0.95     ,2.4-0.5     ,-5,buffer,0.22);    
      }
      drawplayeratt(((float)a-((float)playernum)*0.5+0.5)*0.95,2.4+dazu*0.35,-5,0.35*size,data->player[a].color.r,
               data->player[a].color.g,data->player[a].color.b,0.35,0.7,a%8,data->player[a].money,data->player[a].rubins,data->player[a].item[0],data->player[a].item[1],data->player[a].item[2]);
    }
               
  }                

  //Team Anzeige
  glColor4f(1,1,1,0.5);
  if (teamsize>1)
    for (int a=0;a<playernum/teamsize;a++)
    {
      float x=-2.7;
      float y= 0.5;
      float z=-4.0;
      float s= 0.4;
      if (a%2==1)
        x=-x;
      if (a/2==1)
        y-=1.2;
      switch (a)
      {
        case 0:glColor4f(1,0,0,0.5);break;
        case 1:glColor4f(0,1,0,0.5);break;
        case 2:glColor4f(0,0,1,0.5);break;
        case 3:glColor4f(1,1,0,0.5);break;
      }
      glDisable(GL_TEXTURE_2D);
      glBegin(GL_QUADS);
        glVertex3f(x-0.3,y+0.5,-4);
        glVertex3f(x-0.3,y-0.5,-4);
        glVertex3f(x+0.3,y-0.5,-4);
        glVertex3f(x+0.3,y+0.5,-4);    
      glEnd();
      glEnable(GL_TEXTURE_2D);
      glColor4f(1,1,1,0.7);
      ZWdrawtextmiddle(text,x,y+0.35 ,-4,(char*)"Team",0.25);
      switch (a)
      {
        case 0:ZWdrawtextmiddle(text,x,y+0.15,-4,(char*)"Rot",0.25);break;
        case 1:ZWdrawtextmiddle(text,x,y+0.15,-4,(char*)"Grün",0.25);break;
        case 2:ZWdrawtextmiddle(text,x,y+0.15,-4,(char*)"Blau",0.25);break;
        case 3:ZWdrawtextmiddle(text,x,y+0.15,-4,(char*)"Gelb",0.25);break;
      }

      glTranslatef(x+0.15,y-0.2,z);
      glScalef(s,s,s);    
      glEnable(GL_LIGHTING);
      alllightsoff();

      ZWsetlightambient(maxlightnr+1,0.1,0.1,0.1);
      ZWsetlightdiffuse(maxlightnr+1,1.0,1.0,1.0);
      ZWsetlightposition(maxlightnr+1,0.0,0.0,1.0);
      ZWenablelight(maxlightnr+1);

      glDisable(GL_TEXTURE_2D);
      glPushMatrix();
      glTranslatef(-0.4,0.35,0);
      float w=(float)SDL_GetTicks()/10.0;
      glRotatef(45,1,0,0);
      glRotatef(w,0,1,0);
      glScalef(0.4,0.32,0.4);
      //durchsichtiges Gold:
      ZWsetmaterialambient(0.25,0.22,0.06,0.6);
      ZWsetmaterialdiffuse(0.35,0.31,0.09,0.6);
      ZWsetmaterialspecular(0.80,0.72,0.21,0.6);
      ZWdrawobject(coin);
      glPopMatrix();

      glPushMatrix();
      glTranslatef(-0.4,-0.45,0);
      glRotatef(45,1,0,0);
      glRotatef(w,0,1,0);
      glScalef(0.4,0.32,0.4);
      ZWsetmaterial(lrubin);
      ZWdrawobject(rubin);
      ZWsetmaterial(lnone);
      glPopMatrix();
      glEnable(GL_TEXTURE_2D);

      ZWdisablelight(maxlightnr+1);
      alllightson();
      glDisable(GL_LIGHTING);

      char buffer[16];
      int rubins=0;
      int money=0;
      for (int b=0;b<playernum;b++)
        if (data->player[b].team==a)
        {
          rubins+=data->player[b].rubins;
          money+=data->player[b].money;
        }
      sprintf(buffer,"%i",money);
      glColor4f(0,0,0,0.5);  
      ZWdrawtextmiddle(text,-0.4,0.15,0,buffer,0.8);
      glColor4f(1,1,1,1);  
      ZWdrawtextmiddle(text,-0.35,0.2,0,buffer,0.8);
      sprintf(buffer,"%i",rubins);
      glColor4f(0,0,0,0.5);  
      ZWdrawtextmiddle(text,-0.4,-0.55,0,buffer,0.8);
      glColor4f(1,1,1,1);  
      ZWdrawtextmiddle(text,-0.35,-0.5,0,buffer,0.8);

      glScalef(1.0/s,1.0/s,1.0/s);
      glTranslatef(-x-0.15,-y+0.2,-z);

    }
  
  //Runde
  char buffer[32];
  if (gametype<8)
    sprintf(buffer,"Runde %i von %i",data->round,(gametype+1)*5);
  else
    sprintf(buffer,"Runde %i",data->round);
  glColor4f(0,0,0,0.8);
  ZWdrawtext(text,-11.96,-8.66,-16,buffer,1.0);
  glColor4f(1,1,1,0.9);
  ZWdrawtext(text,-12   ,-8.7 ,-16,buffer,1.0);


  //etwaige Anzeigen ja nach Spielstatus
	if (data->gamestartinfo || data->mainloop_pos==0)
	  draw_gamestartinfo(data,1,1.0);
	if (data->special_step==1)
	  draw_item_shop(data);
	else
  switch (data->mainloop_pos)
  {
		case 5: //Map
		  glColor4f(1,1,1,0.8);
			ZWdrawsprite(dicetex,-1.5    ,-1.0,-2.8,0.4,0.4,4,12);		  
			ZWdrawsprite(dicetex, 1.5-0.4,-1.0,-2.8,0.4,0.4,4,13);		  
			ZWdrawsprite(dicetex, 1.5+0.4,-1.0,-2.8,0.4,0.4,4,14);		  
		break;
    case 8: //Kalibrieren
      draw_ingame_kalib(data);
    break;
    case 2: //Auswahl
      draw_choose(data);
    break;
    case 4: //Item-Auswahl
			switch (data->item_in_script)
			{
				case 0:draw_item_choose(data);break;
				case 2:draw_user_choose(data,data->activplayer,data->player[data->activplayer].item[data->use_item_nr]->friendlyfire);break;
        case 3:
          glColor4f(0,0,0,0.8);
          ZWdrawtextmiddle(text,-0.01, 1.49,-2.8,(char*)"Wähle mit [<] und [>] ein Feld aus, wo das Item drauf",0.18);
          ZWdrawtextmiddle(text,-0.01, 1.34,-2.8,(char*)"angewendet werden soll. Bestätige mit [A]. Zurück mit [C].",0.18);
          glColor4f(1,1,1,0.9);
          ZWdrawtextmiddle(text, 0   , 1.5 ,-2.8,(char*)"Wähle mit [<] und [>] ein Feld aus, wo das Item drauf",0.18);
          ZWdrawtextmiddle(text, 0   , 1.35,-2.8,(char*)"angewendet werden soll. Bestätige mit [A]. Zurück mit [C].",0.18);
        break;
			}
    break;
  }
	
	//Choose Circle
	//glLoadIdentity();
	draw_choosecircle(data);

  //Textboxen
  glLoadIdentity();
  drawtextsite();

  //Fasde Out/In
      glDisable(GL_TEXTURE_2D);
      glColor4f(0,0,0,data->fade_counter);
      glBegin(GL_QUADS);
        glVertex3f(-1,1,-1);
        glVertex3f(-1,-1,-1);
        glVertex3f(1,-1,-1);
        glVertex3f(1,1,-1);      
      glEnd();
      glEnable(GL_TEXTURE_2D);
  
  //Debug_begin
  glTranslatef(0,0,-15);
  //FPS
  sprintf(buffer,"%i",ZWattribute->fps);
  glColor4f(0,0,0,0.4);
  ZWdrawtextright(text,12,-8.2,-1,buffer,2.1);
  glColor4f(1,1,1,0.5);
  ZWdrawtextright(text,12,-8.2,-1,buffer,2.0);
  glDepthFunc(GL_LEQUAL); 
}

int calc_main(pmaindata data)
{
  if (shouldend==1)
  {
    //später kann an dieser Stelle ein Menü kommen
    return 1;
  }
  ZWcalcpartikel(ZWattribute->steps);
  calc_samples(ZWattribute->steps);
  //Sortieren
  recalcsetlist(data->momcameraposition.x,data->momcameraposition.y,data->momcameraposition.z,data);
  if (data->special_step==0)
	{
		if (data->mainloop_pos==3) //ausgelagertes "Case 3:" der Switchanweisung im Thread.
			calc_turn(data);
		else
		if (data->mainloop_pos==6) //ausgelagertes "Case 6:" der Switchanweisung im Thread.
		{
			data->fade_counter+=((float)(ZWattribute->steps))/FADE_OUT_TIME;
			if (data->fade_counter>=1.0)
			{
				data->fade_counter=1;
				data->mainloop_pos=1;
				data->activplayer++;
				if (data->activplayer>=playernum)
				{
					//Hier setzt das Minispiel ein:
					minigame_step(false,0,0,true);
					//Überprüfen zum Ende:
					if (gametype<8)
					{
						if (data->round>=(gametype+1)*5)
							return 1;
					}
					else
					if (gametype<12)
					{
						int rubins[8]={0,0,0,0,0,0,0,0};
						int money[8]={0,0,0,0,0,0,0,0};
						for (int b=0;b<playernum;b++)
							{
								rubins[data->player[b].team]+=data->player[b].rubins;
								money[data->player[b].team]+=data->player[b].money;
							}
					
						int maxmoney=0;
						for (int a=0;a<playernum/teamsize;a++)
							if (money[a]>maxmoney)
								maxmoney=money[a];
						if (maxmoney>=(gametype-7)*111)
							return 1;
					}
					else
					if (gametype<16)
					{
						int rubins[8]={0,0,0,0,0,0,0,0};
						int money[8]={0,0,0,0,0,0,0,0};
						for (int b=0;b<playernum;b++)
							{
								rubins[data->player[b].team]+=data->player[b].rubins;
								money[data->player[b].team]+=data->player[b].money;
							}
							
						int maxrubins=0;
						for (int a=0;a<playernum/teamsize;a++)
							if (rubins[a]>maxrubins)
								maxrubins=rubins[a];
						switch (gametype)
						{
							case 12: if (maxrubins>=3)
													return 1;
												break;            
							case 13: if (maxrubins>=5)
													return 1;
												break;
							case 14: if (maxrubins>=7)
													return 1;
												break;
							default: if (maxrubins>=10)
													return 1;
												break;
						}
					}
					data->activplayer=0;
					data->round++;
				}
				sync_all_position(0.01,data);
				data->destcameraposition=data->player[data->activplayer].momset->cameraposition;
				data->destcamerarotation=data->player[data->activplayer].momset->camerarotation;
				data->player[data->activplayer].time=0.01;
				data->time=0.01;
				delete_all_messages();
			}
		}
  }
  return 0;
}

int calc_main_thread(pmaindata data)
{
	calc_choosecircle(data);
	if (data->mainloop_pos!=4 && data->special_step==0)
    handle_textcontent(data->activplayer);

  srand(SDL_GetTicks()*time(NULL));
  int a;
  float alterwert,temp;
	int itemnum;
  //etwaige Berechnungen je nach mainloop_pos
	int iteration;
	for (iteration=0;iteration<ZWattribute->steps;iteration++)
	{
		if (data->special_step==1) // Itemshop
		{
			data->choose_angle+=0.1;
			if (data->item_choose_move==0 && (*(ZWattribute->joystick[data->activplayer].axis[0])<0 || (data->player[data->activplayer].iscpu && rand()%100==0)))
				data->item_choose_move=-1;
			if (data->item_choose_move==0 && *(ZWattribute->joystick[data->activplayer].axis[0])>0)
				data->item_choose_move=1;
			if (data->item_choose_move<0)
			{
				data->item_choose_move-=1<<1;
				if (data->item_choose_move<=-1000)
				{
					data->item_choose_move=0;
					data->item_choose_choose--;
					if (data->item_choose_choose<0)
						data->item_choose_choose=data->item_shop_item_count-1;
					createtextcontent(gettextbynrandlanguage(data->item_shop_item[data->item_choose_choose]->description_pos,language));
				}      
			}
			if (data->item_choose_move>0)
			{
				data->item_choose_move+=1<<1;
				if (data->item_choose_move>=1000)
				{
					data->item_choose_move=0;
					data->item_choose_choose++;
					if (data->item_choose_choose>data->item_shop_item_count-1)
						data->item_choose_choose=0;
					createtextcontent(gettextbynrandlanguage(data->item_shop_item[data->item_choose_choose]->description_pos,language));
				}
			}
			if (*(ZWattribute->joystick[data->activplayer].button[0])==1 || (data->player[data->activplayer].iscpu && rand()%500==0))
			{
				*(ZWattribute->joystick[data->activplayer].button[0])=0;
				if (data->item_shop_price[data->item_choose_choose]<=data->player[data->activplayer].money)
				{
					if (data->player[data->activplayer].item[2]==NULL)
					{
						char buffer[32];
						data->player[data->activplayer].money-=data->item_shop_price[data->item_choose_choose];
						sprintf(buffer,"-%i Münzen",data->item_shop_price[data->item_choose_choose]);
						new_message(buffer);
						Mix_PlayChannel(-1,snd_negative,0);
						data->item_shop_item_count--;
						if (data->player[data->activplayer].item[0]==NULL)
						  data->player[data->activplayer].item[0]=data->item_shop_item[data->item_choose_choose];
						else
						if (data->player[data->activplayer].item[1]==NULL)
						  data->player[data->activplayer].item[1]=data->item_shop_item[data->item_choose_choose];
						else
						  data->player[data->activplayer].item[2]=data->item_shop_item[data->item_choose_choose];
						if (data->item_shop_item_count<=0)
						{
							data->special_step=0;
							deletetextcontent();
						}
						else
						{
							int a;
							for (a=data->item_choose_choose;a<data->item_shop_item_count;a++)
							{
								data->item_shop_item[a]=data->item_shop_item[a+1];
								data->item_shop_price[a]=data->item_shop_price[a+1];
							}
							if (data->item_choose_choose>=data->item_shop_item_count)
							  data->item_choose_choose-=data->item_shop_item_count;
						}
					}
				}
			}
			if (*(ZWattribute->joystick[data->activplayer].button[2])==1 || (data->player[data->activplayer].iscpu && rand()%4000==0))
			{
				*(ZWattribute->joystick[data->activplayer].button[2])=0;
				data->special_step=0;
				deletetextcontent();
			}
		}
		else
		switch (data->mainloop_pos)
		{
			case 1: //Fade In
				data->fade_counter-=1.0/FADE_IN_TIME;
				if (data->fade_counter<=0.0)
				{
					data->fade_counter=0;
					data->choose_choose=0;
					data->choose_move=0;
					data->choose_angle=0;
					if (data->gamestartinfo)
					{
						data->mainloop_pos=0;
						data->gamestartinfo=0;
					}
					else
					{
						data->mainloop_pos++;
						if (!(data->player[data->activplayer].has_calib))
							data->choose_choose=2;
						if (data->player[data->activplayer].iscpu && data->player[data->activplayer].item[0]!=NULL && rand()%3==0)
						  data->choose_move=-1;
					}
				}
			break;
			case 2: //Choose
				data->choose_angle+=0.1;
				if (data->choose_angle>360.0)
				  data->choose_angle-=360.0;
				if (data->choose_move==0 && *(ZWattribute->joystick[data->activplayer].axis[0])<0)
					data->choose_move=-1;
				if (data->choose_move==0 && *(ZWattribute->joystick[data->activplayer].axis[0])>0)
					data->choose_move=1;
				if (data->choose_move<0)
				{
					data->choose_move-=1<<1;
					if (data->choose_move<=-1000)
					{
						data->choose_move=0;
						data->choose_choose--;
						if (data->choose_choose<0)
							data->choose_choose=3;
					}      
				}
				if (data->choose_move>0)
				{
					data->choose_move+=1<<1;
					if (data->choose_move>=1000)
					{
						data->choose_move=0;
						data->choose_choose++;
						if (data->choose_choose>3)
							data->choose_choose=0;
					}      
				}
				if ((data->player[data->activplayer].iscpu && data->choose_move==0 && 
				    ((data->choose_choose==0) || (data->choose_choose==3))) ||
						*(ZWattribute->joystick[data->activplayer].button[0])==1)
				{
					*(ZWattribute->joystick[data->activplayer].button[0])=0;
					if (data->choose_move>500) data->choose_choose++;
					if (data->choose_move<-500) data->choose_choose--;      
					data->choose_move=0;
					if (data->choose_choose<0) data->choose_choose=3;
					if (data->choose_choose>3) data->choose_choose=0;
					switch (data->choose_choose)
					{
						case 0: //Zug
							data->turn_dice_value[0]=rand()%6+1;
							data->turn_dice_next=0;
							data->turn_pos=1;
							data->mainloop_pos=3;
							//Würfelgeräusch starten:
							dice_channel=Mix_PlayChannel(-1,snd_dice,-1);
						break;
						case 1: //Map
							data->mainloop_pos=5;
							data->map_show_env=0;
							data->map_zoom=data->player[data->activplayer].momset->position.y+10;
							data->map_x=data->player[data->activplayer].momset->position.x;
							data->map_y=data->player[data->activplayer].momset->position.z;
						break;
						case 2: //Neu kalibrieren
							data->ingame_kalib_step=0;
							//Joysticknummer suchen, in dem gesucht wird, wo der 1. Button ist:
							data->ingame_kalib_joystick=data->player[data->activplayer].joystick_nr;
							/*for (a=0;a<ZWattribute->realjoystickcount;a++)
							{
								int b;
								for (b=0;b<ZWattribute->realjoystick[a].buttoncount;b++)
								{
									int c;
									for (c=0;c<4;c++)
										if (ZWattribute->joystick[data->activplayer].button[c]==&(ZWattribute->realjoystick[a].button[b]))
										{
											data->ingame_kalib_joystick=a;
											break;
										}
								}
							}*/					
							data->mainloop_pos=8;
						break;
						case 3: //Item
							if (data->player[data->activplayer].item[0]!=NULL)
							{
								data->mainloop_pos=4;
								data->item_choose_choose=0;
								data->item_choose_move=0;
								data->item_in_script=0;
								createtextcontent(gettextbynrandlanguage(data->player[data->activplayer].item[0]->description_pos,language));
							}
						break;
					}
				}
			break;
			//Zug ist ausgelagert wegen etwagien Unteraufrufen. *seufz*
			case 4: //Item
				//Script?
				if (data->item_in_script==1)
				{
          if (delaycounter<=0)
          {
            float wert=0;
            if (data->item_script_pos>=0)
              wert=callfunction(script->block,&(data->item_script_pos),&(data->item_script_stack));
            if (data->item_script_pos==-1)
						{
              free(data->item_script_stack);
							//Item entfernen:
							switch (data->use_item_nr)
							{
								case 0:
									data->player[data->activplayer].item[0]=data->player[data->activplayer].item[1];
									data->player[data->activplayer].item[1]=data->player[data->activplayer].item[2];
									data->player[data->activplayer].item[2]=NULL;
								break;
								case 1:
									data->player[data->activplayer].item[1]=data->player[data->activplayer].item[2];
									data->player[data->activplayer].item[2]=NULL;
								break;						
								case 2:
									data->player[data->activplayer].item[2]=NULL;
								break;
							}							
							data->item_in_script=0;
							data->mainloop_pos=2;
							if (data->player[data->activplayer].iscpu)
								data->choose_move=1;
						}
            else
            {
              data->item_script_pos++;
              delaycounter=(int)(trunc(wert));
            }
          }
          else
          {
            delaycounter--;
            if (delaycounter<0)
              delaycounter=0;
          }
					break;
				}
				if (data->item_in_script==2) //Userauswahl
				{
					int endnum=playernum;
					if (data->player[data->activplayer].item[data->use_item_nr]->friendlyfire)
						endnum-=1;
					else
						endnum-=teamsize;
					data->choose_angle+=0.1;
					if (data->item_choose_move==0 && (*(ZWattribute->joystick[data->activplayer].axis[0])<0 || (data->player[data->activplayer].iscpu && rand()%100==0)))
						data->item_choose_move=-1;
					if (data->item_choose_move==0 && *(ZWattribute->joystick[data->activplayer].axis[0])>0)
						data->item_choose_move=1;
					if (data->item_choose_move<0)
					{
						data->item_choose_move-=1<<1;
						if (data->item_choose_move<=-1000)
						{
							data->item_choose_move=0;
							data->item_choose_choose--;
							if (data->item_choose_choose<0)
								data->item_choose_choose=endnum-1;
						}      
					}
					if (data->item_choose_move>0)
					{
						data->item_choose_move+=1<<1;
						if (data->item_choose_move>=1000)
						{
							data->item_choose_move=0;
							data->item_choose_choose++;
							if (data->item_choose_choose>endnum-1)
								data->item_choose_choose=0;
						}
					}
					if (*(ZWattribute->joystick[data->activplayer].button[0])==1 || (data->player[data->activplayer].iscpu && data->item_choose_move==0 && rand()%500==0))
					{
     				deletetextcontent();
						*(ZWattribute->joystick[data->activplayer].button[0])=0;
						data->item_script_pos=data->player[data->activplayer].item[data->use_item_nr]->script_pos;
						data->item_script_stack=(float*)malloc(sizeof(float)*script->block[data->item_script_pos].jump);
						int momplayer=-1;
						int counter;
						for (counter=0;counter<=data->item_choose_choose;counter++)
						{
							do
								momplayer++;
							while (momplayer==data->activplayer || (!data->player[data->activplayer].item[data->use_item_nr]->friendlyfire && data->player[momplayer].team==data->player[data->activplayer].team));
						}						
						data->item_script_stack[0]=momplayer;
						data->item_in_script=1;		
					}
					if (*(ZWattribute->joystick[data->activplayer].button[2])==1)
					{
     				deletetextcontent();
						*(ZWattribute->joystick[data->activplayer].button[2])=0;
            data->item_in_script=0;
            data->mainloop_pos=2;
            if (data->player[data->activplayer].iscpu)
              data->choose_move=1;
					}
					break;
				}
				if (data->item_in_script==3) //Feldauswahl
				{
          if (*(ZWattribute->joystick[data->activplayer].axis[0])<0)
          {
            *(ZWattribute->joystick[data->activplayer].axis[0])=0;
            momchoiceset=momchoiceset->choicebefore;
          }
          if (*(ZWattribute->joystick[data->activplayer].axis[0])>0 || (data->player[data->activplayer].iscpu && rand()%100==0))
          {
            *(ZWattribute->joystick[data->activplayer].axis[0])=0;
            momchoiceset=momchoiceset->choicenext;
          }
          if (*(ZWattribute->joystick[data->activplayer].button[0]) || (data->player[data->activplayer].iscpu && rand()%1500==0))
          {
            *(ZWattribute->joystick[data->activplayer].button[0])=0;
            momchoiceset->triggertrap=data->player[data->activplayer].item[data->use_item_nr]->script_pos;
            momchoiceset->trapowner=data->activplayer;
						data->item_script_pos=-1;
						data->item_script_stack=NULL;
            data->item_in_script=1;
          }
					if (*(ZWattribute->joystick[data->activplayer].button[2])==1)
					{
						*(ZWattribute->joystick[data->activplayer].button[2])=0;
            data->item_in_script=0;
            data->mainloop_pos=2;
            if (data->player[data->activplayer].iscpu)
              data->choose_move=1;
					}
          break;
        }
				//sonst:
				//Item zählen
				itemnum=1;
				if (data->player[data->activplayer].item[1]!=NULL)
				{
					if (data->player[data->activplayer].item[2]==NULL)
						itemnum=2;
					else
						itemnum=3;
				}		
				data->choose_angle+=0.1;
				if (data->item_choose_move==0 && (*(ZWattribute->joystick[data->activplayer].axis[0])<0 || (data->player[data->activplayer].iscpu && rand()%100==0)))
					data->item_choose_move=-1;
				if (data->item_choose_move==0 && *(ZWattribute->joystick[data->activplayer].axis[0])>0)
					data->item_choose_move=1;
				if (data->item_choose_move<0)
				{
					data->item_choose_move-=1<<1;
					if (data->item_choose_move<=-1000)
					{
						data->item_choose_move=0;
						data->item_choose_choose--;
						if (data->item_choose_choose<0)
							data->item_choose_choose=itemnum-1;
						createtextcontent(gettextbynrandlanguage(data->player[data->activplayer].item[data->item_choose_choose]->description_pos,language));
					}      
				}
				if (data->item_choose_move>0)
				{
					data->item_choose_move+=1<<1;
					if (data->item_choose_move>=1000)
					{
						data->item_choose_move=0;
						data->item_choose_choose++;
						if (data->item_choose_choose>itemnum-1)
							data->item_choose_choose=0;
						createtextcontent(gettextbynrandlanguage(data->player[data->activplayer].item[data->item_choose_choose]->description_pos,language));
					}
				}
				if (*(ZWattribute->joystick[data->activplayer].button[0])==1 || (data->player[data->activplayer].iscpu && data->item_choose_move==0 && rand()%200==0))
				{
					*(ZWattribute->joystick[data->activplayer].button[0])=0;
					if (data->item_choose_move>500) data->item_choose_choose++;
					if (data->item_choose_move<-500) data->item_choose_choose--;      
					data->item_choose_move=0;
					if (data->item_choose_choose<0) data->item_choose_choose=3;
					if (data->item_choose_choose>3) data->item_choose_choose=0;
					data->use_item_nr=data->item_choose_choose;
					deletetextcontent();
					switch (data->player[data->activplayer].item[data->use_item_nr]->kind)
					{
						case 0: //nothing
							data->item_script_pos=data->player[data->activplayer].item[data->item_choose_choose]->script_pos;
							data->item_script_stack=(float*)malloc(sizeof(float)*script->block[data->item_script_pos].jump);
							data->item_in_script=1;
						break;
						case 1: //user
							data->item_choose_choose=0;
							data->item_choose_move=0;
							createtextcontent((char*)"Wähle einen Mitspieler als Ziel für das Item aus!");
						  data->item_in_script=2;
							if (data->player[data->activplayer].iscpu)
								data->choose_move=1;
						break;
						case 3: //set
							data->item_in_script=3;
							data->map_show_env=0;
							data->map_zoom=data->player[data->activplayer].momset->position.y+data->player[data->activplayer].item[data->use_item_nr]->radius*2.17;
							data->map_x=data->player[data->activplayer].momset->position.x;
							data->map_y=data->player[data->activplayer].momset->position.z;
              //alle Felder suchen, die in Reichweite sind
              firstchoiceset=NULL;
              pset momset=firstset;
              pset lastchoiceset=NULL;
              momchoiceset=NULL;
              while (momset!=NULL)
              {
                if (distance(momset->position.x,0,momset->position.z,data->player[data->activplayer].momset->position.x,0,data->player[data->activplayer].momset->position.z)<=data->player[data->activplayer].item[data->use_item_nr]->radius && strcmp(momset->type,"ghost")!=0  && strcmp(momset->type,"rubin")!=0  && strcmp(momset->type,"none")!=0  && strcmp(momset->type,"shop")!=0)
                {
                  lastchoiceset=momchoiceset;
                  momchoiceset=momset;
                  if (lastchoiceset==NULL)
                    firstchoiceset=momchoiceset;
                  else
                    lastchoiceset->choicenext=momchoiceset;
                  momchoiceset->choicebefore=lastchoiceset;
                  momchoiceset->choicenext=NULL;
                }
                momset=momset->next;
              }
              //von links nach rechts sortieren
              bool change=true;
              while (change)
              {
                change=false;
                pset momset=firstchoiceset;
                while (momset->choicenext!=NULL)
                {
                  if (momset->position.x>momset->choicenext->position.x)
                  {
                    pset set1=momset;
                    pset set2=momset->choicenext;
                    pset before_set1=momset->choicebefore;
                    pset after_set2=momset->choicenext->choicenext;
                    
                    set2->choicenext=set1;
                    set1->choicebefore=set2;
                    
                    set2->choicebefore=before_set1;
                    if (before_set1==NULL)
                      firstchoiceset=set2;
                    else
                      before_set1->choicenext=set2;
                    if (after_set2!=NULL)
                      after_set2->choicebefore=set1;
                    else
                      momchoiceset=set1;
                    set1->choicenext=after_set2;
                    change=true;
                    break;
                  }
                  momset=momset->choicenext;
                }
              }
              firstchoiceset->choicebefore=momchoiceset;
              momchoiceset->choicenext=firstchoiceset;
              momchoiceset=firstchoiceset;
						break;
					}
				}
				if (*(ZWattribute->joystick[data->activplayer].button[2])==1 || (data->player[data->activplayer].iscpu && data->item_choose_move==0 && rand()%3000==0))
				{
					*(ZWattribute->joystick[data->activplayer].button[2])=0;
					deletetextcontent();
					data->mainloop_pos=2;
					if (data->player[data->activplayer].iscpu)
					  data->choose_move=1;
				}
				
			break;
			case 5: //Map
				if (*(ZWattribute->joystick[data->activplayer].button[0]))
				{
					data->mainloop_pos=2;    
					*(ZWattribute->joystick[data->activplayer].button[0])=0;
				}
				if (*(ZWattribute->joystick[data->activplayer].button[1]))
					//for (a=0;a<ZWattribute->steps;a++)
					{
						data->map_zoom*=0.999;
						if (data->map_zoom<=5.0)
							data->map_zoom/=0.999;
					}
				if (*(ZWattribute->joystick[data->activplayer].button[2]))
				{
					data->map_show_env=1-data->map_show_env;
					*(ZWattribute->joystick[data->activplayer].button[2])=0;
				}
				if (*(ZWattribute->joystick[data->activplayer].button[3]))
					//for (a=0;a<ZWattribute->steps;a++)
					{
						data->map_zoom/=0.999;
						if (data->map_zoom>=40.0)
							data->map_zoom*=0.999;
					}
	if (*(ZWattribute->joystick[data->activplayer].axis[0])<0)
					//for (a=0;a<ZWattribute->steps;a++)
						data->map_x-=data->map_zoom*0.0005;
				if (*(ZWattribute->joystick[data->activplayer].axis[0])>0)
					//for (a=0;a<ZWattribute->steps;a++)
						data->map_x+=data->map_zoom*0.0005;
				if (*(ZWattribute->joystick[data->activplayer].axis[1])<0)
					//for (a=0;a<ZWattribute->steps;a++)
						data->map_y-=data->map_zoom*0.0005;
				if (*(ZWattribute->joystick[data->activplayer].axis[1])>0)
					//for (a=0;a<ZWattribute->steps;a++)
						data->map_y+=data->map_zoom*0.0005;
			break;
			case 7:  //Minispielauswahl
				if (data->activplayer+1<playernum)
					data->mainloop_pos=6; //"durchreichen
				else
				if (choosecircle_direction>=2000)
				{
					//gamechoice=choosecircle_get_choice();
					chosen_minigame=get_game_by_choosecircle_choice();
					kill_choosecircle();
					data->mainloop_pos=6;
					Mix_FadeOutMusic(FADE_OUT_TIME);
					Mix_FadeOutChannel(-1,FADE_OUT_TIME);
				}
			break;
			case 8: //Neu kalibrieren
				calc_ingame_kalib(data);
			break;
			case 0: //Game Info
				calc_gamestartinfo(data);
			break;
			case 9: //Intro
				if (delaycounter<=0)
				{
					float wert=callfunction(script->block,&(data->item_script_pos),&(data->item_script_stack));
					if (data->item_script_pos==-1)
					{
						data->mainloop_pos=2;
						if (!(data->player[data->activplayer].has_calib))
							data->choose_choose=2;
						if (data->player[data->activplayer].iscpu && data->player[data->activplayer].item[0]!=NULL && rand()%3==0)
							data->choose_move=-1;
					}
					else
					{
						data->item_script_pos++;
						delaycounter=(int)(trunc(wert));
					}
				}
				else
				{
					delaycounter--;
					if (delaycounter<0)
						delaycounter=0;
				}
			
			break;
			//Fade Out ist im Nicht_Thread wegen eines Unterschleifenaufrufs!
		}
	}
  //Zeitabhängige Bewegeungen
  for (a=0;a<ZWattribute->steps;a++)
  {
    calcmaindata(data);
    calcplayer(data,data->activplayer); 
  }
  calc_messages(ZWattribute->steps);
  
  return 0;
}
