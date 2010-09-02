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

function get_direction(from : integer;dest : integer) : integer;
const next : array [0..19,0..3] of shortint {0 links, 1 oben, 2 rechts, 3 unten} = (( 1,-1, 4, 6),( 8, 2, 0,-1),(10,-1, 3, 1),( 2,-1,12, 4),
                                                                                    ( 0, 3,14,-1),(12,-1,10,16),(11, 0, 9,-1),(14,12,17,-1),
                                                                                    (13,-1, 1,11),( 6,14,18,-1),( 5,-1, 2,13),(19, 8, 6,-1),
                                                                                    ( 3,-1, 5, 7),(15,10, 8,-1),( 4,-1, 7, 9),(16,-1,13,19),
                                                                                    (17, 5,15,-1),( 7,-1,16,18),( 9,17,19,-1),(18,15,11,-1));
var stack : array [0..19] of record
                               father : integer;
                               father_used_dir : integer;
                               used : boolean;
                             end; 
    a,b : integer;
    f : boolean;
begin
  if from=dest then
    exit(-1);
  for a:=0 to 19 do
  begin
    stack[a].father:=-1;
    stack[a].used:=false;
  end;
  stack[from].used:=true;
  f:=false;
  repeat
    for a:=0 to 19 do
      if stack[a].used then
      begin
        for b:=0 to 3 do
        if (next[a,b]>=0) and not stack[next[a,b]].used then
        begin
          stack[next[a,b]].used:=true;
          stack[next[a,b]].father:=a;
          stack[next[a,b]].father_used_dir:=b;
          if next[a,b]=dest then
            f:=true;
        end;
      end;
  until f;
  a:=dest;
  repeat
    b:=a;
    a:=stack[a].father;
  until a=from;
  result:=stack[b].father_used_dir;
end;

procedure find_strategy(data : pdata);
var a,b,p1,p2 : integer;
begin
  with data^,data^.gameinfo^ do
  begin
    if iscpu[mapping[momplayer]]=0 then
      exit;
    repeat
      dest:=random(20);
    until not open[dest];
  end;
end;

procedure interact(data : pdata);
begin
  with data^,data^.gameinfo^ do
  begin
    if iscpu[mapping[momplayer]]=0 then
      exit;
    (joystick[mapping[momplayer]].axis[0])^:=0;
    (joystick[mapping[momplayer]].axis[1])^:=0;
    (joystick[mapping[momplayer]].button[0])^:=0;
    case get_direction(mom_site,dest) of
     -1: (joystick[mapping[momplayer]].button[0])^:=1;
      0: (joystick[mapping[momplayer]].axis[0])^:=-1;
      1: (joystick[mapping[momplayer]].axis[1])^:=-1;
      2: (joystick[mapping[momplayer]].axis[0])^:= 1;
      3: (joystick[mapping[momplayer]].axis[1])^:= 1;    
    end;
  end;
end;
