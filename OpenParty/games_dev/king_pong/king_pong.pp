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
library king_pong;
{$MODE OBJFPC}
{$H+}
{$IFDEF WINDOWS}
  {$CALLING cdecl}
{$ENDIF}
{$IFDEF UNIX}
  {$CALLING cdecl}
{$ENDIF}

uses sdl,sdl_mixer,dglOpenGL,ttf,texture,minigames,sysutils,math;

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
    player_live : array [0..7] of integer; //0 -> dead, begins with 3
    paddle_pos : array [0..7] of single; //0..1
    paddle_width : array [0..7] of single; //0.1..1
    line_length : single;
    ball        : array [1..8] of record
                                    x,y : single; //position
                                    dir : single; //direction
                                    s   : single; //speed
                                  end;
    ballcount   : integer;
    playernum   : integer;
    time        : integer;
    snd_fail,snd_pong : PMix_Chunk;
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
  randomize;
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
  data^.old_music_volume:=Mix_VolumeMusic(round(96*gameinfo^.volume));
  Mix_FadeInMusic(data^.backgroundmusic,-1,500);

	keks:=String(gameinfo^.datafolder)+'sounds/negative.ogg';
  data^.snd_fail := Mix_LoadWAV(@keks[1]);
  Mix_VolumeChunk(data^.snd_fail,round(128*gameinfo^.volume));

	keks:=String(gameinfo^.datafolder)+'sounds/plop.ogg';
  data^.snd_pong := Mix_LoadWAV(@keks[1]);
  Mix_VolumeChunk(data^.snd_pong,round(255*gameinfo^.volume));

  //Spiel ansich vorbereiten
  data^.step:=0;
  data^.fade:=1;
  data^.countdown:=3000;
  for a:=0 to 7 do
  begin
    data^.player_live[a]:=3;
    data^.paddle_pos[a]:=0.5;
    data^.paddle_width[a]:=0.2;
  end;
  if data^.gameinfo^.playernum=2 then //two player should not play on a line
  begin
    data^.playernum:=3;
    data^.player_live[2]:=0;
  end
  else
    data^.playernum:=data^.gameinfo^.playernum;
  data^.line_length:=sqrt((4.0-cos(1/data^.playernum*2.0*pi)*4.0)*(4.0-cos(1/data^.playernum*2.0*pi)*4.0)+
                          sin(1/data^.playernum*2.0*pi)*4.0*sin(1/data^.playernum*2.0*pi)*4.0);
  data^.ballcount:=1;
  data^.ball[1].x:=0;
  data^.ball[1].y:=0;
  data^.ball[1].s:=0;
  data^.ball[1].dir:=random(359)/359*pi;
  data^.time:=0;
end;

function op_calc_game(data:pdata):longint;
var a,b,steps,c,alive : integer;
    controller : array [0..7] of integer;
    w,k : single;
    x1,x2,y1,y2,n : single;
    fail : boolean;
begin
  if data^.step=0 then //Fade In
  begin
    data^.fade:=data^.fade-data^.gameinfo^.steps^/500.0;
    if data^.fade<=0.0 then
    begin
      data^.fade:=0.0;
      data^.step:=1;
    end;
    exit(0);
  end;
  if data^.step=2 then //Fade Out
  begin
    data^.fade:=data^.fade+data^.gameinfo^.steps^/500.0;
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
  fillchar(controller,8*sizeof(integer),0);
  for a:=0 to data^.playernum-1 do
  begin
    w:=(a+0.5)/data^.playernum*360.0;
    if ((data^.gameinfo^.joystick[a].axis[0])^<0) then
    begin
      if (w>0) and (w<180) then
        controller[a]:= 1;
      if (w>180) and (w<360) then
        controller[a]:=-1;
    end;
    if ((data^.gameinfo^.joystick[a].axis[0])^>0) then
    begin
      if (w>0) and (w<180) then
        controller[a]:=-1;
      if (w>180) and (w<360) then
        controller[a]:= 1;
    end;
    if ((data^.gameinfo^.joystick[a].axis[1])^<0) then
    begin
      if (w>90) and (w<270) then
        controller[a]:=-1;
      if (w>270) or (w<90) then
        controller[a]:= 1;
    end;
    if ((data^.gameinfo^.joystick[a].axis[1])^>0) then
    begin
      if (w>90) and (w<270) then
        controller[a]:= 1;
      if (w>270) or (w<90) then
        controller[a]:=-1;
    end;
    with data^ do
    if gameinfo^.iscpu[a]=1 then
    begin
      c:=1;
      n:=1000000000;
      for b:=1 to ballcount do
      begin
        {k:=w-ball[b].dir;
        if k<0 then
          k+=2.0*pi;
        if (k>=90) and (k<=270) then
          continue;}
        
        k:=0.1/sin((playernum-2)*pi/(2.0*playernum));
        x1:=cos(a/data^.playernum*2.0*pi)*(4.0-k)-ball[b].x;
        x2:=cos((a+1)/data^.playernum*2.0*pi)*(4.0-k)-ball[b].x;
        y1:=sin(a/data^.playernum*2.0*pi)*(4.0-k)-ball[b].y;
        y2:=sin((a+1)/data^.playernum*2.0*pi)*(4.0-k)-ball[b].y;
        if x1*y2-y1*x2<n then
        begin
          n:=x1*y2-y1*x2;
          c:=b;
        end;
      end;
      x1:=cos(a/data^.playernum*2.0*pi)*(4.0);
      x2:=cos((a+1)/data^.playernum*2.0*pi)*(4.0);
      y1:=sin(a/data^.playernum*2.0*pi)*(4.0);
      y2:=sin((a+1)/data^.playernum*2.0*pi)*(4.0);
      n:=(cos(ball[c].dir)*(y1-ball[c].y)-sin(ball[c].dir)*(x1-ball[c].x))/
         (sin(ball[c].dir)*(x2-x1)       -cos(ball[c].dir)*(y2-y1));
      if n<paddle_pos[a]-0.01 then
        controller[a]:=-1;
      if n>paddle_pos[a]+0.01 then
        controller[a]:= 1;
    end;
  end;
  
  for steps:=0 to (data^.gameinfo^.steps)^-1 do
  with data^ do
  begin
    inc(time);
    for a:=0 to playernum-1 do
    begin
      if controller[a]<0 then
      begin
        paddle_pos[a]-=0.001;
        if paddle_pos[a]<paddle_width[a]/2.0 then
          paddle_pos[a]:=paddle_width[a]/2.0;
      end;
      if controller[a]>0 then
      begin
        paddle_pos[a]+=0.001;
        if paddle_pos[a]>1.0-paddle_width[a]/2.0 then
          paddle_pos[a]:=1.0-paddle_width[a]/2.0;
      end;
    end;
    if time mod 20000 = 0 then
      if ballcount<8 then
      begin
        inc(ballcount);
        ball[ballcount].x:=0;
        ball[ballcount].y:=0;
        ball[ballcount].s:=0;
        ball[ballcount].dir:=random(359)/359*pi;
      end;
    for b:=1 to ballcount do
    begin
      if ball[b].s<0.001 then
        ball[b].s+=0.000001;
      if ball[b].s<0.002 then
        ball[b].s+=0.0000002;
      ball[b].s+=0.00000004;
      ball[b].x+=cos(ball[b].dir)*ball[b].s;
      ball[b].y+=sin(ball[b].dir)*ball[b].s;
      fail:=false;
      for a:=0 to playernum-1 do
      begin
        k:=0.1/sin((playernum-2)*pi/(2.0*playernum));
        x1:=cos(a/data^.playernum*2.0*pi)*(4.0-k)-ball[b].x;
        x2:=cos((a+1)/data^.playernum*2.0*pi)*(4.0-k)-ball[b].x;
        y1:=sin(a/data^.playernum*2.0*pi)*(4.0-k)-ball[b].y;
        y2:=sin((a+1)/data^.playernum*2.0*pi)*(4.0-k)-ball[b].y;
        if x1*y2-y1*x2<0 then
        begin
          ball[b].x-=cos(ball[b].dir)*ball[b].s;
          ball[b].y-=sin(ball[b].dir)*ball[b].s;
          w:=(a+0.5)/data^.playernum*2.0*pi;
          ball[b].dir:=2.0*w+pi-ball[b].dir;
          if ball[b].dir<0 then
            ball[b].dir+=2.0*pi;
          if ball[b].dir>=2.0*pi then
            ball[b].dir-=2.0*pi;
          if (player_live[a]>0) and
                ((sqrt(x1*x1+y1*y1)-line_length*paddle_width[a]*1.2/2.0>paddle_pos[a]*line_length) or
                 (sqrt(x1*x1+y1*y1)+line_length*paddle_width[a]*1.2/2.0<paddle_pos[a]*line_length)) then
          begin
            dec(player_live[a]);
            Mix_PlayChannel(-1,snd_fail,0);
          end
          else
            Mix_PlayChannel(-1,snd_pong,0);
          alive:=0;
          for c:=0 to playernum-1 do
            if player_live[c]>0 then
              inc(alive);
          if alive<2 then
          begin
            step:=2;
            fade:=0;
            Mix_FadeOutMusic(500);
            exit(0);
          end;
          
          ball[b].x+=cos(ball[b].dir)*ball[b].s;
          ball[b].y+=sin(ball[b].dir)*ball[b].s;
        end;
      end;
    end;
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
    a,b : integer;
    x1,y1,x2,y2 : single;
begin
  glDepthFunc(GL_ALWAYS);  

  glDisable(GL_TEXTURE_2D);
  glBegin(GL_LINES);
  for a:=0 to data^.playernum-1 do
  begin
    glColor4f(data^.gameinfo^.color[a].r*0.6+0.4,data^.gameinfo^.color[a].g*0.6+0.4,data^.gameinfo^.color[a].b*0.6+0.4,1);  
    glVertex3f(cos( a   /data^.playernum*2.0*pi)*4.0,sin( a   /data^.playernum*2.0*pi)*4.0,-8.0);
    glVertex3f(cos((a+1)/data^.playernum*2.0*pi)*4.0,sin((a+1)/data^.playernum*2.0*pi)*4.0,-8.0);
  end;
  glEnd;
  for a:=0 to data^.playernum-1 do
  begin
    glPushMatrix();
      x1:=cos(a/data^.playernum*2.0*pi)*4.0;
      x2:=cos((a+1)/data^.playernum*2.0*pi)*4.0;
      y1:=sin(a/data^.playernum*2.0*pi)*4.0;
      y2:=sin((a+1)/data^.playernum*2.0*pi)*4.0;
      if data^.player_live[a]>0 then
      begin
        glTranslatef(x1+(x2-x1)*data^.paddle_pos[a],y1+(y2-y1)*data^.paddle_pos[a],-8.0);
        glRotatef((a+0.5)/data^.playernum*360.0-90,0,0,1);
        glColor4f(data^.gameinfo^.color[a].r*0.6+0.4,data^.gameinfo^.color[a].g*0.6+0.4,data^.gameinfo^.color[a].b*0.6+0.4,1);  
        glBegin(GL_QUADS);
          glVertex3f(-data^.paddle_width[a]*data^.line_length/2.0, 0.4,0);
          glVertex3f(-data^.paddle_width[a]*data^.line_length/2.0, 0.0,0);
          glVertex3f( data^.paddle_width[a]*data^.line_length/2.0, 0.0,0);
          glVertex3f( data^.paddle_width[a]*data^.line_length/2.0, 0.4,0);
        glEnd;
        glEnable(GL_TEXTURE_2D);
          glColor4f(1,1,1,1);
          ZWdrawsprite(data^.gameinfo^.facetex,0,0.2,0,0.2,0.2,4,data^.gameinfo^.facenr[a]);
          ZWdrawsprite(data^.gameinfo^.maintex,0,0.7,0,0.2,0.2,8,31);
          if data^.player_live[a]>1 then
            ZWdrawsprite(data^.gameinfo^.maintex,-0.25,1.2,0,0.2,0.2,8,31);
          if data^.player_live[a]>2 then
            ZWdrawsprite(data^.gameinfo^.maintex, 0.25,1.2,0,0.2,0.2,8,31);          
        glDisable(GL_TEXTURE_2D);
      end
      else
      begin
        glTranslatef(x1+(x2-x1)*0.5,y1+(y2-y1)*0.5,-8.0);
        glRotatef((a+0.5)/data^.playernum*360.0-90,0,0,1);
        glColor4f(0.2,0.2,0.2,1);  
        glBegin(GL_QUADS);
          glVertex3f(-data^.line_length/2.0, 0.4,0);
          glVertex3f(-data^.line_length/2.0, 0.0,0);
          glVertex3f( data^.line_length/2.0, 0.0,0);
          glVertex3f( data^.line_length/2.0, 0.4,0);
        glEnd;
        glEnable(GL_TEXTURE_2D);
          glColor4f(1,1,1,1);
          for b:=-2 to 2 do
          ZWdrawsprite(data^.gameinfo^.maintex,data^.line_length*b/5.0,0.2,0,0.2,0.2,8,60);
        glDisable(GL_TEXTURE_2D);
      end;      
    glPopMatrix();
  end;  
  glEnable(GL_TEXTURE_2D);
  glColor4f(1,1,1,1);  
  for b:=1 to data^.ballcount do
  with data^ do
  begin
    ZWdrawsprite(gameinfo^.maintex,ball[b].x,ball[b].y,-8,0.1,0.1,2,2);
  end;
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
    if data^.player_live[b]>0 then
      gameresult^.winner[b]:=1
    else
      gameresult^.winner[b]:=0;

  Mix_FreeChunk(data^.snd_fail);    
  Mix_FreeChunk(data^.snd_pong);    

  Mix_VolumeMusic(data^.old_music_volume);
  Mix_FreeMusic(data^.backgroundmusic);
end;

exports op_get_gameinfos, op_get_gamedata_size, op_init_game, op_calc_game, op_calc_game_thread, op_draw_game, op_reset_game;

begin
  SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide,exOverflow, exUnderflow, exPrecision]);
end.
