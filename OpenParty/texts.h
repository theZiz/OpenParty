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
void deletetextcontent(void)
{
  ptextsite textsite=firsttextsite;
  int a;
  for (a=0;a<maxtextsite && textsite!=NULL;a++)
  {
    ptextsite temp=textsite;
    textsite=textsite->next;
    int a;
    for (a=0;a<5;a++)
      if (temp->line[a]!=NULL)
        free(temp->line[a]);
    free(temp);
  }
  firsttextsite=NULL;
  momtextsite=0;
  maxtextsite=0;
}

void createtextcontent(char* content_)
{
  if (content_==NULL) return;
  char* content=(char*)malloc(strlen(content_)+1);
  sprintf(content,"%s",content_);
  //etwaige "alte" Seiten entfernen:
  deletetextcontent();
  firsttextsite=(ptextsite)malloc(sizeof(ttextsite));

  //Konstanten festlegen:
  int sitelines=5;
  float maxlinelen=19.2;//19.4285714;


  ptextsite textsite=firsttextsite;
  int done=0;
  int startpos=0;
  maxtextsite=1;
  while (done==0)
  {
    //Zeilen durchgehen:
    int line=0;
    for (line=0;line<sitelines;line++)
    {
      float momlen=0;
      int mompos=startpos;
      int spacepos=0;
      if (done==1)
        textsite->line[line]=NULL;
      else
      {  
        while (momlen<maxlinelen)
        {
          momlen+=text.breite[content[mompos]];
          if (content[mompos]==' ') spacepos=mompos;
          if (content[mompos]==0) 
          {
            spacepos=mompos;
            done=1;
            break;
          }  
          mompos++;
        }
        char temp=content[spacepos];
        content[spacepos]=0;
        textsite->line[line]=(char*)malloc(strlen(&(content[startpos]))+1);
        sprintf(textsite->line[line],"%s",&(content[startpos]));
        content[spacepos]=temp;
        startpos=spacepos+1;
      }  
    }
    textsite->next=(ptextsite)malloc(sizeof(ttextsite));
    textsite=textsite->next;
    if (done==0) maxtextsite++;
  }
  textsite->next=NULL;
  momtextsite=0;
  free(content);
}

void drawtextsite(void)
{
  if (firsttextsite==NULL) return;
  float fontsize=0.14;
  float linedistance=0.11;  
  glTranslatef(0,-0.11,0.03);
   //Rahmen malen:
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
    glColor4f(0,0,0,0.7);
    glVertex3f(-1.5,-0.2,-2);
    glVertex3f(-1.5,-1  ,-2);
    glVertex3f( 1.5,-1  ,-2);
    glVertex3f( 1.5,-0.2,-2);
    //links
    glColor4f(border.left.r,border.left.g,border.left.b,0.2);
    glVertex3f(-1.5,-0.2,-2);
    glVertex3f(-1.5,-1  ,-2);
    glVertex3f(-1.4,-1  ,-2);
    glVertex3f(-1.4,-0.2,-2);
    //Rechts
    glColor4f(border.right.r,border.right.g,border.right.b,0.2);
    glVertex3f( 1.4,-0.2,-2);
    glVertex3f( 1.4,-1  ,-2);
    glVertex3f( 1.5,-1  ,-2);
    glVertex3f( 1.5,-0.2,-2);
    //oben
    glColor4f(border.up.r,border.up.g,border.up.b,0.2);
    glVertex3f(-1.5,-0.2,-2);
    glVertex3f(-1.5,-0.3,-2);
    glVertex3f( 1.5,-0.3,-2);
    glVertex3f( 1.5,-0.2,-2);
    //unten
    glColor4f(border.down.r,border.down.g,border.down.b,0.2);
    glVertex3f(-1.5,-0.9,-2);
    glVertex3f(-1.5,-1  ,-2);
    glVertex3f( 1.5,-1  ,-2);
    glVertex3f( 1.5,-0.9,-2);
  glEnd();
  glEnable(GL_TEXTURE_2D);
  
  glColor4f(1,1,1,0.3);
  
  //Infos zum Text:
  char buffer[128];
  if (strcmp(language,(char*)"german")==0)
  {
    ZWdrawtextright(text,1.48,-0.26,-2,(char*)"[<] und [>] zum Blättern nutzen!",0.1);
    sprintf(buffer,"Seite %i von %i",momtextsite+1,maxtextsite);
  }
  else
  {
    ZWdrawtextright(text,1.48,-0.26,-2,(char*)"Use [<] und [>] to scroll!",0.1);
    sprintf(buffer,"Page %i of %i",momtextsite+1,maxtextsite);
  }
  
  ZWdrawtextright(text,1.48,-0.965,-2,buffer,0.1);

  glColor4f(1,1,1,0.5);

  ptextsite textsite=firsttextsite;

  int a;
  for (a=0;a<momtextsite;a++)
    if (textsite->next!=NULL)
      textsite=textsite->next;
  
  for (a=0;a<5;a++)
    if (textsite->line[a]!=NULL)
      ZWdrawtext(text,-1.36,-0.4-a*linedistance,-2,textsite->line[a],fontsize);
   glTranslatef(0,0.11,-0.03);
}

void resetborder(void)
{
  border.left.r=1;
  border.left.g=1;
  border.left.b=1;
  border.right=border.left;
  border.up=border.left;
  border.down=border.left;
}

void deletealltexts(void)
{
  ptext text=firsttext;
  while (text!=NULL)
  {
    ptext temp=text;
    text=text->next;
    planguage language=temp->language;
    while (language!=NULL)
    {
      planguage temp=language;
      language=language->next;
      if (temp->language!=NULL)
        free(temp->language);
      if (temp->content!=NULL)  
        free(temp->content);
      free(temp);
    }
    free(temp->name);    
    free(temp);
  }
  firsttext=NULL;
}

ptext newtext(char* name)
{
  ptext text=(ptext)malloc(sizeof(ttext));
  text->language=NULL;
  text->name=(char*)malloc(strlen(name)+1);
  sprintf(text->name,"%s",name);
  text->next=firsttext;
  firsttext=text;
  return text;
}

void addlanguage(ptext text,char* clanguage,char* content)
{
  if (text==NULL) return;
  planguage language=(planguage)malloc(sizeof(tlanguage));
  language->language=(char*)malloc(strlen(clanguage)+1);
  sprintf(language->language,"%s",clanguage);
  language->content=(char*)malloc(strlen(content)+1);
  sprintf(language->content,"%s",content);
  language->next=text->language;
  text->language=language;
}

char* gettextbynrandlanguage(int nr,char* language)
{
  if (nr>=textcount || nr<0)
    return texterror;
  //nach language suchen
  planguage momlan=textlist[nr]->language;
  planguage last=NULL;
  while (momlan!=NULL)
  {
    if (strcmp(momlan->language,language)==0) break;
    last=momlan;
    momlan=momlan->next;
  }
  if (momlan!=NULL) return momlan->content;
  //Sonst wird die erste Sprache zurückgeben:
  return last->content;  
}

void handle_textcontent(int player)
{
  if (maxtextsite<1)
    return;
  int von = player;
  int bis = player+1;
  if (player<0) // ALLE Player
  {
    von=0;
    bis=playernum;
  }
  for (int a=von;a<bis;a++)
  {
    if (*(ZWattribute->joystick[a].axis[0])<0)
    {
      *(ZWattribute->joystick[a].axis[0])=0;
      momtextsite=(momtextsite+maxtextsite-1)%maxtextsite;
    }
    if (*(ZWattribute->joystick[a].axis[0])>0)
    {
      *(ZWattribute->joystick[a].axis[0])=0;
      momtextsite=(momtextsite+1)%maxtextsite;
    }
  }

}
