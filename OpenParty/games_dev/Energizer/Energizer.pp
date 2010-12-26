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
library Energizer;
{$MODE OBJFPC}
{$H+}
{$IFDEF WINDOWS}
  {$CALLING cdecl}
{$ENDIF}
{$IFDEF UNIX}
  {$CALLING cdecl}
{$ENDIF}

uses sdl,sdl_mixer,dglOpenGL,ttf,texture,threedobjects,minigames,zizwindow,sysutils,math;

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
    crystalltex,crystall_standtex,ground : GLuint;
    crystall,crystall_stand,crystall_orig : object3d;
    player : array [0..7] of integer;
    position : array [0..7] of integer;
    gamestep : integer; //0 way to player, 1 Player has to push the button
    rand     : integer;
    point_given : boolean;
    points : array [0..7] of integer;
    time : single;
    momplayer : integer;
    round : integer;
    snd_fail,snd_yeah : PMix_Chunk;
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

procedure changeposition(data : pdata;pos,value : integer);
begin
  case pos of
    0: begin
         data^.crystall_stand.p[ 17].u:=data^.crystall_orig.p[ 17].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 17].v:=data^.crystall_orig.p[ 17].v-(value div 4)*0.25;
         data^.crystall_stand.p[ 20].u:=data^.crystall_orig.p[ 20].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 20].v:=data^.crystall_orig.p[ 20].v-(value div 4)*0.25;
         data^.crystall_stand.p[172].u:=data^.crystall_orig.p[172].u+(value mod 4)*0.25;
         data^.crystall_stand.p[172].v:=data^.crystall_orig.p[172].v-(value div 4)*0.25;
         data^.crystall_stand.p[177].u:=data^.crystall_orig.p[177].u+(value mod 4)*0.25;
         data^.crystall_stand.p[177].v:=data^.crystall_orig.p[177].v-(value div 4)*0.25;
       end;
    1: begin
         data^.crystall_stand.p[  8].u:=data^.crystall_orig.p[  8].u+(value mod 4)*0.25;
         data^.crystall_stand.p[  8].v:=data^.crystall_orig.p[  8].v-(value div 4)*0.25;
         data^.crystall_stand.p[ 12].u:=data^.crystall_orig.p[ 12].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 12].v:=data^.crystall_orig.p[ 12].v-(value div 4)*0.25;
         data^.crystall_stand.p[180].u:=data^.crystall_orig.p[180].u+(value mod 4)*0.25;
         data^.crystall_stand.p[180].v:=data^.crystall_orig.p[180].v-(value div 4)*0.25;
         data^.crystall_stand.p[185].u:=data^.crystall_orig.p[185].u+(value mod 4)*0.25;
         data^.crystall_stand.p[185].v:=data^.crystall_orig.p[185].v-(value div 4)*0.25;
       end;
    2: begin
         data^.crystall_stand.p[  1].u:=data^.crystall_orig.p[  1].u+(value mod 4)*0.25;
         data^.crystall_stand.p[  1].v:=data^.crystall_orig.p[  1].v-(value div 4)*0.25;
         data^.crystall_stand.p[  4].u:=data^.crystall_orig.p[  4].u+(value mod 4)*0.25;
         data^.crystall_stand.p[  4].v:=data^.crystall_orig.p[  4].v-(value div 4)*0.25;
         data^.crystall_stand.p[124].u:=data^.crystall_orig.p[124].u+(value mod 4)*0.25;
         data^.crystall_stand.p[124].v:=data^.crystall_orig.p[124].v-(value div 4)*0.25;
         data^.crystall_stand.p[128].u:=data^.crystall_orig.p[128].u+(value mod 4)*0.25;
         data^.crystall_stand.p[128].v:=data^.crystall_orig.p[128].v-(value div 4)*0.25;
       end;
    3: begin
         data^.crystall_stand.p[ 56].u:=data^.crystall_orig.p[ 56].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 56].v:=data^.crystall_orig.p[ 56].v-(value div 4)*0.25;
         data^.crystall_stand.p[ 60].u:=data^.crystall_orig.p[ 60].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 60].v:=data^.crystall_orig.p[ 60].v-(value div 4)*0.25;
         data^.crystall_stand.p[132].u:=data^.crystall_orig.p[132].u+(value mod 4)*0.25;
         data^.crystall_stand.p[132].v:=data^.crystall_orig.p[132].v-(value div 4)*0.25;
         data^.crystall_stand.p[136].u:=data^.crystall_orig.p[136].u+(value mod 4)*0.25;
         data^.crystall_stand.p[136].v:=data^.crystall_orig.p[136].v-(value div 4)*0.25;
       end;
    4: begin
         data^.crystall_stand.p[ 49].u:=data^.crystall_orig.p[ 49].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 49].v:=data^.crystall_orig.p[ 49].v-(value div 4)*0.25;
         data^.crystall_stand.p[ 52].u:=data^.crystall_orig.p[ 52].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 52].v:=data^.crystall_orig.p[ 52].v-(value div 4)*0.25;
         data^.crystall_stand.p[140].u:=data^.crystall_orig.p[140].u+(value mod 4)*0.25;
         data^.crystall_stand.p[140].v:=data^.crystall_orig.p[140].v-(value div 4)*0.25;
         data^.crystall_stand.p[144].u:=data^.crystall_orig.p[144].u+(value mod 4)*0.25;
         data^.crystall_stand.p[144].v:=data^.crystall_orig.p[144].v-(value div 4)*0.25;
       end;
    5: begin
         data^.crystall_stand.p[ 42].u:=data^.crystall_orig.p[ 42].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 42].v:=data^.crystall_orig.p[ 42].v-(value div 4)*0.25;
         data^.crystall_stand.p[ 44].u:=data^.crystall_orig.p[ 44].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 44].v:=data^.crystall_orig.p[ 44].v-(value div 4)*0.25;
         data^.crystall_stand.p[148].u:=data^.crystall_orig.p[148].u+(value mod 4)*0.25;
         data^.crystall_stand.p[148].v:=data^.crystall_orig.p[148].v-(value div 4)*0.25;
         data^.crystall_stand.p[152].u:=data^.crystall_orig.p[152].u+(value mod 4)*0.25;
         data^.crystall_stand.p[152].v:=data^.crystall_orig.p[152].v-(value div 4)*0.25;
       end;
    6: begin
         data^.crystall_stand.p[ 32].u:=data^.crystall_orig.p[ 32].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 32].v:=data^.crystall_orig.p[ 32].v-(value div 4)*0.25;
         data^.crystall_stand.p[ 37].u:=data^.crystall_orig.p[ 37].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 37].v:=data^.crystall_orig.p[ 37].v-(value div 4)*0.25;
         data^.crystall_stand.p[156].u:=data^.crystall_orig.p[156].u+(value mod 4)*0.25;
         data^.crystall_stand.p[156].v:=data^.crystall_orig.p[156].v-(value div 4)*0.25;
         data^.crystall_stand.p[160].u:=data^.crystall_orig.p[160].u+(value mod 4)*0.25;
         data^.crystall_stand.p[160].v:=data^.crystall_orig.p[160].v-(value div 4)*0.25;
       end;
    7: begin
         data^.crystall_stand.p[ 24].u:=data^.crystall_orig.p[ 24].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 24].v:=data^.crystall_orig.p[ 24].v-(value div 4)*0.25;
         data^.crystall_stand.p[ 28].u:=data^.crystall_orig.p[ 28].u+(value mod 4)*0.25;
         data^.crystall_stand.p[ 28].v:=data^.crystall_orig.p[ 28].v-(value div 4)*0.25;
         data^.crystall_stand.p[164].u:=data^.crystall_orig.p[164].u+(value mod 4)*0.25;
         data^.crystall_stand.p[164].v:=data^.crystall_orig.p[164].v-(value div 4)*0.25;
         data^.crystall_stand.p[168].u:=data^.crystall_orig.p[168].u+(value mod 4)*0.25;
         data^.crystall_stand.p[168].v:=data^.crystall_orig.p[168].v-(value div 4)*0.25;
       end;
  end;
  ZWrefreshdrawlist(@(data^.crystall_stand),0);
end;

procedure op_init_game(data : pdata;gameinfo : pgameinfo);
var a : integer;
    keks : string;
begin	
	InitOpenGL;
	ReadExtensions;
  
  randomize;

  //Text initialisieren
  data^.text.texture:=gameinfo^.texttexture;
  for a:=0 to 255 do
    data^.text.breite[a]:=gameinfo^.textwidth[a];
    
  //Gameinforeferenzpointer speicher:
  data^.gameinfo:=gameinfo;

  //Texturen laden
  keks:=String(gameinfo^.datafolder)+'textures/crystall.png';
  data^.crystall_standtex:=ZWloadtexturefromfileA(@keks[1],0,gameinfo^.texquali);
  keks:=String(gameinfo^.datafolder)+'textures/lava.png';
  data^.crystalltex:=ZWloadtexturefromfileA(@keks[1],0,gameinfo^.texquali);
  keks:=String(gameinfo^.datafolder)+'textures/street.png';
  data^.ground:=ZWloadtexturefromfileA(@keks[1],0,gameinfo^.texquali);
  
  //Objekte laden
  keks:=String(gameinfo^.datafolder)+'models/crystall.3dm';
  ZWload3dm(@(data^.crystall),@keks[1],data^.crystalltex);
  ZWcreatedrawlist(@(data^.crystall),0);
  keks:=String(gameinfo^.datafolder)+'models/crystall_stand.3dm';
  ZWload3dm(@(data^.crystall_orig),@keks[1],0);
  keks:=String(gameinfo^.datafolder)+'models/crystall_stand.3dm';
  ZWload3dm(@(data^.crystall_stand),@keks[1],data^.crystall_standtex);
  ZWcreatedrawlist(@(data^.crystall_stand),0);

  //Ton vorbereiten
	keks:=String(gameinfo^.datafolder)+'sounds/Vegas Glitz.ogg';
  data^.backgroundmusic := Mix_LoadMUS(@keks[1]);
  data^.old_music_volume:=Mix_VolumeMusic(round(96.0*gameinfo^.volume));
  Mix_FadeInMusic(data^.backgroundmusic,-1,500);

	keks:=String(gameinfo^.datafolder)+'sounds/negative.ogg';
  data^.snd_fail := Mix_LoadWAV(@keks[1]);
  Mix_VolumeChunk(data^.snd_fail,round(64*gameinfo^.volume));

	keks:=String(gameinfo^.datafolder)+'sounds/positive.ogg';
  data^.snd_yeah := Mix_LoadWAV(@keks[1]);
  Mix_VolumeChunk(data^.snd_yeah,round(128*gameinfo^.volume));


  //Licht
  ZWenablelight(0);
  ZWsetlightambient(0,0.3,0.3,0.3);
  ZWsetlightdiffuse(0,0.6,0.6,0.6);
  ZWsetmaterial(lnone);
  
  for a:=0 to 7 do
  begin
    data^.player[a]:=-1;
    changeposition(data,a,4);
  end;
  for a:=0 to data^.gameinfo^.playernum-1 do
  begin
    data^.player[a*8 div data^.gameinfo^.playernum]:=a;
    data^.position[a]:=a*8 div data^.gameinfo^.playernum;
    data^.points[a]:=0;
  end;

  //Spiel ansich vorbereiten
  data^.momplayer:=0;
  data^.gamestep:=0;
  data^.time:=1.0;
  data^.step:=0;
  data^.fade:=1;
  data^.countdown:=3000;  
  data^.round:=1;
end;

function op_calc_game(data:pdata):longint;
var a,b,steps : integer;
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

  for steps:=0 to (data^.gameinfo^.steps)^-1 do
  with data^ do
  begin
    case gamestep of
      0: begin
           a:=(momplayer+gameinfo^.playernum-1) mod gameinfo^.playernum;
           b:=position[momplayer]-position[a];
           if b<0 then
             b+=8;
           time-=0.005/b;
           if time<0 then
           begin
             rand:=random(4);
             changeposition(data,position[momplayer],rand);
             point_given:=false;
             gamestep:=1;
             time:=1.0;
           end;      
         end;
      1: begin
           if not(point_given) then
           begin
             if (gameinfo^.iscpu[momplayer]>0) and (random(450)=0) then
             begin
               if random(10)=0 then
                 (gameinfo^.joystick[momplayer].button[(rand+1) mod 4])^:=1
               else
                 (gameinfo^.joystick[momplayer].button[rand])^:=1;
             end;
             if (gameinfo^.joystick[momplayer].button[rand])^>0 then
             begin
               Mix_PlayChannel(-1,snd_yeah,0);
               inc(points[momplayer]);
               point_given:=true;
               (gameinfo^.joystick[momplayer].button[0])^:=0;
               (gameinfo^.joystick[momplayer].button[1])^:=0;
               (gameinfo^.joystick[momplayer].button[2])^:=0;
               (gameinfo^.joystick[momplayer].button[3])^:=0;
             end;
             if ((gameinfo^.joystick[momplayer].button[(rand+1) mod 4])^>0)
             or ((gameinfo^.joystick[momplayer].button[(rand+2) mod 4])^>0)
             or ((gameinfo^.joystick[momplayer].button[(rand+3) mod 4])^>0) then
             begin
               Mix_PlayChannel(-1,snd_fail,0);
               point_given:=true;
               (gameinfo^.joystick[momplayer].button[0])^:=0;
               (gameinfo^.joystick[momplayer].button[1])^:=0;
               (gameinfo^.joystick[momplayer].button[2])^:=0;
               (gameinfo^.joystick[momplayer].button[3])^:=0;
             end;
           end;
           time-=0.0009*sqrt(round/3);
           if time<0 then
           begin
             if not(point_given) then
               Mix_PlayChannel(-1,snd_fail,0);
             changeposition(data,position[momplayer],4);
             gamestep:=0;
             time:=1.0;
             momplayer:=(momplayer+1) mod gameinfo^.playernum;
             if momplayer=0 then
               inc(round);
             if round>10 then
             begin
               data^.step:=2;
               Mix_FadeOutMusic(500);
               exit(0);
             end;
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
    a : integer;
    pre : integer;
    source,dest : single;
begin
  glColor4f(1,1,1,1);

  //Camera:
  glTranslatef(0,0.2,-3);
  glRotatef(60,1,0,0);
  glEnable(GL_LIGHTING);
  case data^.gamestep of
    0: begin
         pre:=data^.momplayer-1;
         if pre<0 then
           pre:=data^.gameinfo^.playernum-1;
         dest:=-data^.position[data^.momplayer]*45.0+11.25;
         source:=-data^.position[pre]*45.0-11.25;
         if source<dest then
           dest-=360.0;
         glRotatef(source+(dest-source)*(1.0-data^.time),0,1,0);
       end;
    1: begin
         glRotatef(-data^.position[data^.momplayer]*45.0-(0.5-data^.time)*22.5,0,1,0);
       end;  
  
  end;
  
  ZWsetlightposition(0,0.0,20.0,0.0);


  glPushMatrix;
    glRotatef(-90,1,0,0);
    glColor4f(1,1,1,1);
    ZWdrawsprite(data^.ground,0,0,0,6,6,3,0);
  glPopMatrix;
  
  for a:=0 to 7 do
  if data^.player[a]<>-1 then
  begin
    glPushMatrix;
    glRotatef(a*45.0,0,1,0);
    glTranslatef(0,0,1.2);
    glScalef(0.2,0.2,0.2);
    glColor4f(0,0,0,1);
    glPushMatrix;
    glRotatef(-90,1,0,0);
    ZWdrawsprite(data^.gameinfo^.maintex,0,0,0.01,1.5,1.5,16,200);
    keks:=inttostr(data^.points[data^.player[a]]);
    glColor4f(0,0,0,0.7);
    ZWdrawtextmiddle(ZWtext(data^.text),0,-2,0.02,@keks[1],2.0);  
    glRotatef( 90,1,0,0);
    glTranslatef(0,1.0,0);
    glRotatef(180,0,1,0);
    glColor4f(data^.gameinfo^.color[data^.player[a]].r,data^.gameinfo^.color[data^.player[a]].g,data^.gameinfo^.color[data^.player[a]].b,1);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    ZWusetexture(data^.gameinfo^.facetex);
    glCallList(data^.gameinfo^.ball[data^.player[a]]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPopMatrix;
  end;

  glColor4f(0,0,0,1);
  glRotatef(-90,1,0,0);
  ZWdrawsprite(data^.gameinfo^.maintex,0,0,0.01,1,1,8,30);
  glRotatef( 90,1,0,0);
  
  glColor4f(1,1,1,1);
  glRotatef(11.25,0,1,0);
  ZWdrawobject(data^.crystall_stand);
  glTranslatef(0,0.5,0);
  ZWusetexture(data^.crystalltex);
  glMatrixMode(GL_TEXTURE);
    glTranslatef((SDL_GetTicks mod 3000)/3000,0,0);
  glMatrixMode(GL_MODELVIEW);
  ZWdrawobject(data^.crystall);
  glMatrixMode(GL_TEXTURE);
    glLoadIdentity;
  glMatrixMode(GL_MODELVIEW);
  
  glDisable(GL_LIGHTING);

  glDepthFunc(GL_ALWAYS);  

  //Countdown
  glLoadIdentity();
  if data^.countdown>0 then
	begin
	  keks:=inttostr(Trunc(data^.countdown/1000+1))+chr(0);
    glColor4f(0.4,0.3,1.0,0.7);
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
var b,biggest : integer;
begin
  biggest:=1;
  for b:=0 to data^.gameinfo^.playernum-1 do
    if data^.points[b]>biggest then
      biggest:=data^.points[b];
  for b:=0 to data^.gameinfo^.playernum-1 do
    if data^.points[b]=biggest then
      gameresult^.winner[b]:=1
    else
      gameresult^.winner[b]:=0;


  ZWdeletetexture(data^.crystalltex);
  ZWdeletetexture(data^.crystall_standtex);
  ZWdeletetexture(data^.ground);
  ZWdeleteobject(data^.crystall);
  ZWdeleteobject(data^.crystall_stand);
  ZWdisablelight(0);
    
  //Musik zurücksetzen:
  Mix_FreeChunk(data^.snd_fail);    
  Mix_FreeChunk(data^.snd_yeah);    
  Mix_VolumeMusic(data^.old_music_volume);
  Mix_FreeMusic(data^.backgroundmusic);
end;

exports op_get_gameinfos, op_get_gamedata_size, op_init_game, op_calc_game, op_calc_game_thread, op_draw_game, op_reset_game;

begin
  SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide,exOverflow, exUnderflow, exPrecision]);
end.
