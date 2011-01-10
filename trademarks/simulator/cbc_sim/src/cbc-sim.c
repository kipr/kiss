/* Includes the Standard IO Library */
#include <stdio.h>
#include <kiss-compat.h>
//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
#include <kiss-input.h>



/* kissSim is an update of iROBOsim, but for KISS-C, the Create & CBC.
Written by David Miller & Charles Winton (C) 2009.

The target robot and computer can be changed by replacing the files simCBC and simCreate.  Those files
create the structures _bob and robot and the functions  kissSimComputerInit (and kissSimRobotInit)
 kissSimComputerUpdate (and kissSimRobotUpdate), kissSimComputerDraw (and kissSimRobotDraw)
They operate on the structures: _bob and robot, resepctively.

The structures should be called _computer and _robot, but I'm tired.  Similarly, the pen should
be coded in the world, rather than partly in this file and partly in simCreate -- but again,
I'm tired -- and hope to have all of this distributed compiled so the teams won't ever need to see
the source.

//////////////////// IROBOsim comments //////////////////////
iROBOsim graphics simulator written by David Miller for IC
   Do not redistribute this file.  For more copies, contact the author or KIPR.
   (C) 2006-2008 David Miller

   Ported for KISS-C by Charles Winton
      1. initial port 6/15/08
	  2. refined port 9/24/08
	  3. added interface buttons and digital sensor displays supported by character generation module 10/19/08
	  4. limited process usage to iROBOupdateState 10/30/08

   The port combines iROBOCreateconstants.ic, iROBO-graphics.ic, iROBOsim-create.ic
   in iROBOsim.h and iROBOsim.c.

   iROBOsim.h is the user include file and has the necessary
   preprocessor #defines and #includes. Colors updated from 16-bit (555) to 24-bit RGB.

   iROBOsim.c has the basic iROBO draw functions

   1/3/08: Changed to be compatible with create. The file simcreatelib.ic is used in
   the case the simulated bot is to behave like a Create module, providing code
   compatibility so the program can also be used with an actual bot.
   It has been ported with only minor adjustment.

   Since KISS-C does not have compile and link edit, iROBOsim.h concludes with
   #include for "iROBOsim.c" and "simcreatelib.c"

   NOTE: commented out from IC are hog_processor() and defer() since not supported in KISS-C
   start_process in iROBOinit has slight syntax adjustment for the way KISS-C handles processes.
*/


/****** Colors (24-bit RGB) ******/
#define RED 255,0,0
#define PINK 255,105,180
#define ORANGE 255,140,0
#define MAGENTA 255,0,255
#define VIOLET 138,43,226
#define GOLD 255,215,0
#define YELLOW 255,255,0
#define LIME 0,255,0
#define GREEN 0,255,0
#define OLIVE 128,128,0
#define TEAL 0,128,128
#define CYAN 0,255,255
#define BLUE 0,0,255
#define ALMOND 255,235,205
#define BROWN 139,69,19
#define WHITE 255,255,255
#define BLACK 0,0,0
#define SHADOW 170,170,170
#define GRAY 220,220,220

#define ClBUMP RED
#define CrBUMP LIME
#define CBODY OLIVE
#define COBST BLUE

#define PI 3.141592654
#define PI2 6.283185307
#define HPI 1.570796327

// Assume a circular robot with a body diameter
#define iROBODIAMETER 32.0
// radius is represented as an int
#define iROBORADIUS 16

// Robot is differentially driven with wheel separation of
// originally 25.8, incremented for turn accuracy
#define iROBOWHEELSEP 25.8

// Each of the drive wheels is driven by a motor with a 1 bit encoder
#define iROBOWHEELDIAM 6.5
#define iROBOTICSPERREV 1600.0
// so the robot has 7.8324283 tics per mm

#define _KISSSIMLOADED 1
void kissSim_init(int world, int rx, int ry, float rt);
void kissSim_end();
void kissSimEnablePause();
void kissSimPause();
void kissSplashBG(int w, int h);//draws splash screen
int kissSimActive();
void kissSimPrintInstructions();

//void kissSimDrawButton(int a, struct __button b, int r, int g, int bb, int rd, int gd, int bd, int rl, int gl, int bl);


struct __window {
	int simWidth;
	int simHeight;
	int rWidth;
	int rHeight;
	int cWidth;
	int cHeight;
	int width;
	int height;
	int world;
} ksWindow = {400,400,400,100,200,500,600,500,0};

struct __button {
	int x1;
	int x2;
	int y1;
	int y2;
	int state;
	int pushState;
	int prevPushState;
	int sticky;
	char ltr;
};

void kissSimDrawButton(struct __button *b, int r, int g, int bb, int rd, int gd, int bd, int rl, int gl, int bl);

#include "bg.c"
#include "character-gen.c"
#include "simWorlds.c"
#include "simCreate.c"

/*#define _KISSSIMLOADED 1
void kissSim_init(int world, int rx, int ry, int rt);
void kissSim_end();
void kissSimEnablePause();
void kissSimPause();
*/
#include "simCBC.c"
#include "simVision.c"
#include "botball.c"


void kissSimPause()
{
	static int state=0, pstate=0;
	state=kiss_get_key(' ');
	_world.simPaused=1;
	while(kissSimActive() && _world.simPaused==1 && !(state && !pstate)){
		pstate=state;
		state=kiss_get_key(' ');
	}
	_world.simPaused=0;
	pstate=state;
}	

void kissSimEnablePause()
{
	static int state=0, pstate=1;
	state=kiss_get_key(' ');
	if(state && !pstate)kissSimPause();
	pstate=state;
}


void kissSplashBG(int w, int h)
{
	int i,offX,offY;
	for (i=0;i<h;i=i+(h/50)+1) {
		offX=38*(((float)i)/(float)(h/8));
		offY=50*(((float)i)/(float)(h/8));
		draw_bg(w/2-offX,h/2-offY,((float)i)/(float)(h/8));
		sleep(.02);
		clear_bg(w/2-offX,h/2-offY,((float)i)/(float)(h/8));
	}
	graphics_fill(255,255,255);
}

void kissSimInBgd()
{
	int r,g,b,help=0;
	static int PenPrevious=0;
	while(!kiss_get_key('Q')){
		if(!kiss_get_key('P')){
			if(PenPrevious){
				PenPrevious=0;
				robot.pen=!robot.pen;
				if(robot.pen && robot.pixelCount==0){
					robot.pixel[0].x=robot.x;
					robot.pixel[0].y=robot.y;
					robot.pixelCount=1;
				}
			}
			PenPrevious=0;
		}
		else PenPrevious=1;
		if(kiss_get_key('H'))help=1;
		else help=0;
		kissSimRobotUpdate();
		kissSimComputerUpdate();
		if(!help && _world.simPaused)graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,GRAY);
		else { 
			if(!help){
				kissSimComputerTone(&r,&g,&b);
				graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,r,g,b);
			}
		}
		kissSimDrawObst();
		kissSimRobotDraw();
		kissSimComputerDraw();
		if(help){//print help screen
			graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,255,200,200);// instruction background is light pink
			kissSimPrintInstructions();
		}
		graphics_update();
		msleep(20);
	}
	kissSim_end();
}


void kissSimPrintInstructions()
{
	int i=0;
	NOBOLD g_printString("          WELCOME TO THE ROBOT SIMULATOR!               ",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("WHEN THE SCREEN IS GRAY, USER CODE IS PAUSED",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("BACKGROUND HUE REFLECTS PITCH (BEEP OR TONE).",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("HIT THE SPACE BAR TO CLEAR INSTRUCTIONS",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("AND HIT THE SPACE BAR AGAIN TO UNPAUSE",5, 5+i*15,TEAL,1.0);
	i=kissSimComputerInstructions(i+2);
	i=kissSimRobotInstructions(i+2);
	i=i+2;
	NOBOLD g_printString("PRESS THE H KEY AT ANY TIME TO GET THIS SCREEN",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("PRESS Q TO EXIT THE SIMULATOR",5, 5+i*15,BLACK,1.0);
}

void kissSim_init(int world, int rx, int ry, float rt)
{
  int i,splash=1;
  if(world<0){world=-world; splash=0;}//a negative world means no splash screen
	kissSimLoadWorld(world);//sets .world, .simHeight & .simWidth (leaving default values if sim is smaller)
	kissSimRobotDim(&ksWindow.rWidth, &ksWindow.rHeight);//set robot window dimensions
	ksWindow.height=ksWindow.simHeight+ksWindow.rHeight;//set overall window height
	kissSimComputerDim(&ksWindow.cWidth, &ksWindow.cHeight);//set computer window dimentsions
	ksWindow.width=ksWindow.simWidth+ksWindow.cWidth;//set overall window width
	graphics_init(ksWindow.width, ksWindow.height);
	graphics_fill(255,255,255);
	glfwSetWindowTitle(_world.title);
	// Draw the spash screen
	if(splash)kissSplashBG(ksWindow.width, ksWindow.height);
	robot.x=rx; robot.y=ry; robot.theta=rt;
	kissSimRobotInit();
	kissSimComputerInit();
	graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,255,200,200);// instruction background is light pink
	kissSimPrintInstructions();
	graphics_update();
	while(!kiss_get_key(' '));while(kiss_get_key(' '));//pause until space bar is hit
	start_process(kissSimInBgd);
	kissSimPause();
}


void kissSim_end() {
printf("Simulation end - "); 
_world.simPaused=0;
if (glfwGetWindowParam(GLFW_OPENED)) { // if we still have graphics
graphics_quit();  // clear out GLFW graphics
}
}

// returns 1 if graphics window is alive and 0 otherwise.
int kissSimActive()
{
int state=!!glfwGetWindowParam(GLFW_OPENED);
if(!state)_world.simPaused=0;
return state;
}


void kissSimDrawButton(struct __button *b, int r, int g, int bb, int rd, int gd, int bd, int rl, int gl, int bl)
{
int x,y;
kiss_get_mouse_position(&x,&y);
b->prevPushState=b->pushState;
b->pushState = kiss_get_mouse_button(0);	
if(x>=b->x1 && x<=b->x2 && y>=b->y1 && y<=b->y2) {
if(!b->prevPushState && b->pushState){
if(b->sticky)b->state=!b->state;
else b->state = 1;
}
}
if(!b->sticky && !b->pushState) b->state=0;
if(b->state){
//		graphics_rectangle_fill(b->x1,b->y1,b->x2-1,b->y2-1,SHADOW);
//		graphics_rectangle_fill(b->x1+1,b->y1+1,b->x2-2,b->y2-2,SHADOW);
graphics_rectangle_fill(b->x1+1,b->y1+1,b->x2+1,b->y2+1,WHITE);
graphics_rectangle_fill(b->x1+2,b->y1+2,b->x2+2,b->y2+2,WHITE);
graphics_rectangle_fill(b->x1+2,b->y1+2,b->x2,b->y2,rd,gd,bd);
}
else {
graphics_rectangle_fill(b->x1+1,b->y1+1,b->x2+1,b->y2+1,SHADOW);
graphics_rectangle_fill(b->x1+2,b->y1+2,b->x2+2,b->y2+2,SHADOW);
graphics_rectangle_fill(b->x1,b->y1,b->x2,b->y2,r,g,bb);
}
BOLD g_printCharacter(b->ltr, b->x1+(b->x2-b->x1)/3, b->y1+(b->y2-b->y1)/4, rl, gl, bl, ((float)(b->y2-b->y1)/11.0));
}






