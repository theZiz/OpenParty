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
int get_turn_dice_value_sum(pmaindata data)
{
	int a,b=0;
	for (a=0;a<data->turn_dice_num;a++)
	  b+=data->turn_dice_value[a];
	return b;
}

void draw_one_dice(int turn_dice_value,float turn_dice_next,char make_smaller)
{
	float dicepos=((float)(turn_dice_value))+turn_dice_next;	
	while (dicepos>=6.0)
	  dicepos-=6.0;
	while (dicepos<0.0)
	  dicepos+=0.0;
	
	glPushMatrix();
  //[0..1[
  if (dicepos<1.0)
  {
    if (!make_smaller)
      glRotatef(turn_dice_next*90,0,-1,0);
    else
    {
      glRotatef(90,0,-1,0);
      glScalef(turn_dice_next,turn_dice_next,turn_dice_next);
    }
  }
  //[1..2[
  if (dicepos>=1.0 && dicepos<2.0)
  {
    glRotatef(90,0,-1,0);
    glRotatef(turn_dice_next*90.0,0,0,1);
  }
  //[2..3[
  if (dicepos>=2.0 && dicepos<3.0)
  {
    glRotatef(90,0,-1,0);
    glRotatef(90.0,0,0,1);
    glRotatef(turn_dice_next*90.0,-1,0,0);
  }
  //[3..4[
  if (dicepos>=3.0 && dicepos<4.0)
  {
    glRotatef(90,0,-1,0);
    glRotatef(90.0,0,0,1);
    glRotatef(90.0,-1,0,0);
    glRotatef(turn_dice_next*90.0,0,-1,0);
  }
  //[4..5[
  if (dicepos>=4.0 && dicepos<5.0)
  {
    glRotatef(90,0,-1,0);
    glRotatef(90.0,0,0,1);
    glRotatef(90.0,-1,0,0);
    glRotatef(90.0,0,-1,0);
    glRotatef(turn_dice_next*90.0,0,0,1);
  }
  //[5..6[
  if (dicepos>=5.0)
  {
    glRotatef(90,0,-1,0);
    glRotatef(90.0,0,0,1);
    glRotatef(90.0,-1,0,0);
    glRotatef(90.0,0,-1,0);
    glRotatef(90.0,0,0,1);
    glRotatef(turn_dice_next*90.0,-1,0,0);
  }
  ZWdrawobject(dice);
	glPopMatrix();
}

void draw_dice(pmaindata data)
{
  if (data->mainloop_pos!=3) return;
  //Würfelpos geht geschmeidig von 0 (6) bis 5.9999 (gaaanz kurz vor 6)

  glPushMatrix();
  glTranslatef(0,3,0);
  glRotatef(data->player[data->activplayer].savedcamera_y,0,1,0);
  glRotatef(diceangle,1,0,0);
  glScalef(0.5,0.5,0.5);
  glColor4f(1,1,1,1);
  draw_one_dice(data->turn_dice_value[0],data->turn_dice_next,data->turn_pos>=5);  
  glPopMatrix();
	int a;
	for (a=0;a<data->turn_dice_num-1;a++)
	{
		glPushMatrix();
		glTranslatef(0,4,0);
		glRotatef(data->player[data->activplayer].savedcamera_y,0,1,0);
		glTranslatef(((float)a-(float)(data->turn_dice_num-2)/2.0)*0.7,0,0);
		glRotatef(diceangle,1,0,0);
		glScalef(0.25,0.25,0.25);
		glColor4f(1,1,1,1);
		draw_one_dice(data->turn_dice_value[a+1],0,0);  
		glPopMatrix();		
	}
	
}
