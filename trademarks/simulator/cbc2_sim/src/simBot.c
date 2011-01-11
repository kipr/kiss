/* Includes the Standard IO Library   */
//#include <stdio.h>
//#include <kiss-compat.h>
//#include "vg-library.h"
//#include "character-gen.c"

//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
//#include <kiss-input.h>

void kissSimCreateUpdateState(struct __kissSimBot *b, struct __world *wo );
void kissSimCBCBotUpdateState(struct __kissSimBot *b, struct __world *wo );
void _setAPort7(int s, struct vg_object *o);
void _cliffs(int s, struct vg_object *o);
float _line_length(struct point p1,struct point p2);
float _triangle_area(struct point p1, struct point p2, struct point p3);
float _point_in_triangle(struct point p1, struct point p2, struct point p3, struct point p);



void createBot()
{
	printf("createbot lives!\n");
}

void CBCBot()
{
	printf("cbc-bot lives!\n");
}



void kissSimChooseBot(struct __kissSimBot *theBot)//displays robots, and click on the one you want
{
	int x,y,i=0;
	
	//*****DB1 robot and collision variables:********************
	//declare pointer variables to the parts of the assembly and the assembly
	struct vg_object *DB1Obj00=malloc(sizeof(struct vg_object));
	struct vg_object *DB1Obj01=malloc(sizeof(struct vg_object));
	struct vg_object *DB1Obj02=malloc(sizeof(struct vg_object));
	struct vg_object *DB1Obj03=malloc(sizeof(struct vg_object));
	struct vg_object *DB1Obj04=malloc(sizeof(struct vg_object));
	struct vg_object *DB1Obj05=malloc(sizeof(struct vg_object));
	struct vg_object *DB1Obj06=malloc(sizeof(struct vg_object));
	struct vg_object *DB1Obj07=malloc(sizeof(struct vg_object));
	struct vg_object *DB1Obj08=malloc(sizeof(struct vg_object));
	struct vg_object *DB1Obj09=malloc(sizeof(struct vg_object));
	struct vg_assembly *DB1=malloc(sizeof(struct vg_assembly));
	
	struct vg_object *collisionDB1Obj00=malloc(sizeof(struct vg_object));
	struct vg_object *collisionDB1Obj01=malloc(sizeof(struct vg_object));
	struct vg_assembly *collisionDB1=malloc(sizeof(struct vg_assembly));
	
	struct __reflectSensor *aPort7=malloc(sizeof(struct __reflectSensor));
	
	
	//*****DB2 robot and collision variables:********************
	//declare pointer variables to the parts of the assembly and the assembly
	struct vg_object *DB2Obj00=malloc(sizeof(struct vg_object));
	struct vg_object *DB2Obj01=malloc(sizeof(struct vg_object));
	struct vg_object *DB2Obj02=malloc(sizeof(struct vg_object));
	struct vg_object *DB2Obj03=malloc(sizeof(struct vg_object));
	struct vg_object *DB2Obj04=malloc(sizeof(struct vg_object));
	struct vg_object *DB2Obj05=malloc(sizeof(struct vg_object));
	struct vg_object *DB2Obj06=malloc(sizeof(struct vg_object));
	struct vg_assembly *DB2=malloc(sizeof(struct vg_assembly));
	
	struct vg_object *collisionDB2Obj00=malloc(sizeof(struct vg_object));
	struct vg_object *collisionDB2Obj01=malloc(sizeof(struct vg_object));
	struct vg_assembly *collisionDB2=malloc(sizeof(struct vg_assembly));
	
	struct __reflectSensor *lcliff=malloc(sizeof(struct __reflectSensor));
	struct __reflectSensor *lfcliff=malloc(sizeof(struct __reflectSensor));
	struct __reflectSensor *rfcliff=malloc(sizeof(struct __reflectSensor));
	struct __reflectSensor *rcliff=malloc(sizeof(struct __reflectSensor));
	
	
	//Code for initializing objects and assembly 
	//DB1
	vg_make_triangle_fill(DB1Obj00,-6.000000,-10.000000,-6.000000,1.000000,5.000000,1.000000,0,0,0,200,200,200);
	vg_make_triangle_fill(DB1Obj01,-6.000000,-10.000000,5.000000,-10.000000,5.000000,1.000000,0,0,0,200,200,200);
	DB1Obj00->next=DB1Obj01;
	vg_make_triangle_fill(DB1Obj02,-4.000000,-8.000000,3.000000,-8.000000,3.000000,-3.000000,235,235,255,200,200,200);
	DB1Obj01->next=DB1Obj02;
	vg_make_triangle_fill(DB1Obj03,-4.000000,-8.000000,-4.000000,-3.000000,3.000000,-3.000000,235,235,255,200,200,200);
	DB1Obj02->next=DB1Obj03;
	vg_make_triangle_fill(DB1Obj04,-7.000000,-2.000000,-7.000000,3.000000,-8.000000,3.000000,50,50,50,200,200,200);
	DB1Obj03->next=DB1Obj04;
	vg_make_triangle_fill(DB1Obj05,-7.000000,-2.000000,-8.000000,-2.000000,-8.000000,3.000000,50,50,50,200,200,200);
	DB1Obj04->next=DB1Obj05;
	vg_make_triangle_fill(DB1Obj06,6.000000,-2.000000,6.000000,3.000000,7.000000,3.000000,50,50,50,200,200,200);
	DB1Obj05->next=DB1Obj06;
	vg_make_triangle_fill(DB1Obj07,6.000000,-2.000000,7.000000,-2.000000,7.000000,3.000000,50,50,50,200,200,200);
	DB1Obj06->next=DB1Obj07;
	vg_make_triangle_fill(DB1Obj08,0.000000,-11.000000,0.000000,-14.000000,3.000000,-14.000000,50,50,50,200,200,200);
	DB1Obj07->next=DB1Obj08;
	vg_make_triangle_fill(DB1Obj09,0.000000,-11.000000,3.000000,-11.000000,3.000000,-14.000000,50,50,50,200,200,200);
	DB1Obj08->next=DB1Obj09;
	vg_make_assembly(DB1, 10, 0.000000, 0.000000, PI/2.0, DB1Obj00);
	DB1->thetaOffset=PI/2;//correction for original drawing along Y axis
	vg_calculate_assembly_collision_parameters(DB1);
	DB1->collisionR=DB1->collisionR+2.;
	
	vg_make_circle(collisionDB1Obj00,DB1->collisionR,0.000000,4.000000,255,0,0,200,200,200);
	vg_make_circle_fill(collisionDB1Obj01,DB1->collisionR,0.000000,2.000000,255,0,0,200,200,200);
	collisionDB1Obj00->next=collisionDB1Obj01;
	vg_make_assembly(collisionDB1, 2, 0.000000, 0.000000, 0.000000, collisionDB1Obj00);
	vg_calculate_assembly_collision_parameters(collisionDB1);
	
	
	//DB2
	vg_make_circle_fill(DB2Obj00,0.000000,0.000000,17.000000,255,255,255,200,200,200);
	vg_make_circle(DB2Obj01,14.000000,0.000000,3.000000,0,0,0,200,200,200);
	DB2Obj00->next=DB2Obj01;
	vg_make_triangle_fill(DB2Obj02,-12.000000,-7.000000,-12.000000,6.000000,2.000000,6.000000,0,0,0,200,200,200);
	DB2Obj01->next=DB2Obj02;
	vg_make_triangle_fill(DB2Obj03,-12.000000,-7.000000,2.000000,-7.000000,2.000000,6.000000,0,0,0,200,200,200);
	DB2Obj02->next=DB2Obj03;
	vg_make_triangle_fill(DB2Obj04,1.000000,-4.000000,1.000000,3.000000,-4.000000,3.000000,255,255,255,200,200,200);
	DB2Obj03->next=DB2Obj04;
	vg_make_triangle_fill(DB2Obj05,1.000000,-4.000000,-4.000000,-4.000000,-4.000000,3.000000,255,255,255,200,200,200);
	DB2Obj04->next=DB2Obj05;
	vg_make_circle(DB2Obj06,0.000000,0.000000,17.000000,0,0,0,200,200,200);
	DB2Obj05->next=DB2Obj06;
	vg_make_assembly(DB2, 7, 0.000000, 0.000000, 0.000000, DB2Obj00);
	vg_calculate_assembly_collision_parameters(DB2);
	
	vg_make_circle(collisionDB2Obj00,DB2->collisionR,0.000000,4.000000,255,0,0,200,200,200);
	vg_make_circle_fill(collisionDB2Obj01,DB2->collisionR,0.000000,2.000000,255,0,0,200,200,200);
	collisionDB2Obj00->next=collisionDB2Obj01;
	vg_make_assembly(collisionDB2, 2, 0.000000, 0.000000, 0.000000, collisionDB2Obj00);
	vg_calculate_assembly_collision_parameters(collisionDB2);
	/**********************************/
	
	
	graphics_line(ksWindow.width/2, 50, ksWindow.width/2, ksWindow.height, 0, 0, 0);
	DB1->x = ksWindow.width/4;
	DB1->y = ksWindow.height/2;
	DB2->x = 3*ksWindow.width/4;
	DB2->y = ksWindow.height/2;
	while(kiss_get_mouse_button(0));
	while(!kiss_get_mouse_button(0)){
		kiss_get_mouse_position(&x,&y);
		if(x<ksWindow.width/2){
			graphics_circle_fill(DB1->x, DB1->y, 100, 100,255,100);
			graphics_circle_fill(DB2->x, DB2->y, 100, 150,150,150);
		}
		else{
			graphics_circle_fill(DB1->x, DB1->y, 100, 150,150,150);
			graphics_circle_fill(DB2->x, DB2->y, 100, 100,255,100);
		}			
		vg_draw_assembly(DB1,DRAW);
		vg_draw_assembly(DB2,DRAW);
		/*NO*/BOLD g_printString("SELECT YOUR ROBOT BY CLICKING ON IT",-250+ksWindow.width/2, +20,TEAL,2.0);
		/*NO*/BOLD g_printString("DB1",-150+ksWindow.width/2, +120,TEAL,3.0);
		NOBOLD g_printString("Left Motor is motor 0",-260+ksWindow.width/2, +150,TEAL,1.0);
		NOBOLD g_printString("Right Motor is motor 3",-260+ksWindow.width/2, +160,TEAL,1.0);
		NOBOLD g_printString("Refelectance Sensor is analog port 7",-260+ksWindow.width/2, +170,TEAL,1.0);
		/*NO*/BOLD g_printString("DB2",50+ksWindow.width/2, +120,TEAL,3.0);
		NOBOLD g_printString("Use create commands for mobility",50+ksWindow.width/2, +150,TEAL,1.0);
		NOBOLD g_printString("Cliff and Bump sensors are active",50+ksWindow.width/2, +160,TEAL,1.0);
		i++;
		graphics_update();
	}
	if(x<ksWindow.width/2){//selected DB1
		aPort7->numSensors=1;
		aPort7->x = -2;
		aPort7->y = 0;
		aPort7->setValue = (*_setAPort7);
		theBot->bot = DB1;
		theBot->collision = collisionDB1;
		theBot->useCreate = 0;
		theBot->botMoveUpdate = (*kissSimCBCBotUpdateState);
		theBot->wheelSep = 13.7;
		theBot->wheelDiameter = 4.96;
		theBot->ticPerRev = 850;//seems to vary 840-900 depending on CBC and motor
		theBot->reflectanceSensors = aPort7;
		return ;		
	}
	else{// selected DB2
		lcliff->numSensors=4;
		lcliff->next=lfcliff;
		lcliff->x=13;
		lcliff->y=7;
		lcliff->setValue = (*_cliffs);
		lfcliff->numSensors=3;
		lfcliff->next=rfcliff;
		lfcliff->x=15;
		lfcliff->y=4;
		lfcliff->setValue = (*_cliffs);
		rfcliff->numSensors=2;
		rfcliff->next=rcliff;
		rfcliff->x=15;
		rfcliff->y=-4;
		rfcliff->setValue = (*_cliffs);
		rcliff->numSensors=1;
		rcliff->x=13;
		rcliff->y=-7;
		rcliff->setValue = (*_cliffs);
		theBot->reflectanceSensors = lcliff;
		theBot->bot = DB2;
		theBot->collision = collisionDB2;
		theBot->useCreate = 1;
		theBot->botMoveUpdate = (*kissSimCreateUpdateState);
		theBot->wheelSep = 25.8;
		theBot->wheelDiameter = 6.5;
		theBot->ticPerRev = 1600;
		theBot->prevLenc=0;
		theBot->prevRenc=0;
		return ;		
	}
}
/*
struct __kissSimBot{
	float wheelSep;
	float wheelDiameter;
	float ticPerRev;
	struct vg_assembly *bot; //this is what gets moved and drawn and checked for collisions
	struct vg_assembly *collision;//image for showing collisions
	float ca[2];//collision angles
	int useCreate; // true if the mobility is the Create
	void (* botMoveUpdate)(); //use this function in sim loop to update robot position
}_kissSimBot;
*/

void kissSimCreateUpdateState(struct __kissSimBot *b, struct __world *wo )
{
	float ctime,ca[2],x1,x2,y1,y2;
	float tdiff, lenc, renc, ldist, rdist, x_inc, y_inc, theta_inc, dist;
	int i,j,offset,obst,partCount,sCount;
	struct vg_object *part;//pointer to point to selected objects
	struct __reflectSensor *sensor=b->reflectanceSensors;
	struct point p;
	if(!glfwGetWindowParam(GLFW_OPENED)) return; // exit if no graphics window
	ctime=seconds();
	if(_world.simPaused)robot.t_update=ctime;
	tdiff=ctime - robot.t_update;
	robot.t_update = ctime; //update update time
	robot.lenc=robot.lenc+(tdiff*(float)robot.lcspeed);//update encoders
	robot.renc=robot.renc+(tdiff*(float)robot.rcspeed);
	ldist = (float)robot.lcspeed*tdiff/10.;//mm to cm
	rdist = (float)robot.rcspeed*tdiff/10.;
	dist = (ldist + rdist)/2.0; //assume dt is small enough for straight distance	
	//	printf("paused %d tdiff %f dist %f\n",_world.simPaused,tdiff,dist);
	robot.x=b->bot->x = b->bot->x + cos(b->bot->theta)*dist; //update position and angle
	robot.y=b->bot->y = b->bot->y + sin(b->bot->theta)*dist;
	robot.ctheta = robot.ctheta + (rdist - ldist)/b->wheelSep;
	robot.theta = b->bot->theta = vg_normalize_angle(b->bot->theta + (ldist - rdist)/b->wheelSep);
	robot.lcbump=robot.rcbump=0;
	obst=vg_assembly_collision(b->bot,wo->obstacles,ca);
	if(obst){
		if(ca[0]>=0.0 && ca[0]<(PI/2.0))robot.lbump=1;
		if(obst==2 && ca[0]>=0.0 && ca[0]<(PI/2.0))robot.lbump=1;
		if(ca[0]>= PI1_5)robot.rbump=1;
		if(obst==2 && ca[0]>= PI1_5)robot.rbump=1;
	}
	//printf("x %f y %f dist %f  tdiff %f time %f\n",robot.x, robot.y, dist,tdiff,seconds());
	if(DEBUG && obst)printf("Obst = %d\n",obst);
	//if(obst){printf("#obst %d; angle1 %f; angle2 %f\n",obst,ca[0],ca[1]);}
	if(b->pen && b->pixelCount > 0 && ((int)b->bot->x != b->pixel[b->pixelCount-1].x || (int)b->bot->y != b->pixel[b->pixelCount-1].y)){
		if(b->pixelCount < MAXPIXELCOUNT){
			b->pixel[b->pixelCount].x=(int)b->bot->x;
			b->pixel[b->pixelCount].y=(int)b->bot->y;
			b->pixelCount++;
		}
		else b->pen=0;
	}
	sCount=sensor->numSensors;
	for(j=sCount;j>0;j--){
		part = wo->markings->part;
		//			printf("i%d j%d\n",i,j);
		partCount=wo->markings->numParts;
		for(i=0;i<wo->markings->numParts;i++){
			if(part->type==CIRCLEFILL){
				x1=b->bot->x + cos(b->bot->theta)*sensor->x + sin(b->bot->theta)*sensor->y;
				y1=b->bot->y + cos(b->bot->theta)*sensor->y + sin(b->bot->theta)*sensor->x;
				x2=part->ap1.x+wo->markings->x;
				y2=part->ap1.y+wo->markings->y;
				dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
				if(dist<=part->radius)sensor->setValue(j-1, part);
				else partCount--;
				//				printf("s:(%f,%f) p:(%f,%f) distance: %f radius %f\n",x1,y1,x2,y2,dist,part->radius);
			}
			else {
				if(part->type==TRIANGLEFILL){
					p.x=b->bot->x + cos(b->bot->theta)*sensor->x + sin(b->bot->theta)*sensor->y - wo->markings->x;
					p.y=b->bot->y + cos(b->bot->theta)*sensor->y + sin(b->bot->theta)*sensor->x - wo->markings->y;
					//next line keeps from doing expensive triangle test in most cases
					if((p.x>part->ap1.x && p.x>part->ap2.x && p.x>part->ap3.x) || (p.x<part->ap1.x && p.x<part->ap2.x && p.x<part->ap3.x) || (p.y>part->ap1.y && p.y>part->ap2.y && p.y>part->ap3.y) || (p.y<part->ap1.y && p.x<part->ap2.y && p.y<part->ap3.y)) partCount--;
					else{
						if(_point_in_triangle(part->ap1,part->ap2,part->ap3,p))sensor->setValue(j-1, part);
						else partCount--;
					}
				}
				else partCount--;
				if(partCount<=0){
					sensor->setValue(j-1, NULL);
					break;
				}
			}
			part = part->next;
		}
		if(j>1)sensor=sensor->next;
	}
}


void kissSimCBCBotUpdateState(struct __kissSimBot *b, struct __world *wo )
{
	float ctime,ca[2],x1,x2,y1,y2;
	float tdiff, ldist, rdist, x_inc, y_inc, theta_inc, dist;
	int i,j,offset,obst,partCount;
	struct vg_object *part;//pointer to point to selected objects
	struct point p;
	if(!glfwGetWindowParam(GLFW_OPENED)) return; // exit if no graphics window
	ctime=seconds();
	if(_world.simPaused)_bob.t_update=ctime;
	tdiff=ctime - _bob.t_update;
	_bob.t_update = ctime; //update update time
	ldist = (_bob.motor_counter[0]-b->prevLenc)/b->ticPerRev*PI*b->wheelDiameter;//mm to cm
	rdist = (_bob.motor_counter[3]-b->prevRenc)/b->ticPerRev*PI*b->wheelDiameter;//mm to cm
	dist = (ldist + rdist)/2.0; //assume dt is small enough for straight distance	
	//	printf("paused %d tdiff %f dist %f\n",_world.simPaused,tdiff,dist);
	b->bot->x = b->bot->x + cos(b->bot->theta)*dist; //update position and angle
	b->bot->y = b->bot->y + sin(b->bot->theta)*dist;
	b->bot->theta = vg_normalize_angle(b->bot->theta + (ldist - rdist)/b->wheelSep);
	//printf("x %f y %f dist %f  tdiff %f time %f\n",robot.x, robot.y, dist,tdiff,seconds());
	if(DEBUG && obst)printf("Obst = %d\n",obst);
	//if(obst){printf("#obst %d; angle1 %f; angle2 %f\n",obst,ca[0],ca[1]);}
	if(b->pen && b->pixelCount > 0 && ((int)b->bot->x != b->pixel[b->pixelCount-1].x || (int)b->bot->y != b->pixel[b->pixelCount-1].y)){
		if(b->pixelCount < MAXPIXELCOUNT){
			b->pixel[b->pixelCount].x=(int)b->bot->x;
			b->pixel[b->pixelCount].y=(int)b->bot->y;
			b->pixelCount++;
		}
		else b->pen=0;
	}
	b->prevLenc=_bob.motor_counter[0];//store the current values to be the new previous values
	b->prevRenc=_bob.motor_counter[3];
	part = wo->markings->part;
	partCount=wo->markings->numParts;
	for(i=0;i<wo->markings->numParts;i++){
		if(part->type==CIRCLEFILL){
			x1=b->bot->x + cos(b->bot->theta)*b->reflectanceSensors->x + sin(b->bot->theta)*b->reflectanceSensors->y;
			y1=b->bot->y + cos(b->bot->theta)*b->reflectanceSensors->y + sin(b->bot->theta)*b->reflectanceSensors->x;
			x2=part->ap1.x+wo->markings->x;
			y2=part->ap1.y+wo->markings->y;
			dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
			if(dist<=part->radius)b->reflectanceSensors->setValue(7,part);
			else partCount--;
			//			printf("s:(%f,%f) p:(%f,%f) distance: %f radius %f\n",x1,y1,x2,y2,dist,part->radius);
		}
		else {
			if(part->type==TRIANGLEFILL){
				p.x=b->bot->x + cos(b->bot->theta)*b->reflectanceSensors->x + sin(b->bot->theta)*b->reflectanceSensors->y-wo->markings->x;
				p.y=b->bot->y + cos(b->bot->theta)*b->reflectanceSensors->y + sin(b->bot->theta)*b->reflectanceSensors->x-wo->markings->y;
				//next line keeps from doing expensive triangle test in most cases
				if((p.x>part->ap1.x && p.x>part->ap2.x && p.x>part->ap3.x) || (p.x<part->ap1.x && p.x<part->ap2.x && p.x<part->ap3.x) || (p.y>part->ap1.y && p.y>part->ap2.y && p.y>part->ap3.y) || (p.y<part->ap1.y && p.x<part->ap2.y && p.y<part->ap3.y)) partCount--;
				else{
					if(_point_in_triangle(part->ap1,part->ap2,part->ap3,p))b->reflectanceSensors->setValue(7,part);
					else partCount--;
				}
			}
			else partCount--;
			if(partCount<=0){
				b->reflectanceSensors->setValue(7,NULL);
				break;
			}
		}
		part = part->next;
		}
		}
		
		void _setAPort7(int s, struct vg_object *o)
		{
		int a7=0;
		if(o==NULL)_bob.analogs[s]=0;
		else{
		a7= 4*(o->r+o->g+o->b);
		if(a7>1023)a7=1023;
		_bob.analogs[s]=1023-a7;
		}
		}
		
		void _cliffs(int s, struct vg_object *o)
		{
		int a=0;
		if(o==NULL)robot.cliffs[s]=1023;
		else{
		a= 16*(o->r+o->g+o->b);
		if(a>1023)a=1023;
		robot.cliffs[s]=a;
		//	printf("oil %d index %d\n",a,s);
		}
		}
		
		float _line_length(struct point p1,struct point p2){
		return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
		}
		
		float _triangle_area(struct point p1, struct point p2, struct point p3){
		float a=_line_length(p1,p2);
		float b=_line_length(p1,p3);
		float c=_line_length(p3,p2);
		float semiPerim = (a+b+c)/2.0;
		return sqrt(semiPerim*(semiPerim-a)*(semiPerim-b)*(semiPerim-c));
		}
		
		float _point_in_triangle(struct point p1, struct point p2, struct point p3, struct point p){
		float a12=_triangle_area(p1,p2,p);
		float a13=_triangle_area(p1,p3,p);
		float a23=_triangle_area(p3,p2,p);
		float a123=_triangle_area(p1,p2,p3);
		//	printf("p %f,%f p1 %f,%f p2 %f,%f p3 %f,%f sum =%f\n",p.x,p.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,a123,a12+a13+a23);
		//	msleep(10);
		if((a123+0.01) > (a12+a13+a23) && (a123-0.01) < (a12+a13+a23)) return 1;
		else return 0;
		}
	
