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
void calc_cpu(pdata data)
{
    int player;
	for (player=0;player<data->gameinfo->playernum;player++)
	{
		if (!(data->gameinfo->iscpu[player]))
		  continue;
		if (data->lastshot[player]>0)
		{
			data->lastshot[player]-=*(data->gameinfo->steps);
		}
	  int a;
	  for (a=0;a<4;a++)
		  *(data->gameinfo->joystick[player].button[a])=0;
	  for (a=0;a<2;a++)
		  *(data->gameinfo->joystick[player].axis[a])=0;
		bool seeplayer=false;
		float r;
		float s;
		int other;
		for (other=0;other<data->gameinfo->playernum;other++)
		{
			if (data->gameinfo->team[other]==data->gameinfo->team[player] || data->live[other]<=0)
			  continue;
			float angle=-data->a[player]*M_PI/180.0;
			float radius=sqrt(5.0/4.0*15.0*15.0);
		  r=point_in_quad_get_r(data->x[player]-sin(angle),data->y[player]-cos(angle),data->x[other],data->y[other],
																  sin(angle-M_PI/3.0)*radius,cos(angle-M_PI/3.0)*radius,sin(angle+M_PI/3.0)*radius,cos(angle+M_PI/3.0)*radius);
		  s=point_in_quad_get_s(data->x[player],data->y[player],data->x[other],data->y[other],
																  sin(angle-M_PI/3.0)*radius,cos(angle-M_PI/3.0)*radius,sin(angle+M_PI/3.0)*radius,cos(angle+M_PI/3.0)*radius);
			if (s+r<=1.0 && s+r>=0.0 && s>=0.0 && s<=1.0 && r>=0.0 && r<=1.0)
			{
				seeplayer=true;
				break;
			}
	  }
		if (seeplayer && !(data->totalkollision[player])) //Ist ein anderer Spieler im Blickfeld?
		{
		  //Auf den Spieler zu bewegen/drehen
			if (fabs(s-r)>0.05) //Wackeln vermeiden und stattdessen drauf zu fahren
			{
				if (s>r)
				{
					*(data->gameinfo->joystick[player].button[1])=1;
					*(data->gameinfo->joystick[player].axis[1])=1;
				}
				else
				{
					*(data->gameinfo->joystick[player].button[3])=1;
					*(data->gameinfo->joystick[player].axis[1])=-1;
				}
		  }
			else
			{
					*(data->gameinfo->joystick[player].button[1])=1;
					*(data->gameinfo->joystick[player].axis[1])=-1;				
			}
			//Ist der Spieler in Schussbahn && letzter Schuss mehr als 2000ms lang her?
			if (fabs(s-r)<0.2 && data->lastshot[player]<=0)
			{			
			  //Schießen
				data->lastshot[player]=2000;
				*(data->gameinfo->joystick[player].button[2])=1;
			}
		}
		else //Sonst:
		{
			if (data->totalkollision[player] || (data->lastchoice[player]<0))
			{
				int left=data->left[player];
				int right=data->right[player];
				do
				{
					data->left[player]=rand()%10-1;
					if (data->left[player]>1)
					  data->left[player]=1;
					data->right[player]=rand()%10-1;
					if (data->right[player]>1)
					  data->right[player]=1;
			  }	while (((data->left[player]==0) && (data->right[player]==0)) || (data->left[player]==left && data->right[player]==right));
				data->lastchoice[player]=rand()%1000;
			}
			*(data->gameinfo->joystick[player].axis[1])=-data->left[player];
			if (data->right[player]<0)
			  *(data->gameinfo->joystick[player].button[3])=1;
			else if (data->right[player]>0)
			  *(data->gameinfo->joystick[player].button[1])=1;
			data->lastchoice[player]-=*(data->gameinfo->steps);
		}
	}
}
