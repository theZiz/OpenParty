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
unit texture;

interface


uses sdl,dglOpenGL;

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

     pZWtexturename = ^tZWtexturename;
     tZWtexturename = record
          filename : array[0..1023] of char;
          texture : GLuint;
          alpha : longint;
          better : longint;
          refcount : longint;
          next : pZWtexturename;
       end;
function ZWloadtexturefromfile(name:pchar; better:longint; reduce:longint):GLuint;
	 external LibName; {$EXTERNALSYM ZWloadtexturefromfile}
function ZWloadtexturefromfileA(name:pchar; better:longint; reduce:longint):GLuint;
	 external LibName; {$EXTERNALSYM ZWloadtexturefromfileA}
procedure ZWdeletetexture(texnr:GLuint);
	 external LibName; {$EXTERNALSYM ZWdeletetexture}
procedure ZWusetexture(texnr:GLuint);
	 external LibName; {$EXTERNALSYM ZWusetexture}
procedure ZWdrawsprite(texnr:GLuint; x:single; y:single; z:single; w:single; 
						h:single; anz:longint; nr:longint);
	 external LibName; {$EXTERNALSYM ZWdrawsprite}
procedure ZWreloadalltextures(reduce:longint);
	 external LibName; {$EXTERNALSYM ZWreloadalltextures}
procedure ZWdrawspritevmirror(texnr:GLuint; x:single; y:single; z:single; w:single; 
						h:single; anz:longint; nr:longint);
	 external LibName; {$EXTERNALSYM ZWdrawspritevhmirror}
procedure ZWdrawspritehmirror(texnr:GLuint; x:single; y:single; z:single; w:single; 
						h:single; anz:longint; nr:longint);
	 external LibName; {$EXTERNALSYM ZWdrawspritehmirror}
procedure ZWdrawspritevhmirror(texnr:GLuint; x:single; y:single; z:single; w:single; 
						h:single; anz:longint; nr:longint);
	 external LibName; {$EXTERNALSYM ZWdrawspritevhmirror}
procedure ZWdrawspritealpha(texnr:GLuint; x:single; y:single; z:single; w:single; 
						h:single; anz:longint; nr:longint; alpha:single);
	 external LibName; {$EXTERNALSYM ZWdrawspritealpha}
procedure ZWsettexturepercent(p:single);
	 external LibName; {$EXTERNALSYM ZWsettexturepercent}
function ZWgettexturepercent:single;
	 external LibName; {$EXTERNALSYM ZWgettexturepercent}
procedure ZWsettextureresolution(texnr:GLuint; w:longint; h:longint);
	 external LibName; {$EXTERNALSYM ZWsettextureresolution}
procedure ZWsetstdreduce(i:longint);
	 external LibName; {$EXTERNALSYM ZWsetstdreduce}
function ZWgetstdreduce:longint;
	 external LibName; {$EXTERNALSYM ZWgetstdreduce}
function ZWgetmomtexture:GLuint;
	 external LibName; {$EXTERNALSYM ZWgetmomtexture}
procedure ZWsetmomtexture(momtexnr : GLuint);
	 external LibName; {$EXTERNALSYM ZWsetmomtexture}

implementation

end.
