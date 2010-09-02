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
program test;
uses dglOpenGL,zizwindow;

{$IFDEF WINDOWS}
  {$CALLING stdcall}
{$ENDIF}
{$IFDEF UNIX}
  {$CALLING cdecl}
{$ENDIF}

var ZWattribute : PZWattribute;
		the_end : boolean=false;

procedure quit_function;
begin
  the_end:=true;
end;

procedure draw(dummy : pointer);
begin
  glTranslatef(0,0,-2.5);
  glBegin(GL_QUADS);
	  glColor4f(1,0,0,1); glVertex3f(-1, 1,0);
	  glColor4f(1,1,0,1); glVertex3f(-1,-1,0);
	  glColor4f(0,1,0,1); glVertex3f( 1,-1,0);
	  glColor4f(0,0,1,1); glVertex3f( 1, 1,0);	
	glEnd();
end;

function calc(dummy : pointer):longint;
begin
  if the_end then
	  calc:=1
	else
	  calc:=0;
end; 


begin
    ZWattribute:=ZWgetattribute;
    ZWinitparameter;
		ZWattribute^.quit_function:=@quit_function;
    ZWinitsdl;
		ZWinitopengl;
		InitOpenGL;
		ReadExtensions;
    glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
		ZWschleife(@draw,@calc,nil,0);
    ZWquitsdl;
end.
