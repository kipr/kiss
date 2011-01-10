/* Includes the Standard IO Library  */
#include <stdio.h>
#include <kiss-compat.h>
//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
//#include <kiss-input.h>

#define __RSTATH 75
#define __RSTATMINW 400

//mark that the robot simulated is a Create
#ifndef iROBO_UPDATE_RATE
   #define iROBO_UPDATE_RATE 10
#endif
//max row dimension of obstable array
# ifndef MMAXOBST
	#define MMAXOBST 50
#endif

void kissSimRobotDim(int *w, int *h);
void kissSimRobotDraw();
void kissSimRobotInit();
void kissSimRobotInitButtons(int x0, int y0);
void kissSimRobotProcessButtons();
void kissSimDrawObst();
void kissSimFlashObst(int i);
void kissSimEraseRobot(float x, float y, float theta);
void kissSimDrawRobot(float x, float y, float theta);
int kissSimBump();
int kissSimCollision();
float kissSimNormalizeRad(float angle); //normalize angles between 0 and 2PI
void kissSimCreateUpdateState();

struct _robot{
	float x;
	float y;
	float theta;
	float ctheta; // create rotations are opposite and offset from graphics
	int led_play;
	int led_advance;
	int led_power_r;
	int led_power_g;
	int cliffs[4];
	int lbump;//1 if bumper hits a wall
	int rbump;
	int lcbump; // if lSimBump or L key
	int rcbump;
	float lenc; //replace iROBOenc[4] and [5] from old sim. are zeroed every time create sensor update is done
	float renc;
	int lcspeed;
	int rcspeed;
	//	int pen;//1 if robot should draw
	//	int pixelCount;//how many pixels have been drawn?
	//	struct __pixel pixel[MAXPIXELCOUNT];
	int adv;//buttons updated by simulator
	int play;
	struct __button buttons[2];//play and advance
	float t_update;// time of last update
	//	struct vg_assembly *bot; //this is what gets moved and drawn and checked for collisions
	//	struct vg_assembly *collision;//image for showing collisions
	//	void (* botDraw)(); //this func erases collisions, robot and then draws robot and collisions (updates position between erases and draw)
	//struct _collisions collision;//defined in simBot.c
	//	int useCreate; // true if the mobility is the Create
	//	void (* botMoveUpdate)(); //use this function in sim loop to update robot position
} robot;


#include "simcreatelib.c"

void kissSimRobotDim(int *w, int *h)
{
	*h = __RSTATH;
	*w = (__RSTATMINW > ksWindow.simWidth) ? __RSTATMINW : ksWindow.simWidth;
	if(*w > ksWindow.simWidth) ksWindow.simWidth = __RSTATMINW;
}


void kissSimRobotDraw()
{
	int i,oriX=0,oriY=ksWindow.simHeight,lrX=ksWindow.simWidth, lrY=ksWindow.height;
	//	char data[]="                       0     0     L: 0  R: 0";
	// D g_printString("CREATE SIMULATOR: PWR  PLAY  ADV    BUMPERS",oriX+35, oriY+10,TEAL,1.0);
	static char data[]="       PEN O                       L:    R:  ";
	//	char *play=&data[23], *adv=&data[29], *lb=&data[38], *rb=&data[44]; 
	if(!_kissSimBot.useCreate){//this robot has no create
		NOBOLD g_printString("DB1: Left Motor:0; Right Motor:3; Reflectance: Analog 7",oriX+3, oriY+20,TEAL,1.0);
		return;
	}
	graphics_rectangle_fill(0,oriY,ksWindow.simWidth,oriY+1,BLACK);
	draw_bg(oriX+5,oriY+9,0.36); // Botguy
	NOBOLD g_printString("CREATE SIMULATOR: PWR  PLAY  ADV    BUMPERS",oriX+35, oriY+10,TEAL,1.0);
	kissSimRobotProcessButtons();
	
	NOBOLD g_printString(data,oriX+35, oriY+30,WHITE,1.0);
	if(_kissSimBot.pen){data[12]='N';data[13]=' ';}
	else{data[12]='F';data[13]='F';}
	if(robot.lcbump)data[38]='1'; else data[38]='0';
	if(robot.rcbump)data[44]='1'; else data[44]='0';
	NOBOLD g_printString(data,oriX+35, oriY+30,TEAL,1.0);
	
	graphics_circle_fill(oriX+170,oriY+34,10,robot.led_power_r, robot.led_power_g, 0);//power led
	graphics_circle(oriX+170,oriY+34,10,BLUE);//power led
	if(robot.led_play){graphics_circle_fill(oriX+216,oriY+35,8,GREEN);graphics_circle(oriX+216,oriY+35,8,BLACK);}
	else {graphics_circle_fill(oriX+216,oriY+35,8,BLACK);graphics_circle(oriX+216,oriY+35,8,BLACK);}
	if(robot.led_advance){graphics_circle_fill(oriX+255,oriY+35,8,GREEN);graphics_circle(oriX+255,oriY+35,8,BLACK);}
	else {graphics_circle_fill(oriX+255,oriY+35,8,BLACK);graphics_circle(oriX+255,oriY+35,8,BLACK);}
	for(i=0; i<_kissSimBot.pixelCount;i++) graphics_pixel(_kissSimBot.pixel[i].x,_kissSimBot.pixel[i].y,0,0,0);
	//	kissSimCreateUpdateState();
}

void kissSimRobotUpdate()
{
	//create_sensor_update(); // user code should do this!!!
	if(!_kissSimBot.useCreate)return;//this robot has no create
	kissSimRobotProcessButtons();
	}
	
	void kissSimRobotInit()
	{
	kissSimRobotInitButtons(0,ksWindow.simHeight);
	robot.lenc=robot.renc=_kissSimBot.pen=_kissSimBot.pixelCount=0;
	robot.lcbump=robot.rcbump=gc_play_button=gc_advance_button=0;
	create_sensor_update();
	kissSimRobotUpdate();
	kissSimRobotDraw();
	//	kissSimDrawRobot(robot.x, robot.y, robot.theta);
	}
	
	void kissSimRobotInitButtons(int x0, int y0)
	{
	int i;
	int xoffsets[8]={190,205,229,244,288,303,327,342};
	int y1offset = 27;
	int y2offset = 42;
	for(i=0;i<2;i++){
	robot.buttons[i].x1=x0+xoffsets[i*2];
	robot.buttons[i].y1=y0+y1offset;
	robot.buttons[i].x2=x0+xoffsets[i*2+1];
	robot.buttons[i].y2=y0+y2offset;
	robot.buttons[i].state=0;
	robot.buttons[i].sticky=0;
	robot.buttons[i].ltr='0';
	}
	kissSimRobotProcessButtons();
	}
	
	void kissSimRobotProcessButtons()
	{
	int i;
	if(!_kissSimBot.useCreate)return;//this robot has no create
	if(gc_play_button == 0)robot.buttons[0].ltr='0'; else robot.buttons[0].ltr='1';
	if(gc_advance_button == 0)robot.buttons[1].ltr='0'; else robot.buttons[1].ltr='1';
	if(robot.lbump || kiss_get_key('L')) robot.lcbump=1 ;else robot.lcbump=0;
	if(robot.rbump || kiss_get_key('R')) robot.rcbump=1 ;else robot.rcbump=0;
	for(i=0;i<2;i++){
	kissSimDrawButton(&robot.buttons[i],GRAY,RED,BLACK);
	}
	robot.play=robot.buttons[0].state;
	robot.adv=robot.buttons[1].state;
	}
	
	int kissSimRobotInstructions(int i)
	{
	NOBOLD g_printString("                SIMULATING THE CREATE            ",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("THE POWER, PLAY AND ADVANCE LEDS ARE SHOWN BELOW.",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("USE MOUSE TO PRESS THE PLAY AND ADVANCE BUTTONS.",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("THE STATE OF THE LEFT AND RIGHT BUMPERS IS SHOWN.",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("BUMPERS ARE PRESSED WHEN ROBOT RUNS INTO WALLS.",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("SIMULATED HITS CAN BE CREATED BY PRESSING ",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("THE L AND R KEYS ON THE KEYBOARD.",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("THE P KEY TOGGLES THE ROBOT'S PEN ON AND OFF.",5, 5+i*15,TEAL,1.0);
	return i;
	}
	
	/*
	void kissSimDrawObst()
	{
	int i;
	if(!glfwGetWindowParam(GLFW_OPENED)) return; // exit if no graphics window
	for(i=1;i<_world.maxobst;i++) {
	graphics_line(_world.obstacles[i][0], _world.obstacles[i][1], _world.obstacles[i][2], _world.obstacles[i][3], COBST);
	}
	}
	
	void kissSimFlashObst(int i)
	{
	if(!glfwGetWindowParam(GLFW_OPENED)) return; // exit if no graphics window
	graphics_line(_world.obstacles[i][0], _world.obstacles[i][1], _world.obstacles[i][2], _world.obstacles[i][3], RED);
	}
	*/
	
	void kissSimDrawRobot(float x, float y, float theta)
	{
	if(!glfwGetWindowParam(GLFW_OPENED)) return; // exit if no graphics window
	int lrx = (int)(x+cos(theta)*(float)iROBORADIUS);
	int lry = (int)(y+sin(theta)*(float)iROBORADIUS);
	int rbx = (int)(x+cos(theta+HPI)*(float)iROBORADIUS);
	int rby = (int)(y+sin(theta+HPI)*(float)iROBORADIUS);
	int bbx = (int)(x+cos(theta+PI)*(float)iROBORADIUS);
	int bby = (int)(y+sin(theta+PI)*(float)iROBORADIUS);
	int lbx = (int)(x+cos(theta+PI+HPI)*(float)iROBORADIUS);
	int lby = (int)(y+sin(theta+PI+HPI)*(float)iROBORADIUS);
	//if(robot.lcbump) graphics_circle_fill(x,y,iROBORADIUS,RED);
	//else graphics_circle_fill(x,y,iROBORADIUS,WHITE);
	
	//if(robot.rcbump) graphics_circle_fill(x,y,iROBORADIUS,LIME); 
	//else 
	graphics_circle_fill(x,y,iROBORADIUS,WHITE);
	graphics_line(lrx, lry, lbx, lby, ClBUMP);
	graphics_line(lrx, lry, rbx, rby, CrBUMP);
	graphics_line(bbx, bby, rbx, rby, CBODY);
	graphics_line(bbx, bby, lbx, lby, CBODY);
	graphics_circle(x,y,iROBORADIUS,CBODY);
	}
	
	/*
	int kissSimBump()
	{
	float dx, dy, otheta, dtheta;
	int rx, ry, ox, oy,quad, oquad, mdx, mdy, dq, endpoint=0, horizontal;
	int i = kissSimCollision(); // 0 if none; + if line; - if end point
	int obstoff=2;
	if (i == 0) {
	robot.lbump=robot.rbump=0;
	return(i);
	}
	rx = (int) robot.x; // do the casting just once
	ry = (int) robot.y;
	
	if(i < 0){i=-i; endpoint = 1;} // if - then it was an endpoint
	
	//if(DEBUG)printf("Collision with wall %d", i);
	
	if(endpoint){
	// it is an end point, determine which one. obstoff=0 means start, obstoff=2 means end
	if((abs(rx-_world.obstacles[i][0]) <= iROBORADIUS) && (abs(ry-_world.obstacles[i][1]) <= iROBORADIUS)) obstoff=0;
	dx = (float)_world.obstacles[i][obstoff] - robot.x;
	dy = (float)_world.obstacles[i][obstoff+1] - robot.y;
	if(dx==0.0){
	if(dy<0.0) otheta = HPI;
	else otheta = -HPI;
	}
	else otheta=atan(dy/dx);
	if (dx < 0.0) otheta = PI + otheta; // quads 3 & 2
	else if (dy < 0.0) otheta = PI2 + otheta; // quad 4
	if(DEBUG)printf("endpoint oo %d, \notheta %f \nrtheta %f\n",obstoff, otheta, robot.theta);
	}
	
	if(!endpoint){
	
	if(_world.obstacles[i][0]!=_world.obstacles[i][2]) horizontal=1; // are X1 and X2 == ?
	else horizontal=0;
	
	if(horizontal){
	if(_world.obstacles[i][1] > ry) otheta = HPI; //collision on bottom of circle
	else otheta=HPI+PI; //collision on top of circle
	}
	else{//vertical obstacle
	if(_world.obstacles[i][0] > rx) otheta = 0.0;//collision on right of circle
	else otheta=PI;//collision on left of circle
	}
	}
	otheta = kissSimNormalizeRad(otheta - robot.theta);
	if(DEBUG)printf("dtheta %f\n", otheta);
	if(otheta <= HPI) {robot.rbump = 1;robot.lbump = 0;}
	else {if(otheta <= PI) {robot.rbump = 0;robot.lbump = 0;}
	else {if(otheta <= PI+HPI) {robot.rbump = 0;robot.lbump = 0;}
	else {robot.rbump = 0;robot.lbump = 1;}
	}
	}
	return(i);
	}
	
	/*
	int kissSimCollision()
	{
	int rx = (int) robot.x;
	int ry = (int) robot.y;
	int i;
	for(i=1; i< _world.maxobst; i++){ //check for end point collision
	if((((rx+iROBORADIUS) >= _world.obstacles[i][0]) &&
	((rx-iROBORADIUS) <= _world.obstacles[i][0]) &&
	((ry+iROBORADIUS) >= _world.obstacles[i][1]) &&
	((ry-iROBORADIUS) <= _world.obstacles[i][1])) ||
	(((rx+iROBORADIUS) >= _world.obstacles[i][2]) &&
	((rx-iROBORADIUS) <= _world.obstacles[i][2]) &&
	((ry+iROBORADIUS) >= _world.obstacles[i][3]) &&
	((ry-iROBORADIUS) <= _world.obstacles[i][3]))){
	if(DEBUG)printf("\n*****endpoint******\n");
	return(-i);
	}
	}
	for(i=1; i< _world.maxobst; i++){//check for line segment collision
	if(((rx+iROBORADIUS) >= _world.obstacles[i][0]) &&
	((rx-iROBORADIUS) <= _world.obstacles[i][2]) &&
	((ry+iROBORADIUS) >= _world.obstacles[i][1]) &&
	((ry-iROBORADIUS) <= _world.obstacles[i][3])) {
	if(DEBUG)printf("\n*****line segment******\n");
	return(i);
	}
	}
	return(0);
	}
	
	float KissSimTics_to_dist(float tics) //use constants to convert tics to distance
	{
	float dist;
	dist = PI * iROBOWHEELDIAM * tics / iROBOTICSPERREV;
	return (dist);
	}
	
	float kissSimNormalizeRad(float angle) //normalize angles between 0 and 2PI
	{
	if(angle >= PI2) {return(angle - PI2);}
	else {
	if(angle <0.0) return (angle + PI2);
	else return(angle);
	}
	}
	
	
	void kissSimCreateUpdateState()
	{
	float ctime;
	float tdiff, lenc, renc, ldist, rdist, x_inc, y_inc, theta_inc, dist;
	int i,j,offset,x,y,obst;
	
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
	
	robot.x = robot.x + cos(robot.theta)*dist; //update position and angle
	robot.y = robot.y + sin(robot.theta)*dist;
	robot.ctheta = robot.ctheta + (rdist - ldist)/iROBOWHEELSEP;
	robot.theta = kissSimNormalizeRad(robot.theta + (ldist - rdist)/iROBOWHEELSEP);
	obst=kissSimBump();
	//printf("x %f y %f dist %f  tdiff %f time %f\n",robot.x, robot.y, dist,tdiff,seconds());
	if(DEBUG && obst)printf("Obst = %d\n",obst);
	if(obst){kissSimFlashObst(obst);}
	kissSimDrawRobot(robot.x, robot.y, robot.theta);
	if(robot.pen && robot.pixelCount > 0 && ((int)robot.x != robot.pixel[robot.pixelCount-1].x || (int)robot.y != robot.pixel[robot.pixelCount-1].y)){
	if(robot.pixelCount < MAXPIXELCOUNT){
	robot.pixel[robot.pixelCount].x=(int)robot.x;
	robot.pixel[robot.pixelCount].y=(int)robot.y;
	robot.pixelCount++;
	}
	else robot.pen=0;
	}		
	}
	*/
