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
void draw_choose(pmaindata data)
{
	float neigung=27.0;
	float y=3.5;
	float angle[4];
	int a,b;
	int position[4];
	for (a=0;a<4;a++)
	{
		angle[a]=((float)(data->choose_choose)+(float)(data->choose_move)/1000.0)*90.0+(float)a*90.0;
		position[a]=a;
	}
	//Sortieren
	for (a=0;a<4;a++)
	  for (b=a+1;b<4;b++)
			if (cos(angle[position[a]]*M_PI/180.0)>cos(angle[position[b]]*M_PI/180.0))
			{
				int temp=position[a];
				position[a]=position[b];
				position[b]=temp;
			}
  glTranslatef(0,0,-15);
	for (a=0;a<4;a++)
	{
		glPushMatrix();
			glTranslatef(0,y,0);
			glRotatef(neigung,1,0,0);
			glRotatef(angle[position[a]],0,1,0);
			glTranslatef(0,0,7);
			glRotatef(angle[position[a]],0,-1,0);
			switch (position[a])
			{
				case 0:
					glColor4f(0,0,0,0.8);
          if (strcmp(language,(char*)"german")==0)
          {
            ZWdrawtextmiddle(text,0.03,-1.97,0,(char*)"Würfeln",1.01);      
            glColor4f(1,1,1,1);
            ZWdrawtextmiddle(text,0,-2,0,(char*)"Würfeln",1.0);     
          }
          else
          {
            ZWdrawtextmiddle(text,0.03,-1.97,0,(char*)"Roll Dice",1.01);      
            glColor4f(1,1,1,1);
            ZWdrawtextmiddle(text,0,-2,0,(char*)"Roll Dice",1.0);     
          }          
					glRotatef(data->choose_angle,0,1,0);
					glRotatef(sin(data->choose_angle*M_PI/90.0)*30.0,0,0,1);
					ZWdrawobject(dice);
				break;
				case 1:
					glColor4f(0,0,0,0.8);
					ZWdrawtextmiddle(text,0.03,-1.97,0,(char*)"Items",1.01);      
					glColor4f(1,1,1,1);
					ZWdrawtextmiddle(text,0,-2,0,(char*)"Items",1.0);      
					glRotatef(data->choose_angle,0,1,0);
					glRotatef(sin(data->choose_angle*M_PI/90.0)*30.0,0,0,1);
					ZWdrawobject(item[0]);
				break;
				case 2:
					glColor4f(0,0,0,0.8);
          if (strcmp(language,(char*)"german")==0)
          {
            ZWdrawtextmiddle(text,0.03,-1.97,0,(char*)"Neu kalibrieren",1.01);      
            if (!(data->player[data->activplayer].has_calib))
              ZWdrawtextmiddle(text,0.03,-2.87,0,(char*)"Dringend empfohlen!",1.51);      
            glColor4f(1,1,1,1);
            ZWdrawtextmiddle(text,0,-2,0,(char*)"Neu kalibrieren",1.0);      
            glColor4f(1,0.7,0.7,1);
            if (!(data->player[data->activplayer].has_calib))
              ZWdrawtextmiddle(text,0.,-2.9,0,(char*)"Dringend empfohlen!",1.5);      
          }
          else
          {
            ZWdrawtextmiddle(text,0.03,-1.97,0,(char*)"Calibrate new",1.01);      
            if (!(data->player[data->activplayer].has_calib))
              ZWdrawtextmiddle(text,0.03,-2.87,0,(char*)"Badly recommended!",1.51);      
            glColor4f(1,1,1,1);
            ZWdrawtextmiddle(text,0,-2,0,(char*)"Calibrate new",1.0);      
            glColor4f(1,0.7,0.7,1);
            if (!(data->player[data->activplayer].has_calib))
              ZWdrawtextmiddle(text,0.,-2.9,0,(char*)"Badly recommended!",1.5);      
          }
          
					glColor4f(1,1,1,1);
					glRotatef(sin(data->choose_angle*M_PI/90.0)*30.0,0,0,1);
					glRotatef(cos(data->choose_angle*M_PI/45.0)*30.0,1,0,0);
					ZWusetexture(maintex);
					glBegin(GL_QUADS);
						glTexCoord2f(0.75,0.625);glVertex3f(-1.5,0.75,0);
						glTexCoord2f(0.75,0.750);glVertex3f(-1.5,-0.75,0);
						glTexCoord2f(1.00,0.750);glVertex3f(1.5,-0.75,0);
						glTexCoord2f(1.00,0.625);glVertex3f(1.5,0.75,0);        
					glEnd();
				break;
				case 3:
					glColor4f(0,0,0,0.8);
          if (strcmp(language,(char*)"german")==0)
          {
            ZWdrawtextmiddle(text,0.03,-1.97,0,(char*)"Übersicht",1.01);      
            glColor4f(1,1,1,1);
            ZWdrawtextmiddle(text,0,-2,0,(char*)"Übersicht",1.0);      
          }
          else
          {
            ZWdrawtextmiddle(text,0.03,-1.97,0,(char*)"Overview",1.01);      
            glColor4f(1,1,1,1);
            ZWdrawtextmiddle(text,0,-2,0,(char*)"Overview",1.0);      
          }
					glRotatef(sin(data->choose_angle*M_PI/90.0)*30.0,0,0,1);
					glRotatef(cos(data->choose_angle*M_PI/45.0)*30.0,1,0,0);
					ZWusetexture(maintex);
					glBegin(GL_QUADS);
						glTexCoord2f(0.625,0.875);glVertex3f(-1,1,0);
						glTexCoord2f(0.625,1.000);glVertex3f(-1,-1,0);
						glTexCoord2f(0.750,1.000);glVertex3f(1,-1,0);
						glTexCoord2f(0.750,0.875);glVertex3f(1,1,0);        
					glEnd();
				break;
			}
		glPopMatrix();
	}
}

void draw_item_choose(pmaindata data)
{
	if (data->player[data->activplayer].item[0]==NULL)
	  return;
	//Item zählen
	int itemnum=1;
	if (data->player[data->activplayer].item[1]!=NULL)
	{
		if (data->player[data->activplayer].item[2]==NULL)
		  itemnum=2;
		else
		  itemnum=3;
	}
	float neigung=27.0;
	float y=3.5;
	int momitem;
  glTranslatef(0,0,-15);
	for (momitem=0;momitem<itemnum;momitem++)
	{
		glPushMatrix();
			glTranslatef(0,y,0);
			glRotatef(neigung,1,0,0);
			glRotatef(((float)(data->item_choose_choose)+(float)(data->item_choose_move)/1000.0)*360.0/(float)itemnum+360.0*(float)(itemnum-momitem)/(float)itemnum,0,1,0);
			glTranslatef(0,0,7);
			glRotatef(((float)(data->item_choose_choose)+(float)(data->item_choose_move)/1000.0)*360.0/(float)itemnum+360.0*(float)(itemnum-momitem)/(float)itemnum,0,-1,0);
			/*glColor4f(0,0,0,0.8);
			ZWdrawtextmiddle(text,0.03,-1.97,0,(char*)"Items",1.01);      
			glColor4f(1,1,1,1);
			ZWdrawtextmiddle(text,0,-2,0,(char*)"Items",1.0);*/
			glRotatef(data->choose_angle,0,1,0);
			glRotatef(sin(data->choose_angle*M_PI/90.0)*30.0,0,0,1);
			glScalef(data->player[data->activplayer].item[momitem]->object_scale.x,data->player[data->activplayer].item[momitem]->object_scale.y,data->player[data->activplayer].item[momitem]->object_scale.z);
			ZWdrawobject(data->player[data->activplayer].item[momitem]->object->realobject);
		glPopMatrix();
	}
	glColor4f(0,0,0,0.8);
  if (strcmp(language,(char*)"german")==0)
  {
    ZWdrawtextmiddle(text,-5.03,1.03,5,(char*)"[A] Einsetzen",1.01);
    ZWdrawtextmiddle(text, 5.03,1.03,5,(char*)"[C] Zurück",1.01);
    glColor4f(1,1,1,1);
    ZWdrawtextmiddle(text,-5   ,1   ,5,(char*)"[A] Einsetzen",1.0);
    ZWdrawtextmiddle(text, 5   ,1   ,5,(char*)"[C] Zurück",1.0);
  }
  else
  {
    ZWdrawtextmiddle(text,-5.03,1.03,5,(char*)"[A] Use",1.01);
    ZWdrawtextmiddle(text, 5.03,1.03,5,(char*)"[C] Back",1.01);
    glColor4f(1,1,1,1);
    ZWdrawtextmiddle(text,-5   ,1   ,5,(char*)"[A] Use",1.0);
    ZWdrawtextmiddle(text, 5   ,1   ,5,(char*)"[C] Back",1.0);
  }
  
}

void draw_user_choose(pmaindata data,int player,char friendlyfire)
{
	//Spieler, die in Frage kommen ermitterln:
	int endnum=playernum;
	if (friendlyfire)
	  endnum-=1;
	else
		endnum-=teamsize;
	
	
	float neigung=27.0;
	float y=3.5;
	int momplayer=-1;
	int counter;
  glTranslatef(0,0,-15);
	for (counter=0;counter<endnum;counter++)
	{
		do
			momplayer++;
		while (momplayer==player || (!friendlyfire && data->player[momplayer].team==data->player[player].team));
		glPushMatrix();
			glTranslatef(0,y,0);
			glRotatef(neigung,1,0,0);
			glRotatef(((float)(data->item_choose_choose)+(float)(data->item_choose_move)/1000.0)*360.0/(float)endnum+360.0*(float)(endnum-counter)/(float)endnum,0,1,0);
			glTranslatef(0,0,7);
			glRotatef(((float)(data->item_choose_choose)+(float)(data->item_choose_move)/1000.0)*360.0/(float)endnum+360.0*(float)(endnum-counter)/(float)endnum,0,-1,0);
			/*glColor4f(0,0,0,0.8);
			ZWdrawtextmiddle(text,0.03,-1.97,0,(char*)"Items",1.01);      
			glColor4f(1,1,1,1);
			ZWdrawtextmiddle(text,0,-2,0,(char*)"Items",1.0);*/
			ZWsetlightambient(0,0.1,0.1,0.1);
			ZWsetlightdiffuse(0,1,1,1);
			ZWsetlightposition(0,0,1,2);
			ZWenablelight(0);
			//glRotatef(data->choose_angle,0,1,0);
			glRotatef(cos(data->choose_angle*M_PI/180.0)*20.0-10.0,1,0,0);
			glRotatef(sin(data->choose_angle*M_PI/90.0)*30.0,0,0,1);
			glColor4f(data->player[momplayer].color.r,data->player[momplayer].color.g,data->player[momplayer].color.b,1);
			glEnable(GL_LIGHTING);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			ZWdrawobject(data->player[momplayer].ball);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			ZWdisablelight(0);
			glDisable(GL_LIGHTING);
		glPopMatrix();
	}
	glColor4f(0,0,0,0.8);
  if (strcmp(language,(char*)"german")==0)
  {
    ZWdrawtextmiddle(text,-5.03,4.03,5,(char*)"[A] Auswählen",1.01);
    ZWdrawtextmiddle(text, 5.03,4.03,5,(char*)"[C] Zurück",1.01);
    glColor4f(1,1,1,1);
    ZWdrawtextmiddle(text,-5   ,4   ,5,(char*)"[A] Auswählen",1.0);
    ZWdrawtextmiddle(text, 5   ,4   ,5,(char*)"[C] Zurück",1.0);
  }
  else
  {
    ZWdrawtextmiddle(text,-5.03,4.03,5,(char*)"[A] Choose",1.01);
    ZWdrawtextmiddle(text, 5.03,4.03,5,(char*)"[C] Back",1.01);
    glColor4f(1,1,1,1);
    ZWdrawtextmiddle(text,-5   ,4   ,5,(char*)"[A] Choose",1.0);
    ZWdrawtextmiddle(text, 5   ,4   ,5,(char*)"[C] Back",1.0);
  }
  
}

void draw_item_shop(pmaindata data)
{
	float neigung=27.0;
	float y=4.0;
	float angle[8];
	int a,b;
	int position[8];
	for (a=0;a<data->item_shop_item_count;a++)
	{
		angle[a]=((float)(data->item_choose_choose)+(float)(data->item_choose_move)/1000.0)*(360.0/(float)(data->item_shop_item_count))+(float)(data->item_shop_item_count-a)*(360.0/(float)(data->item_shop_item_count));
		position[a]=a;
	}
	//Sortieren
	for (a=0;a<data->item_shop_item_count;a++)
	  for (b=a+1;b<data->item_shop_item_count;b++)
			if (cos(angle[position[a]]*M_PI/180.0)>cos(angle[position[b]]*M_PI/180.0))
			{
				int temp=position[a];
				position[a]=position[b];
				position[b]=temp;
			}
  glTranslatef(0,0,-15);
	for (a=0;a<data->item_shop_item_count;a++)
	{
		glPushMatrix();
			glTranslatef(0,y,0);
			glRotatef(neigung,1,0,0);
			glRotatef(angle[position[a]],0,1,0);
			glTranslatef(0,0,7);
			glRotatef(angle[position[a]],0,-1,0);
			  char buffer[32];
        if (strcmp(language,(char*)"german")==0)
          sprintf(buffer,"%i Münzen",data->item_shop_price[position[a]]);
        else
          sprintf(buffer,"%i Münzen",data->item_shop_price[position[a]]);
				glColor4f(0,0,0,0.8);
				ZWdrawtextmiddle(text,0.03,-1.97,0,buffer,1.01);      
				glColor4f(1,1,1,1);
				ZWdrawtextmiddle(text,0,-2,0,buffer,1.0);     
				glRotatef(data->choose_angle,0,1,0);
				glRotatef(sin(data->choose_angle*M_PI/90.0)*30.0,0,0,1);
				ZWdrawobject(data->item_shop_item[position[a]]->object->realobject);
		glPopMatrix();
	}
	glColor4f(0,0,0,0.8);
  if (strcmp(language,(char*)"german")==0)
  {
    ZWdrawtextmiddle(text,-5.23,-1.2 ,5,(char*)"[A] Kaufen",1.01);
    ZWdrawtextmiddle(text, 5.23,-1.2 ,5,(char*)"[C] Zurück",1.01);
    glColor4f(1,1,1,1);
    ZWdrawtextmiddle(text,-5.2 ,-1.23,5,(char*)"[A] Kaufen",1.0);
    ZWdrawtextmiddle(text, 5.2 ,-1.23,5,(char*)"[C] Zurück",1.0);
  }
  else
  {
    ZWdrawtextmiddle(text,-5.23,-1.2 ,5,(char*)"[A] Buy",1.01);
    ZWdrawtextmiddle(text, 5.23,-1.2 ,5,(char*)"[C] Back",1.01);
    glColor4f(1,1,1,1);
    ZWdrawtextmiddle(text,-5.2 ,-1.23,5,(char*)"[A] Buy",1.0);
    ZWdrawtextmiddle(text, 5.2 ,-1.23,5,(char*)"[C] Back",1.0);
  }
  
}

void new_item_choose(pmaindata data,int kind) //kind: 0 "30%-Shop", 1: "Itemland" 2: "Interitem"
{
	float faktor;
	switch (kind)
	{
		case 0: data->item_shop_item_count=3; faktor=0.7; break;
		case 2: data->item_shop_item_count=8; faktor=1.3; break;
		default: data->item_shop_item_count=5; faktor=1.0;
	}
	pitem momitem=firstitem;
	if (momitem==NULL) //Es gibt gar keine Items. -_-
	{
	  data->item_shop_item_count=0;
		return;
	}
	int itemnr;
	for (itemnr=0;itemnr<data->item_shop_item_count;itemnr++)
	{
		//wir iterieren ein bisschen, um zufällig ein Item zu wählen
		while (rand()%66!=0)
		{
			momitem=momitem->next;
			if (momitem==NULL)
			  momitem=firstitem;
		}
		data->item_shop_item[itemnr]=momitem;
		data->item_shop_price[itemnr]=(int)(round((float)(momitem->price)*faktor));
	}
	createtextcontent(gettextbynrandlanguage(data->item_shop_item[data->item_choose_choose]->description_pos,language));
	data->special_step=1;
	data->item_choose_choose=0;
	data->item_choose_move=0;
}
