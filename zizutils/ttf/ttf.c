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

#define TTF_VERSATZ 0.2
#include "ttf.h"

#ifdef WIN32
  #define EXPORT DLLIMPORT
#else
  #define EXPORT
#endif

float ___ZWttf_one_pixel;

EXPORT void ZWinitfont(void)
{
  TTF_Init();
}

EXPORT ZWtext ZWcreatefonttexture(char* fontname,int res,GLbyte r,GLbyte g,GLbyte b)
{
	___ZWttf_one_pixel=1.0/(float)res;
  ZWtext t;
  SDL_Surface *bild=NULL; //an dieser Stelle Bild mit res*res RGBA erstellen!
  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
  bild = SDL_CreateRGBSurface(SDL_SWSURFACE, res, res, 32, rmask, gmask, bmask, amask);
  SDL_SetAlpha(bild,0,0);
  //memset(bild->pixels,0,res*res*4);
  SDL_FillRect(bild, NULL, SDL_MapRGBA(bild->format, 0, 0, 0, 0));
  int delta=res/16;
  TTF_Font *font=TTF_OpenFont(fontname, delta);
  int a;
  int c;
  for (a=0;a<16;a++)
    for (c=0;c<16;c++)
    {
      char buffer[2];
      buffer[0]=a+c*16;
      buffer[1]=0;
      SDL_Color color;
      color.r=r;
      color.g=g;
      color.b=b;
      SDL_Surface* bs=TTF_RenderText_Blended(font, buffer, color);
      //if (bs!=NULL)
      {
        if (bs!=NULL)
          SDL_SetAlpha(bs,0,0);
        SDL_Rect srcrect;
        srcrect.x=0;
        srcrect.y=(int)((float)delta*TTF_VERSATZ);
        srcrect.w=delta;
        srcrect.h=delta;
        SDL_Rect dstrect;
        dstrect.x=delta*a;
        dstrect.y=delta*c;
        dstrect.w=delta;
        dstrect.h=delta;
        int w;
        int h;
        if (c==0 && a==0)
        {
          int delta_3_4=delta*9/10;
          Uint8 *pixels=(Uint8*)(bild->pixels);
          for (w=0;w<delta_3_4;w++)
            for (h=0;h<delta_3_4;h++)
              if ((w-delta_3_4/2)*(w-delta_3_4/2)+(h-delta_3_4/2)*(h-delta_3_4/2)<delta_3_4*delta_3_4/4 &&
                  (w-delta_3_4/2)*(w-delta_3_4/2)+(h-delta_3_4/2)*(h-delta_3_4/2)>delta_3_4*delta_3_4/7)
              {
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4  ]=r;
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4+1]=g;
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4+2]=b;
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4+3]=255;
              }
              else
              if ((w-delta_3_4/2)*(w-delta_3_4/2)+(h-delta_3_4/2)*(h-delta_3_4/2)<delta_3_4*delta_3_4/4)
              {
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4  ]=r;
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4+1]=g;
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4+2]=b;
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4+3]=63;
              }
              else
              {
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4  ]=0;
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4+1]=0;
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4+2]=0;
                pixels[(dstrect.x+w+(dstrect.y+1+h)*res+2)*4+3]=0;
              }
              
        }
        else
        if (bs!=NULL)
          SDL_BlitSurface(bs,&srcrect,bild,&dstrect);        
        TTF_SizeText(font,buffer,&w,&h);
        if (c==0 && a==0)
          t.breite[a+c*16]=0.90+32.0*___ZWttf_one_pixel; //4*16, weil später durch 16 dividiert wird.
        else
          t.breite[a+c*16]=(float)w/(float)delta;
        
        //printf("%s: %f\n",buffer,t.breite[a+c*16]);
          
        if (bs!=NULL)
          SDL_FreeSurface(bs);
      }
    }
  glGenTextures(1, &(t.texture));
  //glBindTexture(GL_TEXTURE_2D, t.texture);
  ZWusetexture(t.texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);    
  //gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bild->w, bild->h, GL_RGBA, GL_UNSIGNED_BYTE, bild->pixels);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, bild->w, bild->h,0, GL_RGBA, GL_UNSIGNED_BYTE, bild->pixels);    
  SDL_FreeSurface(bild);
  TTF_CloseFont(font);

  return t;
}

EXPORT void ZWdrawtext(ZWtext t,float x,float y,float z,char* text,float scale)
{
  ZWusetexture(t.texture);
  float glpos=0;
  int textpos=0;
  glBegin(GL_QUADS);
  while (text[textpos]!=0)
  {
    int pos;
    float dazu_y=0;
    if (text[textpos]=='[' && text[textpos+2]==']')
    {
      pos=0;
      dazu_y=0.1*scale;
    }
    else
      pos=text[textpos];

    if (pos<0) pos+=256;
    float tx=(float)(pos%16)/(float)16;
    float ty=(float)(pos/16)/(float)16;
    glTexCoord2f(tx+___ZWttf_one_pixel                     ,ty+___ZWttf_one_pixel);
    glVertex3f(x+glpos*scale                               ,y+0.5*scale+dazu_y,z);
    glTexCoord2f(tx+___ZWttf_one_pixel                     ,ty+0.0625-___ZWttf_one_pixel);
    glVertex3f(x+glpos*scale                               ,y-0.5*scale+dazu_y,z);
    glTexCoord2f(tx+t.breite[pos]/(float)16-___ZWttf_one_pixel,ty+0.0625-___ZWttf_one_pixel);
    glVertex3f(x+(glpos+t.breite[pos])*scale,y-0.5*scale+dazu_y,z);
    glTexCoord2f(tx+t.breite[pos]/(float)16-___ZWttf_one_pixel,ty+___ZWttf_one_pixel);
    glVertex3f(x+(glpos+t.breite[pos])*scale,y+0.5*scale+dazu_y,z);
    
    if (text[textpos]=='[' && text[textpos+2]==']')
    {
      pos=text[textpos+1];
      if (pos<0) pos+=256;
      float scale_small=scale*0.85;
      float dazu_y=0;
      float nicht_umkehren=1.0;
      switch (text[textpos+1])
      {
        case 'v':
          nicht_umkehren=0.0;
          pos='^';
          if (pos<0) pos+=256;
          scale_small=scale*1.6;
          dazu_y=(0.45)*scale_small;
        break;
        case '^':
          scale_small=scale*1.6;
          dazu_y=(-0.25)*scale_small;
        break;
        case '<': case '>': 
          scale_small=scale*1.1;
          dazu_y=(-0.1)*scale_small;
        break;
      }
      float tx=(float)(pos%16)/(float)16;
      float ty=(float)(pos/16)/(float)16;
      glTexCoord2f(tx+___ZWttf_one_pixel                                       ,ty+(1.0-nicht_umkehren)*(0.0625-___ZWttf_one_pixel)+(    nicht_umkehren)*___ZWttf_one_pixel);
      glVertex3f(x+(glpos+t.breite[0]/2.0)*scale+(-t.breite[pos]/2.0)*scale_small,y+0.5*scale_small+dazu_y,z);
      glTexCoord2f(tx+___ZWttf_one_pixel                                       ,ty+(    nicht_umkehren)*(0.0625-___ZWttf_one_pixel)+(1.0-nicht_umkehren)*___ZWttf_one_pixel);
      glVertex3f(x+(glpos+t.breite[0]/2.0)*scale+(-t.breite[pos]/2.0)*scale_small,y-0.5*scale_small+dazu_y,z);
      glTexCoord2f(tx+t.breite[pos]/(float)16-___ZWttf_one_pixel               ,ty+(    nicht_umkehren)*(0.0625-___ZWttf_one_pixel)+(1.0-nicht_umkehren)*___ZWttf_one_pixel);
      glVertex3f(x+(glpos+t.breite[0]/2.0)*scale+(+t.breite[pos]/2.0)*scale_small,y-0.5*scale_small+dazu_y,z);
      glTexCoord2f(tx+t.breite[pos]/(float)16-___ZWttf_one_pixel               ,ty+(1.0-nicht_umkehren)*(0.0625-___ZWttf_one_pixel)+(    nicht_umkehren)*___ZWttf_one_pixel);
      glVertex3f(x+(glpos+t.breite[0]/2.0)*scale+(+t.breite[pos]/2.0)*scale_small,y+0.5*scale_small+dazu_y,z);
      pos=0;
    }
    
    glpos+=t.breite[pos];
    if (text[textpos]=='[' && text[textpos+2]==']')
      textpos+=3;
    else
      textpos++;
  }
  glEnd();
}

EXPORT void ZWdrawtext_fac(ZWtext t,float x,float y,float z,char* text,float scale,float faktor)
{
/*  ZWusetexture(t.texture);
  //glBindTexture(GL_TEXTURE_2D, t.texture);
  float glpos=0;
  int textpos=0;
  glBegin(GL_QUADS);
  while (text[textpos]!=0)
  {
    int pos=text[textpos];
    if (pos<0) pos+=256;  
    float tx=(float)(pos%16)/(float)16;
    float ty=(float)(pos/16)/(float)16;
    float dazu=(t.breite[pos]*faktor-t.breite[pos])/2.0;
    glTexCoord2f(tx                                       ,ty       );
    glVertex3f(x+(glpos+dazu)*scale                               ,y+0.5*scale,z);
    glTexCoord2f(tx                                       ,ty+0.0625);
    glVertex3f(x+(glpos+dazu)*scale                               ,y-0.5*scale,z);
    glTexCoord2f(tx+t.breite[pos]/(float)16,ty+0.0625);
    glVertex3f(x+(glpos+t.breite[pos]+dazu)*scale,y-0.5*scale,z);
    glTexCoord2f(tx+t.breite[pos]/(float)16,ty       );
    glVertex3f(x+(glpos+t.breite[pos]+dazu)*scale,y+0.5*scale,z);
    glpos+=t.breite[pos]*faktor;
    textpos++;
  }
  glEnd();*/
  ZWusetexture(t.texture);
  float glpos=0;
  int textpos=0;
  glBegin(GL_QUADS);
  while (text[textpos]!=0)
  {
    int pos;
    float dazu_y=0;
    if (text[textpos]=='[' && text[textpos+2]==']')
    {
      pos=0;
      dazu_y=0.1*scale;
    }
    else
      pos=text[textpos];

    if (pos<0) pos+=256;
    float tx=(float)(pos%16)/(float)16;
    float ty=(float)(pos/16)/(float)16;
    float dazu=(t.breite[pos]*faktor-t.breite[pos])/2.0;
    glTexCoord2f(tx+___ZWttf_one_pixel                     ,ty+___ZWttf_one_pixel);
    glVertex3f(x+(glpos+dazu)*scale                               ,y+0.5*scale+dazu_y,z);
    glTexCoord2f(tx+___ZWttf_one_pixel                     ,ty+0.0625-___ZWttf_one_pixel);
    glVertex3f(x+(glpos+dazu)*scale                               ,y-0.5*scale+dazu_y,z);
    glTexCoord2f(tx+t.breite[pos]/(float)16-___ZWttf_one_pixel,ty+0.0625-___ZWttf_one_pixel);
    glVertex3f(x+(glpos+t.breite[pos]+dazu)*scale,y-0.5*scale+dazu_y,z);
    glTexCoord2f(tx+t.breite[pos]/(float)16-___ZWttf_one_pixel,ty+___ZWttf_one_pixel);
    glVertex3f(x+(glpos+t.breite[pos]+dazu)*scale,y+0.5*scale+dazu_y,z);
    
    if (text[textpos]=='[' && text[textpos+2]==']')
    {
      pos=text[textpos+1];
      if (pos<0) pos+=256;
      float scale_small=scale*0.85;
      float dazu_y=0;
      float nicht_umkehren=1.0;
      switch (text[textpos+1])
      {
        case 'v':
          nicht_umkehren=0.0;
          pos='^';
          if (pos<0) pos+=256;
          scale_small=scale*1.6;
          dazu_y=(0.45)*scale_small;
        break;
        case '^':
          scale_small=scale*1.6;
          dazu_y=(-0.25)*scale_small;
        break;
        case '<': case '>': 
          scale_small=scale*1.1;
          dazu_y=(-0.1)*scale_small;
        break;
      }
      float tx=(float)(pos%16)/(float)16;
      float ty=(float)(pos/16)/(float)16;
      glTexCoord2f(tx+___ZWttf_one_pixel                                       ,ty+(1.0-nicht_umkehren)*(0.0625-___ZWttf_one_pixel)+(    nicht_umkehren)*___ZWttf_one_pixel);
      glVertex3f(x+(glpos+t.breite[0]/2.0+dazu)*scale+(-t.breite[pos]/2.0)*scale_small,y+0.5*scale_small+dazu_y,z);
      glTexCoord2f(tx+___ZWttf_one_pixel                                       ,ty+(    nicht_umkehren)*(0.0625-___ZWttf_one_pixel)+(1.0-nicht_umkehren)*___ZWttf_one_pixel);
      glVertex3f(x+(glpos+t.breite[0]/2.0+dazu)*scale+(-t.breite[pos]/2.0)*scale_small,y-0.5*scale_small+dazu_y,z);
      glTexCoord2f(tx+t.breite[pos]/(float)16-___ZWttf_one_pixel               ,ty+(    nicht_umkehren)*(0.0625-___ZWttf_one_pixel)+(1.0-nicht_umkehren)*___ZWttf_one_pixel);
      glVertex3f(x+(glpos+t.breite[0]/2.0+dazu)*scale+(+t.breite[pos]/2.0)*scale_small,y-0.5*scale_small+dazu_y,z);
      glTexCoord2f(tx+t.breite[pos]/(float)16-___ZWttf_one_pixel               ,ty+(1.0-nicht_umkehren)*(0.0625-___ZWttf_one_pixel)+(    nicht_umkehren)*___ZWttf_one_pixel);
      glVertex3f(x+(glpos+t.breite[0]/2.0+dazu)*scale+(+t.breite[pos]/2.0)*scale_small,y+0.5*scale_small+dazu_y,z);
      pos=0;
    }
    
    glpos+=t.breite[pos]*faktor;
    if (text[textpos]=='[' && text[textpos+2]==']')
      textpos+=3;
    else
      textpos++;
  }
  glEnd();  
}

EXPORT void ZWdrawtextmiddle(ZWtext t,float x,float y,float z,char* text,float scale)
{
  float glpos=0;
  int textpos=0;
  while (text[textpos]!=0)
  {
    int pos=text[textpos];
    if (pos<0) pos+=256;
    glpos+=t.breite[pos];
    textpos++;
  }
  ZWdrawtext(t,x-glpos*scale/2.0f,y,z,text,scale);
}

EXPORT void ZWdrawtextmiddle_fac(ZWtext t,float x,float y,float z,char* text,float scale,float faktor)
{
  float glpos=0;
  int textpos=0;
  while (text[textpos]!=0)
  {
    int pos=text[textpos];
    if (pos<0) pos+=256;
    glpos+=t.breite[pos]*faktor;
    textpos++;
  }
  ZWdrawtext_fac(t,x-glpos*scale/2.0f,y,z,text,scale,faktor);
}

EXPORT void ZWdrawtextright(ZWtext t,float x,float y,float z,char* text,float scale)
{
  float glpos=0;
  int textpos=0;
  while (text[textpos]!=0)
  {
    int pos=text[textpos];
    if (pos<0) pos+=256;  
    glpos+=t.breite[pos];
    textpos++;
  }
  ZWdrawtext(t,x-glpos*scale,y,z,text,scale);
}

EXPORT void ZWdrawtextright_fac(ZWtext t,float x,float y,float z,char* text,float scale,float faktor)
{
  float glpos=0;
  int textpos=0;
  while (text[textpos]!=0)
  {
    int pos=text[textpos];
    if (pos<0) pos+=256;  
    glpos+=t.breite[pos]*faktor;
    textpos++;
  }
  ZWdrawtext_fac(t,x-glpos*scale,y,z,text,scale,faktor);
}


EXPORT void ZWdeletefonttexture(ZWtext texture)
{
}

EXPORT void ZWquitfont(void)
{
TTF_Quit();
}

EXPORT void ZWdrawtext_shadow(ZWtext t,float x,float y,float z,char* text,float scale,float faktor,float r,float g,float b,
                              float sr,float sg,float sb,float sa)
{
  glColor4f(sr,sg,sb,sa);
  ZWdrawtext_fac(t,x,y,z,text,scale*faktor,1.0/faktor);
  glColor4f(r,g,b,1.0);
  ZWdrawtext(t,x,y,z,text,scale);
}

EXPORT void ZWdrawtextright_shadow(ZWtext t,float x,float y,float z,char* text,float scale,
                                   float faktor,float r,float g,float b,float sr,float sg,float sb,float sa)
{
  glColor4f(sr,sg,sb,sa);
  ZWdrawtextright_fac(t,x,y,z,text,scale*faktor,1.0/faktor);
  glColor4f(r,g,b,1.0);
  ZWdrawtextright(t,x,y,z,text,scale);
}

EXPORT void ZWdrawtextmiddle_shadow(ZWtext t,float x,float y,float z,char* text,float scale,
                                    float faktor,float r,float g,float b,float sr,float sg,float sb,float sa)
{
  glColor4f(sr,sg,sb,sa);
  ZWdrawtextmiddle_fac(t,x,y,z,text,scale*faktor,1.0/faktor);
  glColor4f(r,g,b,1.0);
  ZWdrawtextmiddle(t,x,y,z,text,scale);
}

