(*
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
*)
library skeleton_pascal;
{$MODE OBJFPC}
{$H+}
{$IFDEF WINDOWS}
  {$CALLING cdecl}
{$ENDIF}
{$IFDEF UNIX}
  {$CALLING cdecl}
{$ENDIF}

uses sdl,sdl_mixer,dglOpenGL,ttf,minigames,sysutils,math;

type
  PData = ^ TData;
	TData = record
		step : integer;
		fade : single;
		countdown : integer;
		text : ZWtext;
		gameinfo : pgameinfo;
		backgroundmusic : PMix_Music;
		old_music_volume : integer;
	end;

procedure op_get_gameinfos(gameinfo : pminigameinfo);
var a,b : integer;
begin
  for a:=0 to 7 do
	  for b:=0 to 3 do
      gameinfo^.playable[a,b]:=1;
end;

function op_get_gamedata_size:longint;
begin
  exit(sizeof(tdata));
end;

procedure op_init_game(data : pdata;gameinfo : pgameinfo);
var a : integer;
    keks : string;
begin	
	InitOpenGL;
	ReadExtensions;

  //Text initialisieren
  data^.text.texture:=gameinfo^.texttexture;
  for a:=0 to 255 do
    data^.text.breite[a]:=gameinfo^.textwidth[a];
    
  //Gameinforeferenzpointer speicher:
  data^.gameinfo:=gameinfo;
  
  //Ton vorbereiten
	keks:=String(gameinfo^.datafolder)+'sounds/Vegas Glitz.ogg';
  data^.backgroundmusic := Mix_LoadMUS(@keks[1]);
  data^.old_music_volume:=Mix_VolumeMusic(longint(96.0*gameinfo^.volume));
  Mix_FadeInMusic(data^.backgroundmusic,-1,500);

  //Spiel ansich vorbereiten
  data^.step:=0;
  data^.fade:=1;
  data^.countdown:=3000;  
end;

function op_calc_game(data:pdata):longint;
var a,b,steps : integer;
begin
  if data^.step=0 then //Fade In
  begin
    data^.fade:=data^.fade-longint(data^.gameinfo^.steps^/500.0);
    if data^.fade<=0.0 then
    begin
      data^.fade:=0.0;
      data^.step:=1;
    end;
    exit(0);
  end;
  if data^.step=2 then //Fade Out
  begin
    data^.fade:=data^.fade+longint(data^.gameinfo^.steps^/500.0);
    if data^.fade>=1.0 then
    begin
      data^.fade:=1.0;
			exit(1);
    end;
    exit(0);
  end;
  if data^.countdown>0 then//Countdown
  begin
    data^.countdown:=data^.countdown-longint(data^.gameinfo^.steps^);
		exit(0);
  end;

  //zeitunabhängige Bewegung:
  for a:=0 to data^.gameinfo^.playernum-1 do
    for b:=0 to 3 do
      if ((data^.gameinfo^.joystick[a].button[b])^>0) then
      begin
        data^.step:=2;
        Mix_FadeOutMusic(500);
        exit(0);
      end;

  for steps:=0 to (data^.gameinfo^.steps)^-1 do
  begin
    //zeitabhängige Berechnungen
  end;
  exit(0);
end;

function op_calc_game_thread(data : pdata):longint;
begin
  if data^.gameinfo^.shouldend^>0 then
	  exit(1);
	exit(0);
end;

procedure op_draw_game(data:pdata);
var keks:string;
begin
  glColor4f(1,1,1,1);
  ZWdrawtextmiddle(ZWtext(data^.text),0,1.2,-4,'Pascal Skeleton',1.0);  
  ZWdrawtextmiddle(ZWtext(data^.text),0,-2.0,-4,'Press any key',0.6);  

  glDepthFunc(GL_ALWAYS);  

  //Countdown
  glLoadIdentity();
  if data^.countdown>0 then
	begin
	  keks:=inttostr(Trunc(data^.countdown/1000+1))+chr(0);
      ZWdrawtextmiddle(ZWtext(data^.text),0,-0.4,-4,@keks[1],2.0);  
	end;

  //Fade
  glLoadIdentity();
  if data^.step<>1 then
  begin
    glDisable(GL_TEXTURE_2D);
    glColor4f(0,0,0,data^.fade);
    glBegin(GL_QUADS);
      glVertex3f(-1,1,-1);
      glVertex3f(-1,-1,-1);
      glVertex3f(1,-1,-1);
      glVertex3f(1,1,-1);      
    glEnd();
    glEnable(GL_TEXTURE_2D);
  end;
  glDepthFunc(GL_LEQUAL);
end;

procedure op_reset_game(data:pdata;gameresult:pgameresult);
var b : integer;
begin
  for b:=0 to data^.gameinfo^.playernum-1 do
    gameresult^.winner[b]:=1; //Bei dem Skeleton sind _alle_ Gewinner ^.^  
    
  //Musik zurücksetzen:
  Mix_VolumeMusic(data^.old_music_volume);
  Mix_FreeMusic(data^.backgroundmusic);
end;

exports op_get_gameinfos, op_get_gamedata_size, op_init_game, op_calc_game, op_calc_game_thread, op_draw_game, op_reset_game;

begin
  SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide,exOverflow, exUnderflow, exPrecision]);
end.
