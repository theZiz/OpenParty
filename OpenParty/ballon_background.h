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
void draw_ballon_background(pmenudata data)
{
  //Hintergrund
  glColor4f(1,1,1,1);
  glNormal3f(0,0,1);
  glTranslatef(0,0,-13);
  float dazu=(data->rotation)/180.0;
  dazu=dazu-trunc(dazu);
  ZWusetexture(sky_menu_tex);
  glBegin(GL_QUADS);
    glTexCoord2f(0+dazu,0);glVertex3f(-10,10,0);
    glTexCoord2f(0+dazu,1);glVertex3f(-10,-10,0);
    glTexCoord2f(1+dazu,1);glVertex3f(10,-10,0);
    glTexCoord2f(1+dazu,0);glVertex3f(10,10,0);  
  glEnd();

  //Licht an
  glEnable(GL_LIGHTING);
  glLoadIdentity();
  ZWenablelight(0);
  ZWsetlightposition(0,0,0,0);
  ZWsetlightambient(0,0.2,0.2,0.2);
  ZWsetlightdiffuse(0,1,1,1);

  
  //Ballon
  glLoadIdentity();
  glTranslatef(3.2+ballonx,-1+cos(data->rotation*M_PI/180.0)/2.0+ballony,-7+ballonz);
  glScalef(0.5,0.5,0.5);
  glTranslatef(0,sin(data->ballmov*M_PI)*3.5-3.0,0);
  glRotatef(20,0,-1,0);
  GLubyte r=data->facecolor%3*127;
  GLubyte g=(data->facecolor/3)%3*127;
  GLubyte b=(data->facecolor/9)%3*127;
  glColor4ub(r,g,b,255);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  ZWdrawobject(data->jumpball);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTranslatef(0,-sin(data->ballmov*M_PI)*3.5+3.0,0);
  glRotatef(20,0,1,0);
  glScalef(8,8,8);
  glRotatef(data->rotation,0,1,0);
  glColor4f(1,1,1,1);
  ZWdrawobject(ballon);
  
  //Überschrift:
  glLoadIdentity();
  glTranslatef(-2.5,2,-5);
  glColor4f(1,0,0,0.7);
  glScalef(0.5,0.5,0.5);
  glRotatef(sin(data->rotation*M_PI/45.0)*20.0,0,1,0);
  ZWdrawobject(letter_o);
  glRotatef(sin(data->rotation*M_PI/45.0)*20.0,0,-1,0);

  glColor4f(0,1,0,0.7);
  glTranslatef(1.5,0,0);
  glRotatef(sin((data->rotation-5.0)*M_PI/45.0)*20.0,0,1,0);
  ZWdrawobject(letter_p);
  glRotatef(sin((data->rotation-5.0)*M_PI/45.0)*20.0,0,-1,0);
  
  glColor4f(1,1,0,0.7);
  glTranslatef(0.8,0,0);
  glRotatef(sin((data->rotation-10.0)*M_PI/45.0)*20.0,0,1,0);
  ZWdrawobject(letter_e);
  glRotatef(sin((data->rotation-10.0)*M_PI/45.0)*20.0,0,-1,0);
  
  glColor4f(0,0,1,0.7);
  glTranslatef(1.3,0,0);
  glRotatef(sin((data->rotation-15.0)*M_PI/45.0)*20.0,0,1,0);
  ZWdrawobject(letter_n);
  glRotatef(sin((data->rotation-15.0)*M_PI/45.0)*20.0,0,-1,0);
  
  glColor4f(1,0,1,0.7);
  glTranslatef(1.5,0,0);
  glRotatef(sin((data->rotation-20.0)*M_PI/45.0)*20.0,0,1,0);
  ZWdrawobject(letter_p);
  glRotatef(sin((data->rotation-20.0)*M_PI/45.0)*20.0,0,-1,0);
  
  glColor4f(0,1,1,0.7);
  glTranslatef(1.3,0,0);
  glRotatef(sin((data->rotation-25.0)*M_PI/45.0)*20.0,0,1,0);
  ZWdrawobject(letter_a);
  glRotatef(sin((data->rotation-25.0)*M_PI/45.0)*20.0,0,-1,0);
  
  glColor4f(1,1,1,0.7);
  glTranslatef(1.3,0,0);
  glRotatef(sin((data->rotation-30.0)*M_PI/45.0)*20.0,0,1,0);
  ZWdrawobject(letter_r);
  glRotatef(sin((data->rotation-30.0)*M_PI/45.0)*20.0,0,-1,0);
  
  glColor4f(0.5,0.5,0.5,0.7);
  glTranslatef(1.5,0,0);
  glRotatef(sin((data->rotation-35.0)*M_PI/45.0)*20.0,0,1,0);
  ZWdrawobject(letter_t);
  glRotatef(sin((data->rotation-35.0)*M_PI/45.0)*20.0,0,-1,0);
  
  glColor4f(0.3,0.5,0.7,0.7);
  glTranslatef(1.5,0,0);
  glRotatef(sin((data->rotation-40.0)*M_PI/45.0)*20.0,0,1,0);
  ZWdrawobject(letter_y);
  glRotatef(sin((data->rotation-40.0)*M_PI/45.0)*20.0,0,-1,0);
  glScalef(2,2,2);

  ZWdisablelight(0);
  glDisable(GL_LIGHTING);

}
