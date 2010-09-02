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
unit minigames;

interface

uses sdl,dglOpenGL;

{$IFDEF FPC}
{$PACKRECORDS C}
{$ENDIF}

  type

     pOPjoystick = ^tOPjoystick;

     tOPjoystick = record
          button : array[0..3] of ^Uint8;
          axis : array[0..1] of ^Sint16;
       end;

     pOPcolor = ^tOPcolor;

     tOPcolor = record
          r : single;
          g : single;
          b : single;
          a : single;
       end;

     pgameinfo = ^tgameinfo;

     tgameinfo = record
          playernum : longint;
          ball : array[0..7] of GLuint;
          color : array[0..7] of tOPcolor;
          money : array[0..7] of longint;
          rubins : array[0..7] of longint;
          facenr : array[0..7] of longint;
          teamsize : longint;
          team : array[0..7] of longint;
          iscpu : array[0..7] of shortint;
          steps : ^Uint32;
          maintex : GLuint;
          facetex : GLuint;
          shouldend : ^longint;
          texttexture : GLuint;
          textwidth : ^single;
          texquali : longint;
          fontquali : longint;
          lightquali : longint;
          windowx : longint;
          windowy : longint;
          resolutionx : longint;
          resolutiony : longint;
          fullscreen : longint;
          stretched : longint;
          volume : single;
          joystick : array[0..7] of tOPjoystick;
          sky_menu_tex : GLuint;
          datafolder : ^char;
       end;

     pminigameinfo = ^tminigameinfo;

     tminigameinfo = record
          playable : array[0..7] of array[0..3] of shortint;
       end;

     pgameresult = ^tgameresult;

     tgameresult = record
          winner : array[0..7] of shortint;
       end;

implementation

end.
