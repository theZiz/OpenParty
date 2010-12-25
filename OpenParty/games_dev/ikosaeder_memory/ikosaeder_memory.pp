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
 provisions of LGPL License are applicable instead of those above.
                                                                            
 For feedback and questions about my Files and Projects please mail me,     
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com                         
*)

library ikosaeder_memory;
{$MODE OBJFPC}
{$H+}
{$IFDEF WINDOWS}
  {$CALLING cdecl}
{$ENDIF}
{$IFDEF UNIX}
  {$CALLING cdecl}
{$ENDIF}

uses sdl,sdl_mixer,dglOpenGL,ttf,minigames,sysutils,threedobjects,zizwindow,math,texture;

type matrixtype = extended; //Genauigkeit einstellen
type pmatrixtype = ^matrixtype;

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
    ikosaeder,arrow : object3d;
    color : array [0..11,0..2] of single;
    ecke_oben : boolean;
    mom_site,old_site : shortint;
    gamestep : integer; //0 wait of input/move (see "way")
                        //1 Triangle Fade In / Arrow Fade Out / "Show Card" Fade Out / "Ok" Fade In
                        //2 Triangle wait "ok"
                        //3 Player Change
                        //4 Triangle Fade Out / Next Player / Arrow Fade In / "Show Card" Fade In / "Ok" Fade Out
    matrix,newmatrix,oldmatrix : array [0..15] of matrixtype;
    way : single;
    momplayer : shortint;
    memory : GLuint;
    places : array [0..19] of byte;
    open : array [0..19] of boolean;
    hasopen : byte;
    first_one : byte;
    equal : boolean;
    owner : array [0..9] of shortint;
    seen : array [0..19] of boolean;
    strategy : integer;
    dest1,dest2 : integer;
    snd_meow,snd_hu : PMix_Chunk;
  end;

{$include ki.inc}

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

procedure rotate_matrix(matrix : pmatrixtype;x,y,z,rad : matrixtype);
var s,c,l : matrixtype;
    rotate,oldmatrix,result : array [0..15] of matrixtype;
    a : integer;
begin
	//Rotationsmatrix:
	s:=sin(rad);
	c:=cos(rad);
	l:=sqrt(x*x+y*y+z*z);
	if l=0 then
	  exit;
	x:=x/l;
	y:=y/l;
	z:=z/l;
    rotate[ 0]:= c+x*x*(1.0-c);   rotate[ 1]:= x*y*(1.0-c)-z*s; rotate[ 2]:= x*z*(1.0-c)+y*s; rotate[ 3]:=0;
		rotate[ 4]:= y*x*(1.0-c)+z*s; rotate[ 5]:= c+y*y*(1.0-c);   rotate[ 6]:=-y*z*(1.0-c)-x*s; rotate[ 7]:=0;
		rotate[ 8]:= z*x*(1.0-c)-y*s; rotate[ 9]:= z*y*(1.0-c)+x*s; rotate[10]:= c+z*z*(1.0-c);   rotate[11]:=0;
		rotate[12]:= 0;               rotate[13]:= 0;               rotate[14]:= 0;               rotate[15]:=1;

    {rotate[ 0]:= c+x*x*(1.0-c);   rotate[ 4]:= x*y*(1.0-c)-z*s; rotate[ 8]:= x*z*(1.0-c)+y*s; rotate[12]:=0;
		rotate[ 1]:= y*x*(1.0-c)+z*s; rotate[ 5]:= c+y*y*(1.0-c);   rotate[ 9]:=-y*z*(1.0-c)-x*s; rotate[13]:=0;
		rotate[ 2]:= z*x*(1.0-c)-y*s; rotate[ 6]:= z*y*(1.0-c)+x*s; rotate[10]:= c+z*z*(1.0-c);   rotate[14]:=0;
		rotate[ 3]:= 0;               rotate[ 7]:= 0;               rotate[11]:= 0;               rotate[15]:=1;}


	//Matrix umdrehen:
  oldmatrix:=matrix;
  for a:=0 to 15 do
    oldmatrix[a]:=matrix[a];
	//move(matrix,oldmatrix,sizeof(float)*16);
	matrix[ 0]:=oldmatrix[ 0]; matrix[ 1]:=oldmatrix[ 4]; matrix[ 2]:=oldmatrix[ 8]; matrix[ 3]:=oldmatrix[12];
	matrix[ 4]:=oldmatrix[ 1]; matrix[ 5]:=oldmatrix[ 5]; matrix[ 6]:=oldmatrix[ 9]; matrix[ 7]:=oldmatrix[13];
	matrix[ 8]:=oldmatrix[ 2]; matrix[ 9]:=oldmatrix[ 6]; matrix[10]:=oldmatrix[10]; matrix[11]:=oldmatrix[14];
	matrix[12]:=oldmatrix[ 3]; matrix[13]:=oldmatrix[ 7]; matrix[14]:=oldmatrix[11]; matrix[15]:=oldmatrix[15];
	//Multiplikation
		
	result[ 0]:=matrix[ 0]*rotate[ 0]+matrix[ 4]*rotate[ 1]+matrix[ 8]*rotate[ 2]+matrix[12]*rotate[ 3];
	result[ 1]:=matrix[ 0]*rotate[ 4]+matrix[ 4]*rotate[ 5]+matrix[ 8]*rotate[ 6]+matrix[12]*rotate[ 7];
	result[ 2]:=matrix[ 0]*rotate[ 8]+matrix[ 4]*rotate[ 9]+matrix[ 8]*rotate[10]+matrix[12]*rotate[11];
	result[ 3]:=matrix[ 0]*rotate[12]+matrix[ 4]*rotate[13]+matrix[ 8]*rotate[14]+matrix[12]*rotate[15];
	
	result[ 4]:=matrix[ 1]*rotate[ 0]+matrix[ 5]*rotate[ 1]+matrix[ 9]*rotate[ 2]+matrix[13]*rotate[ 3];
	result[ 5]:=matrix[ 1]*rotate[ 4]+matrix[ 5]*rotate[ 5]+matrix[ 9]*rotate[ 6]+matrix[13]*rotate[ 7];
	result[ 6]:=matrix[ 1]*rotate[ 8]+matrix[ 5]*rotate[ 9]+matrix[ 9]*rotate[10]+matrix[13]*rotate[11];
	result[ 7]:=matrix[ 1]*rotate[12]+matrix[ 5]*rotate[13]+matrix[ 9]*rotate[14]+matrix[13]*rotate[15];
	
	result[ 8]:=matrix[ 2]*rotate[ 0]+matrix[ 6]*rotate[ 1]+matrix[10]*rotate[ 2]+matrix[14]*rotate[ 3];
	result[ 9]:=matrix[ 2]*rotate[ 4]+matrix[ 6]*rotate[ 5]+matrix[10]*rotate[ 6]+matrix[14]*rotate[ 7];
	result[10]:=matrix[ 2]*rotate[ 8]+matrix[ 6]*rotate[ 9]+matrix[10]*rotate[10]+matrix[14]*rotate[11];
	result[11]:=matrix[ 2]*rotate[12]+matrix[ 6]*rotate[13]+matrix[10]*rotate[14]+matrix[14]*rotate[15];
	
	result[12]:=matrix[ 3]*rotate[ 0]+matrix[ 7]*rotate[ 1]+matrix[11]*rotate[ 2]+matrix[15]*rotate[ 3];
	result[13]:=matrix[ 3]*rotate[ 4]+matrix[ 7]*rotate[ 5]+matrix[11]*rotate[ 6]+matrix[15]*rotate[ 7];
	result[14]:=matrix[ 3]*rotate[ 8]+matrix[ 7]*rotate[ 9]+matrix[11]*rotate[10]+matrix[15]*rotate[11];
	result[15]:=matrix[ 3]*rotate[12]+matrix[ 7]*rotate[13]+matrix[11]*rotate[14]+matrix[15]*rotate[15];

  for a:=0 to 15 do
    matrix[a]:=result[a];
end;


procedure op_init_game(data : pdata;gameinfo : pgameinfo);
const color_pre : array [0..11,0..2] of single = ((0,0,0),(0.5,0.5,0.5),(1,1,1),(1,0,0),(0,1,0),(1,1,0),(0,0,1),(1,0,1),(0,1,1),(0.42,0,0.48),(1,0.65,0),(0.5,0.1,0.1));
var a,b,c : integer;
    d : byte;
    keks : string;
    temp_color : array [0..2] of single;
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
	keks:=String(gameinfo^.datafolder)+'sounds/Radio Martini.ogg';
  data^.backgroundmusic := Mix_LoadMUS(@keks[1]);
  data^.old_music_volume:=Mix_VolumeMusic(round(96*gameinfo^.volume));
  Mix_FadeInMusic(data^.backgroundmusic,-1,500);

	keks:=String(gameinfo^.datafolder)+'sounds/meow.ogg';
  data^.snd_meow := Mix_LoadWAV(@keks[1]);
  Mix_VolumeChunk(data^.snd_meow,round(128*gameinfo^.volume));

	keks:=String(gameinfo^.datafolder)+'sounds/hu.ogg';
  data^.snd_hu := Mix_LoadWAV(@keks[1]);
  Mix_VolumeChunk(data^.snd_hu,round(128*gameinfo^.volume));

  //Texturen laden
  keks:=String(gameinfo^.datafolder)+'textures/memory.png';
  data^.memory:=ZWloadtexturefromfileA(@keks[1],0,1{gameinfo^.texquali});
  
  //Objekte laden
  keks:=String(gameinfo^.datafolder)+'models/ikosaeder_texture.3dm';
  ZWload3dm(@(data^.ikosaeder),@keks[1],0);
  keks:=String(gameinfo^.datafolder)+'models/arrow.3dm';
  ZWload3dm(@(data^.arrow),@keks[1],0);
  ZWcreatedrawlist(@(data^.arrow),0);
  
  
  //Licht von vorne
  glEnable(GL_LIGHTING);
  ZWsetlightposition(0,0,0,0);
  ZWsetlightambient(0,0.2,0.2,0.2);
  ZWsetlightdiffuse(0,0.8,0.8,0.8);
  ZWenablelight(0);
  glDisable(GL_LIGHTING);
  
  glClearColor(1,1,1,0);
  
  data^.color:=color_pre;
  //wir tauschen ein wenig hin und her. ^^
  for a:=1 to 200 do
  begin
    b:=random(12);
    c:=random(12);
    temp_color:=data^.color[b];
    data^.color[b]:=data^.color[c];
    data^.color[c]:=temp_color;
  end;
  for a:=0 to 15 do
  if a mod 5 = 0 then
    data^.matrix[a]:=1
  else
    data^.matrix[a]:=0;
  
  //Spiel ansich vorbereiten
  for a:=0 to 9 do
    data^.owner[a]:=-1;
  for a:=0 to 19 do
  begin
    data^.seen[a]:=false;
    data^.places[a]:=a div 2;
    data^.open[a]:=false;
  end;
  for a:=0 to 200 do
  begin
    b:=random(20);
    c:=random(20);
    d:=data^.places[b];
    data^.places[b]:=data^.places[c];
    data^.places[c]:=d;
  end;
  data^.mom_site:=0;
  data^.step:=0;
  data^.fade:=1;
  data^.countdown:=3000;  
  data^.gamestep:=0;
  data^.way:=0.0;
  data^.momplayer:=0;
  data^.hasopen:=0;
  find_strategy(data);
end;

function op_calc_game(data:pdata):longint;
const next : array [0..19,0..3] of shortint {0 links, 1 oben, 2 rechts, 3 unten} = (( 1,-1, 4, 6),( 8, 2, 0,-1),(10,-1, 3, 1),( 2,-1,12, 4),
                                                                                    ( 0, 3,14,-1),(12,-1,10,16),(11, 0, 9,-1),(14,12,17,-1),
                                                                                    (13,-1, 1,11),( 6,14,18,-1),( 5,-1, 2,13),(19, 8, 6,-1),
                                                                                    ( 3,-1, 5, 7),(15,10, 8,-1),( 4,-1, 7, 9),(16,-1,13,19),
                                                                                    (17, 5,15,-1),( 7,-1,16,18),( 9,17,19,-1),(18,15,11,-1));
var a,b,s : integer;
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

  //zeitunabhÃ¤ngige Bewegung:
  with data^, data^.gameinfo^ do
  begin
    case gamestep of
    0:
      if way<=0.0 then
      begin
        interact(data);
        if (joystick[momplayer].axis[0])^<0 then //links
        begin
          Mix_PlayChannel(-1,snd_hu,0);
          way:=1.0;
          for a:=0 to 15 do
          begin
            oldmatrix[a]:=matrix[a];
            newmatrix[a]:=matrix[a];
          end;
          if mom_site in [0,1,8,13,15,16,17,7,14,4] then
            rotate_matrix(newmatrix,0,1,0,pi*0.2)
          else
          if mom_site in [6,9,11,18,19] then
          begin
            rotate_matrix(newmatrix,1,0,0,pi*0.35);
            rotate_matrix(newmatrix,0,1,0,pi*0.4);
            rotate_matrix(newmatrix,1,0,0,pi*-0.35);
            //rotate_matrix(newmatrix,0,sin(0.35*pi),cos(0.35*pi),pi*0.4);
          end
          else
          begin
            rotate_matrix(newmatrix,1,0,0,pi*-0.35);
            rotate_matrix(newmatrix,0,1,0,pi*0.4);
            rotate_matrix(newmatrix,1,0,0,pi*0.35);
          end;
          old_site:=mom_site;
          mom_site:=next[mom_site][0];
        end
        else
        if (joystick[momplayer].axis[0])^>0 then //Rechts
        begin
          Mix_PlayChannel(-1,snd_hu,0);
          way:=1.0;
          for a:=0 to 15 do
          begin
            oldmatrix[a]:=matrix[a];
            newmatrix[a]:=matrix[a];
          end;
          if mom_site in [0,1,8,13,15,16,17,7,14,4] then
            rotate_matrix(newmatrix,0,1,0,pi*-0.2)
          else
          if mom_site in [6,9,11,18,19] then
          begin
            rotate_matrix(newmatrix,1,0,0,pi*0.35);
            rotate_matrix(newmatrix,0,1,0,pi*-0.4);
            rotate_matrix(newmatrix,1,0,0,pi*-0.35);
          end
          else
          begin
            rotate_matrix(newmatrix,1,0,0,pi*-0.35);
            rotate_matrix(newmatrix,0,1,0,pi*-0.4);
            rotate_matrix(newmatrix,1,0,0,pi*0.35);
          end;
          old_site:=mom_site;
          mom_site:=next[mom_site][2];
        end
        else
        if ((joystick[momplayer].axis[1])^<0) and (next[mom_site][1]<>-1) then //Oben
        begin
          Mix_PlayChannel(-1,snd_hu,0);
          way:=1.0;
          for a:=0 to 15 do
          begin
            oldmatrix[a]:=matrix[a];
            newmatrix[a]:=matrix[a];
          end;
          old_site:=mom_site;
          mom_site:=next[mom_site][1];
          rotate_matrix(newmatrix,1,0,0,pi*0.35);
        end
        else
        if ((joystick[momplayer].axis[1])^>0) and (next[mom_site][3]<>-1) then //Unten
        begin
          Mix_PlayChannel(-1,snd_hu,0);
          way:=1.0;
          for a:=0 to 15 do
          begin
            oldmatrix[a]:=matrix[a];
            newmatrix[a]:=matrix[a];
          end;
          old_site:=mom_site;
          mom_site:=next[mom_site][3];
          rotate_matrix(newmatrix,1,0,0,pi*-0.35);
        end
        else
        if ((joystick[momplayer].button[0])^>0) and not(open[mom_site]) then //(A)
        begin
          (joystick[momplayer].button[0])^:=0;
          inc(gamestep);
          way:=1.0;
        end;
      end;
    2:if ((joystick[momplayer].button[0])^>0) or (iscpu[momplayer]=1) then //(A)
      begin
        (joystick[momplayer].button[0])^:=0;
        if hasopen=2 then
          inc(gamestep);
{        else
        begin
          update_strategy(data);
          gamestep:=4;
        end;}
        way:=1.0;
      end;
    end;
    for s:=0 to steps^-1 do
    begin
      //zeitabhÃ¤ngige Berechnungen
      
      case gamestep of
      0://die Matrix
        if way>0.0 then
        begin
          way:=way-0.004;
          if way<=0.0 then
          begin
            way:=0.0;
            for a:=0 to 15 do
              matrix[a]:=newmatrix[a];
            old_site:=mom_site;
          end
          else
            for a:=0 to 15 do
              matrix[a]:=newmatrix[a]-(newmatrix[a]-oldmatrix[a])*way;
        end;
      1:begin 
          way:=way-0.002;
          if way<=0.0 then
          begin
            seen[mom_site]:=true;
            open[mom_site]:=true;
            inc(hasopen);
            if hasopen=1 then
              first_one:=mom_site
            else
            begin
              equal:=places[first_one]=places[mom_site];              
              if equal then
                Mix_PlayChannel(-1,snd_meow,0);
            end;
            way:=1.0;
            inc(gamestep);
            if hasopen=1 then
            begin
              gamestep:=4; //A drücken überspringen
              update_strategy(data);
            end;
          end;
        end;
      3:begin
          way:=way-0.002;
          if way<=0.0 then
          begin
            if not(equal) then
            begin
              open[first_one]:=false;
              open[mom_site]:=false;
            end
            else
              owner[places[first_one]]:=momplayer;
            way:=1.0;
            inc(gamestep);
            inc(momplayer);
            if momplayer>=gameinfo^.playernum then
              momplayer:=0;
            hasopen:=0;
            //SpielEnde überprüfen:
            a:=1;
            for b:=0 to 9 do
              if owner[b]=-1 then
                a:=0;
            if a<>1 then//Spiel nciht zu Ende
            find_strategy(data);
          end;
        end;
      4:begin 
          way:=way-0.002;
          if way<=0.0 then
          begin
            way:=0.0;
            gamestep:=0;
            //SpielEnde überprüfen:
            a:=1;
            for b:=0 to 9 do
              if owner[b]=-1 then
                a:=0;
            if a=1 then
            begin
              fade:=0.0;
              step:=2;
              Mix_FadeOutMusic(500);
            end;
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
const next : array [0..19,0..3] of shortint {0 links, 1 oben, 2 rechts, 3 unten} = (( 1,-1, 4, 6),( 8, 2, 0,-1),(10,-1, 3, 1),( 2,-1,12, 4),
                                                                                    ( 0, 3,14,-1),(12,-1,10,16),(11, 0, 9,-1),(14,12,17,-1),
                                                                                    (13,-1, 1,11),( 6,14,18,-1),( 5,-1, 2,13),(19, 8, 6,-1),
                                                                                    ( 3,-1, 5, 7),(15,10, 8,-1),( 4,-1, 7, 9),(16,-1,13,19),
                                                                                    (17, 5,15,-1),( 7,-1,16,18),( 9,17,19,-1),(18,15,11,-1));
const typ : array [0..19] of shortint = (2,5,1,0,4,
                                         2,3,3,2,3,
                                         2,5,0,3,0,
                                         2,3,0,4,5);
var keks:string;
    a : integer;
    pos : single;
    alpha,a1,a2,okalpha : single;
    normale : array [0..2] of single;
    matrix : array [0..15] of single;
    px,py : single;
begin
  glColor4f(1,1,1,1);
  glEnable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
    glColor4f(1,1,1,1);
    glTranslatef(0,0,-15);
    glPushMatrix;
      glTranslatef(0,1,0);
      //glRotatef(SDL_GetTicks/20,0.5,1,0.2);
      for a:=0 to 15 do
        matrix[a]:=data^.matrix[a];
      glMultMatrixf(matrix);
      glScalef(7,7,7);
      for a:=0 to 19 do
      begin
        if not(data^.open[a]) or (data^.open[a] and (data^.gamestep=3) and not(data^.equal) and ((a=data^.mom_site) or (a=data^.first_one))) then
        begin
          glBegin(GL_TRIANGLES);
          ZWnormaleberechnen(data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[0]],
                             data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[1]],
                             data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[2]],normale);
          glNormal3fv(normale);
          glColor3fv(@(data^.color[data^.ikosaeder.polohne[a].p[0] div 5]));
          glVertex3fv(@(data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[0]]));
          glColor3fv(@(data^.color[data^.ikosaeder.polohne[a].p[1] div 5]));
          glVertex3fv(@(data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[1]]));
          glColor3fv(@(data^.color[data^.ikosaeder.polohne[a].p[2] div 5]));
          glVertex3fv(@(data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[2]]));
          glEnd;
        end;
        if data^.open[a] or (not(data^.open[a]) and (data^.gamestep=1) and (data^.mom_site=a)) then
        begin
          glEnable(GL_TEXTURE_2D);
          ZWusetexture(data^.memory);
          glBegin(GL_TRIANGLES);
          if not(data^.open[a]) and (data^.gamestep=1) and (data^.mom_site=a) then
            glColor4f(1,1,1,1.0-data^.way)
          else
          if data^.open[a] and (data^.gamestep=3) and not(data^.equal) and ((a=data^.mom_site) or (a=data^.first_one)) then
            glColor4f(1,1,1,data^.way)
          else
            glColor4f(1,1,1,1);
          
          px:=(data^.places[a] mod 4)*0.25;
          py:=(data^.places[a] div 4)*0.25;

          ZWnormaleberechnen(data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[0]],
                             data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[1]],
                             data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[2]],normale);
          glNormal3fv(normale);
          case typ[a] of
            0: glTexCoord2f(px+0.00 ,py+0.22);
            1: glTexCoord2f(px+0.25 ,py+0.22);
            2: glTexCoord2f(px+0.125,py+0.00);
            3: glTexCoord2f(px+0.00 ,py+0.03);
            4: glTexCoord2f(px+0.125,py+0.25);
            5: glTexCoord2f(px+0.25 ,py+0.03);
          end;
          glVertex3fv(@(data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[0]]));
          case typ[a] of
            2: glTexCoord2f(px+0.00 ,py+0.22);
            0: glTexCoord2f(px+0.25 ,py+0.22);
            1: glTexCoord2f(px+0.125,py+0.00);
            5: glTexCoord2f(px+0.00 ,py+0.03);
            3: glTexCoord2f(px+0.125,py+0.25);
            4: glTexCoord2f(px+0.25 ,py+0.03);
          end;
          glVertex3fv(@(data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[1]]));
          case typ[a] of
            1: glTexCoord2f(px+0.00 ,py+0.22);
            2: glTexCoord2f(px+0.25 ,py+0.22);
            0: glTexCoord2f(px+0.125,py+0.00);
            4: glTexCoord2f(px+0.00 ,py+0.03);
            5: glTexCoord2f(px+0.125,py+0.25);
            3: glTexCoord2f(px+0.25 ,py+0.03);
          end;
          glVertex3fv(@(data^.ikosaeder.p[data^.ikosaeder.polohne[a].p[2]]));
          
          glEnd;
          glDisable(GL_TEXTURE_2D);
        end;
      end;
    glPopMatrix;
  glEnable(GL_TEXTURE_2D);
  
  for a:=0 to data^.gameinfo^.playernum-1 do
  begin
    glPushMatrix;
      glTranslatef(0,-2,0);
      glRotatef(-15,1,0,0);
      if data^.gamestep=3 then //player change
        pos:=(1.0-data^.way-a+data^.momplayer)*2*pi/data^.gameinfo^.playernum
      else
        pos:=(data^.momplayer-a)*2*pi/data^.gameinfo^.playernum;
      glTranslatef(sin(pos)*7,-3,cos(pos)*7);
      glColor4f(data^.gameinfo^.color[a].r,data^.gameinfo^.color[a].g,data^.gameinfo^.color[a].b,1);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
      ZWusetexture(data^.gameinfo^.facetex);
      glCallList(data^.gameinfo^.ball[a]);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPopMatrix;
  end;
  

  case data^.gamestep of
    0: alpha:=1.0; //normal
    1: alpha:=data^.way; //Fade out
    2: alpha:=0.0; //wait
    3: alpha:=0.0; //Player Change
    4: alpha:=1.0-data^.way; //Fade in
  end;

  case data^.gamestep of
    0: okalpha:=0.0; //normal
    1: if data^.hasopen=2 then
         okalpha:=1.0-data^.way //Fade out
       else
         okalpha:=0.0;
    2: if data^.hasopen=2 then
         okalpha:=1.0 //wait
       else
         okalpha:=0.0;
    3: okalpha:=data^.way; //Player Change
    4: okalpha:=0.0; //Fade in
  end;
  
  with data^ do
  begin
    if (next[mom_site,1]=-1) and (next[old_site,1]=-1) then      
      a1:=0.0;
    if (next[mom_site,1]<>-1) and (next[old_site,1]=-1) then      
      a1:=1.0-way;
    if (next[mom_site,1]=-1) and (next[old_site,1]<>-1) then      
      a1:=way;
    if (next[mom_site,1]<>-1) and (next[old_site,1]<>-1) then      
      a1:=1.0;      

    if (next[mom_site,3]=-1) and (next[old_site,3]=-1) then      
      a2:=0.0;
    if (next[mom_site,3]<>-1) and (next[old_site,3]=-1) then      
      a2:=1.0-way;
    if (next[mom_site,3]=-1) and (next[old_site,3]<>-1) then      
      a2:=way;
    if (next[mom_site,3]<>-1) and (next[old_site,3]<>-1) then      
      a2:=1.0;      
  end;
  
  
  glDisable(GL_TEXTURE_2D);
    glTranslatef(0,1.1,6.2);
    glColor4f(1.0,1.0,1.0,0.7*alpha);
    glPushMatrix();
      glTranslatef(-2.4,0,0);
      glRotatef(90,0,1,0);
      glRotatef(90,0,0,1);
      ZWdrawobject(data^.arrow);
    glPopMatrix();
    glColor4f(1.0,1.0,1.0,0.7*alpha);
    glPushMatrix();
      glTranslatef(2.4,0,0);
      glRotatef(90,0,-1,0);
      glRotatef(90,0,0,1);
      ZWdrawobject(data^.arrow);
    glPopMatrix();
    glColor4f(1.0,1.0,1.0,0.7*alpha*a1);
    glPushMatrix();
      glTranslatef(0,2.4,0);
      glRotatef(90,1,0,0);
      ZWdrawobject(data^.arrow);
    glPopMatrix();
    glColor4f(1.0,1.0,1.0,0.7*alpha*a2);    
    glPushMatrix();
      glTranslatef(0,-2.4,0);
      glRotatef(90,-1,0,0);
      ZWdrawobject(data^.arrow);
    glPopMatrix();
  glEnable(GL_TEXTURE_2D);
  
  glDisable(GL_LIGHTING);

  glDepthFunc(GL_ALWAYS);  

  glColor4f(1,1,1,0.7*alpha);
  ZWdrawtextmiddle(ZWtext(data^.text),0,0.5,0,'[A]',1.2);
  ZWdrawtextmiddle(ZWtext(data^.text),0,-0.5,0,'Show',0.8);

  glColor4f(1,1,1,0.7*okalpha);
  ZWdrawtextmiddle(ZWtext(data^.text),0,-3.4,0,'[A] Ok',0.8);


  glLoadIdentity();

  //Rechts und Links die fertigen Teile
  for a:=0 to 9 do
  if (data^.owner[a]>=0) or ((data^.owner[a]<0) and (data^.gamestep=3) and (data^.equal) and (a=data^.places[data^.first_one])) then
  begin
    if a<5 then
      px:=-3.1
    else
      px:=3.1;
    py:=2-(a mod 5);
    glDisable(GL_TEXTURE_2D);
    if (data^.owner[a]<0) and (data^.gamestep=3) and (data^.equal) and (a=data^.places[data^.first_one]) then
      glColor4f(data^.gameinfo^.color[data^.momplayer].r,
                data^.gameinfo^.color[data^.momplayer].g,
                data^.gameinfo^.color[data^.momplayer].b,1.0-data^.way)    
    else
      glColor4f(data^.gameinfo^.color[data^.owner[a]].r,
                data^.gameinfo^.color[data^.owner[a]].g,
                data^.gameinfo^.color[data^.owner[a]].b,1);
    glBegin(GL_QUADS);
      glVertex3f(px-0.45,py+0.45,-5);
      glVertex3f(px-0.45,py-0.45,-5);
      glVertex3f(px+0.45,py-0.45,-5);
      glVertex3f(px+0.45,py+0.45,-5);      
    glEnd;
    glEnable(GL_TEXTURE_2D);
    if (data^.owner[a]<0) and (data^.gamestep=3) and (data^.equal) and (a=data^.places[data^.first_one]) then
      glColor4f(1,1,1,1.0-data^.way)
    else
      glColor4f(1,1,1,1);
    ZWdrawsprite(data^.memory,px,py,-5,0.3,0.3,4,a);
  end;

  //Countdown
  if data^.countdown>0 then
	begin
	  keks:=inttostr(Trunc(data^.countdown/1000+1));
    case data^.countdown div 1000+1 of
      1: glColor4f(0.5,1.0,0.5,0.9);
      2: glColor4f(1.0,1.0,0.5,0.9);
    else glColor4f(1.0,0.5,0.5,0.9);
    end;
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
var b,max : integer;
    points : array [0..7] of byte;
begin
  max:=1;
  for b:=0 to data^.gameinfo^.playernum-1 do
    points[b]:=0;
  for b:=0 to 9 do
    if data^.owner[b]>=0 then
    begin
      inc(points[data^.owner[b]]);
      if max<points[data^.owner[b]] then
        max:=points[data^.owner[b]];
    end;
  
  for b:=0 to data^.gameinfo^.playernum-1 do
    if points[b]=max then
      gameresult^.winner[b]:=1
    else
      gameresult^.winner[b]:=0;
    
  //freigeben:
  ZWdeletetexture(data^.memory);
  ZWdeleteobject(data^.ikosaeder);
  ZWdeleteobject(data^.arrow);
  
  ZWdisablelight(0);
  
  glClearColor(0,0,0,0);
    
  //Musik zurÃ¼cksetzen:
  Mix_VolumeMusic(data^.old_music_volume);
  Mix_FreeMusic(data^.backgroundmusic);
  Mix_FreeChunk(data^.snd_meow);
  Mix_FreeChunk(data^.snd_hu);
end;

exports op_get_gameinfos, op_get_gamedata_size, op_init_game, op_calc_game, op_calc_game_thread, op_draw_game, op_reset_game;

begin
  SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide,exOverflow, exUnderflow, exPrecision]);
end.
