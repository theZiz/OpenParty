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
unit xml;

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

     pxmlfile = ^txmlfile;

     txmlfile = record
          file_ : ^SDL_RWops;
          name : ^char;
          ebene : longint;
          last : shortint;
       end;

     pxmlpoint = ^txmlpoint;

     txmlpoint = record
          xml : ^char;
          pos : longint;
          len : longint;
       end;

     pxmlattribute = ^txmlattribute;

     txmlattribute = record
          name : ^char;
          wert : ^char;
          next : pxmlattribute;
       end;

     pxmltag = ^txmltag;

     txmltag = record
          name : ^char;
          inhalt : pxmlpoint;
          attribute : pxmlattribute;
       end;

function ZWnewxmlfile(filename:pchar; name:pchar):pxmlfile;
	 external LibName; {$EXTERNALSYM ZWnewxmlfile}
procedure ZWfinishxmlfile(xmlfile:pxmlfile);
	 external LibName; {$EXTERNALSYM ZWfinishxmlfile}
procedure ZWbegintag(xmlfile:pxmlfile; tag:pchar);
	 external LibName; {$EXTERNALSYM ZWbegintag}
procedure ZWaddattribute_string(xmlfile:pxmlfile; name:pchar; wert:pchar);
	 external LibName; {$EXTERNALSYM ZWaddattribute_string}
procedure ZWaddattribute_number(xmlfile:pxmlfile; name:pchar; wert:longint);
	 external LibName; {$EXTERNALSYM ZWaddattribute_number}
procedure ZWaddattribute_float(xmlfile:pxmlfile; name:pchar; wert:single);
	 external LibName; {$EXTERNALSYM ZWaddattribute_float}
procedure ZWendnormaltag(xmlfile:pxmlfile);
	 external LibName; {$EXTERNALSYM ZWendnormaltag}
procedure ZWendstandalonetag(xmlfile:pxmlfile);
	 external LibName; {$EXTERNALSYM ZWendstandalonetag}
procedure ZWclosetag(xmlfile:pxmlfile; tag:pchar);
	 external LibName; {$EXTERNALSYM ZWclosetag}
function ZWloadxmlfile(filename:pchar):pxmlpoint;
	 external LibName; {$EXTERNALSYM ZWloadxmlfile}
function ZWgetnexttag(xmlpoint:pxmlpoint):pxmltag;
	 external LibName; {$EXTERNALSYM ZWgetnexttag}
procedure ZWfinishloading(xmlpoint:pxmlpoint);
	 external LibName; {$EXTERNALSYM ZWfinishloading}
procedure ZWfreetag(xmltag:pxmltag);
	 external LibName; {$EXTERNALSYM ZWfreetag}
procedure ZWaddplaintext(xmlfile:pxmlfile; text:pchar);
	 external LibName; {$EXTERNALSYM ZWaddplaintext}
procedure ZWclosetagonsameline(xmlfile:pxmlfile; tag:pchar);
	 external LibName; {$EXTERNALSYM ZWclosetagonsameline}
procedure ZWendnormaltagwithoutbreak(xmlfile:pxmlfile);
	 external LibName; {$EXTERNALSYM ZWendnormaltagwithoutbreak}
	
implementation

end.
