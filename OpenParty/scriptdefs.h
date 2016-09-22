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
typedef struct stermelem *ptermelem;
typedef struct stermelem {
  char art; // entspricht dem Rechenzeichen. 'f' ist eine Float, '{' eine Funktion, Rechenzeichen, 'b' Funktionsbeginn,
            // 'w' while, 'x' elihw (until), 'd' do, 'o' od, 'i' if, 'j' fi, 'l' else, 'v' Variable, 's' set,
            // '#' Aufrufende, '^' FULL STOP
  float wert;
  char* function;
  ptermelem before,next,jump;
  int nr;
} ttermelem;

union blockparameter {
  float wert_f;
  int wert_i;
  void* wert_p; //pointer, nicht für das Ausführen nötig
};

typedef struct sblockelem *pblockelem;
typedef struct sblockelem {
  char art; // entspricht dem Rechenzeichen. 'f' ist eine Float, '{' eine Funktion, Rechenzeichen, 'b' Funktionsbeginn,
            // 'w' while, 'x' elihw (until), 'd' do, 'o' od, 'i' if, 'j' fi, 'l' else, 'v' Variable,'s' set,
            // '#' Aufrufende, 'g' globale Variable
  blockparameter param; //bei art=='b' größe des benötigten Stacks (inklusive Parameter)
  long int jump; //Jumpziel oder bei art=='b' Anzahl der Parameter
} tblockelem;

typedef struct sfunctab *pfunctab;
typedef struct sfunctab {
  char* name;
  int jump;
  pfunctab next;
} tfunctab;

typedef struct scompiledblock *pcompiledblock;
typedef struct scompiledblock {
  pblockelem block;
  pfunctab functab;
} tcompiledblock;

typedef struct sknownvar *pknownvar;
typedef struct sknownvar {
  char* name;
  pknownvar next;
  int nr;
} tknownvar;

typedef struct sjumpstack *pjumpstack;
typedef struct sjumpstack {
  int nr;
  pjumpstack next;
} tjumpstack;

//globale Variablen fürs Script
  pcompiledblock script=NULL;
  float globalstack[256];
  char no_text[22]="error: text not found";

float callexfunction(int nr,float* stack)
{
  float temp;
  char buffer[256];
	int a;
  switch (nr)
  {
    case -2: return acos(stack[0]);
    case -3: return M_PI/2.0-atan(stack[0]);
    case -4: return asin(stack[0]);
    case -5: return atan(stack[0]);
    case -6: return cos(stack[0]);
    case -7: return 1.0/tan(stack[0]);
    case -8: return sin(stack[0]);
    case -9: return tan(stack[0]);
    case -10: return M_PI;
    case -11: return pow(stack[0],stack[1]);
    case -12: return trunc(stack[0]);
    case -13: return round(stack[0]);
    case -15: return M_E;
    case -16: return globalstack[(int)(trunc(stack[0]))]; 
    case -17: globalstack[(int)(trunc(stack[0]))]=stack[1]; return 0;
    case -18: return (rand()%((int)(trunc(stack[0]))));
    case -256: printf("Hello World\n");return 0;    
    case -257: printf("Number: %.20f\n",stack[0]);return 0;
    case -1024: //op_player_position_get
      return maindata.player[(int)(trunc(stack[0]))].momset->nr;
    case -1025: //op_player_activ
      return maindata.activplayer;
    case -1026: //op_player_lastposition_get
      return maindata.player[(int)(trunc(stack[0]))].lastset->nr;
    case -1027: //op_set_next_set
      return setbynr[(int)(trunc(stack[0]))]->gamenextnr=(int)(trunc(stack[1]));
    case -1028: //op_set_cameraposition_from_set
      setbynr[(int)(trunc(stack[0]))]->cameraposition=setbynr[(int)(trunc(stack[1]))]->cameraposition;
      return 0;
    case -1029: //op_set_camerarotation_from_set
      setbynr[(int)(trunc(stack[0]))]->camerarotation=setbynr[(int)(trunc(stack[1]))]->camerarotation;
      return 0;
    case -1030: //op_set_position_y_set
      setbynr[(int)(trunc(stack[0]))]->position.y=stack[1];
      return 0;
    case -1031: //op_set_position_y_get
      return setbynr[(int)(trunc(stack[0]))]->position.y;
    case -1032: //op_set_position_from_set
      setbynr[(int)(trunc(stack[0]))]->position=setbynr[(int)(trunc(stack[1]))]->position;
      return 0;
    case -1033: //op_player_rubin_set
      if (stack[1]<0.0)
        stack[1]=0.0;
      temp=(int)(trunc(stack[1]))-maindata.player[(int)(trunc(stack[0]))].rubins;
      if (temp>0)
      {
        if (strcmp(language,(char*)"german")==0)
          sprintf(buffer,"+%i Rubine",(int)(temp));
        else
          sprintf(buffer,"+%i Rubins",(int)(temp));
        Mix_PlayChannel(-1,snd_positive,0);
      }
      else
      {
        if (strcmp(language,(char*)"german")==0)
          sprintf(buffer,"-%i Rubine",(int)(-temp));
        else
          sprintf(buffer,"-%i Rubins",(int)(-temp));
        Mix_PlayChannel(-1,snd_negative,0);
      }
      maindata.player[(int)(trunc(stack[0]))].rubins=(int)(trunc(stack[1]));
      new_message(buffer);
      return 0;
    case -1034: //op_player_rubin_get
      return maindata.player[(int)(trunc(stack[0]))].rubins;
    case -1035: //op_player_money_set
      if (stack[1]<0.0)
        stack[1]=0.0;
      temp=(int)(trunc(stack[1]))-maindata.player[(int)(trunc(stack[0]))].money;
      if (temp>=0)
      {
        if (strcmp(language,(char*)"german")==0)
          sprintf(buffer,"+%i Münzen",(int)(temp));
        else
          sprintf(buffer,"+%i Coins",(int)(temp));
        Mix_PlayChannel(-1,snd_positive,0);
      }
      else
      {
        if (strcmp(language,(char*)"german")==0)
          sprintf(buffer,"-%i Münzen",(int)(-temp));
        else
          sprintf(buffer,"-%i Coins",(int)(-temp));
        
        Mix_PlayChannel(-1,snd_negative,0);
      }
      maindata.player[(int)(trunc(stack[0]))].money=(int)(trunc(stack[1]));
      new_message(buffer);
      return 0;
    case -1036: //op_player_money_get
      return maindata.player[(int)(trunc(stack[0]))].money;
    case -1037: //op_text_content_set
      createtextcontent(gettextbynrandlanguage((int)(trunc(stack[1])),language));
      return 0;
    case -1038: //op_controller_button_get
      if (maindata.player[(int)(trunc(stack[0]))].iscpu)
        return 1;
      else
			if ((int)(trunc(stack[1]))<4)
        return *(ZWattribute->joystick[(int)(trunc(stack[0]))].button[(int)(trunc(stack[1]))]);
			else
			  for (a=0;a<4;a++)
				  if (*(ZWattribute->joystick[(int)(trunc(stack[0]))].button[a]))
					  return 1;
				return 0;
    case -1039: //op_controller_button_set
      if (maindata.player[(int)(trunc(stack[0]))].iscpu==0)
			{
				if ((int)(trunc(stack[1]))<4)
					*(ZWattribute->joystick[(int)(trunc(stack[0]))].button[(int)(trunc(stack[1]))])=(int)(trunc(stack[2]));
				else
			  for (a=0;a<4;a++)
				  *(ZWattribute->joystick[(int)(trunc(stack[0]))].button[a])=(int)(trunc(stack[2]));
			}
      return 0;
    case -1040: //op_text_content_delete
      deletetextcontent();
      return 0;
    case -1041: //op_player_iscpu
      return maindata.player[(int)(trunc(stack[0]))].iscpu;
		case -1042: //op_choosecircle_show
			do_choosecircle_one_string(false,gettextbynrandlanguage((int)(trunc(stack[0])),language));
		  return 0;
		case -1043: //op_choosecircle_get_direction
		  return choosecircle_direction;
		case -1044: //op_choosecircle_get_choice
		  return choosecircle_get_choice();
		case -1045: //op_choosecircle_kill
			kill_choosecircle();
			return 0;
		case -1046: //op_dice_get_value
		  return get_turn_dice_value_sum(&maindata);
		case -1047: //op_choosecircle_show_face
			do_choosecircle_one_string(true,gettextbynrandlanguage((int)(trunc(stack[0])),language));
		  return 0;
		case -1048: //op_choosecircle_show_face_without
		  switch ((int)(trunc(stack[0])))
			{
			  case 0: do_choosecircle(true,NULL,(char*)" 1 ",(char*)" 2 ",(char*)" 3 ",(char*)" 4 ",(char*)" 5 ",(char*)" 6 ",(char*)" 7 "); break;
			  case 1: do_choosecircle(true,(char*)" 0 ",NULL,(char*)" 2 ",(char*)" 3 ",(char*)" 4 ",(char*)" 5 ",(char*)" 6 ",(char*)" 7 "); break;
			  case 2: do_choosecircle(true,(char*)" 0 ",(char*)" 1 ",NULL,(char*)" 3 ",(char*)" 4 ",(char*)" 5 ",(char*)" 6 ",(char*)" 7 "); break;
			  case 3: do_choosecircle(true,(char*)" 0 ",(char*)" 1 ",(char*)" 2 ",NULL,(char*)" 4 ",(char*)" 5 ",(char*)" 6 ",(char*)" 7 "); break;
			  case 4: do_choosecircle(true,(char*)" 0 ",(char*)" 1 ",(char*)" 2 ",(char*)" 3 ",NULL,(char*)" 5 ",(char*)" 6 ",(char*)" 7 "); break;
			  case 5: do_choosecircle(true,(char*)" 0 ",(char*)" 1 ",(char*)" 2 ",(char*)" 3 ",(char*)" 4 ",NULL,(char*)" 6 ",(char*)" 7 "); break;
			  case 6: do_choosecircle(true,(char*)" 0 ",(char*)" 1 ",(char*)" 2 ",(char*)" 3 ",(char*)" 4 ",(char*)" 5 ",NULL,(char*)" 7 "); break;
			  case 7: do_choosecircle(true,(char*)" 0 ",(char*)" 1 ",(char*)" 2 ",(char*)" 3 ",(char*)" 4 ",(char*)" 5 ",(char*)" 6 ",NULL); break;
			  default: do_choosecircle(true,(char*)" 0 ",(char*)" 1 ",(char*)" 2 ",(char*)" 3 ",(char*)" 4 ",(char*)" 5 ",(char*)" 6 ",(char*)" 7 ");
			}	
		  return 0;
		case -1049: //op_fade_get_counter	
			return maindata.fade_counter;
		case -1050: //op_fade_set_counter	
		  maindata.fade_counter=stack[0];
			return 0;
    case -1051: //op_player_position_set
		  if (maindata.player[(int)(trunc(stack[0]))].momset==setbynr[(int)(trunc(stack[1]))])
			  return 0;
      maindata.player[(int)(trunc(stack[0]))].momset=setbynr[(int)(trunc(stack[1]))];
      sync_all_position(0.005,&maindata);
			if ((int)(trunc(stack[0]))==maindata.activplayer)
			{
				maindata.destcameraposition=maindata.player[(int)(trunc(stack[0]))].momset->cameraposition;
				maindata.destcamerarotation=maindata.player[(int)(trunc(stack[0]))].momset->camerarotation;
			}
			if (get_turn_dice_value_sum(&maindata)>0 && (int)(trunc(stack[0]))==maindata.activplayer)
				maindata.player[(int)(trunc(stack[0]))].destface=calcnewdestface((int)(trunc(stack[0])),&maindata);
			else
				maindata.player[(int)(trunc(stack[0]))].destface=0;
      maindata.player[(int)(trunc(stack[0]))].time=0.005;
      maindata.time=0.005;			
			return 0;
		case -1052: //op_choosecircle_show_face_without_team_of	
			do_choosecircle_without_team_of((int)(trunc(stack[0])));
		  return 0;
		case -1053: //op_minigame_duell
			temp=minigame_step(true,(int)(trunc(stack[0])),(int)(trunc(stack[1])),false);
			if (temp==-1)
			  return -1;
			if (temp==0)
			  return stack[0];
			return stack[1];
		case -1054: //op_choosecircle_show_minigames
			set_choosecircle_gamelist((int)(trunc(stack[0])));
			do_minigame_choosecircle();
		  return 0;
		case -1055: //op_choosecircle_calc_minigames_result
		  chosen_minigame=get_game_by_choosecircle_choice();
			return 0;
		case -1056: //op_player_number
		  return playernum;
		case -1057: //op_dice_set_number
			maindata.turn_dice_dest=(int)(trunc(stack[0]));
			return 0;
		case -1058: //op_dice_get_number
			return maindata.turn_dice_dest;
		case -1059: //op_player_add_item
			//Nächstes freies Item suchen
			for (a=0;a<3;a++)
			  if (maindata.player[(int)(trunc(stack[0]))].item[a]==NULL)
				  break;
			if (a!=3)
			{
				//Item mit der Nummer suchen
				pitem momitem=firstitem;
				while (momitem!=NULL)
				{
					if (momitem->number==(int)(trunc(stack[1])))
					  break;
					momitem=momitem->next;
				}
				maindata.player[(int)(trunc(stack[0]))].item[a]=momitem;
			}
		  return 0;
    case -1060: //op_camera_from_set
			maindata.destcameraposition=setbynr[(int)(trunc(stack[0]))]->cameraposition;
			maindata.destcamerarotation=setbynr[(int)(trunc(stack[0]))]->camerarotation;
			maindata.camera_looking_at=8;
			maindata.manuell_camera_point=setbynr[(int)(trunc(stack[0]))]->position;
      maindata.time=0.005;			
			return 0;
		case -1061: //op_camera_set_mode
			maindata.camera_looking_at=(int)(trunc(stack[0]));
		  return 0;
		case -1062: //op_item_shop
			new_item_choose(&maindata,(int)(trunc(stack[0])));
		  return 0;
		case -1063: //op_item_num
			a=0;
			{
				pitem momitem=firstitem;
				while (momitem!=NULL)
				{
					a++;
					momitem=momitem->next;
				}
			}
			return a;
		case -1064: //op_dice_mult_speed
			maindata.dice_speed/=stack[0];
		  return 0;
		case -1065: //op_player_get_team
		  return maindata.player[(int)(trunc(stack[0]))].team;
    default: return 0;
  }
}

int getexfunctionnumber(char *name)
{
  if (strcmp(name,(char*)"acos")==0) return -2;
  if (strcmp(name,(char*)"acot")==0) return -3;
  if (strcmp(name,(char*)"asin")==0) return -4;
  if (strcmp(name,(char*)"atan")==0) return -5;
  if (strcmp(name,(char*)"cos")==0) return -6;
  if (strcmp(name,(char*)"cot")==0) return -7;
  if (strcmp(name,(char*)"sin")==0) return -8;
  if (strcmp(name,(char*)"tan")==0) return -9;
  if (strcmp(name,(char*)"pi")==0) return -10;
  if (strcmp(name,(char*)"power")==0) return -11;
  if (strcmp(name,(char*)"trunc")==0) return -12;
  if (strcmp(name,(char*)"round")==0) return -13;
  if (strcmp(name,(char*)"stack")==0) return -14;
  if (strcmp(name,(char*)"e")==0) return -15;
  if (strcmp(name,(char*)"getglobal")==0) return -16;
  if (strcmp(name,(char*)"setglobal")==0) return -17;
  if (strcmp(name,(char*)"random")==0) return -18;
  if (strcmp(name,(char*)"say_hello")==0) return -256;
  if (strcmp(name,(char*)"say_number")==0) return -257;
  if (strcmp(name,(char*)"op_player_position_get")==0) return -1024;
  if (strcmp(name,(char*)"op_player_activ")==0) return -1025;
  if (strcmp(name,(char*)"op_player_lastposition_get")==0) return -1026;
  if (strcmp(name,(char*)"op_set_next_set")==0) return -1027;
  if (strcmp(name,(char*)"op_set_cameraposition_from_set")==0) return -1028;
  if (strcmp(name,(char*)"op_set_camerarotation_from_set")==0) return -1029;
  if (strcmp(name,(char*)"op_set_position_y_set")==0) return -1030;
  if (strcmp(name,(char*)"op_set_position_y_get")==0) return -1031;
  if (strcmp(name,(char*)"op_set_position_from_set")==0) return -1032;
  if (strcmp(name,(char*)"op_player_rubin_set")==0) return -1033;
  if (strcmp(name,(char*)"op_player_rubin_get")==0) return -1034;
  if (strcmp(name,(char*)"op_player_money_set")==0) return -1035;
  if (strcmp(name,(char*)"op_player_money_get")==0) return -1036;
  if (strcmp(name,(char*)"op_text_content_set")==0) return -1037;
  if (strcmp(name,(char*)"op_controller_button_get")==0) return -1038;
  if (strcmp(name,(char*)"op_controller_button_set")==0) return -1039;
  if (strcmp(name,(char*)"op_text_content_delete")==0) return -1040;
  if (strcmp(name,(char*)"op_player_iscpu")==0) return -1041;
	if (strcmp(name,(char*)"op_choosecircle_show")==0) return -1042;
	if (strcmp(name,(char*)"op_choosecircle_get_direction")==0) return -1043;
	if (strcmp(name,(char*)"op_choosecircle_get_choice")==0) return -1044;
	if (strcmp(name,(char*)"op_choosecircle_kill")==0) return -1045;
	if (strcmp(name,(char*)"op_dice_get_value")==0) return -1046;
	if (strcmp(name,(char*)"op_choosecircle_show_face")==0) return -1047;
	if (strcmp(name,(char*)"op_choosecircle_show_face_without")==0) return -1048;
	if (strcmp(name,(char*)"op_fade_get_counter")==0) return -1049;
	if (strcmp(name,(char*)"op_fade_set_counter")==0) return -1050;
	if (strcmp(name,(char*)"op_player_position_set")==0) return -1051;
	if (strcmp(name,(char*)"op_choosecircle_show_face_without_team_of")==0) return -1052;
	if (strcmp(name,(char*)"op_minigame_duell")==0) return -1053;
	if (strcmp(name,(char*)"op_choosecircle_show_minigames")==0) return -1054;
	if (strcmp(name,(char*)"op_choosecircle_calc_minigames_result")==0) return -1055;
	if (strcmp(name,(char*)"op_player_number")==0) return -1056;
	if (strcmp(name,(char*)"op_dice_set_number")==0) return -1057;
	if (strcmp(name,(char*)"op_dice_get_number")==0) return -1058;
	if (strcmp(name,(char*)"op_player_add_item")==0) return -1059;
	if (strcmp(name,(char*)"op_camera_from_set")==0) return -1060;
	if (strcmp(name,(char*)"op_camera_set_mode")==0) return -1061;
	if (strcmp(name,(char*)"op_item_shop")==0) return -1062;
	if (strcmp(name,(char*)"op_item_num")==0) return -1063;
	if (strcmp(name,(char*)"op_dice_mult_speed")==0) return -1064;
	if (strcmp(name,(char*)"op_player_get_team")==0) return -1065;
  return -1; 
}

int getexfunctionparamanz(int nr)
{
  switch (nr)
  {
    case -2: return 1;
    case -3: return 1;
    case -4: return 1;
    case -5: return 1;
    case -6: return 1;
    case -7: return 1;
    case -8: return 1;
    case -9: return 1;
    case -10: return 0;
    case -11: return 2;
    case -12: return 1;
    case -13: return 1;
    case -14: return 1;
    case -15: return 0;
    case -16: return 1;
    case -17: return 2;
    case -18: return 1;
    case -256: return 1;
    case -257: return 1;
    case -1024: return 1;
    case -1025: return 1;
    case -1026: return 1;
    case -1027: return 2;
    case -1028: return 2;
    case -1029: return 2;
    case -1030: return 2;
    case -1031: return 1;
    case -1032: return 2;
    case -1033: return 2;
    case -1034: return 1;
    case -1035: return 2;
    case -1036: return 1;
    case -1037: return 2;
    case -1038: return 2;
    case -1039: return 3;
    case -1040: return 0;
    case -1041: return 1;
    case -1042: return 1;
    case -1043: return 0;
    case -1044: return 0;
    case -1045: return 0;
    case -1046: return 0;
    case -1047: return 1;
    case -1048: return 1;
    case -1049: return 0;
    case -1050: return 1;
    case -1051: return 2;
    case -1052: return 1;
		case -1053: return 2;
		case -1054: return 1;
		case -1055: return 0;
		case -1056: return 0;
		case -1057: return 1;
		case -1058: return 0;
		case -1059: return 2;
		case -1060: return 1;
		case -1061: return 1;
		case -1062: return 1;
		case -1063: return 0;
		case -1064: return 1;
		case -1065: return 1;
  }
  return 1;
}
