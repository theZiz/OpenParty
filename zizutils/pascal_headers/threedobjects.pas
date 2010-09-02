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
unit threedobjects;

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

     point = record
          x : single;
          y : single;
          z : single;
          uv : char;
          u : single;
          v : single;
       end;

     tex = record
          x : single;
          y : single;
       end;

     pmultigon = ^multigon;

     multigon = record
          panz : word;
          p : ^word;
       end;

     object3d = record
          text : GLuint;
          pointnum : word;
          p : ^point;
          polygonnum : word;
          polygonnumohne : word;
          polygonnummit : word;
          polohne : pmultigon;
          polmit : pmultigon;
          listnr : GLuint;
       end;
			 Pobject3d  = ^object3d;

function ZWnewobject:object3d;
	 external LibName; {$EXTERNALSYM ZWnewobject}
procedure ZWdeleteobject(object_:object3d);
	 external LibName; {$EXTERNALSYM ZWdeleteobject}
procedure ZWdeletedrawlist(object_:object3d);
	 external LibName; {$EXTERNALSYM ZWdeletedrawlist}
procedure ZWloadfromrwx(object_:pobject3d; name:pchar);
	 external LibName; {$EXTERNALSYM ZWloadfromrwx}
procedure ZWcreatedrawlist(object_:pobject3d; betterlight:shortint);
	 external LibName; {$EXTERNALSYM ZWcreatedrawlist}
procedure ZWdrawobject(object_:object3d);
	 external LibName; {$EXTERNALSYM ZWdrawobject}
procedure ZWsave3dm(object_:object3d; name:pchar);
	 external LibName; {$EXTERNALSYM ZWsave3dm}
procedure ZWload3dm(object_:pobject3d; name:pchar; text:GLuint);
	 external LibName; {$EXTERNALSYM ZWload3dm}
procedure ZWrefreshdrawlist(object_:pobject3d; betterlight:shortint);
	 external LibName; {$EXTERNALSYM ZWrefreshdrawlist}
procedure ZWnormaleberechnen(p1,p2,p3 : point; normale : psingle);
	 external LibName; {$EXTERNALSYM ZWnormaleberechnen}

implementation

end.
