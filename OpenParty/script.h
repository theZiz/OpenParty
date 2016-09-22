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
ptermelem addterm(ptermelem beforeelem,char* term)
{
  int a;
  int depth;
  int b;
  int leer;
  int verschub=0;
  ptermelem firstelem=NULL;
  ptermelem lastelem=NULL;
  for (a=0;term[a]!=0;a++)
  {
    float wert=0;
    char art=term[a];
    char* function=NULL;
    int begin=a;
    char temp;
    switch (art) //Wenn "besondere" Dinge passieren sollen
    {
      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        //Ende der Zahl suchen
        for (a++;term[a]!=0;a++)
          if ((term[a]<'0' || term[a]>'9') && term[a]!='.') break;
        temp=term[a];
        term[a]=0;
        wert=atof(&(term[begin]));
        term[a]=temp;
        art='f';
        a--;
      break;
      case '[': //Variable
        //Ende der Variable suchen
        for (a++;term[a]!=0;a++)
          if (term[a]==']') break;
        temp=term[a];
        term[a]=0;
        function=(char*)malloc(strlen(&(term[begin+1]))+1);
        sprintf(function,"%s",&(term[begin+1]));
        term[a]=temp;
        wert=0;
        art='v';
      break;
      case '"': //Text
        //Ende des Textes suchen
        for (a++;term[a]!=0;a++)
          if (term[a]=='"') break;
        temp=term[a];
        term[a]=0;
        function=(char*)malloc(strlen(&(term[begin+1]))+1);
        sprintf(function,"%s",&(term[begin+1]));
        term[a]=temp;
        wert=0;
        art='t';
      break;
      case '{'://Funktion
        //Ende der Funktion und etwaige Parameter suchen
        //Leerzeichen überspringen
        if (term[a+1]==' ')
        {
          a++;
          while (term[a]==' ') a++;
          begin=a-1;
        }  
        for (a++;term[a]!=0;a++) //Name auslesen
          if (term[a]==',' || term[a]=='}' || term[a]==' ') break;
        temp=term[a];
        term[a]=0;
        function=(char*)malloc(strlen(&(term[begin+1]))+1);
        sprintf(function,"%s",&(term[begin+1]));
        term[a]=temp; //Name ist nun bekannt und gespeichert    
        //a--;
      break;
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': 
      case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': 
      case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': 
      case 'y': case 'z': case '_': //schöne Funktion
        //Ende der Funktion und etwaige Parameter suchen
       for (a++;term[a]!=0;a++) //Name auslesen
          if (term[a]=='(' || term[a]==' ') break;
        temp=term[a];
        term[a]=0;
        function=(char*)malloc(strlen(&(term[begin]))+1);
        sprintf(function,"%s",&(term[begin]));
        term[a]=temp; //Name ist nun bekannt und gespeichert
        art='{';
        //Klammer auf suchen
        while (term[a]!='(')
          a++;
        //Klammer zu suchen
        depth=1;
        leer=1;
        for (b=a+1;term[b]!=0;b++)
        {
          if (term[b]=='(') depth++;
          if (term[b]==')')
          {
            depth--;
            if (depth==0)
              break;
          }
          if (term[b]!=' ') leer=0;
        }
        if (leer==0)
          term[a]='A'; //Sei Klammer auf
        else
          term[a]='B';  
        term[b]='C';
        //a--;
      break;
      case 'B':        
        term[a]='(';
        continue;
      break;
      case 'A':
        art=',';
        term[a]='(';
      break;
      case 'C':
        art='}';
        term[a]=')';
      break;
      case '>':
        if (term[a+1]=='=')
        {
          a++;
          art='g'; //_g_reatersame
        }
      break;  
      case '<':
        if (term[a+1]=='=')
        {
          a++;
          art='a'; //lessers_a_me
        }
        else
        if (term[a+1]=='>')
        {
          a++;
          art='!'; //unequal
        }        
      break;
      case ' ': continue;
    }
    ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
    elem->art=art;
    elem->wert=wert;
    elem->function=function;
    elem->next=NULL;
    elem->jump=NULL;
    elem->before=lastelem;
    if (lastelem==NULL)
      elem->nr=beforeelem->nr+1;
    else
      elem->nr=lastelem->nr+1;  
    if (firstelem!=NULL)
      lastelem->next=elem;
    else
    {
      firstelem=elem;
    }  
    lastelem=elem;
  }
  beforeelem->next=firstelem;
  firstelem->before=beforeelem;
  return lastelem;
}


ptermelem createscript(char* name,char* input,int paramanz) //Nimmt den String und gibt eine Ptermelem-Kette zurück
{
  char* scripttext=(char*)malloc(strlen(input)+1);
  int a;
  for (a=0;input[a]!=0;a++)
    if (input[a]>='A' && input[a]<='Z')
      scripttext[a]=input[a]-'A'+'a';
    else
      scripttext[a]=input[a];
  scripttext[a]=0;
  ptermelem script=(ptermelem)malloc(sizeof(ttermelem));
  script->function=(char*)malloc(strlen(name)+1);
  sprintf(script->function,"%s",name);
  script->before=NULL;
  script->next=NULL;
  script->jump=NULL;
  script->wert=paramanz;
  script->art='b';
  script->nr=0;
  ptermelem momelem=script;
  //Scripttext durchgehen, nach Schlüsselwörtern durchsuchen und Termelemte erstellen:
  int counter;
  for (counter=0;counter<strlen(scripttext);counter++)
  { 
    //Function
    if (scripttext[counter]=='{')
    {
      //Klammer zu suchen
      int finish;
      counter++;
      for (finish=counter;finish<strlen(scripttext);finish++)
        if (scripttext[finish]=='}' || scripttext[finish]==',') break;
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='{';
      elem->wert=0;
      elem->jump=NULL;
      elem->nr=momelem->nr+1;
      char temp=scripttext[finish];
      scripttext[finish]=0;
      elem->function=(char*)malloc(strlen(&(scripttext[counter]))+1);
      sprintf(elem->function,"%s",&(scripttext[counter]));
      scripttext[finish]=temp;
      if (scripttext[finish]==',') finish++;
      elem->before=momelem;
      momelem->next=elem;
      elem->next=NULL;
      counter=finish;//counter zeigt auf Komma oder die geschlossene geschweifte Klammer
      int depth=1;
      while (scripttext[finish]!=0)
      {
        if (scripttext[finish]=='}')
        {
          depth--;
          if (depth==0) break;
        }
        if (scripttext[finish]=='{') depth++;
        finish++;
      }
      temp=scripttext[finish];
      scripttext[finish]=0;
      if (finish!=counter)
        momelem=addterm(elem,&(scripttext[counter]));
      else
        momelem=elem;
      scripttext[finish]=temp;
      //Klammer anfügen
      elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='}';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      counter=finish;
      continue;
    }
    //set
    if (scripttext[counter+0]=='s' && scripttext[counter+1]=='e' && scripttext[counter+2]=='t' &&
       (scripttext[counter+3]==' ' || scripttext[counter+3]=='('))
    {
      momelem->next=(ptermelem)malloc(sizeof(ttermelem));
      momelem->next->before=momelem;
      momelem=momelem->next;
      momelem->nr=momelem->before->nr+1;
      momelem->art='s';
      momelem->wert=0;
      momelem->jump=NULL;
      //Klammer auf suchen
      for (counter+=3;counter<strlen(scripttext);counter++)
        if (scripttext[counter]=='(') break;
      //Komma suchen
      int finish;
      counter++;
      for (finish=counter;finish<strlen(scripttext);finish++)
        if (scripttext[finish]==',') break;
      char temp=scripttext[finish];
      scripttext[finish]=0;
      momelem->function=(char*)malloc(strlen(&(scripttext[counter]))+1);
      sprintf(momelem->function,"%s",&(scripttext[counter]));
      scripttext[finish]=temp;
      //Klammer zu suchen
      counter=finish+1;
      int depth=1;
      for (finish=counter;finish<strlen(scripttext);finish++)
      {
        if (scripttext[finish]=='(') depth++;
        if (scripttext[finish]==')')
        {
          depth--;
          if (depth==0)
            break;
        }
      }
      temp=scripttext[finish];
      scripttext[finish]=0;
      momelem=addterm(momelem,&(scripttext[counter]));
      scripttext[finish]=temp;
      //Klammer anfügen
      momelem->next=(ptermelem)malloc(sizeof(ttermelem));
      momelem->next->before=momelem;
      momelem=momelem->next;
      momelem->nr=momelem->before->nr+1;
      momelem->art='#';
      momelem->wert=0;
      momelem->jump=NULL;
      momelem->function=NULL;
      counter=finish;      
      continue;
    }
    
    //while Statement
    if (scripttext[counter+0]=='w' && scripttext[counter+1]=='h' && scripttext[counter+2]=='i' &&
        scripttext[counter+3]=='l' && scripttext[counter+4]=='e')
    {
      //Klammer auf suchen
      for (counter+=5;counter<strlen(scripttext);counter++)
        if (scripttext[counter]=='(') break;
      //Klammer zu suchen
      int finish;
      counter++;
      int depth=1;
      for (finish=counter;finish<strlen(scripttext);finish++)
      {
        if (scripttext[finish]=='(') depth++;
        if (scripttext[finish]==')')
        {
          depth--;
          if (depth==0)
            break;
        }
      }
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='w';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      char temp=scripttext[finish];
      scripttext[finish]=0;
      momelem=addterm(elem,&(scripttext[counter]));
      scripttext[finish]=temp;
      //Klammer anfügen
      elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='#';
      elem->wert=0;
      elem->jump=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      momelem->function=NULL;
      counter=finish;
      continue;
    }
    //elihw Statement
    if (scripttext[counter+0]=='e' && scripttext[counter+1]=='l' && scripttext[counter+2]=='i' &&
        scripttext[counter+3]=='h' && scripttext[counter+4]=='w')
    {
      counter+=4;
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='x';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      continue;
    }
    //sync Statement
    if (scripttext[counter+0]=='s' && scripttext[counter+1]=='y' && scripttext[counter+2]=='n' && scripttext[counter+3]=='c')
    {
      counter+=3;
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='y';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      continue;
    }
    //delay Statement
    if (scripttext[counter+0]=='d' && scripttext[counter+1]=='e' && scripttext[counter+2]=='l' &&
        scripttext[counter+3]=='a' && scripttext[counter+4]=='y')
    {
      //Klammer auf suchen
      for (counter+=5;counter<strlen(scripttext);counter++)
        if (scripttext[counter]=='(') break;
      //Klammer zu suchen
      int finish;
      counter++;
      int depth=1;
      for (finish=counter;finish<strlen(scripttext);finish++)
      {
        if (scripttext[finish]=='(') depth++;
        if (scripttext[finish]==')')
        {
          depth--;
          if (depth==0)
            break;
        }
      }
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='p';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      char temp=scripttext[finish];
      scripttext[finish]=0;
      momelem=addterm(elem,&(scripttext[counter]));
      scripttext[finish]=temp;
      //Klammer anfügen
      elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='#';
      elem->wert=0;
      elem->jump=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      momelem->function=NULL;
      counter=finish;
      continue;
    }
    //od Statement
    if (scripttext[counter+0]=='o' && scripttext[counter+1]=='d')
    {
      //Klammer auf suchen
      for (counter+=2;counter<strlen(scripttext);counter++)
        if (scripttext[counter]=='(') break;
      //Klammer zu suchen
      int finish;
      counter++;
      int depth=1;
      for (finish=counter;finish<strlen(scripttext);finish++)
      {
        if (scripttext[finish]=='(') depth++;
        if (scripttext[finish]==')')
        {
          depth--;
          if (depth==0)
            break;
        }
      }
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='o';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      char temp=scripttext[finish];
      scripttext[finish]=0;
      momelem=addterm(elem,&(scripttext[counter]));
      scripttext[finish]=temp;
      //Klammer anfügen
      elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='#';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      counter=finish;
      continue;
    }
    //do Statement
    if (scripttext[counter+0]=='d' && scripttext[counter+1]=='o')
    {
      counter+=1;
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='d';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      continue;
    }
    //if Statement
    if (scripttext[counter+0]=='i' && scripttext[counter+1]=='f')
    {
      //Klammer auf suchen
      for (counter+=2;counter<strlen(scripttext);counter++)
        if (scripttext[counter]=='(') break;
      //Klammer zu suchen
      int finish;
      counter++;
      int depth=1;
      for (finish=counter;finish<strlen(scripttext);finish++)
      {
        if (scripttext[finish]=='(') depth++;
        if (scripttext[finish]==')')
        {
          depth--;
          if (depth==0)
            break;
        }
      }
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='i';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      char temp=scripttext[finish];
      scripttext[finish]=0;
      momelem=addterm(elem,&(scripttext[counter]));
      scripttext[finish]=temp;
      //Klammer anfügen
      elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='#';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      counter=finish;
      continue;
    }
    //else Statement
    if (scripttext[counter+0]=='e' && scripttext[counter+1]=='l' && scripttext[counter+2]=='s' && scripttext[counter+3]=='e')
    {
      counter+=3;
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='l';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      continue;
    }
    //fi Statement
    if (scripttext[counter+0]=='f' && scripttext[counter+1]=='i')
    {
      counter+=1;
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='j';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      continue;
    }
    //$ Statement
    if (scripttext[counter+0]=='$')
    {
      counter+=0;
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='$';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      continue;
    }
    //return Statement
    if (scripttext[counter+0]=='r' && scripttext[counter+1]=='e' && scripttext[counter+2]=='t' &&
        scripttext[counter+3]=='u' && scripttext[counter+4]=='r' && scripttext[counter+5]=='n')
    {
      //Klammer auf suchen
      for (counter+=6;counter<strlen(scripttext);counter++)
        if (scripttext[counter]=='(') break;
      //Klammer zu suchen
      int finish;
      counter++;
      int depth=1;
      for (finish=counter;finish<strlen(scripttext);finish++)
      {
        if (scripttext[finish]=='(') depth++;
        if (scripttext[finish]==')')
        {
          depth--;
          if (depth==0)
            break;
        }
      }
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='r';
      elem->wert=0;
      elem->jump=NULL;
      elem->function=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      char temp=scripttext[finish];
      scripttext[finish]=0;
      momelem=addterm(elem,&(scripttext[counter]));
      scripttext[finish]=temp;
      //Klammer anfügen
      elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='#';
      elem->wert=0;
      elem->jump=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      momelem->function=NULL;
      counter=finish;
      continue;
    }
    //schöne Function
    if ((scripttext[counter]>='a' && scripttext[counter]<='z') || scripttext[counter]=='_')
    {
      //Klammer auf suchen
      int finish;
      for (finish=counter;finish<strlen(scripttext);finish++)
        if (scripttext[finish]=='(' || scripttext[finish]==' ') break;
      ptermelem elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='{';
      elem->wert=0;
      elem->jump=NULL;
      elem->nr=momelem->nr+1;
      char temp=scripttext[finish];
      scripttext[finish]=0;
      elem->function=(char*)malloc(strlen(&(scripttext[counter]))+1);
      sprintf(elem->function,"%s",&(scripttext[counter]));
      scripttext[finish]=temp;
      elem->before=momelem;
      momelem->next=elem;
      elem->next=NULL;
      while (scripttext[finish]==' ')
        finish++;
      finish++;
      counter=finish;//counter zeigt auf kurz nach die offene Klammer
      //geschlossene Klammer suchen!
      int depth=1;
      while (scripttext[finish]!=0)
      {
        if (scripttext[finish]==')')
        {
          depth--;
          if (depth==0) break;
        }
        if (scripttext[finish]=='(') depth++;
        finish++;
      }
      temp=scripttext[finish];
      scripttext[finish]=0;
      if (finish!=counter)
        momelem=addterm(elem,&(scripttext[counter]));
      else
        momelem=elem;
      scripttext[finish]=temp;
      //Klammer anfügen
      elem=(ptermelem)malloc(sizeof(ttermelem));
      elem->art='}';
      elem->wert=0;
      elem->jump=NULL;
      elem->nr=momelem->nr+1;
      elem->before=momelem;
      momelem->next=elem;
      momelem=elem;
      momelem->function=NULL;
      counter=finish;
      continue;
    }

  }
  momelem->next=(ptermelem)malloc(sizeof(ttermelem));
  momelem->next->before=momelem;
  momelem->next->nr=momelem->nr+1;
  momelem=momelem->next;
  momelem->art='^';
  momelem->jump=NULL;
  momelem->function=NULL;
  momelem->next=NULL;
  momelem->wert=0;

  /*momelem=script; //Ausgabe für Debugzwecke
  while (momelem!=NULL)
  {
    printf("%c",momelem->art);
    if (momelem->function!=NULL)
      printf("  %s\n",momelem->function);
		else
		  printf("\n");
   
    momelem=momelem->next;
  }*/
  
  //script durchgehen und zu allen whiles,elihw, ods, ifs und elses die "JUMP-Adressen" suchen:
  momelem=script;
  while (momelem!=NULL)
  {
    if (momelem->art=='w')
    {
      ptermelem endelem=momelem->next;
      int depth=1;
      while (endelem!=NULL)
      {
        if (endelem->art=='w') depth++;
        if (endelem->art=='x') 
        {
          depth--;
          if (depth==0) break;
        }
        endelem=endelem->next;
      }
      momelem->jump=endelem;
      endelem->jump=momelem;
    }
    if (momelem->art=='o')
    {
      ptermelem endelem=momelem->before;
      int depth=1;
      while (endelem!=NULL)
      {
        if (endelem->art=='o') depth++;
        if (endelem->art=='d') 
        {
          depth--;
          if (depth==0) break;
        }
        endelem=endelem->before;
      }
      momelem->jump=endelem;
    }
    if (momelem->art=='i')
    {
      ptermelem endelem=momelem->next;
      int depth=1;
      while (endelem!=NULL)
      {
        if (endelem->art=='i') depth++; //anderes if
        if (endelem->art=='l') //else
        {
          if (depth==1)
            break;
        }
        if (endelem->art=='j') //fi
        {
          depth--;
          if (depth==0)
            break;
        }
        endelem=endelem->next;
      }
      momelem->jump=endelem;
    }
    if (momelem->art=='l')
    {
      ptermelem endelem=momelem->next;
      int depth=1;
      while (endelem!=NULL)
      {
        if (endelem->art=='i') depth++;
        if (endelem->art=='j') 
        {
          depth--;
          if (depth==0) break;
        }
        endelem=endelem->next;
      }
      momelem->jump=endelem;
    }
    
    momelem=momelem->next;
  }
  free(scripttext);
  return script;
}

ptermelem konkatiniere(ptermelem eins,ptermelem zwei)
{
  if (eins==NULL) return zwei;
  //Ende von eins suchen
  ptermelem now=eins;
  while (now->next!=NULL) now=now->next;
  now->next=zwei;
  zwei->before=now;
  int dazu=now->nr+1;
  now=zwei;
  while (now!=NULL) 
  {
    now->nr+=dazu;
    now=now->next;
  }
  return eins;
}

pcompiledblock createblock(ptermelem term)
{
  ptermelem now=term;
  while (now->next!=NULL) now=now->next;
  int blockanz=now->nr+2;
  pblockelem block=(pblockelem)malloc(sizeof(tblockelem)*blockanz);
  now=term;
  int pos=0;
  int wert=0;
  int momstacksize=0;
  pknownvar vars=NULL;
  pknownvar newvar;
  pfunctab functab=NULL;
  pfunctab ft;
  long int *varanz;
  pjumpstack jumpstack=NULL;
  pjumpstack tempstack;
  while (now!=NULL)
  {
    switch (now->art)
    {
      case 'b': //Funktionsbeginn
        block[pos].art='b';
        block[pos].param.wert_i=now->wert;
        momstacksize=block[pos].param.wert_i;
        block[pos].jump=momstacksize; //Testwert!
        varanz=&(block[pos].jump);
        //etwaige vorherige germerkte Variablen freigeben:
        while (vars!=NULL)
        {
          pknownvar temp=vars->next;
          free(vars);
          vars=temp;
        }
        //Funktion in die Lookup-table eintragen:
        ft=(pfunctab)malloc(sizeof(tfunctab));
        ft->name=now->function;
        ft->jump=pos;
        ft->next=functab;
        functab=ft;
        pos++;
      break;
      case '{': //Funktion
        block[pos].art='{';
        block[pos].param.wert_i=0;
        block[pos].jump=(long int)now->function;
        tempstack=(pjumpstack)malloc(sizeof(tjumpstack));
        tempstack->nr=pos;
        tempstack->next=jumpstack;
        jumpstack=tempstack;
        pos++;
      break;
      case 'v':
        //Suchen, ob die Variable überhaupt definiert ist
        newvar=vars;
        while (newvar!=NULL)
        {
          if (strcmp(newvar->name,now->function)==0) break;
          newvar=newvar->next;
        }
        if (newvar==NULL) //Variablen existiert nicht
        {
          block[pos].art='f';
          block[pos].param.wert_f=0;        
        }
        else
        {
          block[pos].art='v';
          block[pos].param.wert_i=newvar->nr;     
        }
        block[pos].jump=0;        
        pos++;
      break;
      case 't':
        //Text in Texttabelle suchen
        for (wert=0;wert<textcount;wert++)
          if (strcmp(textlist[wert]->name,now->function)==0) break;
        if (wert==textcount) wert=-1;
        block[pos].art='f';
        block[pos].param.wert_f=(float)wert;
        block[pos].jump=0;
        pos++;        
      break;
      case 's':
        //Suchen, ob die Variable überhaupt definiert ist
        newvar=vars;
        while (newvar!=NULL)
        {
          if (strcmp(newvar->name,now->function)==0) break;
          newvar=newvar->next;
        }
        if (newvar==NULL) //Variablen existiert nicht
        {
          block[pos].jump=momstacksize;
          newvar=(pknownvar)malloc(sizeof(tknownvar));
          newvar->name=now->function;
          newvar->next=vars;
          if (vars==NULL) 
            newvar->nr=momstacksize;
          else
          {
            newvar->nr=vars->nr+1;
          }
          (*varanz)++;
          vars=newvar;         
        }
        block[pos].jump=newvar->nr;        
        block[pos].art='s';
        block[pos].param.wert_i=0;
        tempstack=(pjumpstack)malloc(sizeof(tjumpstack));
        tempstack->nr=pos;
        tempstack->next=jumpstack;
        jumpstack=tempstack;        
        pos++;
      break;
      case 'p':
        block[pos].jump=0; //NULL 
        block[pos].art='p';
        block[pos].param.wert_i=0;
        tempstack=(pjumpstack)malloc(sizeof(tjumpstack));
        tempstack->nr=pos;
        tempstack->next=jumpstack;
        jumpstack=tempstack;        
        pos++;
      break;
      case 'j': case 'd': //fi und do
      //Nichts... Werden nicht mit aufgenommen
      break;
      case 'w': case 'o':
        block[pos].art=now->art;
        block[pos].param.wert_f=now->wert;
        if (now->jump==NULL)
          block[pos].jump=0;
        else
          block[pos].jump=now->jump->nr;
        tempstack=(pjumpstack)malloc(sizeof(tjumpstack));
        tempstack->nr=pos;
        tempstack->next=jumpstack;
        jumpstack=tempstack;
        pos++;
      break;
      case 'i': case '(': case 'r': //gleich wie while und od, testweise abgekapselt
        block[pos].art=now->art;
        block[pos].param.wert_f=now->wert;
        if (now->jump==NULL)
          block[pos].jump=0;
        else
          block[pos].jump=now->jump->nr;
        tempstack=(pjumpstack)malloc(sizeof(tjumpstack));
        tempstack->nr=pos;
        tempstack->next=jumpstack;
        jumpstack=tempstack;
        pos++;
      break;
      case ',':// case '>': case '<': case 'g': case 'a': case '=': case '!':
        block[pos].art=now->art;
        block[pos].param.wert_f=now->wert;
        if (now->jump==NULL)
          block[pos].jump=0;
        else
          block[pos].jump=now->jump->nr+1;
        if (jumpstack!=NULL)
        {
          block[jumpstack->nr].param.wert_i=pos;
          jumpstack->nr=pos;
        }
        pos++;
      break;
      case '#': case ')': case '}':
        block[pos].art=now->art;
        block[pos].param.wert_f=now->wert;
        if (now->jump==NULL)
          block[pos].jump=0;
        else
          block[pos].jump=now->jump->nr+1;
        if (jumpstack!=NULL)
        {
          block[jumpstack->nr].param.wert_i=pos;
          tempstack=jumpstack->next;
          free(jumpstack);
          jumpstack=tempstack;
        }
        pos++;        
      break;
      default:
        block[pos].art=now->art;
        block[pos].param.wert_f=now->wert;
        if (now->jump==NULL)
          block[pos].jump=0;
        else
        {
          block[pos].jump=now->jump->nr;
          if (block[pos].art=='l') block[pos].jump++;
        }
        pos++;  
    }
    now=now->next;
  }
  block[pos].art='~';
  block[pos].param.wert_f=0;
  block[pos].jump=0;
  int a;
  for (a=0;a<pos;a++)
    if (block[a].art=='{')
    {
      pfunctab ft=functab;
      while (ft!=NULL)
      {
        if (strcmp(ft->name,(char*)block[a].jump)==0) break;
        ft=ft->next;
      }
      if (ft==NULL) //nicht gefunden. Sinus oder so?
      {
        block[a].jump=getexfunctionnumber((char*)block[a].jump);
          //Negativ == KEIN Sprung, sondern eine externe Funktion, -1 Sonderfall => No match
      }
      else
      {
        block[a].jump=ft->jump;
      }    
    }

  while (term!=NULL)
  {
    ptermelem temp=term->next;
    if (term->function!=NULL && term->art!='b')
      free(term->function);     
    free(term);
    term=temp;
  }
  while (vars!=NULL)
  {
    pknownvar temp=vars->next;
    free(vars);
    vars=temp;
  }
 
  pcompiledblock compiledblock=(pcompiledblock)malloc(sizeof(tcompiledblock));
  compiledblock->block=block;
  compiledblock->functab=functab;
	/*pfunctab _functab=compiledblock->functab;
	while (_functab!=NULL)
	{
		printf("%s : %i\n",_functab->name,functab->jump);
		_functab=_functab->next;
	}
	for (int a=0;a<pos;a++)
	  printf("%4.i: %c Jump: %i Wert: %i / %f\n",a,block[a].art,block[a].jump,block[a].param.wert_i,block[a].param.wert_f);*/
  return compiledblock; 
}

void deleteblock(pcompiledblock compiledblock)
{
  if (compiledblock==NULL) return;
  free(compiledblock->block);
  pfunctab now=compiledblock->functab;
  while (now!=NULL)
  {
    pfunctab temp=now->next;
    free(now->name);
    free(now);
    now=temp;
  }
  free(compiledblock);
}

float callfunction(pblockelem block,int *pos,float* *stackptr);

float solveterm(pblockelem block,int von,int bis,float* stack)
{
  float wert=0;
  float summe=0;
  float produkt=0;
  char lastvz='+';
  int pos=von;
  while (pos<=bis && block[pos].art!='<' && block[pos].art!='>' && block[pos].art!='g' && block[pos].art!='a'
                  && block[pos].art!='=' && block[pos].art!='!' && block[pos].art!='&' && block[pos].art!='|')
  {
    switch (block[pos].art)
    {
      case 'f':
        wert=block[pos].param.wert_f;
        pos++;
      break;
      case 'v':
        wert=stack[block[pos].param.wert_i];
        pos++;
      break;
      case '(':
        wert=solveterm(block,pos+1,block[pos].param.wert_i-1,stack);
        pos=block[pos].param.wert_i+1;
      break;
      case '{':
        if (block[pos].jump>=0) //interne Funktion
        {
          int paranz=block[block[pos].jump].param.wert_i;
          int newpos=block[pos].jump;
          float *newstack=NULL;
          //if (paranz>0)
					//printf("%i\n",block[block[pos].jump].jump);
					if (block[block[pos].jump].jump>0)
            newstack=(float*)malloc(sizeof(float)*block[block[pos].jump].jump);
					//printf("%i %i\n",paranz,block[block[pos].jump].jump);
          int parnr=0;
          //pos=block[pos].param.wert_i;//Sprung zum 1. Komma oder '}'
          for (;parnr<paranz && block[pos].art!='}';parnr++)
          {
            newstack[parnr]=solveterm(block,pos+1,block[pos].param.wert_i-1,stack);
            pos=block[pos].param.wert_i;
          }
          wert=callfunction(block,&newpos,&newstack);
          free(newstack);
          while (block[pos].art!='}') pos=block[pos].param.wert_i;
          pos++;
        }
        else
        if (block[pos].jump<-1) //externe Funktion
        {        
          int paranz=getexfunctionparamanz(block[pos].jump);
          int newpos=block[pos].jump;
          float *newstack=NULL;
          if (paranz>0)
            newstack=(float*)malloc(sizeof(float)*paranz);
          int parnr=0;
          //pos=block[pos].param.wert_i;
          for (;parnr<paranz && block[pos].art!='}';parnr++)
          {
            newstack[parnr]=solveterm(block,pos+1,block[pos].param.wert_i-1,stack);
            pos=block[pos].param.wert_i;
          }
          if (newpos==-14)
            wert=stack[(int)(trunc(newstack[0]))];
          else
          {
            wert=callexfunction(newpos,newstack);
          }
          free(newstack);            
          while (block[pos].art!='}') pos=block[pos].param.wert_i;
          pos++;
        }
        else
        {
          wert=0;
          pos++;
        }
      break;
      case '+': case '-':
        if (lastvz=='+')
          summe+=wert;
        else
        if (lastvz=='-')
          summe-=wert;
        else
        if (lastvz=='*')
          summe+=produkt*wert;
        else
        if (lastvz=='/')
          summe+=produkt/wert;
        else
        if (lastvz=='%')
          summe+=((produkt/wert)-trunc(produkt/wert))*wert; //Anpassen!          
        lastvz=block[pos].art;
        pos++;
      break;
      case '*': case '/': case '%':
        if (lastvz=='+')
          produkt=wert;
        else
        if (lastvz=='-')
          produkt=-wert;
        else
        if (lastvz=='*')
          produkt*=wert;
        else
        if (lastvz=='/')
          produkt/=wert;
        else
        if (lastvz=='%')
          produkt=((produkt/wert)-trunc(produkt/wert))*wert; //Anpassen!          
        lastvz=block[pos].art;
        pos++;
      break;
      default:
        pos++;
    }
  }
  if (lastvz=='+')
    summe+=wert;
  else
  if (lastvz=='-')
    summe-=wert;
  else
  if (lastvz=='*')
    summe+=produkt*wert;
  else
  if (lastvz=='/')
    summe+=produkt/wert;
  else
  if (lastvz=='%')
    summe+=((produkt/wert)-trunc(produkt/wert))*wert; //Anpassen!          

  if (block[pos].art=='<')
  {
    float summe2=solveterm(block,pos+1,bis,stack);
    if (summe<summe2) summe=1; else summe=-1;
  } else
  if (block[pos].art=='>')
  {
    float summe2=solveterm(block,pos+1,bis,stack);
    if (summe>summe2) summe=1; else summe=-1;
  } else
  if (block[pos].art=='a')
  {
    float summe2=solveterm(block,pos+1,bis,stack);
    if (summe<=summe2) summe=1; else summe=-1;
  } else
  if (block[pos].art=='g')
  {
    float summe2=solveterm(block,pos+1,bis,stack);
    if (summe>=summe2) summe=1; else summe=-1;
  } else
  if (block[pos].art=='=')
  {
    float summe2=solveterm(block,pos+1,bis,stack);
    if (summe==summe2) summe=1; else summe=-1;
  } else
  if (block[pos].art=='!')
  {
    float summe2=solveterm(block,pos+1,bis,stack);
    if (summe!=summe2) summe=1; else summe=-1;
  } else
  if (block[pos].art=='&')
  {
    float summe2=solveterm(block,pos+1,bis,stack);
    if (summe<summe2) summe=summe; else summe=summe2;
  } else
  if (block[pos].art=='|')
  { 
    float summe2=solveterm(block,pos+1,bis,stack);
    if (summe<summe2) summe=summe2; else summe=summe;
  }
  return summe;
}

float callfunction(pblockelem block,int *pos,float* *stackptr)
{
  int end;
  float wert=0;
	float* stack=*stackptr;
  while (block[*pos].art!='^')
  {
    switch (block[*pos].art)
    {
      case 's': //set
        stack[block[*pos].jump]=solveterm(block,(*pos)+1,block[*pos].param.wert_i-1,stack);
        *pos=block[*pos].param.wert_i+1;
      break;
      case 'w': case 'o': case 'i': //while, od, if
        wert=solveterm(block,(*pos)+1,block[*pos].param.wert_i-1,stack);
        if (wert>0) //true
        {
          *pos=block[*pos].param.wert_i;
          while (block[*pos].art!='#') *pos=block[*pos].param.wert_i;
          (*pos)++;
        }
        else
          *pos=block[*pos].jump+1;
      break;      
      case 'x': case 'l': //elihw and else
        *pos=block[*pos].jump;
      break;
      case 'r': //return
        wert=solveterm(block,(*pos)+1,block[*pos].param.wert_i-1,stack);
        *pos=-1;
        return wert;
      break;
      case 'y': //sync
        wert=0;
        return wert;
      break;
      case 'p': //delay
        wert=solveterm(block,(*pos)+1,block[*pos].param.wert_i-1,stack);
        *pos=block[*pos].param.wert_i;
        return wert;
      break;
      case '{': //Funktion
        end=block[*pos].param.wert_i;
        while (block[end].art!='}') end=block[end].param.wert_i;
        solveterm(block,(*pos),block[*pos].param.wert_i,stack);
        *pos=end;
      break;
			case '$': //nachfolgende Funktion �berschreibt die jetzige.
			  if (block[*pos+1].art=='{')
				{
					(*pos)=block[*pos+1].jump;
					free(stack);
					stack=(float*)malloc(sizeof(float)*block[*pos].jump);
					(*stackptr)=stack;
				}
				else
				  (*pos)++;
			break;
      default: (*pos)++;
    }
  }
  *pos=-1;
  return wert;
}
