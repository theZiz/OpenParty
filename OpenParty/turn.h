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
int delaycounter=0;

void calc_turn(pmaindata data)
{
  int step=0;
  for (step=0;step<ZWattribute->steps && data->mainloop_pos==3 && data->special_step==0;step++)
  {
    switch (data->turn_pos)
    {
      case 1: //Würfeln
        data->turn_dice_next+=1.0/data->dice_speed;
        while (data->turn_dice_next>1.0)
        {
          data->turn_dice_next-=1.0;
          data->turn_dice_value[0]++;
          if (data->turn_dice_value[0]>6) data->turn_dice_value[0]=1;
        }
        if ((data->player[data->activplayer].iscpu && rand()%200==0) || 
            *(ZWattribute->joystick[data->activplayer].button[0])==1 )
        {
          *(ZWattribute->joystick[data->activplayer].button[0])=0;
          data->turn_pos=2; //Jump up
          data->turn_jump_high=0;
        }
      break;
      case 2: //jump up
        //wie Würfeln:
        data->turn_dice_next+=1.0/data->dice_speed;
        while (data->turn_dice_next>1.0)
        {
          data->turn_dice_next-=1.0;
          data->turn_dice_value[0]++;
          if (data->turn_dice_value[0]>6) data->turn_dice_value[0]=1;
        }
        data->turn_jump_high+=1/500.0;
        if (data->turn_jump_high>1.0)
        {
          data->turn_jump_high=1.0;
          data->turn_pos=3; //Jump down
          //Würfelgeräusch weg:
          Mix_HaltChannel(dice_channel);
          Mix_PlayChannel(-1,snd_dice_plop,0);
        }
      break;
      case 3: //jump down
        data->turn_jump_high-=1.0/500.0;
        if (data->turn_jump_high<=0.0)
        {
          data->turn_jump_high=0.0;
          if (data->turn_dice_next>=0.5)
            data->turn_dice_value[0]++;
          if (data->turn_dice_value[0]>6)
            data->turn_dice_value[0]=1;
										
          data->turn_dice_next=0.0;
					if (data->turn_dice_num<data->turn_dice_dest)
					{
						data->turn_dice_value[data->turn_dice_num]=data->turn_dice_value[0];
						data->turn_dice_value[0]=rand()%6+1;
						data->turn_dice_num++;
						data->turn_pos=1;
						dice_channel=Mix_PlayChannel(-1,snd_dice,-1);
					}
					else
					{
						data->turn_script_pos=0;
						data->turn_pos=4; //Trigger_Out
					}
					//Jump Geräusch
					Mix_PlayChannel(-1,snd_jump,0);
					//Partikeleffekte:
					if (strcmp(data->player[data->activplayer].momset->type,(char*)"blue")==0)
					 ZWnewcolorcircle(data->player[data->activplayer].momposition,32,0.002,maintex,16,200,0.15,0.5,0.7,1,0.75,0.000002,0.2);
					else  
					if (strcmp(data->player[data->activplayer].momset->type,(char*)"red")==0)
						ZWnewcolorcircle(data->player[data->activplayer].momposition,32,0.002,maintex,16,200,0.15,1,0.5,0.5,0.75,0.000002,0.2);
					else
						ZWnewcolorcircle(data->player[data->activplayer].momposition,32,0.002,maintex,16,200,0.15,1.0,1.0,0.7,0.75,0.000002,0.2);
					break;
        }
        if (data->turn_dice_next>=0.5)
        {
          data->turn_dice_next+=(1.0-data->turn_dice_next)*1.0/(data->turn_jump_high*1000.0);
          if (data->turn_dice_next>=1.0) data->turn_dice_next=0.999;
        }
        else
        {
          data->turn_dice_next-=(data->turn_dice_next)*1.0/(data->turn_jump_high*1000.0);
          if (data->turn_dice_next<0.0) data->turn_dice_next=0.0;
        }
      break;
      case 4: //Trigger_Out          
        if (data->player[data->activplayer].momset->triggerfunction_out==NULL || data->turn_script_pos<0)
        {
          data->turn_pos=5; //Move
          data->turn_dice_value[0]--;
          data->turn_dice_next=0.999;
          setplayernext(data);
        }
        else
        {
          if (data->turn_script_pos==0)
          { //Script erstmal vorbereiten
            data->turn_script_pos=data->player[data->activplayer].momset->triggerjump_out;
            data->turn_script_stack=(float*)malloc(sizeof(float)*script->block[data->turn_script_pos].jump);
          }
          //Scriptausführung
          if (delaycounter<=0)
          {
            float wert=callfunction(script->block,&(data->turn_script_pos),&(data->turn_script_stack));
            if (data->turn_script_pos==-1)
              free(data->turn_script_stack);
            else
            {
              data->turn_script_pos++;
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
      case 5: //Move
        data->turn_dice_next-=1.0/1000.0;
        if (data->turn_dice_next<=0.0)
        {
          data->turn_dice_next=0.0;
          data->turn_script_pos=0;
          data->turn_pos=6; //Trigger_In
					//Jump Geräusch
					Mix_PlayChannel(-1,snd_jump,0);
					//Partikeleffekte:
					if (strcmp(data->player[data->activplayer].momset->type,(char*)"blue")==0)
						ZWnewcolorcircle(data->player[data->activplayer].momposition,32,0.002,maintex,16,200,0.15,0.5,0.7,1,0.75,0.000002,0.3);
					else  
					if (strcmp(data->player[data->activplayer].momset->type,(char*)"red")==0)
						ZWnewcolorcircle(data->player[data->activplayer].momposition,32,0.002,maintex,16,200,0.15,1,0.5,0.5,0.75,0.000002,0.3);
					else
						ZWnewcolorcircle(data->player[data->activplayer].momposition,32,0.002,maintex,16,200,0.15,1.0,1.0,0.7,0.75,0.000002,0.3);
        }
        else
          break;
      case 6: //Trigger_In
        if (data->player[data->activplayer].momset->triggerfunction_in==NULL || data->turn_script_pos<0)
        {
          data->turn_pos=7; //Itemscript
          data->turn_script_pos=0;
        }
        else
        {
          if (data->turn_script_pos==0)
          { //Script erstmal vorbereiten
            data->turn_script_pos=data->player[data->activplayer].momset->triggerjump_in;
            data->turn_script_stack=(float*)malloc(sizeof(float)*script->block[data->turn_script_pos].jump);
          }
          //Scriptausführung
          if (delaycounter<=0)
          {
            float wert=callfunction(script->block,&(data->turn_script_pos),&(data->turn_script_stack));
            if (data->turn_script_pos==-1)
              free(data->turn_script_stack);
            else
            {
              data->turn_script_pos++;
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
      case 7: //Item Script
        if (data->player[data->activplayer].momset->triggertrap<0 || data->turn_script_pos<0)
          data->turn_pos=8; //Itemtrigger
        else
        {
          if (data->turn_script_pos==0)
          { //Script erstmal vorbereiten
            data->turn_script_pos=data->player[data->activplayer].momset->triggertrap;
            data->turn_script_stack=(float*)malloc(sizeof(float)*script->block[data->turn_script_pos].jump);
            data->turn_script_stack[0]=data->player[data->activplayer].momset->trapowner;
          }
          //Scriptausführung
          if (delaycounter<=0)
          {
            float wert=callfunction(script->block,&(data->turn_script_pos),&(data->turn_script_stack));
            if (data->turn_script_pos==-1)
              free(data->turn_script_stack);
            else
            {
              data->turn_script_pos++;
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
      case 8: //Item_Trigger
				if (data->turn_dice_num>1 && data->turn_dice_value[0]==0)
				{
					data->turn_dice_num--;
					data->turn_dice_value[0]=data->turn_dice_value[data->turn_dice_num];
				}
				else
        if (get_turn_dice_value_sum(data)<=0) //Ende des Zuges
        {
					data->turn_dice_num=1;
					data->turn_dice_dest=1;
					data->dice_speed=DICE_SPEED;
          //etwaiges Geld oder Antigeld durch blaue und rote Felder
          //blaues Feld
          if (strcmp(data->player[data->activplayer].momset->type,(char*)"blue")==0)
          {
            data->player[data->activplayer].money+=2;
            Mix_PlayChannel(-1,snd_positive,0);
            new_message((char*)"+2 Münzen");
          }
          else
          //rotes Feld
          if (strcmp(data->player[data->activplayer].momset->type,(char*)"red")==0)
          {
            data->player[data->activplayer].money-=2;
            if (data->player[data->activplayer].money<0)
              data->player[data->activplayer].money=0;
            Mix_PlayChannel(-1,snd_negative,0);
            new_message((char*)"-2 Münzen");
          }
					{
            data->mainloop_pos=7; //Minispielauswahl
						if (data->activplayer+1>=playernum)
						{
						  //do_choosecircle(false,(char*)"Simple Snake",(char*)"Spiral Spaß",(char*)"Kugel Krieg",NULL,NULL,NULL,NULL,NULL);
							set_choosecircle_gamelist(playernum);
							do_minigame_choosecircle();
						}
				  }
          break;
        }
        else
        {
          data->turn_script_pos=0;
          data->turn_pos=4; //Trigger_Out
          break;
        }
      break;
    }
  }
}
