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
void load_gameinfotext(char* thefile)
{
	free(gameinfotext);
	gameinfotext=NULL;
	char buffer[256];
  //Text einlesen:
	char content[65536]="no text";
	sprintf(buffer,thefile);
	SDL_RWops *rw=SDL_RWFromFile(buffer,"rb");
	planguage description=NULL;
	if(rw!=NULL)
	{
		int anz=1;
		while (anz>0)
		{
			char c;
			anz=SDL_RWread(rw,&c,1,1);
			int pos=0;
			while (anz>0 && pos<65535 && c!='\n')
			{
				content[pos]=c;
				pos++;
				anz=SDL_RWread(rw,&c,1,1);
			}
			if (pos>0)
				content[pos]=0;
			planguage newlan=(planguage)malloc(sizeof(tlanguage));
			//Doppelpunkt suchen:
			int a;
			for (a=0;content[a]!=0;a++)
				if (content[a]==':')
					break;
			content[a]=0;
			newlan->language=(char*)malloc(strlen(content)+1);
			sprintf(newlan->language,"%s",content);
			newlan->content=(char*)malloc(strlen(&(content[a+1]))+1);
			sprintf(newlan->content,"%s",&(content[a+1]));
			newlan->next=description;
			description=newlan;
		}
		SDL_RWclose(rw);
	}
	//nach language suchen
	planguage momlan=description;
	planguage last=NULL;
	while (momlan!=NULL)
	{
		if (strcmp(momlan->language,language)==0) break;
		last=momlan;
		momlan=momlan->next;
	}
	char* dummy;
	if (momlan!=NULL)
		dummy=momlan->content;
	else
		dummy=last->content; 
	gameinfotext=(char*)malloc(strlen(dummy)+1);
	sprintf(gameinfotext,"%s",dummy);
	//Wieder freigeben:
	while (description!=NULL)
	{
		free(description->content);
		free(description->language);
		planguage next=description->next;
		free(description);
		description=next;
	}
}

void prepare_game(pset startset,int complete)
{
  if (startset==NULL)
    startset=firstset;
  int a;
  for (a=0;a<playernum;a++)
  {
		int b;
		for (b=0;b<3;b++)
		  maindata.player[a].item[b]=NULL;
    maindata.player[a].money=5;
    maindata.player[a].rubins=0;
		//maindata.player[a].destsize=1.0/sqrt((float)playernum);
		maindata.player[a].momsize=0.0;//1.0/sqrt((float)playernum);
		maindata.player[a].momface=0;
    if (startset!=NULL)
    {
      maindata.player[a].momposition=startset->position;
      maindata.player[a].momposition.y+=startset->maxy;
      maindata.player[a].destposition=startset->position;
      maindata.player[a].destposition.y+=startset->maxy;
    }
    else
    {
      maindata.player[a].momposition.x=0;
      maindata.player[a].momposition.y=0;
      maindata.player[a].momposition.z=0;
      maindata.player[a].destposition=maindata.player[a].momposition;
    }    
    maindata.player[a].time=0;
    if (complete)
    {
      maindata.player[a].color.r=(float)(a%2);
      maindata.player[a].color.g=(float)(a/2%2);
      maindata.player[a].color.b=(float)(a/4%2);
      maindata.player[a].facenr=rand()%MAX_FACES;
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
      maindata.player[a].iscpu=all_are_cpu;
      maindata.player[a].team=a%(playernum/teamsize);
//      maindata.player[a].joystick_nr=a;
    }
    maindata.player[a].color.a=1.0;
    maindata.player[a].momset=startset;
    maindata.player[a].lastset=startset;
  }
  if (startset!=NULL)
  {
    maindata.momcamerarotation=startset->camerarotation;
    maindata.momcameraposition=startset->cameraposition;
    maindata.destcameraposition=startset->cameraposition;
    maindata.destcamerarotation=startset->camerarotation;
    maindata.time=0;
  }
  maindata.mainloop_pos=1;
  maindata.activplayer=0;
  sync_all_position(0.5,&maindata);
  
  if (complete)
  {
    gametype=12;
  }
	
	reset_played();
  
  //preparescript starten:
  int pos=0;
  //"prepare" suchen
  pfunctab functab=script->functab;
  while (functab!=NULL)
  {
    if (strcmp(functab->name,(char*)"prepare")==0)
      break;
    functab=functab->next;
  }
  if (functab!=NULL)
  {
    float* stack=(float*)malloc(sizeof(float)*script->block[functab->jump].jump);
    int pos=functab->jump;
    callfunction(script->block,&pos,&stack);
  }
  maindata.round=1;
	for (int a=0;a<8;a++)
	  choosecircle_content[a][0]=0;
	choosecircle_on=false;
	choosecircle_face=false;
	choosecircle_speed=0.0;
	choosecircle_position=rand()%360;
	choosecircle_direction=0;
	maindata.gamestartinfo=1;
	load_gameinfotext((char*)(DATAFOLDER"data/info.txt"));
	for (a=0;a<playernum;a++)
	  maindata.gameinfochoice[a]=maindata.player[a].iscpu;
	maindata.turn_dice_dest=1;
	maindata.turn_dice_num=1;
	maindata.camera_looking_at=-1;
	maindata.special_step=0;
	maindata.dice_speed=DICE_SPEED;
}


void draw_att(float x,float y,float z,float s,float r,float g,float b,float alpha,float alpha_text,int money,int rubins)
{
  glTranslatef(x,y,z);
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
  glTranslatef(-x,-y,-z);
}


void drawplayeratt(float x,float y,float z,float s,float r,float g,float b,float alpha,float alpha_text,int facenr,int money,int rubins,pitem item1,pitem item2,pitem item3)
{
  float addx=0.25*(float)(facenr%4);
  float addy=0.25*(float)(facenr/4);

  glTranslatef(x,y,z);
  glScalef(s,s,s);
  float facezoom=0.2;
  ZWusetexture(maintex);
  glColor4f(r,g,b,alpha);
  glBegin(GL_QUADS);
    glTexCoord2f(0,0.5);
    glVertex3f(-1,1,0);
    glTexCoord2f(0,1);
    glVertex3f(-1,-1,0);
    glTexCoord2f(0.5,1);
    glVertex3f(1,-1,0);
    glTexCoord2f(0.5,0.5);
    glVertex3f(1,1,0);
  glEnd();
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
  sprintf(buffer,"%i",money);
  glColor4f(0,0,0,0.8);  
  ZWdrawtextmiddle(text,-0.4,0.15,0,buffer,0.8);
  glColor4f(1,1,1,0.9);  
  ZWdrawtextmiddle(text,-0.35,0.2,0,buffer,0.8);
	sprintf(buffer,"%i",rubins);
  glColor4f(0,0,0,0.8);  
  ZWdrawtextmiddle(text,-0.4,-0.55,0,buffer,0.8);
  glColor4f(1,1,1,0.9);  
  ZWdrawtextmiddle(text,-0.35,-0.5,0,buffer,0.8);

  glColor4f(1,1,1,0.5);

  //Item 1
	if (item1!=NULL)
	{
		glPushMatrix();
		glTranslatef( 0.2, 0.6,0);
		glScalef(0.15,0.15,0.15);
		glRotatef(w,0.5,1,2);
		glScalef(item1->object_scale.x,item1->object_scale.y,item1->object_scale.z);
		ZWdrawobject(item1->object->realobject);
		glPopMatrix();
	}

	//Item 2
	if (item2!=NULL)
	{
		glPushMatrix();
		glTranslatef( 0.5, 0.0,0);
		glScalef(0.15,0.15,0.15);
		glRotatef(w,1,2,0.5);
		glScalef(item2->object_scale.x,item2->object_scale.y,item2->object_scale.z);
		ZWdrawobject(item2->object->realobject);
		glPopMatrix();
	}

	//Item 3
	if (item3!=NULL)
	{	
		glPushMatrix();
		glTranslatef( 0.2,-0.6,0);
		glScalef(0.15,0.15,0.15);
		glRotatef(w,2,0.5,1);
		glScalef(item3->object_scale.x,item3->object_scale.y,item3->object_scale.z);
		ZWdrawobject(item3->object->realobject);
		glPopMatrix();
	}

  glScalef(1.0/s,1.0/s,1.0/s);
  glTranslatef(-x,-y,-z);
}

void setplayernext(pmaindata data)
{
  data->player[data->activplayer].lastset=data->player[data->activplayer].momset;
  data->player[data->activplayer].momset=setbynr[data->player[data->activplayer].momset->gamenextnr];
  if (get_turn_dice_value_sum(data)>0)
    data->player[data->activplayer].destface=calcnewdestface(data->activplayer,data);
  else
    data->player[data->activplayer].destface=0;
  sync_all_position(0.4,data);
  data->destcameraposition=data->player[data->activplayer].momset->cameraposition;
  data->destcamerarotation=data->player[data->activplayer].momset->camerarotation;
  data->player[data->activplayer].time=1.0;
  data->time=1.0;
}

void reset_after_game(void)
{
  //Hier werden die Spieler wieder freigegeben usw.

}

void draw_gamestartinfo(pmaindata data)
{
	glColor4f(1,1,1,0.8);
	ZWdrawsprite(maintex,-1.2, 0.8,-2.0,0.2,0.2,4,8);		  
	ZWdrawsprite(maintex,-1.2,-0.8,-2.0,0.2,0.2,4,12);		  
	ZWdrawsprite(maintex, 1.2,-0.8,-2.0,0.2,0.2,4,13);		  
	ZWdrawsprite(maintex, 1.2, 0.8,-2.0,0.2,0.2,4,9);		  
	ZWdrawsprite(maintex, 0.0, 0.0,-2.0,1.4,0.6,8,41);		  
	ZWdrawsprite(maintex, 0.0, 0.8,-2.0,1.0,0.2,8,41);		  
	ZWdrawsprite(maintex, 0.0,-0.8,-2.0,1.0,0.2,8,41);		  
	int a;
	glColor4f(0,0,0,0.8);
	float maxlen=20.0;
	int pos=0;
	int lastspace=0;
	float momlen;
	int start;
	for (int a=0;a<17;a++)
	{
		//So lange von Leerzeichen zu Leerzeichen springen bis die Zeile zu lang wird.
		momlen=0;
		start=pos;
		while (gameinfotext[pos]!=0 && momlen<maxlen)
		{
			int c=gameinfotext[pos];
			if (c<0)
			  c+=256;			
			momlen+=text.breite[c];
			if (gameinfotext[pos]==' ')
			  lastspace=pos;
			pos++;
		}
		gameinfotext[lastspace]=0;
	  ZWdrawtextmiddle(text,0,0.82-(float)a*0.1,-2.0,&(gameinfotext[start]),0.12);
		gameinfotext[lastspace]=' ';
		if (gameinfotext[pos]==0)
		  break;
		pos=lastspace+1;
	}
	
	for (a=0;a<playernum;a++)
	{
		glColor4f(data->player[a].color.r,data->player[a].color.g,data->player[a].color.b,0.8);
		if (data->gameinfochoice[a])
			ZWdrawsprite(maintex,((float)a-(float)(playernum-1)/2.0)*0.2,-0.9,-2.0,0.07,0.07,8,42);
		else
			ZWdrawsprite(maintex,((float)a-(float)(playernum-1)/2.0)*0.2,-0.9,-2.0,0.07,0.07,2,2);
		glColor4f(1,1,1,0.8);
		ZWdrawsprite(facetex,((float)a-(float)(playernum-1)/2.0)*0.2,-0.9,-2.0,0.05,0.05,4,data->player[a].facenr);
	}
	
}

void calc_gamestartinfo(pmaindata data)
{
	int a;
  int complete=1;	
	for (a=0;a<playernum;a++)
		if (!data->gameinfochoice[a])
		{
			complete=0;
			if (*(ZWattribute->joystick[a].button[0]))
			{
				*(ZWattribute->joystick[a].button[0])=0;
				data->gameinfochoice[a]=1;
			}
		}
	if (complete)
	{
		//"intro" suchen
		pfunctab functab=script->functab;
		while (functab!=NULL)
		{
			if (strcmp(functab->name,(char*)"intro")==0)
				break;
			functab=functab->next;
		}
		if (functab!=NULL)
		{
			data->mainloop_pos=9;
			data->item_script_pos=functab->jump;
			data->item_script_stack=(float*)malloc(sizeof(float)*script->block[data->item_script_pos].jump);
		}
		else
		{
			data->mainloop_pos=2;
			if (!(data->player[data->activplayer].has_calib))
				data->choose_choose=2;
			if (data->player[data->activplayer].iscpu && data->player[data->activplayer].item[0]!=NULL && rand()%3==0)
			  data->choose_move=-1;
		}
	}
}
