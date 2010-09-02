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
void add_new_sample(char* name,int next_play,int plusminus)
{
  psample newsample=(psample)malloc(sizeof(tsample));
  newsample->next=first_sample;
  first_sample=newsample;
  char buffer[256];
  sprintf(buffer,DATAFOLDER"sounds/%s",name);
  first_sample->chunk=Mix_LoadWAV(buffer);
	int volume=(int)(trunc(4.0*global_volume));
	if (global_volume>0.05 && volume==0)
    volume=1;
	Mix_VolumeChunk(first_sample->chunk,volume);
  
  first_sample->next_play=next_play;
  if (plusminus>=next_play)
    plusminus=next_play-1;
  first_sample->plusminus=plusminus;
  first_sample->real_next_play=rand()%(next_play+1);
}

void calc_samples(int steps)
{
  psample now=first_sample;
  while (now!=NULL)
  {
    now->real_next_play-=steps;
    if (now->real_next_play<=0)
    {
      Mix_PlayChannel(-1,now->chunk,0);
      now->real_next_play=now->next_play+rand()%(now->plusminus*2+1)-now->plusminus;
    }
    now=now->next;
  }
}

void delete_all_samples(void)
{
  while (first_sample!=NULL)
  {
    Mix_FreeChunk(first_sample->chunk);
    psample next=first_sample->next;
    free(first_sample);
    first_sample=next;
  }
}
