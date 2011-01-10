/* Includes the Standard IO Library  */
#include <stdio.h>
#include <kiss-compat.h>
//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
#include <kiss-input.h>
#include "vg-library.h"



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

// These defines causes the main program to magically call the simulator
/*#ifndef NOGRAPHICSSIMULATOR
int __ks_flg=1;
#define main() __fubar(); int main() {kissSim_init(); __fubar();} int __fubar()
#endif
*/


#ifndef NOGRAPHICSSIMULATOR
int __ks_flg=1;
#define main() fake(); int ___main(); void __main_function(){___main(); __ks_flg=0;} int main() {int p; kissSim_init(); sleep(1.0); p=start_process(__main_function); while(__ks_flg) sleep(.05); kissSimPause(); sleep(.25); kill_process(p);} int ___main()
#define printf ks_printf
#define cbc_printf ks_cbc_printf
#define cbc_display_clear ks_cbc_display_clear
#endif


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
#define PI1_5 4.71238898

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

//mac number of pixels in pen line
#define MAXPIXELCOUNT 20000

#ifndef PROW_CNT
// also used in ks_printf.c
  #define PROW_CNT 9
#endif


#define _KISSSIMLOADED 1
void kissSim_init();
void kissSim_end();
void kissSimEnablePause();
void kissSimPause();
void kissSplashBG(int w, int h);//draws splash screen
int kissSimActive();
void kissSimPrintInstructions0();
void kissSimPrintInstructions1();
void kissSimPositionBot(struct vg_assembly *w, struct vg_assembly *b);

//void kissSimDrawButton(int a, struct __button b, int r, int g, int bb, int rd, int gd, int bd, int rl, int gl, int bl);

struct ___pixel{ int x; int y;};

struct __window {
	int simWidth;
	int simHeight;
	int rWidth;
	int rHeight;
	int cWidth;
	int cHeight;
	int tWidth;
	int tHeight;
	int width;
	int height;
	int world;
} ksWindow = {400,400,400,85,200,600,585,140,600,725,0};

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

//to provide 9 line x 80 col glfw spec for window used by ks_printf.c
struct __prtSpec {
	int x;
	int y;
	int r;
	int g;
	int b;
	float size;
	} __prtArea[PROW_CNT] = {
	{5,0,BLACK,1.0},
	{5,0,BLACK,1.0},
	{5,0,BLACK,1.0},
	{5,0,BLACK,1.0},
	{5,0,BLACK,1.0},
	{5,0,BLACK,1.0},
	{5,0,BLACK,1.0},
	{5,0,BLACK,1.0},
	{5,0,BLACK,1.0}
};

#include "character-gen.c"
#include "ks_printf.c"


struct __reflectSensor{
	float x;
	float y;
	struct __reflectSensor *next;
	int numSensors;
	void (* setValue)(int s, struct vg_object *o);
};

struct __kissSimBot{
	int prevLenc;//the old encoder values
	int prevRenc;
	float wheelSep;
	float wheelDiameter;
	float ticPerRev;
	struct vg_assembly *bot; //this is what gets moved and drawn and checked for collisions
	struct vg_assembly *collision;//image for showing collisions
	float ca[2];//collision angles
	int useCreate; // true if the mobility is the Create
	void (* botMoveUpdate)(); //use this function in sim loop to update robot position
	int pen;//1 if robot should draw
	int pixelCount;//how many pixels have been drawn?
	struct ___pixel pixel[MAXPIXELCOUNT];
	struct __reflectSensor *reflectanceSensors;
}_kissSimBot;


struct _collisions{
	int numCollisions; // 0, 1 or 2
	float collisionAngles[2]; //normalized 0-2pi
};

void kissSimDrawButton(struct __button *b, int r, int g, int bb, int rd, int gd, int bd, int rl, int gl, int bl);

#include "bg.c"
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
#include "simBot.c"


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
	int i,r,g,b,help=0;
	static int PenPrevious=0;
	while(!kiss_get_key('Q')){
		if(!kiss_get_key('P')){
			if(PenPrevious){
				PenPrevious=0;
				_kissSimBot.pen=!_kissSimBot.pen;
				if(_kissSimBot.pen && _kissSimBot.pixelCount==0){
					_kissSimBot.pixel[0].x=_kissSimBot.bot->x;
					_kissSimBot.pixel[0].y=_kissSimBot.bot->y;
					_kissSimBot.pixelCount=1;
				}
			}
			PenPrevious=0;
		}
		else PenPrevious=1;
		if(kiss_get_key('H')){help=1;	_world.simPaused=1;}
		else {help=0; }
		if(!help && _world.simPaused)graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,255,200,200);
		else {
			if(!help){
				graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,200,200,200);
			}
		}
		if(help){//print help screen
			graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,255,200,200);// instruction background is light pink
			kissSimPrintInstructions1();
			_kissSimBot.botMoveUpdate(&_kissSimBot, &_world);
			_world.simPaused=0;
		}
		vg_draw_assembly(_kissSimBot.bot,ERASE);
		_kissSimBot.botMoveUpdate(&_kissSimBot, &_world);
		kissSimComputerUpdate();
		vg_draw_assembly(_world.markings,DRAW);
		vg_draw_assembly(_world.obstacles,DRAW);
		for(i=0; i<_kissSimBot.pixelCount;i++) graphics_pixel(_kissSimBot.pixel[i].x,_kissSimBot.pixel[i].y,0,0,0);
		vg_draw_assembly(_kissSimBot.bot,DRAW);
		kissSimRobotDraw();
		kissSimComputerDraw();
		graphics_update();
		msleep(20);
		#ifndef NOGRAPHICSSIMULATOR
		if (__ks_flg==0) break;
		#endif
	}
	NOBOLD g_printString("    Simulation end: press space bar to close graphics window",ksWindow.tWidth-425,ksWindow.height-15,RED,1.0);
    graphics_update();
	kissSimPause();
	#ifndef NOGRAPHICSSIMULATOR
	__ks_flg=0;
	#endif
	kissSim_end();
}


void kissSimPrintInstructions0()
{
	int i=0;
	NOBOLD g_printString("          WELCOME TO THE ROBOT SIMULATOR!               ",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("HIT THE SPACE BAR TO CLEAR INSTRUCTIONS",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("THE NEXT SCREENS WILL ALLOW YOU TO PICK",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("THE WORLD AND THE ROBOT TO USE IN THE SIM",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("AFTER CLICKING ON YOUR SELECTED ROBOT, USE",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("THE MOUSE TO POSITION THE ROBOT IN THE WORLD",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("AND THE LEFT/RIGHT ARROW KEYS TO SPIN THE ROBOT",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("TO THE DESIRED ORIENTATION.  WHEN THE ROBOT IS",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("PROPERLY PLACED, CLICK THE MOUSE TO START",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("RUNNING THE SIMULATOR.",5, 5+i*15,BLACK,1.0);
	graphics_update();
	//	_world.simPaused=1;
	while(!kiss_get_key(' '));while(kiss_get_key(' '));//pause until space bar is hit
}

void kissSimPrintInstructions1()
{
	int i=0;
	NOBOLD g_printString("          WELCOME TO THE ROBOT SIMULATOR!               ",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("WHEN THE SCREEN IS PINK, USER CODE IS PAUSED",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("BEEPS SHOW UP VISUALLY IN UPPER RIGHT CORNER",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("HIT THE SPACE BAR TO CLEAR INSTRUCTIONS",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("AND HIT THE SPACE BAR AGAIN TO UNPAUSE",5, 5+i*15,TEAL,1.0);
	i=kissSimComputerInstructions(i+2);
	if(_kissSimBot.useCreate){
		i=kissSimRobotInstructions(i+2);
	}
	i++;
	NOBOLD g_printString("PRESS Q TO EXIT THE SIMULATOR",5, 5+i*15,BLACK,1.0);
	i=i+2;
	NOBOLD g_printString("PRESS THE H KEY AT ANY TIME TO GET THIS SCREEN",5, 5+i*15,BLACK,1.0);
	graphics_update();
	while(!kiss_get_key(' '));while(kiss_get_key(' '));//pause until space bar is hit
}



void kissSim_init()
{
	int i,splash=1;
	//if(world<0){world=-world; splash=0;}//a negative world means no splash screen
	//ksWindow.simWidth = 400;
	//ksWindow.simHeight = 400;
	//kissSimRobotDim(&ksWindow.rWidth, &ksWindow.rHeight);//set robot window dimensions
	ksWindow.height=ksWindow.simHeight+ksWindow.rHeight+ksWindow.tHeight;//set overall window height
	//kissSimComputerDim(&ksWindow.cWidth, &ksWindow.cHeight);//set computer window dimentsions
	ksWindow.width=ksWindow.simWidth+ksWindow.cWidth;//set overall window width
	ksWindow.tWidth=ksWindow.width;//text window width same as overall width
	graphics_init(ksWindow.width, ksWindow.height);
	graphics_fill(200,200,200);
	// Draw the spash screen
	//	if(splash)kissSplashBG(ksWindow.width, ksWindow.height);
	kissSimPrintInstructions0();
	graphics_fill(200,200,200);
	kissSimLoadWorld(&_world);//sets .world, .simHeight & .simWidth (leaving default values if sim is smaller)
	kissSimChooseBot(&_kissSimBot);//displays robots, and click on the one you want
	graphics_fill(255,255,255);
	kissSimComputerInit();
	graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,255,200,200);// instruction background is light pink
	graphics_update();
	graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,255,200,200);
	/*	vg_draw_assembly(_world.obstacles,DRAW);
	botProps->bot->x=200;
	botProps->bot->y=200;
	vg_draw_assembly(botProps->bot,DRAW);
	graphics_update();
	*/	kissSimPositionBot(_world.obstacles,_kissSimBot.bot);
	graphics_rectangle_fill(0,0,ksWindow.simWidth-1,ksWindow.simHeight-1,255,200,200);
	kissSimPrintInstructions1();
	graphics_fill(255,255,255);
	graphics_rectangle(ksWindow.width-ksWindow.cWidth,0,ksWindow.width,ksWindow.height,WHITE);
	graphics_rectangle(0,ksWindow.height-ksWindow.rHeight,ksWindow.rWidth,ksWindow.height,WHITE);
	graphics_rectangle(0,0,ksWindow.width,ksWindow.height,200,200,200);
	//(draw in border separating text window from sim window)
	graphics_rectangle_fill(0,ksWindow.height-ksWindow.tHeight-1,ksWindow.width,ksWindow.height-ksWindow.tHeight,BLACK);
	for (i=0;i<PROW_CNT;i++) { //set prtArea row locations and initialize text array
		__prtArea[i].y=15*i+5+ksWindow.height-ksWindow.tHeight;
	}
	graphics_update();
	kissSimRobotInit();
	start_process(kissSimInBgd);
	//	kissSimPause();
	//	while(!kiss_get_key(' '));while(kiss_get_key(' '));//pause until space bar is hit
}

void kissSimPositionBot(struct vg_assembly *w, struct vg_assembly *b)
{
	int x,y;
	b->x=200;b->y=200;
	while(kiss_get_mouse_button(0)); //debounce
	while(!kiss_get_mouse_button(0)){
	graphics_circle_fill(-cos(b->theta)*b->collisionY-sin(b->theta)*b->collisionX+b->x,
	-cos(b->theta)*b->collisionX-sin(b->theta)*b->collisionY+b->y,b->collisionR+3,255,200,200);
	//vg_draw_assembly(b,ERASE);
	kiss_get_mouse_position(&x,&y);
	if(x<380)b->x=x;
	if(y<380)b->y=y;
	if(right_button())b->theta=b->theta+0.01;
	if(left_button())b->theta=b->theta-0.01;
	vg_draw_assembly(_world.markings,DRAW);
	vg_draw_assembly(w,DRAW);
	vg_draw_assembly(b,DRAW);
	graphics_update();
	}
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



