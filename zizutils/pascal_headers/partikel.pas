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
unit partikel;

interface

uses sdl,dglOpenGL,zizwindow;

const
{$IFDEF WINDOWS}
  LibName =  'libzizopenparty.dll';
  {$CALLING cdecl}
{$ENDIF}
{$IFDEF UNIX}
	LibName = 'libzizopenparty.so';
  {$CALLING cdecl}
{$ENDIF}

{$IFDEF FPC}
{$PACKRECORDS C}
{$ENDIF}

  type

     pZWpartikel = ^uZWpartikel;

     pZWfloatint = ^uZWfloatint;

     uZWfloatint = record
         case longint of
            0 : ( i : longint );
            1 : ( f : single );
         end;
     tZWfloatint = uZWfloatint;

     pZWpoint = ^tZWpoint;

     tZWpoint = record
          x : single;
          y : single;
          z : single;
       end;

     pZWsprite = ^tZWsprite;

     tZWsprite = record
          _type : GLbyte;
          position : tZWpoint;
          rotate : GLbyte;
          rotation : tZWpoint;
          scale : tZWpoint;
          calc_partikel : function (partikel:pZWpartikel; n:longint):longint;
          alpha : single;
          live : single;
          value : pZWfloatint;
          cameradestination : single;
          nr : longint;
          texture : GLuint;
          cutcount : GLuint;
          spritenr : GLuint;
          color : tZWcolor;
       end;

     pZWmodell = ^tZWmodell;

		 TPartikelFunction=function(partikel:pZWpartikel; n:longint):longint;
		 TPartikelDraw=procedure(partikel:pZWpartikel);


     tZWmodell = record
          _type : GLbyte;
          position : tZWpoint;
          rotate : GLbyte;
          rotation : tZWpoint;
          scale : tZWpoint;
          calc_partikel : TPartikelFunction;
          alpha : single;
          live : single;
          value : pZWfloatint;
          cameradestination : single;
          nr : longint;
          draw_partikel : TPartikelDraw;
       end;

     pZWcommon = ^tZWcommon;

     tZWcommon = record
          _type : GLbyte;
          position : tZWpoint;
          rotate : GLbyte;
          rotation : tZWpoint;
          scale : tZWpoint;
          calc_partikel : TPartikelFunction;
          alpha : single;
          live : single;
          value : pZWfloatint;
          cameradestination : single;
          nr : longint;
       end;

     uZWpartikel = record
         case longint of
            0 : ( _type : GLbyte );
            1 : ( common : tZWcommon );
            2 : ( sprite : tZWsprite );
            3 : ( modell : tZWmodell );
         end;
     tZWpartikel = uZWpartikel;

procedure ZWnewspritepartikel(position:tZWpoint; rotation:tZWpoint; scale:tZWpoint; rotate:GLbyte; calc_partikel:TPartikelFunction; 
						live:single; value:pZWfloatint; texture:GLuint; cutcount:GLuint; spritenr:GLuint; 
						color:tZWcolor);
	 external LibName; {$EXTERNALSYM ZWnewspritepartikel}
procedure ZWnewmodellpartikel(position:tZWpoint; rotation:tZWpoint; scale:tZWpoint; rotate:GLbyte;calc_partikel:TPartikelFunction; 
						live:single; value:pZWfloatint; draw_partikel:TPartikelDraw);
	 external LibName; {$EXTERNALSYM ZWnewmodellpartikel}
procedure ZWdrawpartikel(cameraposition:tZWpoint; camerarotation:tZWpoint);
	 external LibName; {$EXTERNALSYM ZWdrawpartikel}
procedure ZWcalcpartikel(n:longint);
	 external LibName; {$EXTERNALSYM ZWcalcpartikel}
procedure ZWnewpartikelexplosion(position:tZWpoint; partikelanz:longint; power:single; texture:GLuint; cutcount:longint; 
						partikelnr:longint; size:single);
	 external LibName; {$EXTERNALSYM ZWnewpartikelexplosion}
procedure ZWresetpartikel;
	 external LibName; {$EXTERNALSYM ZWresetpartikel}
	
implementation

end.
