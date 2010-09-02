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
void deleteallsets(void)
{

  pset now=firstset;
  while (now!=NULL)
  {
    pset temp=now;
    free(temp->object);
    free(temp->type);
    if (temp->triggerfunction_in!=NULL)
      free(temp->triggerfunction_in);
    if (temp->triggerfunction_out!=NULL)
      free(temp->triggerfunction_out);
    if (temp->endfunction!=NULL)
      free(temp->endfunction);
    if (temp->name!=NULL)
      free(temp->name);
    now=now->next;
    free(temp);
  }
  firstset=NULL;
  lastset=NULL;
  int a;
  for (a=0;a<2048;a++)
    setbynr[a]=NULL; 
}

void newsetbypobject(pobject object,char* type ,char* triggerfunction_in ,
                 char* triggerfunction_out ,char* endfunction ,tZWcolor color,
                 tZWpoint position ,tZWpoint rotation ,tZWpoint scale ,tZWpoint cameraposition ,tZWpoint camerarotation,
                 int nr,char* name,int gamenextnr,int alwaysvisible)
{
  pset set=(pset)malloc(sizeof(tset));
  if (strcmp(type,"ghost")==0)
    set->object=NULL;
  else  
  {
    set->object=(char*)malloc(strlen(object->name)+1);
    sprintf(set->object,"%s",object->name);
  }
  set->realobject=object;
  set->type=(char*)malloc(strlen(type)+1);  
  sprintf(set->type,"%s",type);
  if (triggerfunction_in!=NULL)
  {
    set->triggerfunction_in=(char*)malloc(strlen(triggerfunction_in)+1);
    sprintf(set->triggerfunction_in,"%s",triggerfunction_in);
  }  
  else
    set->triggerfunction_in=NULL;
  if (triggerfunction_out!=NULL)
  {
    set->triggerfunction_out=(char*)malloc(strlen(triggerfunction_out)+1);
    sprintf(set->triggerfunction_out,"%s",triggerfunction_out);
  }  
  else
    set->triggerfunction_out=NULL;
  if (endfunction!=NULL)  
  {
    set->endfunction=(char*)malloc(strlen(endfunction)+1);
    sprintf(set->endfunction,"%s",endfunction);
  } 
  else
    set->endfunction=NULL;  
  if (name!=NULL)  
  {
    set->name=(char*)malloc(strlen(name)+1);
    sprintf(set->name,"%s",name);
  } 
  else
    set->name=NULL;  
  set->color=color;
  set->alwaysvisible=alwaysvisible;
  set->position=position;
  set->rotation=rotation;
  set->scale=scale;
  set->cameraposition=cameraposition;
  set->camerarotation=camerarotation;
  set->triggertrap=-1;
  if (nr<0 || nr>2047 || setbynr[nr]!=NULL)
  {
    int a; //freie Nummer suchen
    for (a=0;a<2047;a++)
      if (setbynr[a]==NULL) break;
    setbynr[a]=set;  
    set->nr=a;
  }
  else
  {
    set->nr=nr;
    setbynr[nr]=set;
  }

  set->next=NULL; 
  if (gamenextnr>=0 && gamenextnr<2048)
    set->gamenextnr=gamenextnr;    
  else  
    set->gamenextnr=0;    
  set->before=lastset;
  if (lastset!=NULL && firstset!=NULL) 
    lastset->next=set;
  else
    firstset=set;
  lastset=set;
  if (object==NULL)
    set->maxy=1.0;
  else
    set->maxy=object->maxy;
}

void newset(char* objectname ,char* type ,char* triggerfunction_in ,
            char* triggerfunction_out ,char* endfunction ,tZWcolor color,
            tZWpoint position ,tZWpoint rotation ,tZWpoint scale ,tZWpoint cameraposition ,tZWpoint camerarotation,
            int nr,char* name,int gamenextnr,int alwaysvisible)
{
  //Schauen, ob das Object mit dem Namen "objectname" überhaupt existiert.
  if (strcmp(type,"ghost")==0)
  {
    color.a=0.35;
    newsetbypobject(NULL,type,NULL,NULL,NULL,color,position,rotation,scale,cameraposition,
                    camerarotation,nr,name,gamenextnr,alwaysvisible);
    return;
  }
  pobject now=firstobject;
  while (now!=NULL)
  {
    if (strcmp(now->name,objectname)==0) break;
    now=now->next;
  }
  if (strcmp(now->name,objectname)==0)
    newsetbypobject(now,type,triggerfunction_in,triggerfunction_out,endfunction,color,position,rotation,scale,cameraposition,camerarotation,
                    nr,name,gamenextnr,alwaysvisible);
}

int teile(int links,int rechts)
{
  int i = links;
  int j = rechts - 1;
  float pivot = setlist[rechts]->abstand;
  do {
    while (setlist[i]->abstand <= pivot && i < rechts) 
      i++; 
    while (setlist[j]->abstand >= pivot && j > links)
      j--;
    if (i < j) 
    {
      pset temp=setlist[i];
      setlist[i]=setlist[j];
      setlist[j]=temp;
    }
  } while (i < j);
  if (setlist[i]->abstand > pivot )
  {
      pset temp=setlist[i];
      setlist[i]=setlist[rechts];
      setlist[rechts]=temp;    
  }
  return i;
}

void sortsetlist(int links,int rechts)
{
  if (links<rechts)
  {
    int teiler=teile(links,rechts);
    sortsetlist(links,teiler-1);
    sortsetlist(teiler+1,rechts);
  }
}

void recalcsetlist(float x,float y,float z,pmaindata data)
{
  int a;
  int visiblecounter=0;
  //Normalenvektor bestimmen
  glRotatef(-data->momcamerarotation.x,1,0,0);
  glRotatef(-data->momcamerarotation.y,0,1,0);
  glRotatef(-data->momcamerarotation.z,0,0,1);
  float matrix[4][4];
  glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)matrix);
  float vektor[3];
  vektor[0]=-matrix[0][2];
  vektor[1]=-matrix[1][2];
  vektor[2]=-matrix[2][2];
  float betrag=sqrt(vektor[0]*vektor[0]+vektor[1]*vektor[1]+vektor[2]*vektor[2]);
  vektor[0]/=betrag;
  vektor[1]/=betrag;
  vektor[2]/=betrag;
  glRotatef(data->momcamerarotation.x,1,0,0);
  glRotatef(data->momcamerarotation.y,0,1,0);
  glRotatef(data->momcamerarotation.z,0,0,1);

	tZWpoint ballpos;
	switch (data->camera_looking_at)
	{
		case -1: ballpos=data->player[data->activplayer].momposition; break;
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: ballpos=data->player[data->camera_looking_at].momposition; break;
		case 8: ballpos=data->manuell_camera_point; break;
	}

  //Abstände berechnen und eintragen
  for (a=0;a<maxsetlist;a++)
  {
    float faktor=1.0;
    if (setlist[a]->alwaysvisible==0 && data->mainloop_pos!=5 && !(data->mainloop_pos==4 && data->item_in_script==3))
      faktor=getobjectalpha(ballpos,data->momcameraposition,setlist[a]->position,2.0,1.5);
			//faktor=newgetobjectalpha(data->momcameraposition,vektor,setlist[a]->position,2.0,1.5);
    setlist[a]->realalpha=setlist[a]->color.a*faktor;
		if (setlist[a]->alwaysvisible==0 && (data->mainloop_pos==5 || (data->mainloop_pos==4 && data->item_in_script==3)) && !data->map_show_env && strcmp(setlist[a]->type,"none")==0) //Map
		  setlist[a]->realalpha*=0.4;
    float distance=fabs((setlist[a]->position.x-data->momcameraposition.x)*vektor[0]+
                        (setlist[a]->position.y-data->momcameraposition.y)*vektor[1]+
                        (setlist[a]->position.z-data->momcameraposition.z)*vektor[2]);
    if (setlist[a]->realalpha==1.0)
      setlist[a]->abstand=distance;
    else
      setlist[a]->abstand=100000.0-distance;
    if (setlist[a]->alwaysvisible==1)
    {
      pset temp=setlist[visiblecounter];
      setlist[visiblecounter]=setlist[a];
      setlist[a]=temp;
      visiblecounter++;
    }
  }
  sortsetlist(visiblecounter,minalphasetlist-1);
  sortsetlist(minalphasetlist,maxsetlist-1);
}

void refreshsetlist(void)
{
  if (setlist!=NULL)
    free(setlist);
  //sets zählen
  if (firstset==NULL)
  {
    setlist=NULL;
    return;
  }
  pset momset=firstset;
  int anz=0;
  while (momset!=NULL)
  {
    anz++;
    momset=momset->next;
  }
  maxsetlist=anz;
  //Speicher allokieren
  setlist=(pset*)malloc(sizeof(pset*)*anz);
  //Liste iterieren und Pointer einsortieren
  momset=firstset;
  anz=0;
  int anz2=maxsetlist;
  while (momset!=NULL)
  {
    if (momset->color.a==1.0)
    {
      setlist[anz]=momset;
      anz++;
    }
    else
    {
      anz2--;
      setlist[anz2]=momset;
    }
    minalphasetlist=anz2;
    momset=momset->next;
  }
}

pset getsetbyname(char *name)
{
  pset now=firstset;
  while (now!=NULL)
  {
    if (now->name!=NULL && strcmp(name,now->name)==0)
      return now;
    now=now->next;
  }
  return NULL;
}

float calcnewdestface(pset momset)
{
  if (momset!=NULL)
  {
    float v1x=(setbynr[momset->gamenextnr])->position.x-momset->position.x;
    float v1z=(setbynr[momset->gamenextnr])->position.z-momset->position.z;
    if (v1x==0 && v1z==0)
    {
      v1x=(setbynr[momset->gamenextnr])->position.x-momset->position.x;
      v1z=(setbynr[momset->gamenextnr])->position.z-momset->position.z;
    }

    float v2x=momset->cameraposition.x-momset->position.x;
    float v2z=momset->cameraposition.z-momset->position.z;
    float b1=sqrt(v1x*v1x+v1z*v1z);
    float b2=sqrt(v2x*v2x+v2z*v2z);
    float winkel=90;
    if (b1*b2!=0)
      winkel=acos((v1x*v2x+v1z*v2z)/(b1*b2))*180.0/M_PI;
    if (v1x*v2z-v1z*v2x<0) winkel=-winkel;
    return winkel;
  }
  return 0;
}
