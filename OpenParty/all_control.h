/*
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
*/

int is_any_button_down(int delafter)
{
  int a,b;
  for (a=0;a<ZWattribute->realjoystickcount;a++)
    for (b=0;b<ZWattribute->realjoystick[a].buttoncount;b++)
      if (ZWattribute->realjoystick[a].button[b]==1) 
      {
        ZWattribute->realjoystick[a].button[b]*=(1-delafter);
        return 1;
      }
  if (ZWattribute->keypressed[SDLK_RETURN])
  {
    ZWattribute->keypressed[SDLK_RETURN]=(1-delafter);
    return 1;
  }
  return 0;
}

int is_any_axis_left(int delafter)
{
  int a,b;
  for (a=0;a<ZWattribute->realjoystickcount;a++)
    for (b=0;b<ZWattribute->realjoystick[a].axiscount/2;b++)
      if (ZWattribute->realjoystick[a].axis[b*2]==-1)
      {
        if (delafter)
          ZWattribute->realjoystick[a].axis[b*2]*=0;
        return 1;
      }
  return 0;
}

int left_axis_counter=0;
int left_axis_timeout=0;

int is_any_axis_left_wait()
{
  left_axis_timeout--;
  int a,b;
  for (a=0;a<ZWattribute->realjoystickcount;a++)
    for (b=0;b<ZWattribute->realjoystick[a].axiscount/2;b++)
      if (ZWattribute->realjoystick[a].axis[b*2]==-1)
      {
        if (left_axis_timeout<0)
        {
          if (left_axis_counter==0)
            left_axis_timeout=300;
          else if (left_axis_counter<4)
            left_axis_timeout=150;
          else if (left_axis_counter<10)
            left_axis_timeout=80;
          else
            left_axis_timeout=40;
          left_axis_counter++;
          return 1; 
        }
        return 0;
      }
  left_axis_counter=0;
  if (left_axis_timeout<0)
    left_axis_timeout=0;
  return 0;
}


int is_any_axis_right(int delafter)
{
  int a,b;
  for (a=0;a<ZWattribute->realjoystickcount;a++)
    for (b=0;b<ZWattribute->realjoystick[a].axiscount/2;b++)
      if (ZWattribute->realjoystick[a].axis[b*2]==1)
      {
        if (delafter)
          ZWattribute->realjoystick[a].axis[b*2]*=0;
        return 1;
      }
  return 0;
}

int right_axis_counter=0;
int right_axis_timeout=0;

int is_any_axis_right_wait()
{
  right_axis_timeout--;
  int a,b;
  for (a=0;a<ZWattribute->realjoystickcount;a++)
    for (b=0;b<ZWattribute->realjoystick[a].axiscount/2;b++)
      if (ZWattribute->realjoystick[a].axis[b*2]==1)
      {
        if (right_axis_timeout<0)
        {
          if (right_axis_counter==0)
            right_axis_timeout=400;
          else if (right_axis_counter<4)
            right_axis_timeout=200;
          else if (right_axis_counter<10)
            right_axis_timeout=100;
          else
            right_axis_timeout=40;
          right_axis_counter++;
          return 1; 
        }
        return 0;
      }
  right_axis_counter=0;
  if (right_axis_timeout<0)
    right_axis_timeout=0;
  return 0;
}


int is_any_axis_up(int delafter)
{
  int a,b;
  for (a=0;a<ZWattribute->realjoystickcount;a++)
    for (b=0;b<ZWattribute->realjoystick[a].axiscount/2;b++)
      if (b*2+1<ZWattribute->realjoystick[a].axiscount && ZWattribute->realjoystick[a].axis[b*2+1]==-1)
      {
        if (delafter)
          ZWattribute->realjoystick[a].axis[b*2+1]*=0;
        return 1;
      }
  return 0;
}

int is_any_axis_down(int delafter)
{
  int a,b;
  for (a=0;a<ZWattribute->realjoystickcount;a++)
    for (b=0;b<ZWattribute->realjoystick[a].axiscount/2;b++)
      if (b*2+1<ZWattribute->realjoystick[a].axiscount && ZWattribute->realjoystick[a].axis[b*2+1]==1)
      {
        if (delafter)
          ZWattribute->realjoystick[a].axis[b*2+1]*=0;
        return 1;
      }
  return 0;
}
