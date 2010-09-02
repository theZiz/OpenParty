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
#ifdef FORINSTALL
  #define DATAFOLDER "/usr/share/openparty/"
#else
  #define DATAFOLDER "./"
#endif

#define JOYSTICK_MIN -16384
#define JOYSTICK_MAX  16383

#include "zizwindow.h"

#ifdef WIN32
  #define EXPORT DLLIMPORT
#else
  #define EXPORT
#endif

#include "./../texture/texture.h"

tZWattribute ZWattribute___;
pZWnewaxis ZWnewaxis___=NULL;
pZWnewkeyboard ZWnewkeyboard___=NULL;



EXPORT pZWattribute ZWgetattribute(void)
{
	return &ZWattribute___;
}

EXPORT void ZWinitparameter(void)
{
  ZWattribute___.strech=0;
  ZWattribute___.surface=NULL;
  ZWattribute___.info=NULL;
  ZWattribute___.width=640;
  ZWattribute___.height=480;
  ZWattribute___.bpp=16;
  ZWattribute___.sdlflags = SDL_OPENGL | SDL_DOUBLEBUF | SDL_HWPALETTE | SDL_RESIZABLE;
  ZWattribute___.initflags=SDL_INIT_VIDEO | SDL_INIT_JOYSTICK;
  ZWattribute___.glnear=0.1;
  ZWattribute___.glfar=100;
  ZWattribute___.perspektive=4.0f/3.0f; //Standardperspektive ist 4 zu 3
  ZWattribute___.on_resize=NULL;
  ZWattribute___.quit_function=NULL;
  ZWattribute___.newticks=0;
  ZWattribute___.oldticks=0;
	ZWattribute___.keyboard_joystick_num=0;
  memset(ZWattribute___.keypressed,0,512);
  memset(ZWattribute___.unicode,0,2*65536);
  ZWattribute___.minwait=10;
  ZWattribute___.pausescreen=0;
}

EXPORT void ZWinitsdl(void)
{
    if( SDL_Init(ZWattribute___.initflags) < 0 ) {
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
      SDL_Quit( );
      exit( 1 );
    }

    ZWattribute___.info = SDL_GetVideoInfo( );
    if( !ZWattribute___.info ) {
        fprintf( stderr, "Video query failed: %s\n",
             SDL_GetError( ) );
      SDL_Quit( );
      exit( 1 );
    }
    ZWattribute___.bpp = ZWattribute___.info->vfmt->BitsPerPixel;

    // Kann das Surface in den Speicher?
    if ( ZWattribute___.info->hw_available != 0)
      ZWattribute___.sdlflags = ZWattribute___.sdlflags | SDL_HWSURFACE;
    else
      ZWattribute___.sdlflags = ZWattribute___.sdlflags | SDL_SWSURFACE;
    // Wird hardware blitting unterstützt?
    if ( ZWattribute___.info->blit_hw != 0 ) ZWattribute___.sdlflags = ZWattribute___.sdlflags | SDL_HWACCEL;

    //alle Auflösungen speichern
    //Anzahl ermitteln
    SDL_Rect** modes;
    int i;
    modes = SDL_ListModes(NULL, SDL_FULLSCREEN|ZWattribute___.sdlflags);
    if (modes == (SDL_Rect**)0)
    {
      ZWattribute___.resolution=NULL;
      ZWattribute___.resolutioncount=0;
      return;
    }
    if (modes == (SDL_Rect**)-1)
    {
      ZWattribute___.resolution=NULL;
      ZWattribute___.resolutioncount=-1;
      return;
    }
    ZWattribute___.resolutioncount=0;
    for (i=0; modes[i]; ++i)
      ZWattribute___.resolutioncount++;
    ZWattribute___.resolution=(pZWresolution)malloc(sizeof(tZWresolution)*ZWattribute___.resolutioncount);
    for (i=0; modes[i]; ++i)
    {
      ZWattribute___.resolution[i].x=modes[i]->w;
      ZWattribute___.resolution[i].y=modes[i]->h;
    }
    
    //Schauen, ob das geladene überhaupt zulässig ist:
    if ((ZWattribute___.sdlflags & SDL_FULLSCREEN)>0)
    {
      //Auflösung suchen:
      int a;
      for (a=0;a<ZWattribute___.resolutioncount;a++)
        if (ZWattribute___.resolution[a].x==ZWattribute___.width && ZWattribute___.resolution[a].y==ZWattribute___.height)
          break;
      if (a==ZWattribute___.resolutioncount) //Auflösung nicht gefunden!
      {
        ZWattribute___.width=600;
        ZWattribute___.height=450;
        //Fullscreen entfernen:
        ZWattribute___.sdlflags=(ZWattribute___.sdlflags | SDL_FULLSCREEN)-SDL_FULLSCREEN;
      }
    }

    ZWattribute___.surface = SDL_SetVideoMode( ZWattribute___.width, ZWattribute___.height, ZWattribute___.bpp, ZWattribute___.sdlflags );
    if( ZWattribute___.surface == NULL ) {
        fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
      SDL_Quit( );
      exit( 1 );
    }
    SDL_ShowCursor(SDL_DISABLE);
    SDL_EnableUNICODE(SDL_ENABLE);
    //Joysticks einlesen
		int kjn=ZWattribute___.keyboard_joystick_num;
    ZWattribute___.realjoystickcount=SDL_NumJoysticks()+kjn; //zweie virtuelle Tastaturjoysticks
    ZWattribute___.realjoystick=(pZWrealjoystick)malloc(sizeof(tZWrealjoystick)*ZWattribute___.realjoystickcount);
    //Tasturen hinzufügen
		memset(ZWattribute___.key_mapped,-1,512);
		int a;
    pZWnewkeyboard momkeyboard=ZWnewkeyboard___;
		while (momkeyboard!=NULL)
    {
			a=momkeyboard->joynr;
      ZWattribute___.realjoystick[a].joy=NULL;
      ZWattribute___.realjoystick[a].buttoncount=4;
      ZWattribute___.realjoystick[a].only_axiscount=2;
      ZWattribute___.realjoystick[a].hatcount=0;
      ZWattribute___.realjoystick[a].button=(Uint8*)malloc(ZWattribute___.realjoystick[a].buttoncount*sizeof(Uint8));
			ZWattribute___.realjoystick[a].button_mapped_axis=NULL;
			ZWattribute___.realjoystick[a].button_mapped_direction=NULL;
      ZWattribute___.realjoystick[a].axis=(Sint16*)malloc(ZWattribute___.realjoystick[a].only_axiscount*sizeof(Sint16));
      ZWattribute___.realjoystick[a].axis_free=NULL;
      memset(ZWattribute___.realjoystick[a].button,0,ZWattribute___.realjoystick[0].buttoncount);
      memset(ZWattribute___.realjoystick[a].axis,0,ZWattribute___.realjoystick[0].only_axiscount*2);
      ZWattribute___.realjoystick[a].name=momkeyboard->name;
      ZWattribute___.realjoystick[a].axiscount=2;
			int b;
			for (b=0;b<8;b++)
			{
			  ZWattribute___.key_mapped[momkeyboard->key[b]]=a;
				ZWattribute___.key_dest[momkeyboard->key[b]]=b;
			}
			momkeyboard=momkeyboard->next;
    }
    //richtige Joysticks einlesen
		for (a=kjn;a<ZWattribute___.realjoystickcount;a++)
    {
      ZWattribute___.realjoystick[a].joy=SDL_JoystickOpen(a-kjn);
      ZWattribute___.realjoystick[a].buttoncount=SDL_JoystickNumButtons(ZWattribute___.realjoystick[a].joy);
      ZWattribute___.realjoystick[a].only_axiscount=SDL_JoystickNumAxes(ZWattribute___.realjoystick[a].joy);
      ZWattribute___.realjoystick[a].hatcount=SDL_JoystickNumHats(ZWattribute___.realjoystick[a].joy);
      ZWattribute___.realjoystick[a].button=(Uint8*)malloc(ZWattribute___.realjoystick[a].buttoncount*sizeof(Uint8));
      ZWattribute___.realjoystick[a].button_mapped_direction=(char*)malloc(ZWattribute___.realjoystick[a].buttoncount*sizeof(char));
			memset(ZWattribute___.realjoystick[a].button_mapped_direction,0,ZWattribute___.realjoystick[a].buttoncount);
      ZWattribute___.realjoystick[a].button_mapped_axis=(Uint8*)malloc(ZWattribute___.realjoystick[a].buttoncount*sizeof(Uint8));
      char* namebuffer=(char*)SDL_JoystickName(a-kjn);
			int virtuell_count=0;
			pZWnewaxis momaxis=ZWnewaxis___;
			while (momaxis!=NULL)
			{
				if (strcmp(momaxis->name,namebuffer)==0)
				{
					momaxis->joynr=a;
					momaxis->axisnr=(ZWattribute___.realjoystick[a].only_axiscount+ZWattribute___.realjoystick[a].hatcount*2)+virtuell_count;
					virtuell_count++;
					ZWattribute___.realjoystick[a].button_mapped_direction[momaxis->left]=-1;
					ZWattribute___.realjoystick[a].button_mapped_axis[momaxis->left]=momaxis->axisnr;
					ZWattribute___.realjoystick[a].button_mapped_direction[momaxis->right]=1;
					ZWattribute___.realjoystick[a].button_mapped_axis[momaxis->right]=momaxis->axisnr;
				}
				momaxis=momaxis->next;
			}
      ZWattribute___.realjoystick[a].axis=(Sint16*)malloc((ZWattribute___.realjoystick[a].only_axiscount+ZWattribute___.realjoystick[a].hatcount*2+virtuell_count)*sizeof(Sint16));
      ZWattribute___.realjoystick[a].axis_free=(char*)malloc(ZWattribute___.realjoystick[a].only_axiscount);
      memset(ZWattribute___.realjoystick[a].button,0,ZWattribute___.realjoystick[a].buttoncount);
      memset(ZWattribute___.realjoystick[a].axis,0,(ZWattribute___.realjoystick[a].only_axiscount+ZWattribute___.realjoystick[a].hatcount*2+virtuell_count)*sizeof(Sint16));
      memset(ZWattribute___.realjoystick[a].axis_free,1,ZWattribute___.realjoystick[a].only_axiscount);
      ZWattribute___.realjoystick[a].name=(char*)malloc(strlen(namebuffer)+1);
      sprintf(ZWattribute___.realjoystick[a].name,"%s",namebuffer);
      ZWattribute___.realjoystick[a].axiscount=ZWattribute___.realjoystick[a].only_axiscount+ZWattribute___.realjoystick[a].hatcount*2+virtuell_count;
    }
  ZWreset_virtuell_joytick();
}

EXPORT void ZWreset_virtuell_joytick(void)
{
  int a;
  for (a=0;a<8;a++)
  {
    int b;
    for (b=0;b<4;b++)
		{
      ZWattribute___.joystick[a].button[b]=(Uint8*)&(ZWattribute___.joystick[a].nullbutton[b]);
			ZWattribute___.joystick[a].nullbutton[b]=0;
		}
    for (b=0;b<2;b++)
		{
      ZWattribute___.joystick[a].axis[b]=(Sint16*)&(ZWattribute___.joystick[a].nullaxis[b]);  
			ZWattribute___.joystick[a].nullaxis[b]=0;
		}
  }
}

EXPORT void ZWinitopengl(void)
{
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    ZWenableculling();
    glEnable(GL_TEXTURE);  

    /* Licht*/
    //glEnable( GL_LIGHTING );
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_NORMALIZE);

    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.05);

/*    glActiveTexture(GL_TEXTURE1);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glActiveTexture(GL_TEXTURE0);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);*/
    
		if (ZWfileexists(DATAFOLDER"textures/keyboard.png"))
		{
			ZWattribute___.keyboardtexture=ZWloadtexturefromfileA(DATAFOLDER"textures/keyboard.png",1,ZWgetstdreduce());
			ZWattribute___.keyboardtexture_exists=1;
			ZWattribute___.keyboardtexture_position=0;
		}
		else
		  ZWattribute___.keyboardtexture_exists=0;

		if (ZWfileexists(DATAFOLDER"textures/pausescreen.png"))
		{
			ZWattribute___.pausetexture=ZWloadtexturefromfileA(DATAFOLDER"textures/pausescreen.png",1,ZWgetstdreduce());
			ZWattribute___.pausetexture_exists=1;
		}
		else
		  ZWattribute___.pausetexture_exists=0;
		

    ZWglResizeWindow( );

}

EXPORT void ZWquitsdl(void)
{
	  if (ZWattribute___.keyboardtexture_exists)
		  ZWdeletetexture(ZWattribute___.keyboardtexture);
    int a;
    for (a=0;a<ZWattribute___.realjoystickcount;a++)
    {
      if (a>=ZWattribute___.keyboard_joystick_num)
      {
        SDL_JoystickClose(ZWattribute___.realjoystick[a].joy);
        //free(ZWattribute___.realjoystick[a].axis_null_position);
				free(ZWattribute___.realjoystick[a].name); //bei der virtuellen Joysticks sp�ter
      }
      free(ZWattribute___.realjoystick[a].button);
      free(ZWattribute___.realjoystick[a].axis);
    }
  free(ZWattribute___.resolution);
	while (ZWnewaxis___!=NULL)
	{
		free(ZWnewaxis___->name);
		pZWnewaxis temp=ZWnewaxis___;
		ZWnewaxis___=ZWnewaxis___->next;
		free(temp);
	}
	while (ZWnewkeyboard___!=NULL)
	{
		free(ZWnewkeyboard___->name);
		pZWnewkeyboard temp=ZWnewkeyboard___;
		ZWnewkeyboard___=ZWnewkeyboard___->next;
		free(temp);
	}
  SDL_Quit();
}

EXPORT int ZWschleife(void (*draw_scene)(void* data),int (*calc_proc)(void* data),void *data,int size)
{
    Uint32 bigsteps=0;
    int done=0;
    Uint32 frames=0;
    int back=0;
    void *datacopy=NULL;
    if (size>0)
      datacopy=malloc(size);
    SDL_Thread *calcthread=NULL;
    Uint32 steps=0;
    while( done==0 ) {
        ZWattribute___.oldticks=ZWattribute___.newticks;
        ZWattribute___.newticks=SDL_GetTicks();
        ZWprocess_events( );
        steps+=ZWattribute___.newticks-ZWattribute___.oldticks;
        if (steps>ZWattribute___.minwait)
        {
          if (!ZWattribute___.pausescreen)
          {
            ZWattribute___.steps=steps;
            if (data!=NULL && size>0)
              memcpy(datacopy,data,size);
            if (calc_proc!=NULL)
              calcthread=SDL_CreateThread(calc_proc,data);
          }
          if (draw_scene!=NULL)
          {
            //Es hat sich gezeigt, dass hier eine kleine Pause DRINGEND von Nöten ist - vorallem beim Debuggen mit "gdb"
            SDL_Delay(1);
            ZWclearall();
            draw_scene(datacopy);
						//Keyboard anzeigen
						if (ZWattribute___.keyboardtexture_exists && ZWattribute___.keyboardtexture_position>0)
						{
							float dazu=0.8;
							if (ZWattribute___.keyboardtexture_position==1)
							  dazu*=-1.0;
              char was_enabled=1;
              if (!(glIsEnabled(GL_TEXTURE_2D)))
              {
                was_enabled=0;
                glEnable(GL_TEXTURE_2D);
              }
							glLoadIdentity();
							glDepthFunc(GL_ALWAYS);
							glDisable(GL_LIGHTING);
							ZWsetmomtexture(ZWattribute___.keyboardtexture);
              glBindTexture(GL_TEXTURE_2D,ZWattribute___.keyboardtexture);
							glColor4f(1,1,1,0.8);
							glBegin(GL_QUADS);
								glTexCoord2f(0,0);
								glVertex3f(-2.0, 0.5+dazu,-2.5);
								glTexCoord2f(0,1);
								glVertex3f(-2.0,-0.5+dazu,-2.5);
								glTexCoord2f(1,1);
								glVertex3f( 2.0,-0.5+dazu,-2.5);
								glTexCoord2f(1,0);
								glVertex3f( 2.0, 0.5+dazu,-2.5);
							glEnd();
							glDepthFunc(GL_LEQUAL); 
              if (!was_enabled)
                glDisable(GL_TEXTURE_2D);
						}
						if (ZWattribute___.pausetexture_exists && ZWattribute___.pausescreen)
						{
              char was_enabled=1;
              if (!(glIsEnabled(GL_TEXTURE_2D)))
              {
                was_enabled=0;
                glEnable(GL_TEXTURE_2D);
              }
							glLoadIdentity();
							glDepthFunc(GL_ALWAYS);
							glDisable(GL_LIGHTING);
							ZWsetmomtexture(ZWattribute___.pausetexture);
              glBindTexture(GL_TEXTURE_2D,ZWattribute___.pausetexture);
							glColor4f(1,1,1,1);
							glBegin(GL_QUADS);
								glTexCoord2f(0,0);
								glVertex3f(-1.8, 0.9,-2.5);
								glTexCoord2f(0,1);
								glVertex3f(-1.8,-0.9,-2.5);
								glTexCoord2f(1,1);
								glVertex3f( 1.8,-0.9,-2.5);
								glTexCoord2f(1,0);
								glVertex3f( 1.8, 0.9,-2.5);
							glEnd();
							glDepthFunc(GL_LEQUAL);
              if (!was_enabled)
                glDisable(GL_TEXTURE_2D);
						}
            ZWflipbuffer();
          }
          if (calc_proc!=NULL && !ZWattribute___.pausescreen)
          {
            SDL_WaitThread(calcthread,&back);
            if (back!=0) done=1;
          }
          frames++;
          bigsteps=bigsteps+steps;
          while (bigsteps>=1000)
          {
            bigsteps-=1000;
            ZWattribute___.fps=frames;
            frames=0;
          }
          steps=0;
        }
        else
        SDL_Delay(1);
    }
    if (datacopy!=NULL)
      free(datacopy);
    return back;
}

EXPORT int ZWschleifeex(void (*draw_scene)(void* data),int (*calc_proc)(void* data),int (*calc_procex)(void* data),void *data,int size)
{
    Uint32 bigsteps=0;
    int done=0;
    Uint32 frames=0;
    int back=0;
    void *datacopy=NULL;
    if (size>0)
      datacopy=malloc(size);
    SDL_Thread *calcthread=NULL;
    Uint32 steps=0;
    while( done==0 ) {
        ZWattribute___.oldticks=ZWattribute___.newticks;
        ZWattribute___.newticks=SDL_GetTicks();
        ZWprocess_events( );
        steps+=ZWattribute___.newticks-ZWattribute___.oldticks;
        if (steps>ZWattribute___.minwait) 
        {
          if (!ZWattribute___.pausescreen)
          {
            ZWattribute___.steps=steps;
            if (calc_procex!=NULL)
              back=calc_procex(data);
            if (data!=NULL && size>0)
              memcpy(datacopy,data,size);
            if (calc_proc!=NULL)
              calcthread=SDL_CreateThread(calc_proc,data);
          }
          if (draw_scene!=NULL)
          {
            //Es hat sich gezeigt, dass hier eine kleine Pause DRINGEND von Nöten ist - vorallem beim Debuggen mit "gdb"
            SDL_Delay(1);
            ZWclearall();
            draw_scene(datacopy);
						//Keyboard anzeigen
						if (ZWattribute___.keyboardtexture_exists && ZWattribute___.keyboardtexture_position>0)
						{
							float dazu=0.8;
							if (ZWattribute___.keyboardtexture_position==1)
							  dazu*=-1.0;
              char was_enabled=1;
              if (!(glIsEnabled(GL_TEXTURE_2D)))
              {
                was_enabled=0;
                glEnable(GL_TEXTURE_2D);
              }
							glLoadIdentity();
							glDepthFunc(GL_ALWAYS);
							glDisable(GL_LIGHTING);
							ZWsetmomtexture(ZWattribute___.keyboardtexture);
              glBindTexture(GL_TEXTURE_2D,ZWattribute___.keyboardtexture);
							glColor4f(1,1,1,0.8);
							glBegin(GL_QUADS);
								glTexCoord2f(0,0);
								glVertex3f(-2.0, 0.5+dazu,-2.5);
								glTexCoord2f(0,1);
								glVertex3f(-2.0,-0.5+dazu,-2.5);
								glTexCoord2f(1,1);
								glVertex3f( 2.0,-0.5+dazu,-2.5);
								glTexCoord2f(1,0);
								glVertex3f( 2.0, 0.5+dazu,-2.5);
							glEnd();
							glDepthFunc(GL_LEQUAL); 
              if (!was_enabled)
                glDisable(GL_TEXTURE_2D);
						}
						if (ZWattribute___.pausetexture_exists && ZWattribute___.pausescreen)
						{
              char was_enabled=1;
              if (!(glIsEnabled(GL_TEXTURE_2D)))
              {
                was_enabled=0;
                glEnable(GL_TEXTURE_2D);
              }
							glLoadIdentity();
							glDepthFunc(GL_ALWAYS);
							glDisable(GL_LIGHTING);
							ZWsetmomtexture(ZWattribute___.pausetexture);
              glBindTexture(GL_TEXTURE_2D,ZWattribute___.pausetexture);
							glColor4f(1,1,1,1);
							glBegin(GL_QUADS);
								glTexCoord2f(0,0);
								glVertex3f(-1.9, 0.95,-2.5);
								glTexCoord2f(0,1);
								glVertex3f(-1.9,-0.95,-2.5);
								glTexCoord2f(1,1);
								glVertex3f( 1.9,-0.95,-2.5);
								glTexCoord2f(1,0);
								glVertex3f( 1.9, 0.95,-2.5);
							glEnd();
							glDepthFunc(GL_LEQUAL); 
              if (!was_enabled)
                glDisable(GL_TEXTURE_2D);
						}
            ZWflipbuffer();
          } 
          if (!ZWattribute___.pausescreen)
          {
            if (calc_proc!=NULL)
            {
              int temp=back;
              SDL_WaitThread(calcthread,&back);
              if (temp!=0) back=temp;
              if (back!=0) done=1;
            }
            else
            if (calc_procex!=NULL)
              if (back!=0) done=1;
          }
          frames++;
          bigsteps=bigsteps+steps;
          while (bigsteps>=1000)
          {
            bigsteps-=1000;
            ZWattribute___.fps=frames;
            frames=0;
          }
          steps=0;
        }
        else
        SDL_Delay(1);
    }
    if (datacopy!=NULL)
      free(datacopy);
    return back;
}


EXPORT void ZWglResizeWindow(void)
{
// Verhindern von "Division by Zero"
  if ( ZWattribute___.height == 0 ) ZWattribute___.height = 1;
   
  // Viewport und Projektions-Matrix aktualisieren
  float faktor=((float)ZWattribute___.width/(float)ZWattribute___.height)/ZWattribute___.perspektive;
  if (ZWattribute___.strech==0)
  {
    if (faktor<1)
      glViewport(0                    , ((float)ZWattribute___.height-(float)ZWattribute___.height*faktor)/2.0 ,
                 ZWattribute___.width , ZWattribute___.height*faktor                                           );
    else
      glViewport(((float)ZWattribute___.width-(float)ZWattribute___.width/faktor)/2.0 , 0                     ,
                 ZWattribute___.width/faktor                                          , ZWattribute___.height );
  }
  else
  {
//    if (faktor<1)
//      glViewport(0,0, ZWattribute___.width, ZWattribute___.height);
//    else
      glViewport(0,0, ZWattribute___.width,ZWattribute___.height);
  }
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 60.0, (float)ZWattribute___.width/(faktor* (float)ZWattribute___.height), ZWattribute___.glnear, ZWattribute___.glfar );
  glMatrixMode( GL_MODELVIEW );
 
  // Rücksetzen der World-Matrix
  glLoadIdentity();
  if (ZWattribute___.on_resize!=NULL)
    ZWattribute___.on_resize();
}

EXPORT void ZWhandle_key_down( SDL_keysym* keysym )
{
    if (keysym->sym==SDLK_F1)
		  ZWattribute___.keyboardtexture_position=(ZWattribute___.keyboardtexture_position+1)%3;
    if (keysym->sym==SDLK_ESCAPE)
		  ZWattribute___.pausescreen=1-ZWattribute___.pausescreen;
    if (ZWattribute___.pausescreen && keysym->sym==SDLK_RETURN)
      if (ZWattribute___.quit_function!=NULL)
      {
        ZWattribute___.pausescreen=0;
        ZWattribute___.quit_function();
      }
		if (ZWattribute___.key_mapped[keysym->sym]>=0)
		{
			if (ZWattribute___.key_dest[keysym->sym]<4)
			  ZWattribute___.realjoystick[ZWattribute___.key_mapped[keysym->sym]].button[ZWattribute___.key_dest[keysym->sym]]=1;
			else
			switch (ZWattribute___.key_dest[keysym->sym])
			{
				case 4: ZWattribute___.realjoystick[ZWattribute___.key_mapped[keysym->sym]].axis[0]=-1; break;
				case 5: ZWattribute___.realjoystick[ZWattribute___.key_mapped[keysym->sym]].axis[0]= 1; break;
				case 6: ZWattribute___.realjoystick[ZWattribute___.key_mapped[keysym->sym]].axis[1]=-1; break;
				case 7: ZWattribute___.realjoystick[ZWattribute___.key_mapped[keysym->sym]].axis[1]= 1; break;
			}
		}

    ZWattribute___.keypressed[keysym->sym]=1; 
    ZWattribute___.unicode[keysym->sym]=keysym->unicode;
}

EXPORT void ZWhandle_key_up( SDL_keysym* keysym )
{
		if (ZWattribute___.key_mapped[keysym->sym]>=0)
		{
			if (ZWattribute___.key_dest[keysym->sym]<4)
			  ZWattribute___.realjoystick[ZWattribute___.key_mapped[keysym->sym]].button[ZWattribute___.key_dest[keysym->sym]]=0;
			else
			if (ZWattribute___.key_dest[keysym->sym]<6)
			  ZWattribute___.realjoystick[ZWattribute___.key_mapped[keysym->sym]].axis[0]=0;
			else
			  ZWattribute___.realjoystick[ZWattribute___.key_mapped[keysym->sym]].axis[1]=0;
		}
    ZWattribute___.keypressed[keysym->sym]=0; 
    ZWattribute___.unicode[keysym->sym]=0;
}

EXPORT void ZWprocess_events( void )
{
	  int kjn=ZWattribute___.keyboard_joystick_num;
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
        case SDL_JOYBUTTONDOWN:
				  if (ZWattribute___.realjoystick[event.jbutton.which+kjn].button_mapped_direction[event.jbutton.button]==0)
						ZWattribute___.realjoystick[event.jbutton.which+kjn].button[event.jbutton.button]=1;
					else
					  ZWattribute___.realjoystick[event.jbutton.which+kjn].axis[ZWattribute___.realjoystick[event.jbutton.which+kjn].button_mapped_axis[event.jbutton.button]]=ZWattribute___.realjoystick[event.jbutton.which+kjn].button_mapped_direction[event.jbutton.button];
        break;
        case SDL_JOYBUTTONUP:
				  if (ZWattribute___.realjoystick[event.jbutton.which+kjn].button_mapped_direction[event.jbutton.button]==0)
						ZWattribute___.realjoystick[event.jbutton.which+kjn].button[event.jbutton.button]=0;
					else
					  ZWattribute___.realjoystick[event.jbutton.which+kjn].axis[ZWattribute___.realjoystick[event.jbutton.which+kjn].button_mapped_axis[event.jbutton.button]]=0;
        break;
        case SDL_JOYAXISMOTION:
          if (event.jaxis.value<JOYSTICK_MIN && ZWattribute___.realjoystick[event.jaxis.which+kjn].axis_free[event.jaxis.axis])
          {
            ZWattribute___.realjoystick[event.jaxis.which+kjn].axis[event.jaxis.axis]=-1;
            ZWattribute___.realjoystick[event.jaxis.which+kjn].axis_free[event.jaxis.axis]=0;
          }
          else 
          if (event.jaxis.value>JOYSTICK_MAX && ZWattribute___.realjoystick[event.jaxis.which+kjn].axis_free[event.jaxis.axis])
          {
            ZWattribute___.realjoystick[event.jaxis.which+kjn].axis[event.jaxis.axis]= 1;
            ZWattribute___.realjoystick[event.jaxis.which+kjn].axis_free[event.jaxis.axis]=0;
          }
          else
          if (event.jaxis.value>JOYSTICK_MIN && event.jaxis.value<JOYSTICK_MAX)
          {
            ZWattribute___.realjoystick[event.jaxis.which+kjn].axis[event.jaxis.axis]= 0;
            ZWattribute___.realjoystick[event.jaxis.which+kjn].axis_free[event.jaxis.axis]=1;
          }
        break;
        case SDL_JOYHATMOTION:
          if ((event.jhat.value & SDL_HAT_LEFT) == SDL_HAT_LEFT)
            ZWattribute___.realjoystick[event.jhat.which+kjn].axis[event.jhat.hat*2
                         +ZWattribute___.realjoystick[event.jhat.which+kjn].only_axiscount]=-1;
          if ((event.jhat.value & SDL_HAT_RIGHT) == SDL_HAT_RIGHT)
            ZWattribute___.realjoystick[event.jhat.which+kjn].axis[event.jhat.hat*2
                         +ZWattribute___.realjoystick[event.jhat.which+kjn].only_axiscount]= 1;
          if ((event.jhat.value & SDL_HAT_UP) == SDL_HAT_UP)
            ZWattribute___.realjoystick[event.jhat.which+kjn].axis[event.jhat.hat*2+1
                         +ZWattribute___.realjoystick[event.jhat.which+kjn].only_axiscount]=-1;
          if ((event.jhat.value & SDL_HAT_DOWN) == SDL_HAT_DOWN)
            ZWattribute___.realjoystick[event.jhat.which+kjn].axis[event.jhat.hat*2+1
                         +ZWattribute___.realjoystick[event.jhat.which+kjn].only_axiscount]= 1;
          if (event.jhat.value == SDL_HAT_CENTERED)
          {
            ZWattribute___.realjoystick[event.jhat.which+kjn].axis[event.jhat.hat*2
                         +ZWattribute___.realjoystick[event.jhat.which+kjn].only_axiscount]=0;
            ZWattribute___.realjoystick[event.jhat.which+kjn].axis[event.jhat.hat*2+1
                         +ZWattribute___.realjoystick[event.jhat.which+kjn].only_axiscount]=0;
          }
        break;
        case SDL_KEYDOWN:
            if (((event.key.keysym.mod & KMOD_LALT)==KMOD_LALT) && (event.key.keysym.sym==SDLK_F4))
              if (ZWattribute___.quit_function!=NULL)
                ZWattribute___.quit_function();                                               
            ZWhandle_key_down( &event.key.keysym );
            break;
        case SDL_KEYUP:
            ZWhandle_key_up( &event.key.keysym );
            break;
        case SDL_QUIT:
            if (ZWattribute___.quit_function!=NULL)
              ZWattribute___.quit_function();
            break;
        case SDL_VIDEORESIZE :
          ZWattribute___.width=event.resize.w;
          ZWattribute___.height=event.resize.h;
          #ifndef WIN32
            ZWattribute___.surface = SDL_SetVideoMode( ZWattribute___.width, ZWattribute___.height, ZWattribute___.bpp, ZWattribute___.sdlflags );
          #endif
          ZWglResizeWindow( );
          break;
        }
    }
}

EXPORT void ZWenablefog_near(float fognear, float fogfar)
{
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf (GL_FOG_START, fognear);
    glFogf (GL_FOG_END, fogfar); 
}

EXPORT void ZWenablefog_ext(float d)
{
}

EXPORT void ZWenablefog_ext2(float d)
{
}

EXPORT void ZWdisablefog(void)
{
    glDisable(GL_FOG);
}

EXPORT void ZWsetmaterialambient(float r,float g,float b,float a)
{
GLfloat material[4]={r,g,b,a};
glMaterialfv(GL_FRONT,GL_AMBIENT,&material[0]);
}

EXPORT void ZWsetmaterialspecular(float r,float g,float b,float a)
{
GLfloat material[4]={r,g,b,a};
glMaterialfv(GL_FRONT,GL_SPECULAR,&material[0]);
}

EXPORT void ZWsetmaterialdiffuse(float r,float g,float b,float a)
{
GLfloat material[4]={r,g,b,a};
glMaterialfv(GL_FRONT,GL_DIFFUSE,&material[0]);
}

EXPORT void ZWsetmaterialshininess(float s)
{
glMaterialfv(GL_FRONT,GL_SHININESS,&s);
}

EXPORT void ZWsetlightambient(int licht,float r,float g,float b)
{
GLfloat material[4]={r,g,b,1.0};
glLightfv(GL_LIGHT0+licht,GL_AMBIENT,&material[0]);
}

EXPORT void ZWsetlightdiffuse(int licht,float r,float g,float b)
{
GLfloat material[4]={r,g,b,1.0};
glLightfv(GL_LIGHT0+licht,GL_DIFFUSE,&material[0]);
}

EXPORT void ZWsetlightposition(int licht,float x,float y,float z)
{
GLfloat material[4]={x,y,z,1.0};
glLightfv(GL_LIGHT0+licht,GL_POSITION,&material[0]);
}

EXPORT void ZWdisablelight(int licht)
{
glDisable( GL_LIGHT0+licht );
}

EXPORT void ZWenablelight(int licht)
{
glEnable( GL_LIGHT0+licht );
}

EXPORT void ZWsetmaterial(int material)
{
glDisable(GL_COLOR_MATERIAL);
switch (material)
{
case lbronze:
    ZWsetmaterialambient(0.21,0.13,0.05,1.0);
    ZWsetmaterialdiffuse(0.71,0.43,0.18,1.0);
    ZWsetmaterialspecular(0.39,0.27,0.17,1.0);
    ZWsetmaterialshininess(25.6);
break;
case lbronzep:
    ZWsetmaterialambient(0.25,0.15,0.06,1.0);
    ZWsetmaterialdiffuse(0.40,0.24,0.10,1.0);
    ZWsetmaterialspecular(0.77,0.46,0.20,1.0);
    ZWsetmaterialshininess(76.8);
break;
case lchrom:
    ZWsetmaterialambient(0.25,0.25,0.25,1.0);
    ZWsetmaterialdiffuse(0.40,0.40,0.40,1.0);
    ZWsetmaterialspecular(0.77,0.77,0.77,1.0);
    ZWsetmaterialshininess(76.8);
break;
case lgold:
    ZWsetmaterialambient(0.25,0.20,0.07,1.0);
    ZWsetmaterialdiffuse(0.75,0.61,0.23,1.0);
    ZWsetmaterialspecular(0.63,0.65,0.37,1.0);
    ZWsetmaterialshininess(51.2);
break;
case lgoldp:
    ZWsetmaterialambient(0.25,0.22,0.06,1.0);
    ZWsetmaterialdiffuse(0.35,0.31,0.09,1.0);
    ZWsetmaterialspecular(0.80,0.72,0.21,1.0);
    ZWsetmaterialshininess(83.2);
break;
case lkupfer:
    ZWsetmaterialambient(0.19,0.07,0.02,1.0);
    ZWsetmaterialdiffuse(0.70,0.27,0.08,1.0);
    ZWsetmaterialspecular(0.26,0.14,0.09,1.0);
    ZWsetmaterialshininess(12.8);
break;
case lkupferp:
    ZWsetmaterialambient(0.23,0.09,0.03,1.0);
    ZWsetmaterialdiffuse(0.55,0.21,0.07,1.0);
    ZWsetmaterialspecular(0.58,0.22,0.07,1.0);
    ZWsetmaterialshininess(51.2);
break;
case lmessing:
    ZWsetmaterialambient(0.33,0.22,0.03,1.0);
    ZWsetmaterialdiffuse(0.78,0.57,0.11,1.0);
    ZWsetmaterialspecular(0.99,0.94,0.81,1.0);
    ZWsetmaterialshininess(27.9);
break;
case lsilber:
    ZWsetmaterialambient(0.19,0.19,0.19,1.0);
    ZWsetmaterialdiffuse(0.51,0.51,0.51,1.0);
    ZWsetmaterialspecular(0.51,0.51,0.51,1.0);
    ZWsetmaterialshininess(51.2);
break;
case lsilberp:
    ZWsetmaterialambient(0.23,0.23,0.23,1.0);
    ZWsetmaterialdiffuse(0.28,0.28,0.28,1.0);
    ZWsetmaterialspecular(0.77,0.77,0.77,1.0);
    ZWsetmaterialshininess(89.6);
break;
case lzinn:
    ZWsetmaterialambient(0.11,0.06,0.11,1.0);
    ZWsetmaterialdiffuse(0.43,0.47,0.54,1.0);
    ZWsetmaterialspecular(0.33,0.33,0.52,1.0);
    ZWsetmaterialshininess(09.8);
break;
case lgummi:
    ZWsetmaterialambient(0.02,0.02,0.02,1.0);
    ZWsetmaterialdiffuse(0.01,0.01,0.01,1.0);
    ZWsetmaterialspecular(0.40,0.40,0.40,1.0);
    ZWsetmaterialshininess(10.0);
break;
case lplastik:
    ZWsetmaterialambient(0.00,0.00,0.00,1.0);
    ZWsetmaterialdiffuse(0.01,0.01,0.01,1.0);
    ZWsetmaterialspecular(0.50,0.50,0.50,1.0);
    ZWsetmaterialshininess(32.0);
break;
case ljade:
    ZWsetmaterialambient(0.14,0.22,0.16,0.9);
    ZWsetmaterialdiffuse(0.54,0.89,0.63,0.9);
    ZWsetmaterialspecular(0.32,0.32,0.32,0.9);
    ZWsetmaterialshininess(12.8);
break;
case lobsidian:
    ZWsetmaterialambient(0.05,0.05,0.07,0.8);
    ZWsetmaterialdiffuse(0.18,0.17,0.23,0.8);
    ZWsetmaterialspecular(0.33,0.33,0.35,0.8);
    ZWsetmaterialshininess(38.4);
break;
case lperle:
    ZWsetmaterialambient(0.25,0.21,0.21,0.9);
    ZWsetmaterialdiffuse(0.99,0.83,0.83,0.9);
    ZWsetmaterialspecular(0.30,0.30,0.30,0.9);
    ZWsetmaterialshininess(11.3);
break;
case lrubin:
    ZWsetmaterialambient(0.17,0.01,0.01,0.5);
    ZWsetmaterialdiffuse(0.61,0.04,0.04,0.5);
    ZWsetmaterialspecular(0.73,0.63,0.63,0.5);
    ZWsetmaterialshininess(76.8);
break;
case lsmaragd:
    ZWsetmaterialambient(0.02,0.17,0.02,0.5);
    ZWsetmaterialdiffuse(0.08,0.61,0.08,0.5);
    ZWsetmaterialspecular(0.63,0.73,0.63,0.5);
    ZWsetmaterialshininess(76.8);
break;
case ltuerkis:
    ZWsetmaterialambient(0.10,0.19,0.17,0.8);
    ZWsetmaterialdiffuse(0.40,0.74,0.69,0.8);
    ZWsetmaterialspecular(0.30,0.31,0.31,0.8);
    ZWsetmaterialshininess(12.8);
break;
default: glEnable(GL_COLOR_MATERIAL);
}
}

EXPORT void ZWenableculling(void)
{
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
}

EXPORT void ZWdisableculling(void)
{
    glDisable( GL_CULL_FACE );
}

EXPORT void ZWsetfogcolor(float r,float g,float b)
{
    GLfloat color[3]={r,g,b};
    glFogfv(GL_FOG_COLOR,color);
}

EXPORT void ZWclearall(void)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}

EXPORT void ZWflipbuffer(void)
{
SDL_GL_SwapBuffers( );
}

float ZWsqr(float a)
{
  return a*a;
}

EXPORT void ZWNormalizePlane(pZWfrustum frustum,int pPlane)
{
  float Magnitude;
  Magnitude = sqrt(ZWsqr(frustum->frustum[pPlane][1]) + ZWsqr(frustum->frustum[pPlane][2]) + ZWsqr(frustum->frustum[pPlane][3]));
  frustum->frustum[pPlane][0] = frustum->frustum[pPlane][0] / Magnitude;
  frustum->frustum[pPlane][1] = frustum->frustum[pPlane][1] / Magnitude;
  frustum->frustum[pPlane][2] = frustum->frustum[pPlane][2] / Magnitude;
  frustum->frustum[pPlane][3] = frustum->frustum[pPlane][3] / Magnitude;
}

EXPORT void ZWfrustum_calculate(pZWfrustum frustum)
{
  float ProjM[16], ModM[16], clip[16];
  glGetFloatv(GL_PROJECTION_MATRIX, ProjM);
  glGetFloatv(GL_MODELVIEW_MATRIX, ModM);
  clip[ 0] = ModM[ 0]*ProjM[ 0] + ModM[ 1]*ProjM[ 4] +
             ModM[ 2]*ProjM[ 8] + ModM[ 3]*ProjM[12];
  clip[ 1] = ModM[ 0]*ProjM[ 1] + ModM[ 1]*ProjM[ 5] +
             ModM[ 2]*ProjM[ 9] + ModM[ 3]*ProjM[13];
  clip[ 2] = ModM[ 0]*ProjM[ 2] + ModM[ 1]*ProjM[ 6] +
             ModM[ 2]*ProjM[10] + ModM[ 3]*ProjM[14];
  clip[ 3] = ModM[ 0]*ProjM[ 3] + ModM[ 1]*ProjM[ 7] +
             ModM[ 2]*ProjM[11] + ModM[ 3]*ProjM[15];
  clip[ 4] = ModM[ 4]*ProjM[ 0] + ModM[ 5]*ProjM[ 4] +
             ModM[ 6]*ProjM[ 8] + ModM[ 7]*ProjM[12];
  clip[ 5] = ModM[ 4]*ProjM[ 1] + ModM[ 5]*ProjM[ 5] +
             ModM[ 6]*ProjM[ 9] + ModM[ 7]*ProjM[13];
  clip[ 6] = ModM[ 4]*ProjM[ 2] + ModM[ 5]*ProjM[ 6] +
             ModM[ 6]*ProjM[10] + ModM[ 7]*ProjM[14];
  clip[ 7] = ModM[ 4]*ProjM[ 3] + ModM[ 5]*ProjM[ 7] +
             ModM[ 6]*ProjM[11] + ModM[ 7]*ProjM[15];
  clip[ 8] = ModM[ 8]*ProjM[ 0] + ModM[ 9]*ProjM[ 4] +
             ModM[10]*ProjM[ 8] + ModM[11]*ProjM[12];
  clip[ 9] = ModM[ 8]*ProjM[ 1] + ModM[ 9]*ProjM[ 5] +
             ModM[10]*ProjM[ 9] + ModM[11]*ProjM[13];
  clip[10] = ModM[ 8]*ProjM[ 2] + ModM[ 9]*ProjM[ 6] +
             ModM[10]*ProjM[10] + ModM[11]*ProjM[14];
  clip[11] = ModM[ 8]*ProjM[ 3] + ModM[ 9]*ProjM[ 7] +
             ModM[10]*ProjM[11] + ModM[11]*ProjM[15];
  clip[12] = ModM[12]*ProjM[ 0] + ModM[13]*ProjM[ 4] +
             ModM[14]*ProjM[ 8] + ModM[15]*ProjM[12];
  clip[13] = ModM[12]*ProjM[ 1] + ModM[13]*ProjM[ 5] +
             ModM[14]*ProjM[ 9] + ModM[15]*ProjM[13];
  clip[14] = ModM[12]*ProjM[ 2] + ModM[13]*ProjM[ 6] +
             ModM[14]*ProjM[10] + ModM[15]*ProjM[14];
  clip[15] = ModM[12]*ProjM[ 3] + ModM[13]*ProjM[ 7] +
             ModM[14]*ProjM[11] + ModM[15]*ProjM[15];

  frustum->frustum[0][0] = clip[ 3] - clip[ 0];
  frustum->frustum[0][1] = clip[ 7] - clip[ 4];
  frustum->frustum[0][2] = clip[11] - clip[ 8];
  frustum->frustum[0][3] = clip[15] - clip[12];
  ZWNormalizePlane(frustum, 0);
 
  frustum->frustum[1][0] = clip[ 3] + clip[ 0];
  frustum->frustum[1][1] = clip[ 7] + clip[ 4];
  frustum->frustum[1][2] = clip[11] + clip[ 8];
  frustum->frustum[1][3] = clip[15] + clip[12];
  ZWNormalizePlane(frustum, 1);
 
  frustum->frustum[2][0] = clip[ 3] + clip[ 1];
  frustum->frustum[2][1] = clip[ 7] + clip[ 5];
  frustum->frustum[2][2] = clip[11] + clip[ 9];
  frustum->frustum[2][3] = clip[15] + clip[13];
  ZWNormalizePlane(frustum, 2);
 
  frustum->frustum[3][0] = clip[ 3] - clip[ 1];
  frustum->frustum[3][1] = clip[ 7] - clip[ 5];
  frustum->frustum[3][2] = clip[11] - clip[ 9];
  frustum->frustum[3][3] = clip[15] - clip[13];
  ZWNormalizePlane(frustum, 3);
 
  frustum->frustum[4][0] = clip[ 3] - clip[ 2];
  frustum->frustum[4][1] = clip[ 7] - clip[ 6];
  frustum->frustum[4][2] = clip[11] - clip[10];
  frustum->frustum[4][3] = clip[15] - clip[14];
  ZWNormalizePlane(frustum, 4);
 
  frustum->frustum[5][0] = clip[ 3] + clip[ 2];
  frustum->frustum[5][1] = clip[ 7] + clip[ 6];
  frustum->frustum[5][2] = clip[11] + clip[10];
  frustum->frustum[5][3] = clip[15] + clip[14];
  ZWNormalizePlane(frustum, 5);
}

EXPORT int ZWfrustum_isboxwithin(pZWfrustum frustum,float pX,float pY,float pZ,float pB,float pH,float pT)
{
  int i;
  int Result = 1;
  for (i=0;i<=5;i++)
  {
    if (frustum->frustum[i][0]*(pX-pB) + frustum->frustum[i][1]*(pY-pH) +
        frustum->frustum[i][2]*(pZ-pT) + frustum->frustum[i][3]>0)
          continue;
    if (frustum->frustum[i][0]*(pX+pB) + frustum->frustum[i][1]*(pY-pH) +
        frustum->frustum[i][2]*(pZ-pT) + frustum->frustum[i][3]>0)
          continue;
    if (frustum->frustum[i][0]*(pX-pB) + frustum->frustum[i][1]*(pY+pH) +
        frustum->frustum[i][2]*(pZ-pT) + frustum->frustum[i][3]>0)
          continue;
    if (frustum->frustum[i][0]*(pX+pB) + frustum->frustum[i][1]*(pY+pH) +
        frustum->frustum[i][2]*(pZ-pT) + frustum->frustum[i][3]>0)
          continue;
    if (frustum->frustum[i][0]*(pX-pB) + frustum->frustum[i][1]*(pY-pH) +
        frustum->frustum[i][2]*(pZ+pT) + frustum->frustum[i][3]>0)
          continue;
    if (frustum->frustum[i][0]*(pX+pB) + frustum->frustum[i][1]*(pY-pH) +
        frustum->frustum[i][2]*(pZ+pT) + frustum->frustum[i][3]>0)
          continue;
    if (frustum->frustum[i][0]*(pX-pB) + frustum->frustum[i][1]*(pY+pH) +
        frustum->frustum[i][2]*(pZ+pT) + frustum->frustum[i][3]>0)
          continue;
    if (frustum->frustum[i][0]*(pX+pB) + frustum->frustum[i][1]*(pY+pH) +
        frustum->frustum[i][2]*(pZ+pT) + frustum->frustum[i][3]>0)
          continue;
    Result = 0;
    break;
  }
  return Result;
}

EXPORT int ZWfileexists(char* filename)
{
  SDL_RWops *file = SDL_RWFromFile(filename, "rb");
  if (file==NULL) return 0;
  SDL_RWclose(file);
  return 1;
}


/*void* ZWpushlight(int num)
{
  float* lights=(float*)malloc(num*sizeof(float)*10);
  // lights[     0] -> Licht aktiviert
  // lights[ 1.. 3] -> Position
  // lights[ 4.. 6] -> Ambient
  // lights[ 7.. 9] -> Diffuse
  
  for (int a=0;a<num;a++)
  {
    if (glIsEnabled(GL_LIGHT0+a))
    {
      lights[a*10+0]=1;
    
    }
    else
      lights[a*10+0]=0;
  }
}*/

EXPORT void ZWnewaxis(char* name,int left,int right)
{
	pZWnewaxis newaxis=(pZWnewaxis)malloc(sizeof(tZWnewaxis));
	if (ZWnewaxis___==NULL)
	  ZWnewaxis___=newaxis;
	else
	{
		pZWnewaxis lastaxis=ZWnewaxis___;
		while (lastaxis->next!=NULL)
		  lastaxis=lastaxis->next;
		lastaxis->next=newaxis;
	}
	newaxis->next=NULL;
	newaxis->name=(char*)malloc(strlen(name)+1);
	sprintf(newaxis->name,"%s",name);
	newaxis->left=left;
	newaxis->right=right;
}

EXPORT void ZWnewkeyboard(char* name,char* b1,char* b2,char* b3,char* b4,char* a1l,char* a1r,char* a2l,char* a2r)
{
	char* key[8]={b1,b2,b3,b4,a1l,a1r,a2l,a2r};
	pZWnewkeyboard newkeyboard=(pZWnewkeyboard)malloc(sizeof(tZWnewkeyboard));
	if (ZWnewkeyboard___==NULL)
	  ZWnewkeyboard___=newkeyboard;
	else
	{
		pZWnewkeyboard lastkeyboard=ZWnewkeyboard___;
		while (lastkeyboard->next!=NULL)
		  lastkeyboard=lastkeyboard->next;
		lastkeyboard->next=newkeyboard;
	}
	newkeyboard->next=NULL;
	newkeyboard->name=(char*)malloc(strlen(name)+1);
	sprintf(newkeyboard->name,"%s",name);
	int a;
	for (a=0;a<8;a++)
	{
		if (key[a]==NULL)
		  newkeyboard->key[a]=0;
		else
		if (key[a][0]==0)
		  newkeyboard->key[a]=0;
		else
		if (key[a][1]==0)
		  newkeyboard->key[a]=key[a][0];
		else
		{
			if (key[a][0]=='n' && key[a][1]=='u' && key[a][2]=='m') //num0..9
			  newkeyboard->key[a]=key[a][3]-'0'+SDLK_KP0;
			else
			if (key[a][0]=='F' && key[a][1]=='1') //F10..15
			  newkeyboard->key[a]=key[a][2]-'0'+SDLK_F10;
			else
			if (key[a][0]=='F') //F1..9
			  newkeyboard->key[a]=key[a][1]-'0'+SDLK_F1;
			else
			if (strcmp(key[a],"backspace")==0)
			  newkeyboard->key[a]=SDLK_BACKSPACE;
			else
			if (strcmp(key[a],"tab")==0)
			  newkeyboard->key[a]=SDLK_TAB;
			else
			if (strcmp(key[a],"return")==0)
			  newkeyboard->key[a]=SDLK_RETURN;
			else
			if (strcmp(key[a],"dblquote")==0)
			  newkeyboard->key[a]=SDLK_QUOTEDBL;
			else
			if (strcmp(key[a],"delete")==0)
			  newkeyboard->key[a]=SDLK_DELETE;
			else
			if (strcmp(key[a],"up")==0)
			  newkeyboard->key[a]=SDLK_UP;
			else
			if (strcmp(key[a],"down")==0)
			  newkeyboard->key[a]=SDLK_DOWN;
			else
			if (strcmp(key[a],"left")==0)
			  newkeyboard->key[a]=SDLK_LEFT;
			else
			if (strcmp(key[a],"right")==0)
			  newkeyboard->key[a]=SDLK_RIGHT;
			else
			if (strcmp(key[a],"insert")==0)
			  newkeyboard->key[a]=SDLK_INSERT;
			else
			if (strcmp(key[a],"home")==0)
			  newkeyboard->key[a]=SDLK_HOME;
			else
			if (strcmp(key[a],"end")==0)
			  newkeyboard->key[a]=SDLK_END;
			else
			if (strcmp(key[a],"pageup")==0)
			  newkeyboard->key[a]=SDLK_PAGEUP;
			else
			if (strcmp(key[a],"pagedown")==0)
			  newkeyboard->key[a]=SDLK_PAGEDOWN;
			else
			if (strcmp(key[a],"rshift")==0)
			  newkeyboard->key[a]=SDLK_RSHIFT;
			else
			if (strcmp(key[a],"lshift")==0)
			  newkeyboard->key[a]=SDLK_LSHIFT;
			else
			if (strcmp(key[a],"lalt")==0)
			  newkeyboard->key[a]=SDLK_LALT;
			else
			if (strcmp(key[a],"ralt")==0)
			  newkeyboard->key[a]=SDLK_RALT;
			else
			if (strcmp(key[a],"rmeta")==0)
			  newkeyboard->key[a]=SDLK_RMETA;
			else
			if (strcmp(key[a],"lmeta")==0)
			  newkeyboard->key[a]=SDLK_LMETA;
			else
			if (strcmp(key[a],"lsuper")==0)
			  newkeyboard->key[a]=SDLK_LSUPER;
			else
			if (strcmp(key[a],"rsuper")==0)
			  newkeyboard->key[a]=SDLK_RSUPER;
			else
			if (strcmp(key[a],"capslock")==0)
			  newkeyboard->key[a]=SDLK_CAPSLOCK;
			else
			if (strcmp(key[a],"help")==0)
			  newkeyboard->key[a]=SDLK_HELP;
			else
			if (strcmp(key[a],"print")==0)
			  newkeyboard->key[a]=SDLK_PRINT;
			else
			if (strcmp(key[a],"sysrq")==0)
			  newkeyboard->key[a]=SDLK_SYSREQ;
			else
			if (strcmp(key[a],"break")==0)
			  newkeyboard->key[a]=SDLK_BREAK;
			else
			if (strcmp(key[a],"menu")==0)
			  newkeyboard->key[a]=SDLK_MENU;
			else
			if (strcmp(key[a],"undo")==0)
			  newkeyboard->key[a]=SDLK_UNDO;
			else
			if (strcmp(key[a],"num/")==0)
			  newkeyboard->key[a]=SDLK_KP_DIVIDE;
			else
			if (strcmp(key[a],"num*")==0)
			  newkeyboard->key[a]=SDLK_KP_MULTIPLY;
			else
			if (strcmp(key[a],"num-")==0)
			  newkeyboard->key[a]=SDLK_KP_MINUS;
			else
			if (strcmp(key[a],"num+")==0)
			  newkeyboard->key[a]=SDLK_KP_PLUS;
			else
			if (strcmp(key[a],"num,")==0)
			  newkeyboard->key[a]=SDLK_KP_PERIOD;
			else
			if (strcmp(key[a],"numreturn")==0)
			  newkeyboard->key[a]=SDLK_KP_ENTER;
			else
			if (strcmp(key[a],"numlock")==0)
			  newkeyboard->key[a]=SDLK_NUMLOCK;
		}
	}
	newkeyboard->joynr=ZWattribute___.keyboard_joystick_num;
	ZWattribute___.keyboard_joystick_num++;
}

#ifdef WIN32
BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
    switch (reason)
    {
      case DLL_PROCESS_ATTACH:
        break;

      case DLL_PROCESS_DETACH:
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}
#endif
