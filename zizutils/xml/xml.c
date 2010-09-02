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
#include "xml.h"

#ifdef WIN32
  #define EXPORT DLLIMPORT
#else
  #define EXPORT
#endif

EXPORT pxmlfile ZWnewxmlfile(char* filename,char* name)
{
  pxmlfile xmlfile=(pxmlfile)malloc(sizeof(txmlfile));
  xmlfile->file=SDL_RWFromFile(filename,"w");
  char* buffer=(char*)malloc(100);
  sprintf(buffer,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
  SDL_RWwrite(xmlfile->file,buffer,strlen(buffer),1);
  free(buffer);
  int size=strlen(name)+4;
  buffer=(char*)malloc(size);
  sprintf(buffer,"<%s>\n",name);
  SDL_RWwrite(xmlfile->file,buffer,size-1,1);
  xmlfile->name=(char*)malloc(strlen(name)+1);
  sprintf(xmlfile->name,"%s",name);
  xmlfile->ebene=2;
  free(buffer);
  return xmlfile;
}

EXPORT void ZWbegintag(pxmlfile xmlfile,char* tag)
{
  int a;
  char c=' ';
  for (a=0;a<xmlfile->ebene;a++)
    SDL_RWwrite(xmlfile->file,&c,1,1);
  int size=strlen(tag)+2;
  char* buffer=(char*)malloc(size);  
  sprintf(buffer,"<%s",tag);
  SDL_RWwrite(xmlfile->file,buffer,size-1,1);    
  free(buffer);
}

EXPORT void ZWaddattribute_string(pxmlfile xmlfile,char* name,char* wert)
{
  int size=strlen(name)+strlen(wert)+5;
  char* buffer=(char*)malloc(size);
  sprintf(buffer," %s=\"%s\"",name,wert);
  SDL_RWwrite(xmlfile->file,buffer,size-1,1);
  free(buffer);
}

EXPORT void ZWaddplaintext(pxmlfile xmlfile,char* text)
{
  SDL_RWwrite(xmlfile->file,text,strlen(text),1);
}

EXPORT void ZWaddattribute_number(pxmlfile xmlfile,char* name,int wert)
{
  int size=strlen(name)+10+5;
  char* buffer=(char*)malloc(size);
  sprintf(buffer," %s=\"%i\"",name,wert);
  SDL_RWwrite(xmlfile->file,buffer,strlen(buffer),1);
  free(buffer);
}

EXPORT void ZWaddattribute_float(pxmlfile xmlfile,char* name,float wert)
{
  int size=strlen(name)+20+5;
  char* buffer=(char*)malloc(size);
  sprintf(buffer," %s=\"%f\"",name,wert);
  SDL_RWwrite(xmlfile->file,buffer,strlen(buffer),1);
  free(buffer);
}

EXPORT void ZWendnormaltag(pxmlfile xmlfile)
{
  char buffer[3];
  sprintf(buffer,">\n");
  SDL_RWwrite(xmlfile->file,buffer,2,1);
  xmlfile->ebene+=2;
}

EXPORT void ZWendnormaltagwithoutbreak(pxmlfile xmlfile)
{
  char buffer[2];
  sprintf(buffer,">");
  SDL_RWwrite(xmlfile->file,buffer,1,1);
  xmlfile->ebene+=2;
}


EXPORT void ZWendstandalonetag(pxmlfile xmlfile)
{
  char buffer[4];
  sprintf(buffer,"/>\n");
  SDL_RWwrite(xmlfile->file,buffer,3,1);
}

EXPORT void ZWclosetag(pxmlfile xmlfile,char* tag)
{
  xmlfile->ebene-=2;
  int a;
  char c=' ';
  for (a=0;a<xmlfile->ebene;a++)
    SDL_RWwrite(xmlfile->file,&c,1,1);
  int size=strlen(tag)+5;
  char* buffer=(char*)malloc(size);  
  sprintf(buffer,"</%s>\n",tag);
  SDL_RWwrite(xmlfile->file,buffer,size-1,1);    
  free(buffer);
}

EXPORT void ZWclosetagonsameline(pxmlfile xmlfile,char* tag)
{
  xmlfile->ebene-=2;
  int size=strlen(tag)+5;
  char* buffer=(char*)malloc(size);  
  sprintf(buffer,"</%s>\n",tag);
  SDL_RWwrite(xmlfile->file,buffer,size-1,1);    
  free(buffer);
}


EXPORT void ZWfinishxmlfile(pxmlfile xmlfile)
{
  int size=strlen(xmlfile->name)+4;
  char* buffer=(char*)malloc(size);
  sprintf(buffer,"</%s>",xmlfile->name);
  SDL_RWwrite(xmlfile->file,buffer,size-1,1);  
  SDL_RWclose(xmlfile->file);
  free(xmlfile);
}

EXPORT pxmlpoint ZWloadxmlfile(char* filename)
{
  pxmlpoint xmlpoint=(pxmlpoint)malloc(sizeof(txmlpoint));
  SDL_RWops *file=SDL_RWFromFile(filename,"r");
  char c=' ';
  do SDL_RWread(file,&c,1,1); while (c!='<');
  //Überprüfungen der XML-Kopf-Daten lassen wa ma weg, wa? :-P
  do SDL_RWread(file,&c,1,1); while (c!='>');
  //Die Datei ist gerade an der Position "eins nach dem '>'"
  int pos=SDL_RWseek(file,0,SEEK_CUR);
  int end=SDL_RWseek(file,0,SEEK_END);  
  xmlpoint->xml=(char*)malloc(end-pos);
  xmlpoint->len=end-pos;
  xmlpoint->pos=0;
  SDL_RWseek(file,pos,SEEK_SET);
  SDL_RWread(file,xmlpoint->xml,end-pos,1);
  SDL_RWclose(file);  
  return xmlpoint;
}

EXPORT pxmltag ZWgetnexttag(pxmlpoint xmlpoint)
{
  //Es wird das nächste '<' gesucht, weil da "wahrscheinlich" ein Tag beginnnt
  int tagbegin;
  for (tagbegin=xmlpoint->pos;tagbegin<xmlpoint->len;tagbegin++)
    if (xmlpoint->xml[tagbegin]=='<') break;
  if (tagbegin==xmlpoint->len)
  {
    return NULL; //Es wurde kein Tag gefunden
  }
  //Es wird davon ausgegangen, dass ein Tag DIREKT nach dem '<' beginnt. Es wird der Tagname ausgelesen
  //bis ein Leerzeichen erscheint und etwaige Attribute folgen oder ein '>' oder ein '/'.
  //zurerst wird die Länge bestimmt:
  int pos;
  for (pos=tagbegin+1;pos<xmlpoint->len;pos++)
    if (xmlpoint->xml[pos]==' ' || xmlpoint->xml[pos]=='/' || xmlpoint->xml[pos]=='>') break;
  if (pos==xmlpoint->len) 
  {
  printf("Fehler: Tag wird scheinbar nicht geschlossen\n");
  return NULL; //Fehler
  }
  
  pxmltag xmltag=(pxmltag)malloc(sizeof(txmltag)); //Speicher für Struktur reservieren
  xmltag->name=(char*)malloc(pos-tagbegin);//mit Terminiersymbol  
  memcpy(xmltag->name,&(xmlpoint->xml[tagbegin+1]),pos-tagbegin-1);
  xmltag->name[pos-tagbegin-1]=0;
  xmltag->attribute=NULL; 
  int a,b,c;
  int attnamelen;
  
  //"Weiterspringen" bis an der Position "pos" kein Leerzeichen ist:
  while (pos<xmlpoint->len)
  {
  while (xmlpoint->xml[pos]==' ' && pos<xmlpoint->len)
    pos++;
  if (pos==xmlpoint->len) break;
  switch (xmlpoint->xml[pos])
  {
  case '/': //alleinstehender Tag
    xmlpoint->pos=pos+2; // '/>' überspringen
    xmltag->inhalt=NULL;
    return xmltag;
  break;
  case '>': //normaler Tag
    xmltag->inhalt=(pxmlpoint)malloc(sizeof(txmlpoint));
    xmltag->inhalt->xml=&(xmlpoint->xml[pos+1]);
    xmltag->inhalt->pos=0;
    //Ende des Tag suchen, indem das nächste vorkommen von dem Tagnamen gesucht wird
    //Nächstes '<' suchen und schauen, ob ein '/' dahinter ist:
    //WICHTIG: Verkapselte IDENTISCHE Tags rufen Fehler hervor. Hierfür ist diese simple Bibliothek NICHT geeignet
    for (a=pos+1;a<xmlpoint->len-1;a++)
      if (xmlpoint->xml[a]=='<' && xmlpoint->xml[a+1]=='/') //irgendwas schließendes gefunden
      {
        int same=1;
        for (b=0;b<pos-tagbegin-1 && xmlpoint->xml[a+2+b]!='>' && xmlpoint->xml[a+2+b]!=' ';b++)
          if (xmltag->name[b]!=xmlpoint->xml[a+2+b])
          {
            same=0;
            break;
          }
        if (same==1) //Überprüfen, ob es nicht "ähnlich" weitergeht
          if (xmlpoint->xml[a+2+b]!=' ' && xmlpoint->xml[a+2+b]!='>')
          {
            same=0;
          }
        if (same==1) //Endtag gefunden :-D
        {
          xmltag->inhalt->len=a-pos-1;
          //'>' suchen
          for (c=a+2+b;c<xmlpoint->len;c++)
            if (xmlpoint->xml[c]=='>') break;
          xmlpoint->pos=c+1;
          return xmltag;
        }
      }  
  break;
  default:
    //es wird das '=' gesucht. Es wird davon ausgegangen, dass es DIREKT nach dem Attribut kommt.
    for (attnamelen=1;pos+attnamelen<xmlpoint->len;attnamelen++)
      if (xmlpoint->xml[pos+attnamelen]=='=') break;
    if (pos+attnamelen==xmlpoint->len) break; //Bei 'nem Fehler wird einfach abgebrochen. :-)
    pxmlattribute newatt=(pxmlattribute)malloc(sizeof(txmlattribute));
    newatt->next=xmltag->attribute;
    xmltag->attribute=newatt;
    newatt->name=(char*)malloc(attnamelen+1);
    memcpy(newatt->name,&(xmlpoint->xml[pos]),attnamelen);
    newatt->name[attnamelen]=0;
    pos+=attnamelen+2;//pos zeigt nun auf die Stelle hinter dem '=' und dem '"'
    //es wird das '"' gesucht. Es wird davon ausgegangen, dass es DIREKT nach dem Attribut kommt.
    for (attnamelen=1;pos+attnamelen<xmlpoint->len;attnamelen++)
      if (xmlpoint->xml[pos+attnamelen]=='"') break;
    newatt->wert=(char*)malloc(attnamelen+1);
    memcpy(newatt->wert,&(xmlpoint->xml[pos]),attnamelen);
    newatt->wert[attnamelen]=0;  
    pos+=attnamelen+1;
  break;
  }
  }
  //Wenn der Tag nicht richtig geschlossen wird:
  xmltag->inhalt->len=xmlpoint->len-pos-1;
  xmlpoint->pos=xmlpoint->len;
  return xmltag;
}

EXPORT void ZWfreetag(pxmltag xmltag)
{
  if (xmltag==NULL) return;
  free(xmltag->name);
  pxmlattribute now=xmltag->attribute;
  while (now!=NULL)
  {
    pxmlattribute temp=now;
    now=now->next;
    free(temp->name);
    free(temp->wert);
    free(temp);
  }
  if (xmltag->inhalt!=NULL)
    free(xmltag->inhalt);
  free(xmltag);
}

EXPORT void ZWfinishloading(pxmlpoint xmlpoint)
{
  free(xmlpoint->xml);
  free(xmlpoint);
}
