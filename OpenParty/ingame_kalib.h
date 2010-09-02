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
void draw_ingame_kalib(pmaindata data)
{
	glColor4f(1,1,1,0.8);
	ZWdrawsprite(maintex,-1.2, 0.8,-2.0,0.2,0.2,4,8);		  
	ZWdrawsprite(maintex,-1.2,-0.8,-2.0,0.2,0.2,4,12);		  
	ZWdrawsprite(maintex, 1.2,-0.8,-2.0,0.2,0.2,4,13);		  
	ZWdrawsprite(maintex, 1.2, 0.8,-2.0,0.2,0.2,4,9);		  
	ZWdrawsprite(maintex, 0.0, 0.0,-2.0,1.4,0.6,8,41);		  
	ZWdrawsprite(maintex, 0.0, 0.8,-2.0,1.0,0.2,8,41);		  
	ZWdrawsprite(maintex, 0.0,-0.8,-2.0,1.0,0.2,8,41);		  
	
	float abstand=0.18;

  if (data->ingame_kalib_step==0)
	{
  	glColor4f(1,1,1,1);
		ZWdrawsprite(maintex, 1.0						 , 0.52-0.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,45);
		ZWdrawsprite(maintex, 1.0+abstand*0.9, 0.52-0.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,47);
	}
	else
	{
		glColor4f(0,0,0,0.8);
	  ZWdrawtextmiddle(text,1.1, 0.5-0.0*abstand,-2.0,(char*)"Ok",0.15);
	}

  if (data->ingame_kalib_step==1)
	{
  	glColor4f(1,1,1,1);
		ZWdrawsprite(maintex, 1.0						 , 0.52-1.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,44);
		ZWdrawsprite(maintex, 1.0+abstand*0.9, 0.52-1.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,47);
	}
	else if (data->ingame_kalib_step>1)
	{
		glColor4f(0,0,0,0.8);
	  ZWdrawtextmiddle(text,1.1, 0.5-1.0*abstand,-2.0,(char*)"Ok",0.15);
	}

  if (data->ingame_kalib_step==2)
	{
  	glColor4f(1,1,1,1);
		ZWdrawsprite(maintex, 1.0						 , 0.52-2.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,45);
		ZWdrawsprite(maintex, 1.0+abstand*0.9, 0.52-2.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,36);
	}
	else if (data->ingame_kalib_step>2)
	{
		glColor4f(0,0,0,0.8);
	  ZWdrawtextmiddle(text,1.1, 0.5-2.0*abstand,-2.0,(char*)"Ok",0.15);
	}

  if (data->ingame_kalib_step==3)
	{
  	glColor4f(1,1,1,1);
		ZWdrawsprite(maintex, 1.0						 , 0.52-3.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,45);
		ZWdrawsprite(maintex, 1.0+abstand*0.9, 0.52-3.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,37);
	}
	else if (data->ingame_kalib_step>3)
	{
		glColor4f(0,0,0,0.8);
	  ZWdrawtextmiddle(text,1.1, 0.5-3.0*abstand,-2.0,(char*)"Ok",0.15);
	}

  if (data->ingame_kalib_step==4)
	{
  	glColor4f(1,1,1,1);
		ZWdrawsprite(maintex, 1.0						 , 0.52-4.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,45);
		ZWdrawsprite(maintex, 1.0+abstand*0.9, 0.52-4.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,38);
	}
	else if (data->ingame_kalib_step>4)
	{
		glColor4f(0,0,0,0.8);
	  ZWdrawtextmiddle(text,1.1, 0.5-4.0*abstand,-2.0,(char*)"Ok",0.15);
	}

  if (data->ingame_kalib_step==5)
	{
  	glColor4f(1,1,1,1);
		ZWdrawsprite(maintex, 1.0						 , 0.52-5.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,45);
		ZWdrawsprite(maintex, 1.0+abstand*0.9, 0.52-5.0*abstand,-2.0,abstand*0.45,abstand*0.45,8,39);
	}
	else if (data->ingame_kalib_step>5)
	{
		glColor4f(0,0,0,0.8);
	  ZWdrawtextmiddle(text,1.1, 0.5-5.0*abstand,-2.0,(char*)"Ok",0.15);
	}

	glColor4f(0,0,0,0.8);
	ZWdrawtextmiddle(text,0, 0.75,-2.0,(char*)"Neu kalibrieren",0.4);
	char buffer[64];
	float one_texel=(float)texquali/1024.0;
	switch (data->ingame_kalib_step)
	{
		case 6: glColor4f(1,1,1,1);
						ZWdrawsprite(maintex, 0.42			, -0.5,-2.0,0.2,0.2,8,46);
						ZWdrawsprite(maintex, 0.42+0.4	, -0.5,-2.0,0.2,0.2,8,47);
						ZWusetexture(maintex);
						sprintf(buffer,"Test: ");
						if (*(ZWattribute->joystick[data->activplayer].axis[0])<0)
						{
						  sprintf(buffer,"%s[<]",buffer);							
							glBegin(GL_TRIANGLES);
							  glTexCoord2f(0.6875-0.0625+one_texel,0.6875-0.0625+one_texel); glVertex3f(0.42-0.2,-0.5+0.2,-2);
							  glTexCoord2f(0.6875-0.0625+one_texel,0.6875+0.0625-one_texel); glVertex3f(0.42-0.2,-0.5-0.2,-2);
							  glTexCoord2f(0.6875                 ,0.6875                 ); glVertex3f(0.42    ,-0.5    ,-2);
							glEnd();
						}
						if (*(ZWattribute->joystick[data->activplayer].axis[0])>0)
						{
						  sprintf(buffer,"%s[>]",buffer);
							glBegin(GL_TRIANGLES);
							  glTexCoord2f(0.6875+0.0625-one_texel,0.6875+0.0625-one_texel); glVertex3f(0.42+0.2,-0.5-0.2,-2);
							  glTexCoord2f(0.6875+0.0625-one_texel,0.6875-0.0625+one_texel); glVertex3f(0.42+0.2,-0.5+0.2,-2);
							  glTexCoord2f(0.6875                 ,0.6875                 ); glVertex3f(0.42    ,-0.5    ,-2);
							glEnd();
						}
						if (*(ZWattribute->joystick[data->activplayer].axis[1])<0)
						{
						  sprintf(buffer,"%s[^]",buffer);
							glBegin(GL_TRIANGLES);
							  glTexCoord2f(0.5625+0.0625-one_texel,0.6875-0.0625+one_texel); glVertex3f(0.42+0.2,-0.5+0.2,-2);
							  glTexCoord2f(0.5625-0.0625+one_texel,0.6875-0.0625+one_texel); glVertex3f(0.42-0.2,-0.5+0.2,-2);
							  glTexCoord2f(0.5625                 ,0.6875                 ); glVertex3f(0.42    ,-0.5    ,-2);
							glEnd();
						}
						if (*(ZWattribute->joystick[data->activplayer].axis[1])>0)
						{
						  sprintf(buffer,"%s[v]",buffer);
							glBegin(GL_TRIANGLES);
							  glTexCoord2f(0.5625-0.0625+one_texel,0.6875+0.0625-one_texel); glVertex3f(0.42-0.2,-0.5-0.2,-2);
							  glTexCoord2f(0.5625+0.0625-one_texel,0.6875+0.0625-one_texel); glVertex3f(0.42+0.2,-0.5-0.2,-2);
							  glTexCoord2f(0.5625                 ,0.6875                 ); glVertex3f(0.42    ,-0.5    ,-2);
							glEnd();
						}
						if (*(ZWattribute->joystick[data->activplayer].button[0]))
						{
						  sprintf(buffer,"%s[A]",buffer);
							glBegin(GL_TRIANGLES);
							  glTexCoord2f(0.5625-0.0625+one_texel,0.5625-0.0625+one_texel); glVertex3f(0.82-0.2,-0.5+0.2,-2);
							  glTexCoord2f(0.5625-0.0625+one_texel,0.5625+0.0625-one_texel); glVertex3f(0.82-0.2,-0.5-0.2,-2);
							  glTexCoord2f(0.5625                 ,0.5625                 ); glVertex3f(0.82    ,-0.5    ,-2);
							glEnd();
						}
						if (*(ZWattribute->joystick[data->activplayer].button[1]))
						{
						  sprintf(buffer,"%s[B]",buffer);
							glBegin(GL_TRIANGLES);
							  glTexCoord2f(0.6875+0.0625-one_texel,0.5625-0.0625+one_texel); glVertex3f(0.82+0.2,-0.5+0.2,-2);
							  glTexCoord2f(0.6875-0.0625+one_texel,0.5625-0.0625+one_texel); glVertex3f(0.82-0.2,-0.5+0.2,-2);
							  glTexCoord2f(0.6875                 ,0.5625                 ); glVertex3f(0.82    ,-0.5    ,-2);
							glEnd();
						}
						if (*(ZWattribute->joystick[data->activplayer].button[2]))
						{
						  sprintf(buffer,"%s[C]",buffer);
							glBegin(GL_TRIANGLES);
							  glTexCoord2f(0.8125+0.0625-one_texel,0.5625+0.0625-one_texel); glVertex3f(0.82+0.2,-0.5-0.2,-2);
							  glTexCoord2f(0.8125+0.0625-one_texel,0.5625-0.0625+one_texel); glVertex3f(0.82+0.2,-0.5+0.2,-2);
							  glTexCoord2f(0.8125                 ,0.5625                 ); glVertex3f(0.82    ,-0.5    ,-2);
							glEnd();
						}
						if (*(ZWattribute->joystick[data->activplayer].button[3]))
						{
						  sprintf(buffer,"%s[D]",buffer);
							glBegin(GL_TRIANGLES);
							  glTexCoord2f(0.9375-0.0625+one_texel,0.5625+0.0625-one_texel); glVertex3f(0.82-0.2,-0.5-0.2,-2);
							  glTexCoord2f(0.9375+0.0625-one_texel,0.5625+0.0625-one_texel); glVertex3f(0.82+0.2,-0.5-0.2,-2);
							  glTexCoord2f(0.9375                 ,0.5625                 ); glVertex3f(0.82    ,-0.5    ,-2);
							glEnd();
						}
						glColor4f(0,0,0,0.8);
						ZWdrawtext(text,-1.3, 0.5-6.0*abstand,-2.0,buffer,0.15);
		        ZWdrawtextmiddle(text,0, 0.5-7.0*abstand,-2.0,(char*)"[A][C] zum Beenden und [B][D] für einen Reset drücken!",0.12);						
		case 5: ZWdrawtext(text,-1.3, 0.5-5.0*abstand,-2.0,(char*)"Drück den Button für [D]",0.15);
		case 4: ZWdrawtext(text,-1.3, 0.5-4.0*abstand,-2.0,(char*)"Drück den Button für [C]",0.15);
		case 3: ZWdrawtext(text,-1.3, 0.5-3.0*abstand,-2.0,(char*)"Drück den Button für [B]",0.15);
		case 2: ZWdrawtext(text,-1.3, 0.5-2.0*abstand,-2.0,(char*)"Drück den Button für [A]",0.15);
		case 1: ZWdrawtext(text,-1.3, 0.5-1.0*abstand,-2.0,(char*)"Betätige die Achse für [^] und [v]",0.15);
		case 0: ZWdrawtext(text,-1.3, 0.5-0.0*abstand,-2.0,(char*)"Betätige die Achse für [<] und [>]",0.15);
	}
	ZWdrawtextmiddle(text,0,-0.9 ,-2.0,(char*)"Hinweis: Die Tasten so drücken, so dass",0.1);
	ZWdrawtextmiddle(text,0,-0.97 ,-2.0,(char*)"die Belegung wie auf dem Beispielcontroller ist!",0.1);
	
	
}

void calc_ingame_kalib(pmaindata data)
{
	int a;
	switch (data->ingame_kalib_step)
	{
		case 0: //Links-Rechts
		  for (a=0;a<ZWattribute->realjoystick[data->ingame_kalib_joystick].axiscount;a++)
			  if (ZWattribute->realjoystick[data->ingame_kalib_joystick].axis[a]!=0)
				{
					ZWattribute->joystick[data->activplayer].axis[0]=&(ZWattribute->realjoystick[data->ingame_kalib_joystick].axis[a]);
					data->ingame_kalib_map[0]=a;
					data->ingame_kalib_step++;
					break;
				}		
		break;
		case 1: //Oben-Unten
		  for (a=0;a<ZWattribute->realjoystick[data->ingame_kalib_joystick].axiscount;a++)
			{
				if (ZWattribute->joystick[data->activplayer].axis[0]==&(ZWattribute->realjoystick[data->ingame_kalib_joystick].axis[a]))
				  continue;
			  if (ZWattribute->realjoystick[data->ingame_kalib_joystick].axis[a]!=0)
				{
					ZWattribute->joystick[data->activplayer].axis[1]=&(ZWattribute->realjoystick[data->ingame_kalib_joystick].axis[a]);
					data->ingame_kalib_map[1]=a;
					data->ingame_kalib_step++;
					break;
				}		
			}
		break;
		case 2: //Button A
		  for (a=0;a<ZWattribute->realjoystick[data->ingame_kalib_joystick].buttoncount;a++)
			  if (ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a])
				{
					ZWattribute->joystick[data->activplayer].button[0]=&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]);
					data->ingame_kalib_map[2]=a;
					data->ingame_kalib_step++;
					break;
				}		
		break;
		case 3: //Button B
		  for (a=0;a<ZWattribute->realjoystick[data->ingame_kalib_joystick].buttoncount;a++)
			{
				if (ZWattribute->joystick[data->activplayer].button[0]==&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]))
				  continue;
			  if (ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a])
				{
					ZWattribute->joystick[data->activplayer].button[1]=&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]);
					data->ingame_kalib_map[3]=a;
					data->ingame_kalib_step++;
					break;
				}		
			}
		break;
		case 4: //Button C
		  for (a=0;a<ZWattribute->realjoystick[data->ingame_kalib_joystick].buttoncount;a++)
			{
				if (ZWattribute->joystick[data->activplayer].button[0]==&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]))
				  continue;
				if (ZWattribute->joystick[data->activplayer].button[1]==&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]))
				  continue;
			  if (ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a])
				{
					ZWattribute->joystick[data->activplayer].button[2]=&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]);
					data->ingame_kalib_map[4]=a;
					data->ingame_kalib_step++;
					break;
				}		
			}
		break;
		case 5: //Button D
		  for (a=0;a<ZWattribute->realjoystick[data->ingame_kalib_joystick].buttoncount;a++)
			{
				if (ZWattribute->joystick[data->activplayer].button[0]==&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]))
				  continue;
				if (ZWattribute->joystick[data->activplayer].button[1]==&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]))
				  continue;
				if (ZWattribute->joystick[data->activplayer].button[2]==&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]))
				  continue;
			  if (ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a])
				{
					ZWattribute->joystick[data->activplayer].button[3]=&(ZWattribute->realjoystick[data->ingame_kalib_joystick].button[a]);
					data->ingame_kalib_map[5]=a;
					data->ingame_kalib_step++;
					break;
				}		
			}
		break;
		case 6: //Beenden oder von vorne
			if (*(ZWattribute->joystick[data->activplayer].button[0]))
			if (*(ZWattribute->joystick[data->activplayer].button[2]))
			{
				*(ZWattribute->joystick[data->activplayer].button[0])=0;
				*(ZWattribute->joystick[data->activplayer].button[2])=0;
			  data->mainloop_pos=2;
				new_controller_db_entry(ZWattribute->realjoystick[data->ingame_kalib_joystick].name,data->ingame_kalib_map[2],data->ingame_kalib_map[3],data->ingame_kalib_map[4],data->ingame_kalib_map[5],data->ingame_kalib_map[0],data->ingame_kalib_map[1]);
				save_controller_db();
				data->player[data->activplayer].has_calib=1;
			}
			if (*(ZWattribute->joystick[data->activplayer].button[1]))
			if (*(ZWattribute->joystick[data->activplayer].button[3]))
			{
				*(ZWattribute->joystick[data->activplayer].button[1])=0;
				*(ZWattribute->joystick[data->activplayer].button[3])=0;
			  data->ingame_kalib_step=0;
			}
		break;
		
	}
}
