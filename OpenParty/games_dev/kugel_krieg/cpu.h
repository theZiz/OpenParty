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

void recalc_path(pdata data,int start_player)
{
  int path_cost=9999; //=infinity :P
  int a,b;
  for (a=0;a<20;a++)
    for (b=0;b<20;b++)
      data->color[a][b]=0; //white = a virgin field
  
  int startx=(int)(data->x[start_player])/2;
  int starty=(int)(data->y[start_player])/2;
  
  data->color[startx][starty]=1; //blue
  data->path_cost[startx][starty]=0;
  
  int player;
  for (player=0;player<data->gameinfo->playernum;player++)
  {
    if (player==start_player || data->live[player]<=0 || data->gameinfo->team[player]==data->gameinfo->team[start_player])
      continue;
    int found_blue=1;
    while (found_blue)
    {
      found_blue=0;
      //searching all blue fields
      for (a=0;a<20;a++)
        for (b=0;b<20;b++)
          if (data->color[a][b]==1) //blue
          {
            data->color[a][b]=2; //black
            found_blue=1;
            //locking at all 4 neighbours
            //left
            if (a-1>=0 && data->block[a-1][b]==0) //neighbour exists?
            {
              switch (data->color[a-1][b])
              {
                case 0: //white
                        data->path_cost[a-1][b]=data->path_cost[a][b]+1;
                        data->color[a-1][b]=1; //blue
                        data->prex[a-1][b]=a;
                        data->prey[a-1][b]=b;
                        break;
                case 1: case 2://blue or black
                        if (data->path_cost[a-1][b]>data->path_cost[a][b]+1 || (data->path_cost[a-1][b]==data->path_cost[a][b]+1 && (a+b)%2==0))
                        {
                          data->path_cost[a-1][b]=data->path_cost[a][b]+1;
                          data->prex[a-1][b]=a;
                          data->prey[a-1][b]=b;
                        }
                        break;
              }
            }
            //right
            if (a+1<20 && data->block[a+1][b]==0) //neighbour exists?
            {
              switch (data->color[a+1][b])
              {
                case 0: //white
                        data->path_cost[a+1][b]=data->path_cost[a][b]+1;
                        data->color[a+1][b]=1; //blue
                        data->prex[a+1][b]=a;
                        data->prey[a+1][b]=b;
                        break;
                case 1: case 2://blue or black
                        if (data->path_cost[a+1][b]>data->path_cost[a][b]+1 || (data->path_cost[a+1][b]==data->path_cost[a][b]+1 && (a+b)%2==0))
                        {
                          data->path_cost[a+1][b]=data->path_cost[a][b]+1;
                          data->prex[a+1][b]=a;
                          data->prey[a+1][b]=b;
                        }
                        break;
              }
            }
            //up
            if (b-1>=0 && data->block[a][b-1]==0) //neighbour exists?
            {
              switch (data->color[a][b-1])
              {
                case 0: //white
                        data->path_cost[a][b-1]=data->path_cost[a][b]+1;
                        data->color[a][b-1]=1; //blue
                        data->prex[a][b-1]=a;
                        data->prey[a][b-1]=b;
                        break;
                case 1: case 2://blue or black
                        if (data->path_cost[a][b-1]>data->path_cost[a][b]+1 || (data->path_cost[a][b-1]==data->path_cost[a][b]+1 && (a+b)%2==0))
                        {
                          data->path_cost[a][b-1]=data->path_cost[a][b]+1;
                          data->prex[a][b-1]=a;
                          data->prey[a][b-1]=b;
                        }
                        break;
              }
            }
            //down
            if (b+1<20 && data->block[a][b+1]==0) //neighbour exists?
            {
              switch (data->color[a][b+1])
              {
                case 0: //white
                        data->path_cost[a][b+1]=data->path_cost[a][b]+1;
                        data->color[a][b+1]=1; //blue
                        data->prex[a][b+1]=a;
                        data->prey[a][b+1]=b;
                        break;
                case 1: case 2://blue or black
                        if (data->path_cost[a][b+1]>data->path_cost[a][b]+1 || (data->path_cost[a][b+1]==data->path_cost[a][b]+1 && (a+b)%2==0))
                        {
                          data->path_cost[a][b+1]=data->path_cost[a][b]+1;
                          data->prex[a][b+1]=a;
                          data->prey[a][b+1]=b;
                        }
                        break;
              }
            }
          }
    }
    //calculating path cost
    int momx=(int)(data->x[player])/2;
    int momy=(int)(data->y[player])/2;
    int cost=0;
    while (!((momx==(int)(data->x[start_player])/2) && (momy==(int)(data->y[start_player])/2)))
    {      
      cost++;
      int old_momx=momx;
      momx=data->prex[momx][momy];
      momy=data->prey[old_momx][momy];
    }
    if (cost<path_cost)
    {
      path_cost=cost;
      //searching farest away seeable block
      momx=(int)(data->x[player])/2;
      momy=(int)(data->y[player])/2;
      int beforex=momx;
      int beforey=momy;
      while (!((momx==(int)(data->x[start_player])/2) && (momy==(int)(data->y[start_player])/2)))
      {      
        //Calculating the Vector from the momblock to the start
        float testx=(float)momx*2.0+1.0;
        float testy=(float)momy*2.0+1.0;
        float vectorx=data->x[start_player]-testx;
        float vectory=data->y[start_player]-testy;
        float l=sqrt(vectorx*vectorx+vectory*vectory)*2.0;
        int count=(int)(l);
        if (l==0) l=1;
        vectorx/=l;
        vectory/=l;
        char succes=1;
        int a;
        for (a=0;a<count;a++)
        {
          testx+=vectorx;
          testy+=vectory;
          if (data->block[(int)(testx    )/2][(int)(testy    )/2] ||
              data->block[(int)(testx-1.0)/2][(int)(testy    )/2] ||
              data->block[(int)(testx+1.0)/2][(int)(testy    )/2] ||
              data->block[(int)(testx    )/2][(int)(testy+1.0)/2] ||
              data->block[(int)(testx    )/2][(int)(testy-1.0)/2])
          {
            succes=0;
            break; 
          }
        }
        if (succes)
          break;
        beforex=momx;
        beforey=momy;
        int old_momx=momx;
        momx=data->prex[momx][momy];
        momy=data->prey[old_momx][momy];
      }
      data->goalx[start_player]=momx;
      data->goaly[start_player]=momy;
      if (data->goalx[start_player]==(int)(data->x[start_player])/2 && data->goaly[start_player]==(int)(data->y[start_player])/2)
      {
        data->goalx[start_player]=beforex;
        data->goaly[start_player]=beforey;
      }
      if ((int)(data->x[player])/2==data->goalx[start_player] && (int)(data->y[player])/2==data->goaly[start_player])
        data->goal_is_enemy[start_player]=1;
      else
        data->goal_is_enemy[start_player]=0;
      data->enemy[start_player]=player;
    }
    /*//if (player==0) //Debugausgabe?
    {
      printf("Pfad von Spieler %i (%f:%f) zu Spieler %i (%f:%f) (Rückwärts):\n",start_player,data->x[start_player],data->y[start_player],player,data->x[player],data->y[player]);
      momx=(int)(data->x[player])/2;
      momy=(int)(data->y[player])/2;
      int color[20][20];
      for (b=0;b<20;b++)
        for (a=0;a<20;a++)    
          color[a][b]=0;
      while (!((momx==(int)(data->x[start_player])/2) && (momy==(int)(data->y[start_player])/2)))
      {
        printf("%i:%i <-",momx,momy);
        if (momx==(int)(data->x[player])/2 && momy==(int)(data->y[player])/2)
          color[momx][momy]=1;
        else
          color[momx][momy]=2;
        int old_momx=momx;
        momx=data->prex[momx][momy];
        momy=data->prey[old_momx][momy];
      }
      color[momx][momy]=3;
      color[data->goalx[start_player]][data->goaly[start_player]]=4;      
      printf("%i:%i\n",momx,momy);
      if (data->goal_is_enemy[start_player])
        printf("Goal is enemy!\n");
      
      for (b=0;b<20;b++)
      {
        for (a=0;a<20;a++)
          if (data->block[a][b])
            printf("#");
          else
          switch (color[a][b])
          {
            case 0: printf(" "); break;
            case 1: printf("G"); break;
            case 2: printf("."); break;
            case 3: printf("S"); break;
            case 4: printf("*"); break;
          }
        printf("\n");  
      }
    }*/
  }
}

void recalc_paths(pdata data)
{
  int player;
	for (player=0;player<data->gameinfo->playernum;player++)
	{
		if (!(data->gameinfo->iscpu[player]) || data->live[player]<=0)
		  continue;
    recalc_path(data,player);
  }
  //printf("---\n");
}

void calc_cpu(pdata data)
{
  int player;
  for (player=0;player<data->gameinfo->playernum;player++)
  {
    if ((data->oldboxx[player]!=(int)(data->x[player])/2) || (data->oldboxy[player]!=(int)(data->y[player])/2))
      data->refresh=1;
    data->oldboxx[player]=(int)(data->x[player])/2;
    data->oldboxy[player]=(int)(data->y[player])/2;
  }
  if (data->refresh)
    recalc_paths(data);
  data->refresh=0;
	for (player=0;player<data->gameinfo->playernum;player++)
	{
		if (!(data->gameinfo->iscpu[player]) || data->live[player]<=0)
		  continue;
    
    //looking at the goal?
    float vectorx;
    float vectory;
    if (data->goal_is_enemy[player])
    {
      vectorx=data->x[data->enemy[player]]-data->x[player];
      vectory=data->y[data->enemy[player]]-data->y[player];
    }
    else
    {
      vectorx=(float)(data->goalx[player])*2.0+1.0-data->x[player];
      vectory=(float)(data->goaly[player])*2.0+1.0-data->y[player];
    }

    float l=sqrt(vectorx*vectorx+vectory*vectory);
    if (l==0) l=1;
    vectorx/=l;
    vectory/=l;
    float goal_angle=180.0-acos(vectory)*180.0/M_PI;
    if (vectorx<0)
      goal_angle=360.0-goal_angle;
    
    //go for it, if the angle distance is <10
    float distance=fabs(goal_angle-data->a[player]);
    if (distance>180.0)
      distance=360.0-distance;

    //printf("Eigener Winkel: %f\nWinkel des Ziels: %f\nEntfernung: %f\n",data->a[player],goal_angle,distance);
    
		if (data->lastshot[player]>0)
		{
			data->lastshot[player]-=1;//*(data->gameinfo->steps);
		}
	  int a;
	  for (a=0;a<4;a++)
		  *(data->gameinfo->joystick[player].button[a])=0;
	  for (a=0;a<2;a++)
		  *(data->gameinfo->joystick[player].axis[a])=0;
    if (distance<10.0)
    {
			*(data->gameinfo->joystick[player].button[1])=1;
			*(data->gameinfo->joystick[player].axis[1])=-1;
      if ((data->goal_is_enemy[player]) && (l<13.0) && (data->lastshot[player]<=0))
			{			
			  //Shot
				data->lastshot[player]=2000;
				*(data->gameinfo->joystick[player].button[2])=1;
			}
    }
    else
    {
      int direction=0; //-1 left, 1 right
      if (data->a[player]<goal_angle)
      {
        if (goal_angle-data->a[player]<180.0)
          direction=-1;
        else
          direction=1;
      }
      else
      {
        if (data->a[player]-goal_angle<180.0)
          direction=1;
        else
          direction=-1;
      }      
      
      if (direction>0)
      {
        *(data->gameinfo->joystick[player].button[1])=1;
        if (rand()%2==0)
          *(data->gameinfo->joystick[player].axis[1])=1;        
      }
      else
      {
        if (rand()%2==0)
          *(data->gameinfo->joystick[player].button[3])=1;
        *(data->gameinfo->joystick[player].axis[1])=-1;        
      }
      
    }
    
/*		if (data->lastshot[player]>0)
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
			  //SchieÃŸen
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
		}*/
	}
}
