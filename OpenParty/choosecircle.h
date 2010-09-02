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
void draw_choosecircle(pmaindata data)
{
	if (!choosecircle_on)
	  return;
		
	int mapping[8];
	int number=0;
	int a;
	if (choosecircle_face)
	{
		for (a=0;a<8 && number+1<playernum;a++)
			if (choosecircle_content[a][0]!=0)
			{
				mapping[number]=a;
				number++;
			}
	}
	else
		for (number=0;number<8;number++)
		{
			if (choosecircle_content[number][0]==0)
				break;
			mapping[number]=number;
		}
	if (number==0)
		return;
  int per_piece=100 / number;
	int max=per_piece*number;
  glPushMatrix();
	glTranslatef(-0.4,0,0.0);
	glScalef(1.3,1.3,1.0);
	glRotatef(choosecircle_position,0,0,1);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.8,0.8,0.8,0.9);
	glVertex3f(0,0,-2.5);
	float alpha;
	int marked=-1;
	for (int point=max;point>=0;point--)
	{
		if (point%per_piece==0)
		{
			int count=(point/per_piece)%number;
			//ist diese Ecke, die rechts?
			float temp_position=choosecircle_position+180.0/number;
			if (temp_position>=360.0)
			  temp_position-=360.0;
			int count2=count;
			if (count2==0)
			  count2=number;
			if (temp_position<360.0/(float)number*(float)(count2) &&
					temp_position>=360.0/(float)number*(float)((count+number-1)%number))
			{
			  alpha=2.0;
				marked=count;
			}
			else
				alpha=0.5;
			if (choosecircle_face)
				glColor4f(data->player[mapping[count]].color.r,data->player[mapping[count]].color.g,data->player[mapping[count]].color.b,alpha);
			else
				switch (count)
				{
					case 0: glColor4f(1.0,0.0,0.0,alpha); break;
					case 1: glColor4f(0.0,1.0,0.0,alpha); break;
					case 2: glColor4f(1.0,1.0,0.0,alpha); break;
					case 3: glColor4f(0.0,0.0,1.0,alpha); break;
					case 4: glColor4f(1.0,0.0,1.0,alpha); break;
					case 5: glColor4f(0.0,1.0,1.0,alpha); break;
					case 6: glColor4f(0.8,0.3,0.0,alpha); break;
					case 7: glColor4f(0.5,0.5,0.5,alpha); break;
				}
		}
		float x=sin(((float)point-(float)per_piece/2.0)/((float)max)*2.0*M_PI+0.5*M_PI);
		float y=cos(((float)point-(float)per_piece/2.0)/((float)max)*2.0*M_PI+0.5*M_PI);
		glVertex3f(x,y,-2.5);		
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	for (int a=0;a<number;a++)
	{
		//ist diese Ecke, die rechts?
		if (marked==(a+1)%number)
			alpha=1.0;
		else
			alpha=0.5;
		glColor4f(1,1,1,alpha);
		float glpos=0;
		int textpos=0;
		int p=a;
		if (choosecircle_face)
		  p=(a+1)%number;
		while (choosecircle_content[mapping[p]][textpos]!=0)
		{
			int pos=choosecircle_content[mapping[p]][textpos];
			if (pos<0) pos+=256;
			glpos+=text.breite[pos];
			textpos++;
		}		
		if (choosecircle_face)
			ZWdrawsprite(facetex,0.7,0,-2.5,0.2,0.2,4,data->player[mapping[p]].facenr);
		else
			ZWdrawtextmiddle_fac(text,0.55,-0.17/glpos,-2.5,choosecircle_content[mapping[p]],0.9/glpos,0.95);
    
    

		glRotatef(-360.0/(float)number,0,0,1);
	}
	glPopMatrix();
	glPushMatrix();
	ZWenablelight(0);
	ZWsetlightambient(0,0.1,0.1,0.1);
	ZWsetlightdiffuse(0,0.5,0.5,0.5);
	ZWsetlightposition(0,1.5,0,0);
	glTranslatef(1.5,0,-2.5);
	glScalef(0.4,0.4,0.4);
	glRotatef(90,1,0,0);
	glRotatef(90,0,1,0);
	if (choosecircle_face)
    glColor4f(data->player[mapping[marked]].color.r,data->player[mapping[marked]].color.g,data->player[mapping[marked]].color.b,1);
	else
	switch (marked)
	{
		case 0: glColor4f(1.0,0.0,0.0,1); break;
		case 1: glColor4f(0.0,1.0,0.0,1); break;
		case 2: glColor4f(1.0,1.0,0.0,1); break;
		case 3: glColor4f(0.0,0.0,1.0,1); break;
		case 4: glColor4f(1.0,0.0,1.0,1); break;
		case 5: glColor4f(0.0,1.0,1.0,1); break;
		case 6: glColor4f(0.8,0.3,0.0,1); break;
		case 7: glColor4f(0.5,0.5,0.5,1); break;
	}
	glEnable(GL_LIGHTING);
	ZWdrawobject(arrow);
	ZWdisablelight(0);
	glDisable(GL_LIGHTING);
	glPopMatrix();
}

void calc_choosecircle(pmaindata data)
{
	if (!choosecircle_on)
	  return;
	for (int a=0;a<ZWattribute->steps;a++)
	{
		if (choosecircle_direction==0)		
		{
		  choosecircle_speed+=0.0002;
			if (choosecircle_speed>=0.4)
			{
				choosecircle_direction=1;
				choosecircle_time_moved=2000;
			}
		}
		else
		if (choosecircle_direction==1)					
		{
			choosecircle_time_moved--;
			if (choosecircle_time_moved<=0 || rand()%(choosecircle_time_moved/3)==0)
			  choosecircle_direction=2;
		}
		else
		if (choosecircle_direction==2)
		{
			if (choosecircle_speed>0.03)
		    choosecircle_speed*=0.9995;
			else
		    choosecircle_speed-=0.00002;
			
			if (choosecircle_speed<=0.0)
			{
			  choosecircle_speed=0.0;
				choosecircle_direction=3;
			}
		}
		else //"Standphase"
		{
			choosecircle_direction++; //Es werden die Millisekunden in der Verweilphase gezählt.
			if (choosecircle_direction>20000)
			  choosecircle_direction=20000;
		}
		choosecircle_position-=choosecircle_speed;
		if (choosecircle_position>=360.0)
		  choosecircle_position-=360.0;
		if (choosecircle_position<0.0)
		  choosecircle_position+=360.0;
	}
}

void do_choosecircle(bool ___choosecircle_face,char* element0,char* element1,char* element2,
                                               char* element3,char* element4,char* element5,
																							 char* element6,char* element7)
{
	choosecircle_face=___choosecircle_face;
	for (int a=0;a<8;a++)
	{
		char* momelement=NULL;
		switch (a)
		{
			case 0: momelement=element0; break;
			case 1: momelement=element1; break;
			case 2: momelement=element2; break;
			case 3: momelement=element3; break;
			case 4: momelement=element4; break;
			case 5: momelement=element5; break;
			case 6: momelement=element6; break;
			case 7: momelement=element7; break;			
		}
		if (momelement==NULL)
		{
		  choosecircle_content[a][0]=0;
			continue;
		}
		int b;
		for (b=0;b<31 && momelement[b]!=0;b++)
		  choosecircle_content[a][b]=momelement[b];
		choosecircle_content[a][b]=0;
	}
	choosecircle_on=true;
	choosecircle_speed=0.0;
	choosecircle_position=rand()%360;
	choosecircle_direction=0;
}

void kill_choosecircle(void)
{
	int a;
	for (a=0;a<8;a++)
	  choosecircle_content[a][0]=0;
	choosecircle_on=false;
	choosecircle_face=false;
	choosecircle_speed=0.0;
	choosecircle_position=rand()%360;
	choosecircle_direction=0;
}

int choosecircle_get_choice(void)
{
	if (!choosecircle_on)
	  return 0;
	int mapping[8];
	int number=0;
	int a;
	if (choosecircle_face)
	{
		for (a=0;a<8 && number+1<playernum;a++)
			if (choosecircle_content[a][0]!=0)
			{
				mapping[number]=a;
				number++;
			}
	}
	else
		for (number=0;number<8;number++)
		{
			if (choosecircle_content[number][0]==0)
				break;
			mapping[number]=number;
		}
	if (number==0)
		return 0;
	int count;
	for (count=number-1;count>=0;count--)
	{
		//ist diese Ecke, die rechts?
		float temp_position=choosecircle_position+180.0/number;
		if (temp_position>=360.0)
			temp_position-=360.0;
		int count2=count;
		if (count2==0)
			count2=number;
		if (temp_position<360.0/(float)number*(float)(count2) &&
				temp_position>=360.0/(float)number*(float)((count+number-1)%number))
			break;
	}
	if (choosecircle_face)
	  return mapping[(count)%number];
	else
	  return mapping[(count+number-1)%number];
}

void do_choosecircle_one_string(bool ___choosecircle_face,char* s)
{
	char tempc[8][32];
	int a;
	for (a=0;a<8;a++)
	  tempc[a][0]=0;
	int begin=0;
	int number=0;
	int count=0;
	for (a=0;s[a]!=0 && number<8;a++)
	{
		if (s[a]=='|' || count>30)
		{
			s[a]=0;
			sprintf(tempc[number],"%s",&(s[begin]));
			s[a]='|';
			begin=a+1;
			number++;
			count=0;
		}
		else
			count++;
	}
  do_choosecircle(___choosecircle_face,tempc[0],tempc[1],tempc[2],tempc[3],tempc[4],tempc[5],tempc[6],tempc[7]);
}

void do_choosecircle_without_team_of(int p)
{
	int team=maindata.player[p].team;
	char dummy[6]="dummy";
	char* tempc[8];
	int a;
	for (a=0;a<8;a++)
	  if (a<playernum && maindata.player[a].team!=team)
      tempc[a]=dummy;
		else
		  tempc[a]=NULL;
	do_choosecircle(true,tempc[0],tempc[1],tempc[2],tempc[3],tempc[4],tempc[5],tempc[6],tempc[7]);
}
