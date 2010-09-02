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
bool line_ok(pdata data,int street,int line)
{
	if (street<0)
	  street=0;
	if (street>3)
	  street=3;
	if (line<0)
	  line=0;
	if (line>63)
	  line=63;
	return data->floor_green[street][line];
}

bool gap_in_street(pdata data,int street,float x,float left,float right)
{
	int car;
	for (car=0;car<10;car++)
	{
	  if (data->street_pos[street][car]==INFINITY)
		  continue;
		if (fabs(data->street_pos[street][car]-x)<=right)
		  return false;
		if (fabs(data->street_pos[street][car]-x)<=left)
		  return false;
	}
	return true;
}

bool gap_in_disco(pdata data,int street,float x,int left,int right)
{
	int line=(int)(round(32.0*x/27.0+31.5));
	int von=line-left;
	if (von<0)
	  von=0;
	int bis=line+right;
	if (bis>63)
	  bis=63;
	for (line=von;line<=bis;line++)
	  if (!(line_ok(data,street,line)))
      return false;
	return true;
}


void calc_cpu(pdata data,int p)
{
	if (!(data->gameinfo->iscpu[p]))
	  return;
	*(data->gameinfo->joystick[p].axis[0])=0;
	*(data->gameinfo->joystick[p].axis[1])=0;
	pphysik_object player=data->player[p];
	
	if (data->ki_impuls[p]>0)
	{
		data->ki_impuls[p]--;
		if (rand()%10<data->ki_speed[p])
		{
			
			switch (data->ki_impuls_dir[p])
			{
				case 0: *(data->gameinfo->joystick[p].axis[1])=-1; break;
				case 1: if (player->position.x<24.0) *(data->gameinfo->joystick[p].axis[0])= 1; 
								*(data->gameinfo->joystick[p].axis[1])=-1; break;
				case 2: *(data->gameinfo->joystick[p].axis[0])= 1; break;			
				case 3: if (player->position.x<24.0) *(data->gameinfo->joystick[p].axis[0])= 1; 
								*(data->gameinfo->joystick[p].axis[1])= 1; break;
				case 4: *(data->gameinfo->joystick[p].axis[1])= 1; break;
				case 5: if (player->position.x>-24.0) *(data->gameinfo->joystick[p].axis[0])=-1; 
								*(data->gameinfo->joystick[p].axis[1])= 1; break;
				case 6: *(data->gameinfo->joystick[p].axis[0])=-1; break;			
				case 7: if (player->position.x>-24.0) *(data->gameinfo->joystick[p].axis[0])=-1; 
								*(data->gameinfo->joystick[p].axis[1])=-1; break;
			}
		}
	}
	
	if (player->position.y<-17.8) // Zone "unten"
	{
		if (data->ki_impuls[p]<=0)
		{
			data->ki_impuls[p]=rand()%2000;
			data->ki_impuls_dir[p]=(rand()%3==0)?2:6;
			data->ki_speed[p]=10;
		}
		if (gap_in_street(data,3,player->position.x,6.0,5.0))
		{
			data->ki_speed[p]=10;
			data->ki_impuls[p]=1;
			data->ki_impuls_dir[p]=1;
		}
	}
	else
	if (player->position.y<-15.8) //so halb auf der 3. Straße
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=1;
		data->ki_speed[p]=5;
	}
	else
	if (player->position.y<-13.8) //Trucks verfolgen und Lücke in Straße 2 suchen
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=2;
		data->ki_speed[p]=4;
		if (gap_in_street(data,2,player->position.x,5.0,1.0))
		{
			data->ki_speed[p]=10;
			data->ki_impuls[p]=1;
			data->ki_impuls_dir[p]=1;
		}
	}
	else
	if (player->position.y<-12.0) //so halb auf der 2. Straße
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=1;
		data->ki_speed[p]=5;
	}	
	else
	if (player->position.y<-9.5) //Autos verfolgen und Lücke in Straße 1 suchen
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=2;
		data->ki_speed[p]=10;
		if (gap_in_street(data,1,player->position.x,0.0,8.0))
		{
			data->ki_speed[p]=10;
			data->ki_impuls[p]=1;
			data->ki_impuls_dir[p]=7;
		}
	}
	else
	if (player->position.y<-7.2) //so halb auf der 1. Straße
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=7;
		data->ki_speed[p]=5;
	}	
	else
	if (player->position.y<-5.4) //Autos verfolgen und Lücke in Straße 0 suchen
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=6;
		data->ki_speed[p]=10;
		if (gap_in_street(data,0,player->position.x,5.0,6.0))
		{
			data->ki_speed[p]=10;
			data->ki_impuls[p]=1;
			data->ki_impuls_dir[p]=7;
		}
	}
	else
	if (player->position.y<-2.0) //Hauptsache hoch!
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=7;
		data->ki_speed[p]=7;
	}	
	else
	if (player->position.y<3.5) //1. Breakpoint
	{
		if ((player->position.x<=-16.0) ||
			 ((player->position.x>-8.5) && (player->position.x<=-2.5)) ||
			 ((player->position.x> 8.5) && (player->position.x<=14.5)))
	  {
			data->ki_impuls[p]=1;
			data->ki_impuls_dir[p]=2;
			data->ki_speed[p]=5;
		}
		else
		if ((player->position.x> 16.0) ||
			 ((player->position.x>  2.5) && (player->position.x<= 8.5)) ||
			 ((player->position.x>-14.5) && (player->position.x<=-8.5)))
	  {
			data->ki_impuls[p]=1;
			data->ki_impuls_dir[p]=6;
			data->ki_speed[p]=5;
		}
		else
		if (gap_in_disco(data,0,player->position.x,7,-1))
		{
			data->ki_impuls[p]=1;
		  data->ki_impuls_dir[p]=0;
			data->ki_speed[p]=5;
		}
		
	}	
	else
	if (player->position.y<4.0) // auf der 0. Bahn (unten)
	{
			data->ki_impuls[p]=1;
			data->ki_impuls_dir[p]=1;
			data->ki_speed[p]=5;
	}
	else
	if (player->position.y<6.5) // auf der 0. Bahn (oben) und Lücke suchen
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=2;
		data->ki_speed[p]=8;
		if (gap_in_disco(data,1,player->position.x,-1,7))
		{
			data->ki_impuls[p]=1;
		  data->ki_impuls_dir[p]=7;
			data->ki_speed[p]=7;
		}
	}
	else
	if (player->position.y<8.0) // auf der 1. Bahn (unten)
	{
			data->ki_impuls[p]=1;
			data->ki_impuls_dir[p]=7;
			data->ki_speed[p]=7;
	}
	else
	if (player->position.y<10.0) // auf der 1. Bahn (oben) und Lücke suchen
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=6;
		data->ki_speed[p]=8;
		if (gap_in_disco(data,2,player->position.x,7,-1))
		{
			data->ki_impuls[p]=1;
		  data->ki_impuls_dir[p]=1;
			data->ki_speed[p]=7;
		}
	}
	else
	if (player->position.y<11.0) // auf der 2. Bahn (unten)
	{
			data->ki_impuls[p]=1;
			data->ki_impuls_dir[p]=1;
			data->ki_speed[p]=7;
	}
	else
	if (player->position.y<13.0) // auf der 2. Bahn (oben) und Lücke suchen
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=2;
		data->ki_speed[p]=8;
		if (gap_in_disco(data,3,player->position.x,-1,7))
		{
			data->ki_impuls[p]=1;
		  data->ki_impuls_dir[p]=1;
			data->ki_speed[p]=10;
		}
	}
	else
	{
		data->ki_impuls[p]=1;
		data->ki_impuls_dir[p]=0;
		data->ki_speed[p]=10;
	}
	

}
