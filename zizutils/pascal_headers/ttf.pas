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
unit ttf;

interface

uses sdl,dglOpenGL,texture;

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
{$IFDEF FPC}
{$PACKRECORDS C}
{$ENDIF}

  type

     pZWtext = ^ZWtext;
  {GLuint vbo; }

     ZWtext = record
          texture : GLuint;
          breite : array[0..255] of single;
       end;

procedure ZWinitfont;
	 external LibName; {$EXTERNALSYM ZWinitfont}
function ZWcreatefonttexture(fontname:pchar; res:longint; r:GLbyte; g:GLbyte; b:GLbyte):ZWtext;
	 external LibName; {$EXTERNALSYM ZWcreatefonttexture}
procedure ZWdrawtext(t:ZWtext; x:single; y:single; z:single; text:pchar; 
						scale:single);
	 external LibName; {$EXTERNALSYM ZWdrawtext}
procedure ZWdrawtextmiddle(t:ZWtext; x:single; y:single; z:single; text:pchar; 
						scale:single);
	 external LibName; {$EXTERNALSYM ZWdrawtextmiddle}
procedure ZWdrawtextright(t:ZWtext; x:single; y:single; z:single; text:pchar; 
						scale:single);
	 external LibName; {$EXTERNALSYM ZWdrawtextright}
procedure ZWdrawtext_fac(t:ZWtext; x:single; y:single; z:single; text:pchar; 
						scale:single; faktor:single);
	 external LibName; {$EXTERNALSYM ZWdrawtext_fac}
procedure ZWdrawtextmiddle_fac(t:ZWtext; x:single; y:single; z:single; text:pchar; 
						scale:single; faktor:single);
	 external LibName; {$EXTERNALSYM ZWdrawtextmiddle_fac}
procedure ZWdrawtextright_fac(t:ZWtext; x:single; y:single; z:single; text:pchar; 
						scale:single; faktor:single);
	 external LibName; {$EXTERNALSYM ZWdrawtextright_fac}
procedure ZWdrawtext_shadow(t:ZWtext; x:single; y:single; z:single; text:pchar; 
						scale:single; faktor:single; r:single; g:single; b:single; 
						sr:single; sg:single; sb:single; sa:single);
	 external LibName; {$EXTERNALSYM ZWdrawtext_shadow}
procedure ZWdrawtextright_shadow(t:ZWtext; x:single; y:single; z:single; text:pchar; 
						scale:single; faktor:single; r:single; g:single; b:single; 
						sr:single; sg:single; sb:single; sa:single);
	 external LibName; {$EXTERNALSYM ZWdrawtextright_shadow}
procedure ZWdrawtextmiddle_shadow(t:ZWtext; x:single; y:single; z:single; text:pchar; 
						scale:single; faktor:single; r:single; g:single; b:single; 
						sr:single; sg:single; sb:single; sa:single);
	 external LibName; {$EXTERNALSYM ZWdrawtextmiddle_shadow}
procedure ZWdeletefonttexture(texture:ZWtext);
	 external LibName; {$EXTERNALSYM ZWdeletefonttexture}
procedure ZWquitfont;
	 external LibName; {$EXTERNALSYM ZWquitfont}

implementation

end.
