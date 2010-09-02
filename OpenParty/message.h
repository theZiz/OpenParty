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
typedef struct smessage *pmessage;
typedef struct smessage {
  float alter;
  char* inhalt;
  pmessage next;
} tmessage;

pmessage messages=NULL;

void new_message(char* inhalt)
{
  pmessage next=messages;
  messages=(pmessage)malloc(sizeof(tmessage));
  messages->inhalt=(char*)malloc(strlen(inhalt)+1);
  sprintf(messages->inhalt,"%s",inhalt);
  messages->alter=0;
  messages->next=next;
}

void calc_messages(int steps)
{
  pmessage *beforenextptr=&messages;
  while (*beforenextptr!=NULL)
  {
    (*beforenextptr)->alter+=((float)(steps))/1000.0;
    if ((*beforenextptr)->alter>=1.5)
    {
      pmessage temp=*beforenextptr;
      *beforenextptr=(*beforenextptr)->next;
      free(temp->inhalt);
      free(temp);
    }
    else
      beforenextptr=&((*beforenextptr)->next);
  }
}

void draw_messages(void)
{
  pmessage message=messages;
  while (message!=NULL)
  {
    float faktor=1.5-message->alter;
    if (faktor>1.0) faktor=1.0;
    glColor4f(0,0,0,0.7*faktor);
    ZWdrawtextmiddle(text,0,message->alter*1.5,-5,message->inhalt,1.00);
    glColor4f(1,1,1,faktor);
    ZWdrawtextmiddle(text,0,message->alter*1.5,-5,message->inhalt,0.98);
    message=message->next;
  }  
}

void delete_all_messages(void)
{
  while (messages!=NULL)
  {
    pmessage temp=messages->next;
    free(messages->inhalt);
    free(messages);
    messages=temp;
  }
}
