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
int loadlevel(char *filename)
{
  if (script!=NULL)
    deleteblock(script);
  script=NULL;
  deleteallsets();
  deleteallobjects();
  deletealllights();
  deletealltexts();
  deletetextcontent();  	
  resetborder();
	while (firstitem!=NULL)
	{
		free(firstitem->description);
		free(firstitem->name);
		free(firstitem->object_name);
		free(firstitem->script);
		pitem temp=firstitem->next;
		free(firstitem);
		firstitem=temp;
	}
	firstitem=NULL;
  skyboxart=0;
  if (backgroundmusic!=NULL)
    Mix_FreeMusic(backgroundmusic);
  backgroundmusic=NULL;
  char buffer[256];
  sprintf(buffer,DATAFOLDER"levels/%s",filename);
  delete_all_samples();
  if (not(ZWfileexists(buffer))) return 1;
  ptermelem firsttermelem=NULL;
  
  pxmlpoint xmlpoint=ZWloadxmlfile(buffer);
  pxmltag maintag=ZWgetnexttag(xmlpoint);
  //Untertags 1. Ordnung durchgehen:
  pxmltag first=ZWgetnexttag(maintag->inhalt);
  while (first!=NULL)
  {
   if (strcmp(first->name,"sound")==0)
    {
      //Untertag 2. Ordnung von "objects" durchgehen.
      pxmltag second=ZWgetnexttag(first->inhalt);
      while (second!=NULL)
      {
        if (strcmp(second->name,"background")==0)
        {
          pxmlattribute attribute=second->attribute;//etwaiges erstes Attribut für Schleife
          while (attribute!=NULL)
          {
            if (strcmp(attribute->name,"filename")==0)
            {
              char filename[256];
              sprintf(filename,DATAFOLDER"sounds/%s",attribute->wert);
              backgroundmusic=Mix_LoadMUS(filename);
            }
            attribute=attribute->next;
          }        
        }
        if (strcmp(second->name,"sample")==0)
        {
          pxmlattribute attribute=second->attribute;//etwaiges erstes Attribut für Schleife
          char *filename=NULL;
          int next_play=1000;
          int plusminus=0;
          while (attribute!=NULL)
          {
            if (strcmp(attribute->name,"filename")==0)
              filename=attribute->wert;
            if (strcmp(attribute->name,"play_all")==0)
              next_play=atoi(attribute->wert);
            if (strcmp(attribute->name,"plusminus")==0)
              plusminus=atoi(attribute->wert);
            attribute=attribute->next;
          }
          add_new_sample(filename,next_play,plusminus);
        }
        ZWfreetag(second);
        second=ZWgetnexttag(first->inhalt);
      }    
    }
    if (strcmp(first->name,"objects")==0)
    {
      //Untertag 2. Ordnung von "objects" durchgehen.
      pxmltag second=ZWgetnexttag(first->inhalt);
      while (second!=NULL)
      {
        pxmlattribute attribute=second->attribute;//etwaiges erstes Attribut für Schleife
        char *name;
        char filename[256],texture[256];
        bool simple=false;
        while (attribute!=NULL)
        {
          if (strcmp(attribute->name,"name")==0)
            name=attribute->wert;  
          if (strcmp(attribute->name,"filename")==0)
            sprintf(filename,DATAFOLDER"models/%s",attribute->wert);
          if (strcmp(attribute->name,"texture")==0)
            sprintf(texture,DATAFOLDER"textures/%s",attribute->wert);
          if (strcmp(attribute->name,"light")==0)
            simple=(strcmp(attribute->wert,"simple")==0);
          attribute=attribute->next;
        }
        newobject(filename,name,texture,simple);
        ZWfreetag(second);
        second=ZWgetnexttag(first->inhalt);
      }
    }
    if (strcmp(first->name,"design")==0)
    {
      pxmlattribute attribute=first->attribute;//etwaiges erstes Attribut für Schleife
      diceangle=0;
      while (attribute!=NULL)
      {
        if (strcmp(attribute->name,"diceangle")==0)
          diceangle=atof(attribute->wert);
        attribute=attribute->next;  
      }
    
    
      pxmltag second=ZWgetnexttag(first->inhalt);
      while (second!=NULL)
      {
        if (strcmp(second->name,"skysphere")==0)
        {
          skyboxart=1;
          if (second->attribute!=NULL && strcmp(second->attribute->name,"texture")==0)
          {
            char buffer[512];
            sprintf(buffer,DATAFOLDER"textures/%s",second->attribute->wert);
            sprintf(sky.texture,"%s",second->attribute->wert);
            GLuint tex=ZWloadtexturefromfileA(buffer,0,texquali);
            ZWload3dm(&(sky.sky),(char*)DATAFOLDER"models/skysphere.3dm",tex);
            int a;
            for (a=0;a<sky.sky.polygonnum;a++)//Umdrehen der Polygone
            {
              int b;
              for (b=0;b<sky.sky.polohne[a].panz/2;b++)
              {
                int temp=sky.sky.polohne[a].p[b];
                sky.sky.polohne[a].p[b]=sky.sky.polohne[a].p[sky.sky.polohne[a].panz-b-1];
                sky.sky.polohne[a].p[sky.sky.polohne[a].panz-b-1]=temp;
              }   
            }
            ZWcreatedrawlist(&(sky.sky),1);    
            pxmltag third=ZWgetnexttag(second->inhalt);
            while (third!=NULL)
            {
              if (strcmp(third->name,"size")==0)
              {
                sky.size.x=1;
                sky.size.y=1;          
                sky.size.z=1;
                pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
                while (attribute!=NULL)
                {
                  if (strcmp(attribute->name,"x")==0)
                    sky.size.x=atof(attribute->wert)/2.0;
                  if (strcmp(attribute->name,"y")==0)
                    sky.size.y=atof(attribute->wert)/2.0;
                  if (strcmp(attribute->name,"z")==0)
                    sky.size.z=atof(attribute->wert)/2.0;
                  if (strcmp(attribute->name,"r")==0)
                  {
                    sky.size.x=atof(attribute->wert);
                    sky.size.y=atof(attribute->wert);
                    sky.size.z=atof(attribute->wert);
                  }  
                  attribute=attribute->next;
                }
              }
              ZWfreetag(third);
              third=ZWgetnexttag(second->inhalt);
            }
          }
        }
        if (strcmp(second->name,"redskybox")==0)
        {
          skyboxart=3;
          if (second->attribute!=NULL && strcmp(second->attribute->name,"texture")==0)
          {
            char buffer[512];
            sprintf(buffer,DATAFOLDER"textures/%s",second->attribute->wert);
            sprintf(sky.texture,"%s",second->attribute->wert);
            GLuint tex=ZWloadtexturefromfileA(buffer,0,texquali);
            ZWload3dm(&(sky.sky),(char*)DATAFOLDER"models/redskybox.3dm",tex);
            int a;
            for (a=0;a<sky.sky.polygonnum;a++)//Umdrehen der Polygone
            {
              int b;
              for (b=0;b<sky.sky.polohne[a].panz/2;b++)
              {
                int temp=sky.sky.polohne[a].p[b];
                sky.sky.polohne[a].p[b]=sky.sky.polohne[a].p[sky.sky.polohne[a].panz-b-1];
                sky.sky.polohne[a].p[sky.sky.polohne[a].panz-b-1]=temp;
              }   
            }
            ZWcreatedrawlist(&(sky.sky),1);    
            pxmltag third=ZWgetnexttag(second->inhalt);
            while (third!=NULL)
            {
              if (strcmp(third->name,"size")==0)
              {
                sky.size.x=1;
                sky.size.y=1;          
                sky.size.z=1;
                pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
                while (attribute!=NULL)
                {
                  if (strcmp(attribute->name,"x")==0)
                    sky.size.x=atof(attribute->wert)/2.0;
                  if (strcmp(attribute->name,"y")==0)
                    sky.size.y=atof(attribute->wert)/2.0;
                  if (strcmp(attribute->name,"z")==0)
                    sky.size.z=atof(attribute->wert)/2.0;
                  if (strcmp(attribute->name,"r")==0)
                  {
                    sky.size.x=atof(attribute->wert);
                    sky.size.y=atof(attribute->wert);
                    sky.size.z=atof(attribute->wert);
                  }  
                  attribute=attribute->next;
                }
              }
              ZWfreetag(third);
              third=ZWgetnexttag(second->inhalt);
            }
          }
        }
        if (strcmp(second->name,"skybox")==0)
        {
          skyboxart=2;
          if (second->attribute!=NULL && strcmp(second->attribute->name,"texture")==0)
          {
            char buffer[512];
            sprintf(buffer,DATAFOLDER"textures/%s",second->attribute->wert);
            sprintf(sky.texture,"%s",second->attribute->wert);            
            GLuint tex=ZWloadtexturefromfileA(buffer,0,texquali);
            ZWload3dm(&(sky.sky),(char*)DATAFOLDER"models/skybox.3dm",tex);
            int a;
            for (a=0;a<sky.sky.polygonnum;a++)//Umdrehen der Polygone
            {
              int b;
              for (b=0;b<sky.sky.polohne[a].panz/2;b++)
              {
                int temp=sky.sky.polohne[a].p[b];
                sky.sky.polohne[a].p[b]=sky.sky.polohne[a].p[sky.sky.polohne[a].panz-b-1];
                sky.sky.polohne[a].p[sky.sky.polohne[a].panz-b-1]=temp;
              }   
            }
            ZWcreatedrawlist(&(sky.sky),1);    
            pxmltag third=ZWgetnexttag(second->inhalt);
            while (third!=NULL)
            {
              if (strcmp(third->name,"size")==0)
              {
                sky.size.x=1;
                sky.size.y=1;          
                sky.size.z=1;
                pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
                while (attribute!=NULL)
                {
                  if (strcmp(attribute->name,"x")==0)
                    sky.size.x=atof(attribute->wert)/2.0;
                  if (strcmp(attribute->name,"y")==0)
                    sky.size.y=atof(attribute->wert)/2.0;
                  if (strcmp(attribute->name,"z")==0)
                    sky.size.z=atof(attribute->wert)/2.0;
                  if (strcmp(attribute->name,"r")==0)
                  {
                    sky.size.x=atof(attribute->wert);
                    sky.size.y=atof(attribute->wert);
                    sky.size.z=atof(attribute->wert);
                  }  
                  attribute=attribute->next;
                }
              }
              ZWfreetag(third);
              third=ZWgetnexttag(second->inhalt);
            }
          }
        }
        if (strcmp(second->name,"light")==0)
        {
          tZWpoint position;
            position.x=0;
            position.y=0;
            position.z=0;
          tZWcolor ambient;
            ambient.r=0;
            ambient.g=0;
            ambient.b=0;
          tZWcolor diffuse=ambient;
          pxmltag third=ZWgetnexttag(second->inhalt);
          while (third!=NULL)
          {
            if (strcmp(third->name,"position")==0)
            {
              pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"x")==0)
                  position.x=atof(attribute->wert);
                if (strcmp(attribute->name,"y")==0)
                  position.y=atof(attribute->wert);
                if (strcmp(attribute->name,"z")==0)
                  position.z=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            if (strcmp(third->name,"ambient")==0)
            {
              pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"r")==0)
                  ambient.r=atof(attribute->wert);
                if (strcmp(attribute->name,"g")==0)
                  ambient.g=atof(attribute->wert);
                if (strcmp(attribute->name,"b")==0)
                  ambient.b=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            if (strcmp(third->name,"diffuse")==0)
            {
              pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"r")==0)
                  diffuse.r=atof(attribute->wert);
                if (strcmp(attribute->name,"g")==0)
                  diffuse.g=atof(attribute->wert);
                if (strcmp(attribute->name,"b")==0)
                  diffuse.b=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            ZWfreetag(third);
            third=ZWgetnexttag(second->inhalt);
          }        
          newlight(position,ambient,diffuse);
        }
        if (strcmp(second->name,"set")==0)
        {
          //Attribute des "sets" Nullsetzen:
          tZWpoint position,rotation,scale,cameraposition,camerarotation;
          position.x=0; position.y=0; position.z=0;
          rotation.x=0; rotation.y=0; rotation.z=0;
          scale.x=1; scale.y=1; scale.z=1;
          cameraposition.x=0; cameraposition.y=0; cameraposition.z=0;
          camerarotation.x=0; camerarotation.y=0; camerarotation.z=0;
          tZWcolor color;
          color.r=1; color.g=1; color.b=1; color.a=1;
          char object[256],type[256],triggerfunction_in[256],triggerfunction_out[256],endfunction[256],
               name[256];
          int nr=-1;int gamenextnr=0;int alwaysvisible=0;
          object[0]=0; type[0]=0; triggerfunction_in[0]=0; triggerfunction_out[0]=0; endfunction[0]=0;
          name[0]=0;
          //Attribute des set-Tags durchgehen und u.U. setzen:
          pxmlattribute attribute=second->attribute;
          while (attribute!=NULL)
          {
            if (strcmp(attribute->name,"object")==0)
              sprintf(object,"%s",attribute->wert);
            if (strcmp(attribute->name,"type")==0)
              sprintf(type,"%s",attribute->wert);
            if (strcmp(attribute->name,"triggerfunction_in")==0)
              sprintf(triggerfunction_in,"%s",attribute->wert);
            if (strcmp(attribute->name,"triggerfunction_out")==0)
              sprintf(triggerfunction_out,"%s",attribute->wert);
            if (strcmp(attribute->name,"endfunction")==0)
              sprintf(endfunction,"%s",attribute->wert);
            if (strcmp(attribute->name,"name")==0)
              sprintf(name,"%s",attribute->wert);
            if (strcmp(attribute->name,"nr")==0)
              nr=atoi(attribute->wert);
            if (strcmp(attribute->name,"next")==0)
              gamenextnr=atoi(attribute->wert);
            if (strcmp(attribute->name,"alwaysvisible")==0)
            {
              if (strcmp(attribute->wert,(char*)"True")==0)
                alwaysvisible=1;
            }

            attribute=attribute->next;
          }
          //Durchgehen der Untertags von "set":
          pxmltag third=ZWgetnexttag(second->inhalt);
          while (third!=NULL)
          {
            if (strcmp(third->name,"color")==0)
            {
              pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"r")==0)
                  color.r=atof(attribute->wert);
                if (strcmp(attribute->name,"g")==0)
                  color.g=atof(attribute->wert);
                if (strcmp(attribute->name,"b")==0)
                  color.b=atof(attribute->wert);
                if (strcmp(attribute->name,"a")==0)
                  color.a=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            if (strcmp(third->name,"position")==0)
            {
              pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"x")==0)
                  position.x=atof(attribute->wert);
                if (strcmp(attribute->name,"y")==0)
                  position.y=atof(attribute->wert);
                if (strcmp(attribute->name,"z")==0)
                  position.z=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            if (strcmp(third->name,"rotation")==0)
            {
              pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"x")==0)
                  rotation.x=atof(attribute->wert);
                if (strcmp(attribute->name,"y")==0)
                  rotation.y=atof(attribute->wert);
                if (strcmp(attribute->name,"z")==0)
                  rotation.z=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            if (strcmp(third->name,"scale")==0)
            {
              pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"x")==0)
                  scale.x=atof(attribute->wert);
                if (strcmp(attribute->name,"y")==0)
                  scale.y=atof(attribute->wert);
                if (strcmp(attribute->name,"z")==0)
                  scale.z=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            if (strcmp(third->name,"cameraposition")==0)
            {
              pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"x")==0)
                  cameraposition.x=atof(attribute->wert);
                if (strcmp(attribute->name,"y")==0)
                  cameraposition.y=atof(attribute->wert);
                if (strcmp(attribute->name,"z")==0)
                  cameraposition.z=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            if (strcmp(third->name,"camerarotation")==0)
            {
              pxmlattribute attribute=third->attribute;//etwaiges erstes Attribut für Schleife
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"x")==0)
                  camerarotation.x=atof(attribute->wert);
                if (strcmp(attribute->name,"y")==0)
                  camerarotation.y=atof(attribute->wert);
                if (strcmp(attribute->name,"z")==0)
                  camerarotation.z=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            ZWfreetag(third);
            third=ZWgetnexttag(second->inhalt);
          } 
          char* triggerdummy_in=NULL;
          if (triggerfunction_in[0]!=0)
            triggerdummy_in=triggerfunction_in;
          char* triggerdummy_out=NULL;
          if (triggerfunction_out[0]!=0)
            triggerdummy_out=triggerfunction_out;
          char* enddummy=NULL;
          if (endfunction[0]!=0)
            enddummy=endfunction;
          char* namedummy=NULL;
          if (name[0]!=0)
            namedummy=name;
          if (strcmp("ghost",type)==0)
            newset(NULL,type,NULL,NULL,NULL,color,position,rotation,scale,cameraposition,
                   camerarotation,nr,namedummy,gamenextnr,alwaysvisible);          
          else  
            newset(object,type,triggerdummy_in,triggerdummy_out,enddummy,color,position,rotation,scale,
                   cameraposition,camerarotation,nr,namedummy,gamenextnr,alwaysvisible);
        }
        ZWfreetag(second);
        second=ZWgetnexttag(first->inhalt);
        
      }
    }
    if (strcmp(first->name,"output")==0)
    {
      pxmltag second=ZWgetnexttag(first->inhalt);
      while (second!=NULL)
      {
        if (strcmp(second->name,"border")==0)
        {
          pxmltag third=ZWgetnexttag(second->inhalt);
          while (third!=NULL)
          {
            pZWcolor usedcolor=NULL;
            if (strcmp(third->name,"left")==0)
              usedcolor=&(border.left);
            if (strcmp(third->name,"right")==0)
              usedcolor=&(border.right);
            if (strcmp(third->name,"up")==0)
              usedcolor=&(border.up);
            if (strcmp(third->name,"down")==0)
              usedcolor=&(border.down);
            if (usedcolor!=NULL)
            {
              pxmlattribute attribute=third->attribute;
              while (attribute!=NULL)
              {
                if (strcmp(attribute->name,"r")==0)
                  usedcolor->r=atof(attribute->wert);
                if (strcmp(attribute->name,"g")==0)
                  usedcolor->g=atof(attribute->wert);
                if (strcmp(attribute->name,"b")==0)
                  usedcolor->b=atof(attribute->wert);
                attribute=attribute->next;
              }
            }
            ZWfreetag(third);
            third=ZWgetnexttag(second->inhalt);        
          }
          ZWfreetag(third);
        }
        if (strcmp(second->name,"text")==0)
        {
          ptext text=NULL;
          if (strcmp(second->attribute->name,"name")==0)
            text=newtext(second->attribute->wert);
          if (text!=NULL)
          {  
            pxmltag third=ZWgetnexttag(second->inhalt);
            while (third!=NULL)
            {
              //Inhalt auslesen
              char temp=third->inhalt->xml[third->inhalt->len];
              third->inhalt->xml[third->inhalt->len]=0;
              addlanguage(text,third->name,third->inhalt->xml);
              third->inhalt->xml[third->inhalt->len]=temp;   
              ZWfreetag(third);
              third=ZWgetnexttag(second->inhalt);        
            }
            ZWfreetag(third);
          }
        }        
        ZWfreetag(second);
        second=ZWgetnexttag(first->inhalt);        
      }
    }
    if (strcmp(first->name,"opl")==0)
    {
      pxmltag second=ZWgetnexttag(first->inhalt);
      while (second!=NULL)
      {
        if (strcmp(second->name,"script")==0)
        {
          char *scriptname=NULL;
          int paramanz=0;
          pxmlattribute attribute=second->attribute;
          while (attribute!=NULL)
          {
            if (strcmp(attribute->name,"name")==0)
              scriptname=attribute->wert;
            if (strcmp(attribute->name,"parameter")==0)
              paramanz=atoi(attribute->wert);
            attribute=attribute->next;
          }
          char temp=second->inhalt->xml[second->inhalt->len];
          second->inhalt->xml[second->inhalt->len]=0;
          ptermelem newscript;
          if (scriptname==NULL)
            newscript=createscript((char*)"no_name",second->inhalt->xml,paramanz);
          else
            newscript=createscript(scriptname,second->inhalt->xml,paramanz);
          second->inhalt->xml[second->inhalt->len]=temp;
          firsttermelem=konkatiniere(firsttermelem,newscript);
        }        
        ZWfreetag(second);
        second=ZWgetnexttag(first->inhalt);        
      }
    }      
    if (strcmp(first->name,"utils")==0)
    {
      pxmltag second=ZWgetnexttag(first->inhalt);
      while (second!=NULL)
      {
        if (strcmp(second->name,"item")==0)
        {
					char* name=NULL;
					char* script=NULL;
					char* object_name=NULL;
					char* description=NULL;
					int kind=0;
					tZWpoint scale;
					scale.x=1;
					scale.y=1;
					scale.z=1;
					int number=0;
					int price=0;
					char friendlyfire=0;
          int radius=0;
          pxmlattribute attribute=second->attribute;
          while (attribute!=NULL)
          {
            if (strcmp(attribute->name,"name")==0)
              name=attribute->wert;
            if (strcmp(attribute->name,"script")==0)
              script=attribute->wert;
            if (strcmp(attribute->name,"description")==0)
              description=attribute->wert;
            if (strcmp(attribute->name,"number")==0)
              number=atoi(attribute->wert);
            if (strcmp(attribute->name,"radius")==0)
              radius=atoi(attribute->wert);
            if (strcmp(attribute->name,"price")==0)
              price=atoi(attribute->wert);
            if (strcmp(attribute->name,"friendlyfire")==0)
						{
              if (strcmp(attribute->wert,(char*)"True")==0)
							  friendlyfire=1;
							else
								friendlyfire=0;
						}

            if (strcmp(attribute->name,"destination")==0)
						{
              if (strcmp(attribute->wert,(char*)"nothing")==0)
							  kind=0;
              if (strcmp(attribute->wert,(char*)"user")==0)
							  kind=1;
              if (strcmp(attribute->wert,(char*)"team")==0)
							  kind=2;
              if (strcmp(attribute->wert,(char*)"set")==0)
							  kind=3;
              if (strcmp(attribute->wert,(char*)"user_on_set")==0)
							  kind=4;
						}
            attribute=attribute->next;
          }
					pxmltag third=ZWgetnexttag(second->inhalt);
					while (third!=NULL)
					{
						if (strcmp(third->name,"model")==0)
						{
							pxmlattribute attribute=third->attribute;
							while (attribute!=NULL)
							{
								if (strcmp(attribute->name,"name")==0)
								{
									free(object_name);
									object_name=(char*)malloc(strlen(attribute->wert)+1);
									sprintf(object_name,"%s",attribute->wert);
								}
								attribute=attribute->next;
							}
							pxmltag fourth=ZWgetnexttag(third->inhalt);
							while (fourth!=NULL)
							{
								if (strcmp(fourth->name,"scale")==0)
								{
									pxmlattribute attribute=fourth->attribute;
									while (attribute!=NULL)
									{
										if (strcmp(attribute->name,"x")==0)
											scale.x=atof(attribute->wert);
										if (strcmp(attribute->name,"y")==0)
											scale.y=atof(attribute->wert);
										if (strcmp(attribute->name,"z")==0)
											scale.z=atof(attribute->wert);
										attribute=attribute->next;
									}
								}
								ZWfreetag(fourth);
								fourth=ZWgetnexttag(third->inhalt);
							}
						}
						ZWfreetag(third);
						third=ZWgetnexttag(second->inhalt);
					}
					if (name!=NULL && description!=NULL && script!=NULL && object_name!=NULL)
					{
						pitem newitem=(pitem)malloc(sizeof(titem));
						newitem->name=(char*)malloc(strlen(name)+1);
						newitem->description=(char*)malloc(strlen(description)+1);
						newitem->script=(char*)malloc(strlen(script)+1);
						newitem->object_name=object_name;
						sprintf(newitem->name,"%s",name);
						sprintf(newitem->description,"%s",description);
						sprintf(newitem->script,"%s",script);
						newitem->object_scale=scale;
						newitem->kind=kind;
						newitem->number=number;
						newitem->price=price;
						newitem->friendlyfire=friendlyfire;
            newitem->radius=radius;
						newitem->next=firstitem;
						firstitem=newitem;
					}
					else
					  free(object_name);
        }        
        ZWfreetag(second);
        second=ZWgetnexttag(first->inhalt);        
      }
    }      
    ZWfreetag(first);
    first=ZWgetnexttag(maintag->inhalt);
  }
  ZWfreetag(first);
  ZWfreetag(maintag);
  ZWfinishloading(xmlpoint);

  //Texte in Textlist einordnen
  if (textlist!=NULL)
    free(textlist);
  textlist=NULL;
  ptext momtext=firsttext;
  int anz=0;
  while (momtext!=NULL)
  {
    anz++;
    momtext=momtext->next;
  }
  if (anz!=0)
  {
    textlist=(stext**)malloc(sizeof(stext*)*anz);
    anz=0;
    momtext=firsttext;
    while (momtext!=NULL)
    {
      textlist[anz]=momtext;
      anz++;
      momtext=momtext->next;
    }  
  }
  textcount=anz;
  //Kompilieren
  if (firsttermelem!=NULL)
    script=createblock(firsttermelem);
  //Alle Sets durchgehen und Einsprung setzen
  pset momset=firstset;
  while (momset!=NULL)
  {
    if (momset->triggerfunction_in!=NULL)
    {
      //triggerfunction in lookuptable suchen
      momset->triggerjump_in=0;
      pfunctab momjump=script->functab;
      while (momjump!=NULL)
      {
        if (strcmp(momjump->name,momset->triggerfunction_in)==0)
        {
          momset->triggerjump_in=momjump->jump;
          break;
        }
        momjump=momjump->next;
      }
    }
    if (momset->triggerfunction_out!=NULL)
    {
      //triggerfunction in lookuptable suchen
      momset->triggerjump_out=0;
      pfunctab momjump=script->functab;
      while (momjump!=NULL)
      {
        if (strcmp(momjump->name,momset->triggerfunction_out)==0)
        {
          momset->triggerjump_out=momjump->jump;
          break;
        }
        momjump=momjump->next;
      }
    }
    if (momset->endfunction!=NULL)
    {
      //endfunction in lookuptable suchen
      momset->endjump=0;
      pfunctab momjump=script->functab;
      while (momjump!=NULL)
      {
        if (strcmp(momjump->name,momset->endfunction)==0)
        {
          momset->endjump=momjump->jump;
          break;
        }
        momjump=momjump->next;
      }
    }
    momset=momset->next;
  }
	
	//Item durchgehen und Sprungpunkte suchen
	pitem momitem=firstitem;
	while (momitem!=NULL)
	{
	  pfunctab functab=script->functab;
		while (functab!=NULL)
		{
			if (strcmp(functab->name,momitem->script)==0)
				break;
			functab=functab->next;
		}
    if (functab==NULL)
		  momitem->script_pos=0;
		else
		  momitem->script_pos=functab->jump;
		pobject object=firstobject;
		while (object!=NULL)
		{
			if (strcmp(object->name,momitem->object_name)==0) break;
			object=object->next;
		}
		if (object==NULL)
		  momitem->object=firstobject;
		else
		  momitem->object=object;
		for (momitem->description_pos=textcount-1;momitem->description_pos>=0;momitem->description_pos--)
		  if (strcmp(textlist[momitem->description_pos]->name,momitem->description)==0)
			  break;
		momitem=momitem->next;
	}
	return 0;
}

