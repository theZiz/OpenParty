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
typedef struct sphysic_object *pphysik_object;
typedef struct sphysic_object {
	struct {float x; float y;}  position, old_position;
	float kin_energy,old_kin_energy;
	float direction,old_direction;
	float absorption;
	float radius;
	union {float radius; struct {float x;float y;} dimensions;}  dimensions;
	float mass;
	int type;
	bool move;
	float rotation[16];
	int kollision_number;
	float kollision_radius[8];
	bool kollision;
	pphysik_object kollision_partner[8];
	pphysik_object next;	
} tphysic_object;

float myasin(float x)
{
	if (x<-1.0) x=-1.0;
	if (x>1.0) x=1.0;
	return asin(x);
}

float myacos(float x)
{
	if (x<-1.0) x=-1.0;
	if (x>1.0) x=1.0;
	return acos(x);
}


float angle_distance(float a1,float a2)
{
	float d=fabs(a1-a2);
	if (d>M_PI)
	  d=fabs(2.0*M_PI-d);
	return d;
}

float distance(float px,float py,float qx,float qy)
{
	return sqrt((px-qx)*(px-qx)+(py-qy)*(py-qy));
}

void rotate_matrix(float *matrix,float x,float y,float z,float rad)
{
	//Rotationsmatrix:
	float s=sin(rad);
	float c=cos(rad);
	float l=sqrt(x*x+y*y+z*z);
	if (l==0)
	  return;
	x/=l;
	y/=l;
	z/=l;
	float rotate[16];
    rotate[ 0]= c+x*x*(1.0-c);   rotate[ 1]= x*y*(1.0-c)-z*s; rotate[ 2]= x*z*(1.0-c)+y*s; rotate[ 3]=0;
		rotate[ 4]= y*x*(1.0-c)+z*s; rotate[ 5]= c+y*y*(1.0-c);   rotate[ 6]=-y*z*(1.0-c)-x*s; rotate[ 7]=0;
		rotate[ 8]= z*x*(1.0-c)-y*s; rotate[ 9]= z*y*(1.0-c)+x*s; rotate[10]= c+z*z*(1.0-c);   rotate[11]=0;
		rotate[12]= 0;               rotate[13]= 0;               rotate[14]= 0;               rotate[15]=1;

	//Matrix umdrehen:
	float oldmatrix[16];
	memcpy(oldmatrix,matrix,sizeof(float)*16);
	matrix[ 0]=oldmatrix[ 0]; matrix[ 1]=oldmatrix[ 4]; matrix[ 2]=oldmatrix[ 8]; matrix[ 3]=oldmatrix[12];
	matrix[ 4]=oldmatrix[ 1]; matrix[ 5]=oldmatrix[ 5]; matrix[ 6]=oldmatrix[ 9]; matrix[ 7]=oldmatrix[13];
	matrix[ 8]=oldmatrix[ 2]; matrix[ 9]=oldmatrix[ 6]; matrix[10]=oldmatrix[10]; matrix[11]=oldmatrix[14];
	matrix[12]=oldmatrix[ 3]; matrix[13]=oldmatrix[ 7]; matrix[14]=oldmatrix[11]; matrix[15]=oldmatrix[15];
	
	//Multiplikation
	
	//alle Werte vertauschen:
/*	int a;
	for (a=0;a<16;a++)
	{
		float temp=matrix[a];
		matrix[a]=rotate[a];
		rotate[a]=temp;
	}*/
	
	float result[16];
	result[ 0]=matrix[ 0]*rotate[ 0]+matrix[ 4]*rotate[ 1]+matrix[ 8]*rotate[ 2]+matrix[12]*rotate[ 3];
	result[ 1]=matrix[ 0]*rotate[ 4]+matrix[ 4]*rotate[ 5]+matrix[ 8]*rotate[ 6]+matrix[12]*rotate[ 7];
	result[ 2]=matrix[ 0]*rotate[ 8]+matrix[ 4]*rotate[ 9]+matrix[ 8]*rotate[10]+matrix[12]*rotate[11];
	result[ 3]=matrix[ 0]*rotate[12]+matrix[ 4]*rotate[13]+matrix[ 8]*rotate[14]+matrix[12]*rotate[15];
	
	result[ 4]=matrix[ 1]*rotate[ 0]+matrix[ 5]*rotate[ 1]+matrix[ 9]*rotate[ 2]+matrix[13]*rotate[ 3];
	result[ 5]=matrix[ 1]*rotate[ 4]+matrix[ 5]*rotate[ 5]+matrix[ 9]*rotate[ 6]+matrix[13]*rotate[ 7];
	result[ 6]=matrix[ 1]*rotate[ 8]+matrix[ 5]*rotate[ 9]+matrix[ 9]*rotate[10]+matrix[13]*rotate[11];
	result[ 7]=matrix[ 1]*rotate[12]+matrix[ 5]*rotate[13]+matrix[ 9]*rotate[14]+matrix[13]*rotate[15];
	
	result[ 8]=matrix[ 2]*rotate[ 0]+matrix[ 6]*rotate[ 1]+matrix[10]*rotate[ 2]+matrix[14]*rotate[ 3];
	result[ 9]=matrix[ 2]*rotate[ 4]+matrix[ 6]*rotate[ 5]+matrix[10]*rotate[ 6]+matrix[14]*rotate[ 7];
	result[10]=matrix[ 2]*rotate[ 8]+matrix[ 6]*rotate[ 9]+matrix[10]*rotate[10]+matrix[14]*rotate[11];
	result[11]=matrix[ 2]*rotate[12]+matrix[ 6]*rotate[13]+matrix[10]*rotate[14]+matrix[14]*rotate[15];
	
	result[12]=matrix[ 3]*rotate[ 0]+matrix[ 7]*rotate[ 1]+matrix[11]*rotate[ 2]+matrix[15]*rotate[ 3];
	result[13]=matrix[ 3]*rotate[ 4]+matrix[ 7]*rotate[ 5]+matrix[11]*rotate[ 6]+matrix[15]*rotate[ 7];
	result[14]=matrix[ 3]*rotate[ 8]+matrix[ 7]*rotate[ 9]+matrix[11]*rotate[10]+matrix[15]*rotate[11];
	result[15]=matrix[ 3]*rotate[12]+matrix[ 7]*rotate[13]+matrix[11]*rotate[14]+matrix[15]*rotate[15];

	//Matrix umdrehen:
	matrix[ 0]=result[ 0]; matrix[ 1]=result[ 4]; matrix[ 2]=result[ 8]; matrix[ 3]=result[12];
	matrix[ 4]=result[ 1]; matrix[ 5]=result[ 5]; matrix[ 6]=result[ 8]; matrix[ 7]=result[13];
	matrix[ 8]=result[ 2]; matrix[ 9]=result[ 6]; matrix[10]=result[10]; matrix[11]=result[14];
	matrix[12]=result[ 3]; matrix[13]=result[ 7]; matrix[14]=result[11]; matrix[15]=result[15];
	
	memcpy(matrix,result,sizeof(float)*16);
}

pphysik_object new_object(float px,float py,float direction,int type,float dx,float dy,float mass,bool move,pphysik_object* first)
{
	pphysik_object object=(pphysik_object)malloc(sizeof(tphysic_object));
	object->kin_energy=0;
	object->old_kin_energy=0;
	object->position.x=px;
	object->position.y=py;
	object->old_position=object->position;
	object->direction=direction;
	object->old_direction=direction;
	object->type=type;
	switch (object->type)
	{
		case 0: object->dimensions.radius=dx; break;//Kugel
		case 1: object->dimensions.dimensions.x=dx;
		        object->dimensions.dimensions.y=dy; break;//Quader
		case 2: object->dimensions.dimensions.x=dx;
		        object->dimensions.dimensions.y=dy; break;//Antiquader
	}
	object->mass=mass;
	object->move=move;
	object->kollision_number=0;
	int b;
	for (b=0;b<16;b++)
		if (b%5==0)
	    object->rotation[b]=1;
		else
	    object->rotation[b]=0;
	object->next=*first;
	(*first)=object;
	return object;
}

void collapse_world(pphysik_object *world)
{
	while (*world!=NULL);
	{
		pphysik_object temp=(*world)->next;
		free(*world);
		*world=temp;
	}
}

void add_kin_energy(pphysik_object object,float energy,float direction)
{
	float dx=object->kin_energy*sin(object->direction)+energy*sin(direction);
	float dy=object->kin_energy*cos(object->direction)+energy*cos(direction);
	float l=sqrt(dx*dx+dy*dy);
	object->kin_energy=l;
	dx/=l;
	dy/=l;
	object->direction=myasin(dx);
	if (dy<0)
	  object->direction=M_PI-object->direction;
	if (object->direction<0.0)
	  object->direction+=2.0*M_PI;
	if (object->direction>=2.0*M_PI)
	  object->direction-=2.0*M_PI;
}

bool sphere_block_kollision(float sx,float sy,float radius,float bx,float by,float w,float h,float *angle)
{
	//Punkte durchtesten
	//links unten
	if (distance(sx,sy,bx,by)<radius)
	{
		float dx=bx-sx;
		float dy=by-sy;
		float l=sqrt(dx*dx+dy*dy);
		dx/=l;
		dy/=l;
		*angle=myasin(dx);
		return true;
	}
	//rechts unten
	if (distance(sx,sy,bx+w,by)<radius)
	{
		float dx=bx+w-sx;
		float dy=by-sy;
		float l=sqrt(dx*dx+dy*dy);
		dx/=l;
		dy/=l;
		*angle=myasin(dx)+2.0*M_PI;
		return true;
	}
	//links oben
	if (distance(sx,sy,bx,by+h)<radius)
	{
		float dx=bx-sx;
		float dy=by+h-sy;
		float l=sqrt(dx*dx+dy*dy);
		dx/=l;
		dy/=l;
		*angle=M_PI-myasin(dx);
		return true;
	}
	//rechts oben
	if (distance(sx,sy,bx+w,by+h)<radius)
	{
		float dx=bx+w-sx;
		float dy=by+h-sy;
		float l=sqrt(dx*dx+dy*dy);
		dx/=l;
		dy/=l;
		*angle=M_PI-myasin(dx);
		return true;
	}
	//Seiten durchtesten
	//Unten
	if (sx>bx && sx<bx+w && by-sy<radius && by-sy>0)
	{
		*angle=0;
		return true;
	}
	//Oben
	if (sx>bx && sx<bx+w && sy-(by+h)<radius && sy-(by+h)>0)
	{
		*angle=M_PI;
		return true;
	}
	//Links
	if (sy>by && sy<by+h && bx-sx<radius && bx-sx>0)
	{
		*angle=M_PI/2.0;
		return true;
	}
	//Rechts
	if (sy>by && sy<by+h && sx-(bx+w)<radius && sx-(bx+w)>0)
	{
		*angle=3.0*M_PI/2.0;
		return true;
	}
	return false;
}

void search_kollision(pphysik_object world)
{
	//Kollisionen suchen und rausfinden, WO sie sind
	pphysik_object object=world;
	while (object!=NULL)
	{
		pphysik_object other=object->next;
		while (other!=NULL)
		{
			//Alle Kombinationen durchgehen
			//Eines sollte sich bewegen...
			if (object->move || other->move)
			{
				//Wir suchen Kollisionen
				//Sphere-Sphere
				if (object->type==0 && other->type==0 && object->move && other->move)
				{
					if (distance(object->position.x,object->position.y,other->position.x,other->position.y)<object->dimensions.radius+other->dimensions.radius)
					{
						//Winkel berechnen:
						float dx=object->position.x-other->position.x;
						float dy=object->position.y-other->position.y;
						float l=sqrt(dx*dx+dy*dy);
						dx/=l;
						dy/=l;
						object->kollision_radius[object->kollision_number]=myasin(dx);
						if (dy<0)
							object->kollision_radius[object->kollision_number]=M_PI-object->kollision_radius[object->kollision_number];
						if (object->kollision_radius[object->kollision_number]<0.0)
							object->kollision_radius[object->kollision_number]+=2.0*M_PI;
						if (object->kollision_radius[object->kollision_number]>=2.0*M_PI)
							object->kollision_radius[object->kollision_number]-=2.0*M_PI;
						other->kollision_radius[other->kollision_number]=M_PI+object->kollision_radius[object->kollision_number];
						//if (other->kollision_radius[other->kollision_number]>=2.0*M_PI)
							//other->kollision_radius[other->kollision_number]-=2.0*M_PI;
						object->kollision_partner[object->kollision_number]=other;
						//other->kollision_partner[other->kollision_number]=object;
						//other->kollision_number++;
						other->kollision=true;
						object->kollision_number++;
					}
				}
				//Sphere-Block
				if ((object->type==0 && other->type==1) || (object->type==1 && other->type==0))
				{
					pphysik_object block=object;
					pphysik_object sphere=other;
					if (sphere->type==1)
					{
						block=other;
						sphere=object;
					}
					float angle;
					if (sphere_block_kollision(sphere->position.x,sphere->position.y,sphere->dimensions.radius,block->position.x,block->position.y,block->dimensions.dimensions.x,block->dimensions.dimensions.y,&angle))
					{
						sphere->kollision_radius[sphere->kollision_number]=angle;
						sphere->kollision_partner[sphere->kollision_number]=block;
						sphere->kollision_number++;
					}						
				}
				//Sphere-Antiblock
				if ((object->type==0 && other->type==2) || (object->type==2 && other->type==0))
				{
					pphysik_object block=object;
					pphysik_object sphere=other;
					if (sphere->type==2)
					{
						block=other;
						sphere=object;
					}
					//Seiten durchtesten
					//Unten
					if (sphere->position.x>block->position.x && sphere->position.x<block->position.x+block->dimensions.dimensions.x && block->position.y-sphere->position.y>-sphere->dimensions.radius && block->position.y-sphere->position.y<0)
					{
						sphere->kollision_radius[sphere->kollision_number]=M_PI;
						sphere->kollision_partner[sphere->kollision_number]=block;
						sphere->kollision_number++;
					}
					//Oben
					if (sphere->position.x>block->position.x && sphere->position.x<block->position.x+block->dimensions.dimensions.x && sphere->position.y-(block->position.y+block->dimensions.dimensions.y)>-sphere->dimensions.radius && sphere->position.y-(block->position.y+block->dimensions.dimensions.y)<0)
					{
						sphere->kollision_radius[sphere->kollision_number]=0;
						sphere->kollision_partner[sphere->kollision_number]=block;
						sphere->kollision_number++;
					}
					//Links
					if (sphere->position.y>block->position.y && sphere->position.y<block->position.y+block->dimensions.dimensions.y && block->position.x-sphere->position.x>-sphere->dimensions.radius && block->position.x-sphere->position.x<0)
					{
						sphere->kollision_radius[sphere->kollision_number]=3.0*M_PI/2.0;
						sphere->kollision_partner[sphere->kollision_number]=block;
						sphere->kollision_number++;
					}
					//Rechts
					if (sphere->position.y>block->position.y && sphere->position.y<block->position.y+block->dimensions.dimensions.y && sphere->position.x-(block->position.x+block->dimensions.dimensions.x)>-sphere->dimensions.radius && sphere->position.x-(block->position.x+block->dimensions.dimensions.x)<0)
					{
						sphere->kollision_radius[sphere->kollision_number]=M_PI/2.0;
						sphere->kollision_partner[sphere->kollision_number]=block;
						sphere->kollision_number++;
					}
				}
			}
			other=other->next;
		}
		object=object->next;
	}
}


void calc_world_one_step(pphysik_object world,float drag)
{
	pphysik_object object=world;
	while (object!=NULL)
	{
		object->old_position=object->position;
		object->kollision_number=0;
		object->kollision=false;
		object->old_direction=object->direction;
		object->old_kin_energy=object->kin_energy;
		object=object->next;
	}		

  object=world;
  //neue Position berechnen
	while (object!=NULL)
	{
		if (object->move)
		{
			float speed=sqrt(2.0*object->kin_energy/object->mass);
			object->kin_energy-=drag*(pow(speed+0.001,3.0))*object->mass;
			if (object->kin_energy<0.0)
				object->kin_energy=0.0;
			speed=sqrt(2.0*object->kin_energy/object->mass);
			object->position.x+=sin(object->direction)*speed;
			object->position.y+=cos(object->direction)*speed;
			//Rotation
			rotate_matrix(object->rotation,sin(object->direction-M_PI/2.0),cos(object->direction-M_PI/2.0),0,speed/(object->dimensions.radius));
		}

		object=object->next;
	}
	
  search_kollision(world);

  //Kollisionen ausführen:
 	object=world;
	while (object!=NULL)
	{
		if (object->kollision_number>0)
		{
			object->position=object->old_position;
			int number;
			for (number=0;number<object->kollision_number;number++)
			switch (object->kollision_partner[number]->type)
			{
				case 0: {
					object->kollision_partner[number]->position=object->kollision_partner[number]->old_position;
					float dx=sin(object->old_direction)*object->old_kin_energy;
					float dy=cos(object->old_direction)*object->old_kin_energy;
					float ex=sin(object->kollision_partner[number]->old_direction)*object->kollision_partner[number]->old_kin_energy;
					float ey=cos(object->kollision_partner[number]->old_direction)*object->kollision_partner[number]->old_kin_energy;
					float p=sin(angle_distance(object->old_direction,object->kollision_radius[number]));
					float temp=object->kollision_radius[number]+M_PI;
					if (temp>=2.0*M_PI)
					  temp-=2.0*M_PI;
					float q=sin(angle_distance(object->kollision_partner[number]->old_direction,temp));
					float fx=p*dx+(1.0-q)*ex*(1.0-object->kollision_partner[number]->absorption);
					float fy=p*dy+(1.0-q)*ey*(1.0-object->kollision_partner[number]->absorption);
					float l=sqrt(fx*fx+fy*fy);
					object->kin_energy=l;
					//printf("fx:%.2f fy:%.2f dx:%.2f dy:%.2f ex:%.2f ey:%.2f p:%.2f q:%.2f\n",fx,fy,dx,dy,ex,ey,p,q);
					if (l==0.0)
					  object->direction=0;
					else
					{
						fx/=l;
						fy/=l;
						object->direction=myasin(fx);
						if (fy<0)
							object->direction=M_PI-object->direction;
						if (object->direction<0.0)
							object->direction+=2.0*M_PI;
						if (object->direction>=2.0*M_PI)
							object->direction-=2.0*M_PI;
					}
					fx=q*ex+(1.0-p)*dx*(1.0-object->absorption);
					fy=q*ey+(1.0-p)*dy*(1.0-object->absorption);
					l=sqrt(fx*fx+fy*fy);
					object->kollision_partner[number]->kin_energy=l;
					if (l==0.0)
					  object->kollision_partner[number]->direction=0;
					else
					{
						fx/=l;
						fy/=l;
						object->kollision_partner[number]->direction=myasin(fx);
						if (fy<0)
							object->kollision_partner[number]->direction=M_PI-object->kollision_partner[number]->direction;
						if (object->kollision_partner[number]->direction<0.0)
							object->kollision_partner[number]->direction+=2.0*M_PI;
						if (object->kollision_partner[number]->direction>=2.0*M_PI)
							object->kollision_partner[number]->direction-=2.0*M_PI;
					}	
					object->old_direction=object->direction;
					object->old_kin_energy=object->kin_energy;
					object->kollision_partner[number]->old_direction=object->kollision_partner[number]->direction;
					object->kollision_partner[number]->old_kin_energy=object->kollision_partner[number]->kin_energy;
				} break;
				case 1: case 2:
				  object->kin_energy=object->old_kin_energy*(1.0-object->kollision_partner[number]->absorption);
					object->old_kin_energy=object->kin_energy;
					object->direction=2.0*object->kollision_radius[number]-object->old_direction+M_PI;
					object->old_direction=object->direction;
				break;
			}
		}
		else
		if (!(object->kollision))
		{
			object->old_kin_energy=object->kin_energy;
		}
		
		object=object->next;
	}
	
}
