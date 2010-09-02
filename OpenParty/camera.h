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
void resetcamera(void)
{
  maindata.destcameraposition.x=0;
  maindata.destcameraposition.y=0;
  maindata.destcameraposition.z=0;
  maindata.destcamerarotation.x=0;
  maindata.destcamerarotation.y=0;
  maindata.destcamerarotation.z=0;

  maindata.momcameraposition=maindata.destcameraposition;
  maindata.momcamerarotation=maindata.destcamerarotation;
  maindata.time=0;
}

void calcmaindata(pmaindata data)
{
  if (data->time>0)
  {
    //Cameraposition
    float von=data->momcameraposition.x;
    float bis=data->destcameraposition.x;
    float abstand=bis-von;
    data->momcameraposition.x+=abstand*(0.001/data->time);

    von=data->momcameraposition.y;
    bis=data->destcameraposition.y;
    abstand=bis-von;
    data->momcameraposition.y+=abstand*(0.001/data->time);

    von=data->momcameraposition.z;
    bis=data->destcameraposition.z;
    abstand=bis-von;
    data->momcameraposition.z+=abstand*(0.001/data->time);

    //Camerarotation
    von=data->momcamerarotation.x;
    bis=data->destcamerarotation.x;
    if (abs(von-bis)>180)
    {
      if (von<bis) von+=360;
        else von-=360;   
    }
    abstand=bis-von;
    data->momcamerarotation.x=von+abstand*(0.001/data->time);
    
    von=data->momcamerarotation.y;
    bis=data->destcamerarotation.y;
    if (abs(von-bis)>180)
    {
      if (von<bis) von+=360;
        else von-=360;   
    }
    abstand=bis-von;
    data->momcamerarotation.y=von+abstand*(0.001/data->time);
    
    von=data->momcamerarotation.z;
    bis=data->destcamerarotation.z;
    if (abs(von-bis)>180)
    {
      if (von<bis) von+=360;
        else von-=360;   
    }
    abstand=bis-von;
    data->momcamerarotation.z=von+abstand*(0.001/data->time);
        
    data->time-=0.001;
  }  
}

void calcplayer(pmaindata data,int activplayer)
{
  int a;
  for (a=0;a<playernum;a++)
  {
    if (data->player[a].time>0)
    {
      //Position
      float von=data->player[a].momposition.x;
      float bis=data->player[a].destposition.x;
      float abstand=bis-von;
      data->player[a].momposition.x+=abstand*(0.001/data->player[a].time);

      von=data->player[a].momposition.y;
      bis=data->player[a].destposition.y;
      abstand=bis-von;
      data->player[a].momposition.y+=abstand*(0.001/data->player[a].time);

      von=data->player[a].momposition.z;
      bis=data->player[a].destposition.z;
      abstand=bis-von;
      data->player[a].momposition.z+=abstand*(0.001/data->player[a].time);

      //Größe
      von=data->player[a].momsize;
      bis=data->player[a].destsize;
      abstand=bis-von;
      data->player[a].momsize+=abstand*(0.001/data->player[a].time);
			
      //Face    
      
      if (a==activplayer)
      {
				von=data->player[a].momface;
				bis=data->player[a].destface;
				if (abs(von-bis)>180)
				{
					if (von<bis) von+=360;
						else von-=360;   
				}
				abstand=bis-von;
				data->player[a].momface=von+abstand*(0.001/data->player[a].time);    
  			data->player[a].savedcamera_y=+data->momcamerarotation.y;
		  }
      data->player[a].time-=0.001;
    }  
  }
}

float calcnewdestface(int playernr,pmaindata data)
{
  if (data->player[playernr].momset!=NULL)
  {
    float v1x=(setbynr[data->player[playernr].momset->gamenextnr])->position.x-data->player[playernr].momset->position.x;
    float v1z=(setbynr[data->player[playernr].momset->gamenextnr])->position.z-data->player[playernr].momset->position.z;
    if (v1x==0 && v1z==0)
    {
      v1x=(setbynr[data->player[playernr].momset->gamenextnr])->position.x-data->player[playernr].momset->position.x;
      v1z=(setbynr[data->player[playernr].momset->gamenextnr])->position.z-data->player[playernr].momset->position.z;
    }

    float v2x=data->player[playernr].momset->cameraposition.x-data->player[playernr].momset->position.x;
    float v2z=data->player[playernr].momset->cameraposition.z-data->player[playernr].momset->position.z;
    float b1=sqrt(v1x*v1x+v1z*v1z);
    float b2=sqrt(v2x*v2x+v2z*v2z);
    float winkel=90;
    if (b1*b2!=0)
      winkel=acos((v1x*v2x+v1z*v2z)/(b1*b2))*180.0/M_PI;
    if (v1x*v2z-v1z*v2x<0) winkel=-winkel;
    return winkel;
  }
  return 0;
}

void sync_all_position(float time,pmaindata data)
{
  int playercount;
  for (playercount=0;playercount<playernum;playercount++)
  {
    //schauen, ob ein anderer Player hier auch steht:
    int other=0;
    int nr=0;
    int a;
		int activplayer=-1;
		int activplayer_nr=0;
    for (a=0;a<playernum;a++)
    {
      if (data->player[playercount].momset==data->player[a].momset)
      {
        other++;
				if (a<=data->activplayer || activplayer==-1)
				  activplayer=a;
				if (a<data->activplayer)
				  activplayer_nr++;
        if (a<playercount)
          nr++;
      }
    }
		float winkel_add;
		if (activplayer<0)
		  activplayer=playercount;
		winkel_add=M_PI/2.0-data->player[activplayer].momset->camerarotation.y*M_PI/180.0-((float)activplayer_nr)*M_PI*2.0/((float)other);
    data->player[playercount].destposition=data->player[playercount].momset->position;
		float size=pow((float)other,-0.2);
    if (other>1)
    {
      float winkel=((float)nr)*M_PI*2.0/((float)other)+winkel_add;
      float r=size/2.0/sin(M_PI/((float)(other)));
      data->player[playercount].destposition.x+=cos(winkel)*r;
      data->player[playercount].destposition.z+=sin(winkel)*r;
    }
		data->player[playercount].destsize=size;
    data->player[playercount].destposition.y+=data->player[playercount].momset->maxy;
    data->player[playercount].time=time; 
  }
}
