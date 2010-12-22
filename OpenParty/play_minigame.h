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

#define NEEDED_REVISION 3

int minigame_step(bool duell,int player1,int player2,bool aftershowparty)
{
	if (duell && player1==player2)
	  return -1;
	//alle Lichter speichern und deaktivieren:
	alllightsoff();
	int oldplayernum=playernum;
	tplayer oldplayer1=maindata.player[0];
	tplayer oldplayer2=maindata.player[1];
	tZWjoystick oldjoystick1=ZWattribute->joystick[0];
	tZWjoystick oldjoystick2=ZWattribute->joystick[1];
	if (duell)
	{
	  playernum=2;
		maindata.player[0]=maindata.player[player1];
		if (player2==0)
		  maindata.player[1]=oldplayer1;
		else
		  maindata.player[1]=maindata.player[player2];
			
		//Joystickmapping verwurschteln
    ZWattribute->joystick[0]=ZWattribute->joystick[player1];
		if (player2==0)
		  ZWattribute->joystick[1]=oldjoystick1;
		else
		  ZWattribute->joystick[1]=ZWattribute->joystick[player2];
	}

	int result=0;
	tgameresult gameresult;
	while (result!=1 && (shouldend==0))
	{
		for (int player=0;player<playernum;player++)
		{
//			if (!(maindata.player[player].iscpu))
	//			continue;
			for (int a=0;a<4;a++)
				*(ZWattribute->joystick[player].button[a])=0;
			for (int a=0;a<2;a++)
				*(ZWattribute->joystick[player].axis[a])=0;
		}					
		//Minispielvorauswahl
			result=prepare_minigame(chosen_minigame->filename,&maindata);
		//Minispiel starten  
			play_minigame(chosen_minigame->filename,0,&gameresult,result-1);
	}
	
	int winner=-1;
	int a;
	for (a=0;a<playernum;a++)
	  if (gameresult.winner[a])
		{
			if (winner==-1)
			  winner=a;
			else
			{
				winner=-1;
				break;
			}
		}
	
	//Minispielergebnis
	if (aftershowparty)
	  after_minigame(&maindata,&gameresult);
  Mix_FadeInMusic(backgroundmusic,-1,FADE_IN_TIME);
	if (duell)
	{
		playernum=oldplayernum;
		maindata.player[0]=oldplayer1;
		maindata.player[1]=oldplayer2;
		ZWattribute->joystick[0]=oldjoystick1;
		ZWattribute->joystick[1]=oldjoystick2;
  }
	for (int player=0;player<playernum;player++)
	{
//			if (!(maindata.player[player].iscpu))
//			continue;
		for (int a=0;a<4;a++)
			*(ZWattribute->joystick[player].button[a])=0;
		for (int a=0;a<2;a++)
			*(ZWattribute->joystick[player].axis[a])=0;
	}					
	//Lichter wieder aktivieren
	alllightson();
	return winner;
}

void add_game_to_list(char* name,char* filename,char ingame,char installed) //Fügt ein Spiel hinzu, dass gespielt werden kann
{
  pminigame newgame=(pminigame)malloc(sizeof(tminigame));
	newgame->name=(char*)malloc(strlen(name)*sizeof(char)+1);
	sprintf(newgame->name,"%s",name);
	newgame->filename=(char*)malloc(strlen(filename)*sizeof(char)+1);
	sprintf(newgame->filename,"%s",filename);
  newgame->ingame=ingame;
  newgame->installed=installed;
	newgame->next=firstminigame;
	firstminigame=newgame;
	minigamenum++;
}

void clean_gamelist(void)
{
	while (firstminigame!=NULL)
	{
		free(firstminigame->name);
		free(firstminigame->filename);
		pminigame temp=firstminigame;
		firstminigame=firstminigame->next;
		free(temp);
	}
	minigamenum=0;
}

void set_choosecircle_gamelist(int playernum) //wird später die Spiele wählen, die in die engere Auswahl kommen - gewichtet
{
	int games=rand()%3+3;
	//Spiele zählen und momplayed auf den Minimalen Wert setzen
	int momplayed=-1;
	int gamenum=0;
	pminigame minigame=firstminigame;
	while (minigame!=NULL)
	{
		gamenum++;
		if (momplayed==-1 || minigame->played<momplayed)
		  momplayed=minigame->played;
		minigame=minigame->next;
	}
	if (games>gamenum)
	  games=gamenum;
	
	//Die mit der geringsten Spielanzahl einfügen
	choosecircle_gamelistnum=0;
	minigame=firstminigame;
  pminigame prechoice=NULL;
  pminigame *prechoicepointer=NULL;
  int prechoicenum=0;
	while (choosecircle_gamelistnum<games)
	{
		if (momplayed==minigame->played && minigame->ingame && minigame->installed)
		{
      if (prechoice==NULL)
        prechoice=minigame;
      else
        (*prechoicepointer)=minigame;
      minigame->prechoicenext=NULL;
      prechoicepointer=&(minigame->prechoicenext);
      prechoicenum++;
		}
		minigame=minigame->next;
		if (minigame==NULL)
		{
      if (prechoice!=NULL)
      {
        //Durchzirkulieren und zufällig Minispiele rausnehmen. Wenn am Ende keines mehr da ist oder zu viele, terminieren 
        pminigame momgame=prechoice;
        pminigame beforepointer=prechoice;
        while (beforepointer->prechoicenext!=NULL)
          beforepointer=beforepointer->prechoicenext;
        (*prechoicepointer)=prechoice; //Zirkel!
        while (prechoicenum>0 && choosecircle_gamelistnum<games)
        {
          if (rand()%23==0)
          {
            choosecircle_gamelist[choosecircle_gamelistnum]=momgame;
            choosecircle_gamelistnum++;
            beforepointer->prechoicenext=momgame->prechoicenext;
            prechoicenum--;
          }
          beforepointer=momgame;
          momgame=momgame->prechoicenext;
        }
        prechoice=NULL;
        prechoicepointer=NULL;
      }
			minigame=firstminigame;
			momplayed++;
		}
	}
	int a;
	for (a=games;a<5;a++)
	  choosecircle_gamelist[a]=&dummyminigame;
}

pminigame get_game_by_choosecircle_choice(void) //gibt mithilfe von choosecircle_get_choice das gewählte Spiel zurück
{
	choosecircle_gamelist[choosecircle_get_choice()]->played++;
	return choosecircle_gamelist[choosecircle_get_choice()];
}

void reset_played(void)
{
	pminigame momgame=firstminigame;
	while (momgame!=NULL)
	{
		momgame->played=0;
		momgame=momgame->next;
	}
}

void new_minigame_std_db(void)
{
  add_game_to_list((char*)"Simple Snake",(char*)"simple_snake",1,1);
  add_game_to_list((char*)"Spiral Spaß",(char*)"spiral_spass",1,1);
  add_game_to_list((char*)"Kugel Krieg",(char*)"kugel_krieg",1,1);
  add_game_to_list((char*)"Wettschwimmen",(char*)"wettschwimmen",1,1);
  add_game_to_list((char*)"Reflexrübe",(char*)"reflexruebe",1,1);
  add_game_to_list((char*)"Mini Game",(char*)"mini_game",1,1);
  add_game_to_list((char*)"Roller",(char*)"roller",1,1);
  add_game_to_list((char*)"Beat Paddle",(char*)"beat_paddle",1,1);
  add_game_to_list((char*)"Ikosaeder Memory",(char*)"ikosaeder_memory",1,1);
  add_game_to_list((char*)"Ikosaeder Implosion",(char*)"ikosaeder_implosion",1,1);
  add_game_to_list((char*)"King Pong",(char*)"king_pong",1,1);
}

void load_minigames(void)
{
	clean_gamelist();
	char buffer[256];
	#ifndef WIN32
		const char *homenv = getenv("HOME");
		sprintf(buffer,"%s/.config/openparty/minigames.xml",homenv);
	#else
		sprintf(buffer,"./minigames.xml");	
	#endif
  if (not(ZWfileexists(buffer)))
	{
    new_minigame_std_db();
    return;
	}
	clean_gamelist();
  pxmlpoint xmlpoint=ZWloadxmlfile(buffer);
  pxmltag maintag=ZWgetnexttag(xmlpoint);
  pxmltag first=ZWgetnexttag(maintag->inhalt);
	char dummy[1]="";
  int version=0;
  while (first!=NULL)
  {
    if (strcmp(first->name,"version")==0)
    {
      pxmlattribute attribute=first->attribute;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"revision")==0)
				  version=atoi(attribute->wert);
        attribute=attribute->next;
      }
    }
    if (version<NEEDED_REVISION)
    {
      //Zu alt: Wir setzen die DB von Hand und überspringen den ganzen Rest
      new_minigame_std_db();
      while (first!=NULL)
      {
        ZWfreetag(first);
        first=ZWgetnexttag(maintag->inhalt);
      }
      break;
    }
    if (strcmp(first->name,"minigame")==0)
    {
      pxmlattribute attribute=first->attribute;
			char* name=dummy;
			char* filename=dummy;
      char ingame=1;
      char installed=1;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"name")==0)
				  name=attribute->wert;
        if (strcmp(attribute->name,"filename")==0)
				  filename=attribute->wert;
        if (strcmp(attribute->name,"ingame")==0)
        {
          if (strcmp(attribute->wert,"True")==0)
				    ingame=1;
          else
            ingame=0;
        }
        if (strcmp(attribute->name,"installed")==0)
        {
          if (strcmp(attribute->wert,"True")==0)
				    installed=1;
          else
            installed=0;
        }
        attribute=attribute->next;
      }
			add_game_to_list(name,filename,ingame,installed);
    }
    ZWfreetag(first);
    first=ZWgetnexttag(maintag->inhalt);
  }
  ZWfreetag(maintag);
  ZWfinishloading(xmlpoint);
}

void save_minigames(void)
{
	char buffer[256];
	#ifndef WIN32
		const char *homenv = getenv("HOME");
		sprintf(buffer,"%s/.config",homenv);
		mkdir(buffer,DIR_MODUS);
		sprintf(buffer,"%s/.config/openparty",homenv);
		mkdir(buffer,DIR_MODUS);
		sprintf(buffer,"%s/.config/openparty/minigames.xml",homenv);
	#else
		sprintf(buffer,"./minigames.xml");	
	#endif
  pxmlfile xmlfile=ZWnewxmlfile(buffer,(char*)"minigames");

  ZWbegintag(xmlfile,(char*)"version");
    ZWaddattribute_number(xmlfile,(char*)"revision",NEEDED_REVISION);
  ZWendstandalonetag(xmlfile);
	
	pminigame momgame=firstminigame;
	while (momgame!=NULL)
	{
		ZWbegintag(xmlfile,(char*)"minigame");
			ZWaddattribute_string(xmlfile,(char*)"filename",momgame->filename);
			ZWaddattribute_string(xmlfile,(char*)"name",momgame->name);
      if (momgame->ingame==1)
			  ZWaddattribute_string(xmlfile,(char*)"ingame",(char*)"True");
      else
        ZWaddattribute_string(xmlfile,(char*)"ingame",(char*)"False");
		ZWendstandalonetag(xmlfile);
		momgame=momgame->next;
	}
	
  ZWfinishxmlfile(xmlfile);
}


void do_minigame_choosecircle(void)
{
	do_choosecircle(false,choosecircle_gamelist[0]->name,choosecircle_gamelist[1]->name,choosecircle_gamelist[2]->name,choosecircle_gamelist[3]->name,choosecircle_gamelist[4]->name,NULL,NULL,NULL);
}
