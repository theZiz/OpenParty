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
void alllightsoff(void)
{
  plight light=firstlight;
  while (light!=NULL)
  {
    ZWdisablelight(light->lightnr);
    light=light->next;
  }
}

void alllightson(void)
{
  plight light=firstlight;
  while (light!=NULL)
  {
    ZWenablelight(light->lightnr);
    light=light->next;
  }
}

void newlight(tZWpoint position,tZWcolor ambient,tZWcolor diffuse)
{
  plight light=(plight)malloc(sizeof(tlight));
  light->position=position;
  light->ambient=ambient;
  light->diffuse=diffuse;
  light->next=firstlight;
  light->lightnr=0;
  while (glIsEnabled(GL_LIGHT0+light->lightnr)!=0 && light->lightnr<256)
    light->lightnr++;   
  ZWsetlightposition(light->lightnr,position.x,position.y,position.z);
  ZWsetlightambient(light->lightnr,ambient.r,ambient.g,ambient.b);
  ZWsetlightdiffuse(light->lightnr,diffuse.r,diffuse.g,diffuse.b);
  ZWenablelight(light->lightnr);
  if (light->lightnr>maxlightnr) maxlightnr=light->lightnr;
  firstlight=light;
}

void refreshlightposition(void)
{
  plight light=firstlight;
  while (light!=NULL)
  {
		ZWenablelight(light->lightnr);
    ZWsetlightposition(light->lightnr,light->position.x,light->position.y,light->position.z);    
    ZWsetlightambient(light->lightnr,light->ambient.r,light->ambient.g,light->ambient.b);
    ZWsetlightdiffuse(light->lightnr,light->diffuse.r,light->diffuse.g,light->diffuse.b);
    light=light->next;
  }
}

void deletealllights(void)
{
  plight light=firstlight;
  while (light!=NULL)
  {
    plight temp=light;
    light=light->next;
    ZWdisablelight(temp->lightnr);  
    free(temp);
  }
  firstlight=NULL;
}
