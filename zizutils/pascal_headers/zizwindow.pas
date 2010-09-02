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
unit zizwindow;

interface

uses sdl,dglOpenGL;

{$IFDEF FPC}
{$PACKRECORDS C}
{$ENDIF}

const
{$IFDEF WINDOWS}
  LibName =  'libzizopenparty.dll';
  {$CALLING cdecl}
{$ENDIF}
{$IFDEF UNIX}
	LibName = 'libzizopenparty.so';
  {$CALLING cdecl}
{$ENDIF}

    Type
    Pchar  = ^char;
    PSDL_keysym  = ^TSDL_keysym;

{$IFDEF FPC}
{$PACKRECORDS C}
{$ENDIF}

  const
     lnone = 0;     
     lbronze = 100;     
     lbronzep = 101;     
     lchrom = 102;     
     lgold = 103;     
     lgoldp = 104;     
     lkupfer = 105;     
     lkupferp = 106;     
     lmessing = 107;     
     lsilber = 108;     
     lsilberp = 109;     
     lzinn = 110;     
     lgummi = 111;     
     lplastik = 112;     
     ljade = 113;     
     lobsidian = 114;     
     lperle = 115;     
     lrubin = 116;     
     lsmaragd = 117;     
     ltuerkis = 118;     

  type

     pZWcolor = ^tZWcolor;

     tZWcolor = record
          r : single;
          g : single;
          b : single;
          a : single;
       end;

     pZWrealjoystick = ^tZWrealjoystick;
  {0 not mapped, 1 right, -1 left }

     tZWrealjoystick = record
          joy : ^TSDL_Joystick;
          buttoncount : Uint8;
          only_axiscount : Uint8;
          hatcount : Uint8;
          axiscount : Uint8;
          button : ^Uint8;
          button_mapped_direction : ^shortint;
          button_mapped_axis : ^Uint8;
          axis : ^Sint16;
          name : ^char;
          axis_free : ^shortint;
       end;

     pZWjoystick = ^tZWjoystick;

     tZWjoystick = record
          button : array[0..3] of ^Uint8;
          axis : array[0..1] of ^Sint16;
          nullbutton : array[0..3] of Uint8;
          nullaxis : array[0..1] of Sint16;
       end;

     pZWresolution = ^tZWresolution;

     tZWresolution = record
          x : longint;
          y : longint;
       end;

     pZWnewaxis = ^tZWnewaxis;

     tZWnewaxis = record
          name : ^char;
          joynr : longint;
          axisnr : longint;
          left : longint;
          right : longint;
          next : pZWnewaxis;
       end;

     pZWnewkeyboard = ^tZWnewkeyboard;
  {left,up,right,down ABCD }

     tZWnewkeyboard = record
          name : ^char;
          joynr : longint;
          key : array[0..7] of longint;
          next : pZWnewkeyboard;
       end;

     pZWattribute = ^tZWattribute;
  {-1 nein, sonst Joysticknr; }
  {0..3 Buttons, 4..7 Achsen }

     tZWattribute = record
          strech : longint;
          on_resize : procedure; 
          surface : ^TSDL_Surface;
          info : ^TSDL_VideoInfo;
          width : longint;
          height : longint;
          bpp : longint;
          sdlflags : longint;
          initflags : longint;
          glnear : single;
          glfar : single;
          keypressed : array[0..511] of shortint;
          key_mapped : array[0..511] of Sint8;
          key_dest : array[0..511] of Sint8;
          unicode : array[0..65535] of smallint;
          perspektive : single;
          steps : Uint32;
          fps : longint;
          realjoystickcount : longint;
          realjoystick : pZWrealjoystick;
          joystick : array[0..7] of tZWjoystick;
          quit_function : procedure ; cdecl;
          minwait : longint;
          newticks : Uint32;
          oldticks : Uint32;
          resolution : pZWresolution;
          resolutioncount : longint;
          keyboardtexture : GLuint;
          keyboardtexture_exists : shortint;
          keyboardtexture_position : shortint;
          keyboard_joystick_num : longint;
          pausescreen : Shortint;
          pausestexture : GLuint;
          pausetexture_exists : shortint;
       end;

     pZWfrustum = ^tZWfrustum;

     tZWfrustum = record
          frustum : array[0..5] of array[0..3] of single;
       end;
			 
TDrawFunction=procedure(data:pointer); cdecl;		 
TCalcFunction=function(data:pointer):longint; cdecl;

procedure ZWinitparameter;
   external LibName; {$EXTERNALSYM ZWinitparameter}
procedure ZWinitsdl;
   external LibName; {$EXTERNALSYM ZWinitsdl}
procedure  ZWinitopengl;
   external LibName; {$EXTERNALSYM ZWinitopengl}
procedure ZWquitsdl;
   external LibName; {$EXTERNALSYM ZWinitparameter}
function ZWschleife(draw_scene:TDrawFunction; calc_proc:TCalcFunction; data:pointer; size:longint):longint;
   external LibName; {$EXTERNALSYM ZWschleife}
function ZWschleifeex(draw_scene:TDrawFunction; calc_proc:TCalcFunction; calc_procex:TCalcFunction; data:pointer; size:longint):longint;
   external LibName; {$EXTERNALSYM ZWschleifeex}
procedure ZWprocess_events;
   external LibName; {$EXTERNALSYM ZWprocess_events}
procedure ZWenablefog_near(near_ : single;far_ : single);
   external LibName; {$EXTERNALSYM ZWenablefog_near}
procedure ZWenablefog_ext(d : single);
   external LibName; {$EXTERNALSYM ZWenablefog_ext}
procedure ZWenablefog_ext2(d : single);
   external LibName; {$EXTERNALSYM ZWenablefog_ext2}
procedure ZWdisablefog;
   external LibName; {$EXTERNALSYM ZWdisablefog}
procedure ZWsetfogcolor(r,g,b : single);
   external LibName; {$EXTERNALSYM ZWsetfogcolor}
procedure ZWsetmaterialambient(r,g,b,a : single);
   external LibName; {$EXTERNALSYM ZWsetmaterialambient}
procedure ZWsetmaterialspecular(r,g,b,a : single);
   external LibName; {$EXTERNALSYM ZWsetmaterialspecular}
procedure ZWsetmaterialdiffuse(r,g,b,a : single);
   external LibName; {$EXTERNALSYM ZWsetmaterialdiffuse}
procedure ZWsetmaterialshininess(s : single);
   external LibName; {$EXTERNALSYM ZWsetmaterialshininess}
procedure ZWsetmaterial(material : longint);
   external LibName; {$EXTERNALSYM ZWsetmaterial}
procedure ZWsetlightambient(licht : longint;r,g,b : single);
   external LibName; {$EXTERNALSYM ZWsetlightambient}
procedure ZWsetlightdiffuse(licht : longint;r,g,b : single);
   external LibName; {$EXTERNALSYM ZWsetlightdiffuse}
procedure ZWsetlightposition(licht : longint;x,y,z : single);
   external LibName; {$EXTERNALSYM ZWsetlightposition}
procedure ZWenableculling;
   external LibName; {$EXTERNALSYM ZWenableculling}
procedure ZWdisableculling;
   external LibName; {$EXTERNALSYM ZWdisableculling}
procedure ZWdisablelight(licht : longint);
   external LibName; {$EXTERNALSYM ZWdisablelight}
procedure ZWenablelight(licht : longint);
   external LibName; {$EXTERNALSYM ZWenablelight}
procedure ZWclearall;
   external LibName; {$EXTERNALSYM ZWclearall}
procedure ZWflipbuffer;
   external LibName; {$EXTERNALSYM ZWflipbuffer}
procedure ZWhandle_key_down( keysym: PSDL_keysym);
   external LibName; {$EXTERNALSYM ZWhandle_key_down}
procedure ZWhandle_key_up( keysym: PSDL_keysym);
   external LibName; {$EXTERNALSYM ZWhandle_key_up}
procedure ZWglResizeWindow;
   external LibName; {$EXTERNALSYM ZWglResizeWindow}
function ZWgetattribute : PZWattribute;
   external LibName; {$EXTERNALSYM ZWgetattribute}
procedure ZWNormalizePlane(frustum : pZWfrustum;pPlane : longint);
   external LibName; {$EXTERNALSYM ZWNormalizePlane}
procedure ZWfrustum_calculate(frustum:pZWfrustum);
   external LibName; {$EXTERNALSYM ZWfrustum_calculate}
function ZWfrustum_isboxwithin(frustum : pZWfrustum;pX,pY,pZ,pB,pH,pT : single):longint;
   external LibName; {$EXTERNALSYM ZWfrustum_isboxwithin}
function ZWfileexists(filename:PChar):longint;
   external LibName; {$EXTERNALSYM ZWfileexists}
procedure ZWreset_virtuell_joytick;
   external LibName; {$EXTERNALSYM ZWreset_virtuell_joytick}
procedure ZWnewaxis(name:pchar;left,right : longint);
   external LibName; {$EXTERNALSYM ZWnewaxis}
procedure ZWnewkeyboard(name,b1,b2,b3,b4,a1l,a1r,a2l,a2r : PChar);
   external LibName; {$EXTERNALSYM ZWnewkeyboard}

implementation

end.
