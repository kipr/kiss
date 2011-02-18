/* Includes the Standard IO Library */
#include <stdio.h>
#include <kiss-compat.h>
//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
//#include <kiss-input.h>

void kissSimLoadWorld(int world);

//max row dimension of obstable array
# ifndef MMAXOBST
#define MMAXOBST 50
#endif
//worlds
#define SIMPLEWORLD 0
#define BB08WORLD 1
#define EMPTYWORLD 2
#define RULERWORLD 3
#define BIGEMPTYWORLD 4
#define BIGARENA 5
#define BIGARENA2 6
#define BB09WORLD 7
/*#ifndef iROBOworld
#define iROBOworld EMPTYWORLD
#endif
*/
#define DEBUG 0

/*//mark that the robot simulated is a Create
#ifndef iROBOCREATE
   #define iROBOCREATE 1
#endif
#ifndef iROBO_UPDATE_RATE
   #define iROBO_UPDATE_RATE 10
#endif
*/
struct __world{
	int maxobst; // # of obstacles in that world
	char title[40]; // title of world
	int obstacles[MMAXOBST][4];//the actual line coordinates for the obstacles
	int simPaused; // 1 if sim paused
} _world;

void kissSimLoadObstacles(int world);


void kissSimLoadWorld(int world)
{
	ksWindow.simWidth = 400;
	ksWindow.simHeight = 400;
	kissSimLoadObstacles(world);
}

void kissSimCopyString(char s1[], int n, char s2[])
{
	strncpy(s1, s2, n);
}

// this function (and the define at the end)
// specify the obstacles in the robot's world
// currently worlds must have only horizontal or vertical lines.
void kissSimLoadObstacles(int world)
{
	_world.simPaused=0;
	switch(world)
	{
		case BB09WORLD:
		{
			_world.maxobst=11; kissSimCopyString(_world.title,40,"kissSim: BB09 Game World");
			
			_world.obstacles[1][0]=0;
			_world.obstacles[1][1]=122;
			_world.obstacles[1][2]=0;
			_world.obstacles[1][3]=244;
			
			_world.obstacles[2][0]=0;
			_world.obstacles[2][1]=122;
			_world.obstacles[2][2]=122;
			_world.obstacles[2][3]=122;

			_world.obstacles[3][0]=122;
			_world.obstacles[3][1]=0;
			_world.obstacles[3][2]=122;
			_world.obstacles[3][3]=122;
			
			_world.obstacles[4][0]=0;
			_world.obstacles[4][1]=244;
			_world.obstacles[4][2]=244;
			_world.obstacles[4][3]=244;
			
			_world.obstacles[5][0]=180;
			_world.obstacles[5][1]=0;
			_world.obstacles[5][2]=180;
			_world.obstacles[5][3]=61;
			
			_world.obstacles[6][0]=186;
			_world.obstacles[6][1]=183;
			_world.obstacles[6][2]=186;
			_world.obstacles[6][3]=244;
			
			_world.obstacles[7][0]=244;
			_world.obstacles[7][1]=122;
			_world.obstacles[7][2]=244;
			_world.obstacles[7][3]=244;
			
			_world.obstacles[8][0]=122;
			_world.obstacles[8][1]=0;
			_world.obstacles[8][2]=366;
			_world.obstacles[8][3]=0;
			//projections
			_world.obstacles[9][0]=244;
			_world.obstacles[9][1]=122;
			_world.obstacles[9][2]=366;
			_world.obstacles[9][3]=122;
			
			_world.obstacles[10][0]=366;
			_world.obstacles[10][1]=0;
			_world.obstacles[10][2]=366;
			_world.obstacles[10][3]=122;
		}
		break;
		case SIMPLEWORLD:
		{
			_world.maxobst=3; kissSimCopyString(_world.title,40,"kissSim: Two Obstacle World");
			
			_world.obstacles[1][0]=0;
			_world.obstacles[1][1]=150;
			_world.obstacles[1][2]=300;
			_world.obstacles[1][3]=150;
			
			_world.obstacles[2][0]=250;
			_world.obstacles[2][1]=10;
			_world.obstacles[2][2]=250;
			_world.obstacles[2][3]=150;
		}
		break;
		case BIGARENA2:
		{
			_world.maxobst=19; kissSimCopyString(_world.title,20,"BIGARENA2");
			// perimeter
			_world.obstacles[1][0]=0;
			_world.obstacles[1][1]=122;
			_world.obstacles[1][2]=0;
			_world.obstacles[1][3]=244;
			
			_world.obstacles[2][0]=0;
			_world.obstacles[2][1]=122;
			_world.obstacles[2][2]=122;
			_world.obstacles[2][3]=122;
			
			_world.obstacles[3][0]=122;
			_world.obstacles[3][1]=0;
			_world.obstacles[3][2]=122;
			_world.obstacles[3][3]=122;
			
			_world.obstacles[4][0]=122;
			_world.obstacles[4][1]=0;
			_world.obstacles[4][2]=366;
			_world.obstacles[4][3]=0;
			
			_world.obstacles[5][0]=366;
			_world.obstacles[5][1]=0;
			_world.obstacles[5][2]=366;
			_world.obstacles[5][3]=122;
			
			_world.obstacles[6][0]=244;
			_world.obstacles[6][1]=122;
			_world.obstacles[6][2]=366;
			_world.obstacles[6][3]=122;
			
			_world.obstacles[7][0]=244;
			_world.obstacles[7][1]=122;
			_world.obstacles[7][2]=244;
			_world.obstacles[7][3]=244;
			
			_world.obstacles[8][0]=0;
			_world.obstacles[8][1]=244;
			_world.obstacles[8][2]=244;
			_world.obstacles[8][3]=244;
			//projections
			_world.obstacles[9][0]=160;
			_world.obstacles[9][1]=0;
			_world.obstacles[9][2]=160;
			_world.obstacles[9][3]=61;
			
			_world.obstacles[10][0]=206;
			_world.obstacles[10][1]=183;
			_world.obstacles[10][2]=206;
			_world.obstacles[10][3]=244;
			
			_world.obstacles[11][0]=305;
			_world.obstacles[11][1]=0;
			_world.obstacles[11][2]=305;
			_world.obstacles[11][3]=31;
			
			_world.obstacles[12][0]=305;
			_world.obstacles[12][1]=91;
			_world.obstacles[12][2]=305;
			_world.obstacles[12][3]=122;
			
			_world.obstacles[13][0]=336;
			_world.obstacles[13][1]=0;
			_world.obstacles[13][2]=336;
			_world.obstacles[13][3]=15;
			
			_world.obstacles[14][0]=336;
			_world.obstacles[14][1]=107;
			_world.obstacles[14][2]=336;
			_world.obstacles[14][3]=122;
			
			_world.obstacles[15][0]=31;
			_world.obstacles[15][1]=122;
			_world.obstacles[15][2]=31;
			_world.obstacles[15][3]=137;
			
			_world.obstacles[16][0]=31;
			_world.obstacles[16][1]=229;
			_world.obstacles[16][2]=31;
			_world.obstacles[16][3]=244;
			
			_world.obstacles[17][0]=61;
			_world.obstacles[17][1]=122;
			_world.obstacles[17][2]=61;
			_world.obstacles[17][3]=153;
			
			_world.obstacles[18][0]=61;
			_world.obstacles[18][1]=213;
			_world.obstacles[18][2]=61;
			_world.obstacles[18][3]=244;
		}	
		break;		
		case BIGARENA:
		{
			_world.maxobst=19; kissSimCopyString(_world.title,20,"BIGARENA");
			_world.obstacles[1][0]=0;
			_world.obstacles[1][1]=0;
			_world.obstacles[1][2]=122;
			_world.obstacles[1][3]=0;
			
			_world.obstacles[2][0]=183;
			_world.obstacles[2][1]=0;
			_world.obstacles[2][2]=305;
			_world.obstacles[2][3]=0;
			
			_world.obstacles[3][0]=82;
			_world.obstacles[3][1]=61;
			_world.obstacles[3][2]=122;
			_world.obstacles[3][3]=61;
			
			_world.obstacles[4][0]=82;
			_world.obstacles[4][1]=61;
			_world.obstacles[4][2]=82;
			_world.obstacles[4][3]=101;
			
			_world.obstacles[5][0]=122;
			_world.obstacles[5][1]=0;
			_world.obstacles[5][2]=122;
			_world.obstacles[5][3]=101;
			
			_world.obstacles[6][0]=122;
			_world.obstacles[6][1]=101;
			_world.obstacles[6][2]=183;
			_world.obstacles[6][3]=101;
			
			_world.obstacles[7][0]=265;
			_world.obstacles[7][1]=0;
			_world.obstacles[7][2]=265;
			_world.obstacles[7][3]=38;
			
			_world.obstacles[8][0]=122;
			_world.obstacles[8][1]=143;
			_world.obstacles[8][2]=183;
			_world.obstacles[8][3]=143;
			
			_world.obstacles[9][0]=122;
			_world.obstacles[9][1]=143;
			_world.obstacles[9][2]=122;
			_world.obstacles[9][3]=244;
			
			_world.obstacles[10][0]=183;
			_world.obstacles[10][1]=143;
			_world.obstacles[10][2]=183;
			_world.obstacles[10][3]=244;
			
			_world.obstacles[11][0]=183;
			_world.obstacles[11][1]=183;
			_world.obstacles[11][2]=223;
			_world.obstacles[11][3]=183;
			
			_world.obstacles[12][0]=223;
			_world.obstacles[12][1]=143;
			_world.obstacles[12][2]=223;
			_world.obstacles[12][3]=183;
			
			_world.obstacles[13][0]=0;
			_world.obstacles[13][1]=0;
			_world.obstacles[13][2]=0;
			_world.obstacles[13][3]=244;
			
			_world.obstacles[14][0]=40;
			_world.obstacles[14][1]=208;
			_world.obstacles[14][2]=40;
			_world.obstacles[14][3]=244;
			
			_world.obstacles[15][0]=305;
			_world.obstacles[15][1]=0;
			_world.obstacles[15][2]=305;
			_world.obstacles[15][3]=244;
			
			_world.obstacles[16][0]=0;
			_world.obstacles[16][1]=244;
			_world.obstacles[16][2]=122;
			_world.obstacles[16][3]=244;
			
			_world.obstacles[17][0]=183;
			_world.obstacles[17][1]=244;
			_world.obstacles[17][2]=305;
			_world.obstacles[17][3]=244;
			
			_world.obstacles[18][0]=183;
			_world.obstacles[18][1]=0;
			_world.obstacles[18][2]=183;
			_world.obstacles[18][3]=101;
			
		}
		break;
		case BIGEMPTYWORLD:
		{
			_world.maxobst=5; kissSimCopyString(_world.title,20,"BIGEMPTYWORLD");
			_world.obstacles[1][0]=0;
			_world.obstacles[1][1]=0;
			_world.obstacles[1][2]=499;
			_world.obstacles[1][3]=0;
			
			_world.obstacles[2][0]=499;
			_world.obstacles[2][1]=0;
			_world.obstacles[2][2]=499;
			_world.obstacles[2][3]=499;
			
			_world.obstacles[3][0]=0;
			_world.obstacles[3][1]=499;
			_world.obstacles[3][2]=499;
			_world.obstacles[3][3]=499;
			
			_world.obstacles[4][0]=0;
			_world.obstacles[4][1]=0;
			_world.obstacles[4][2]=0;
			_world.obstacles[4][3]=499;
		}
		break;
		case BB08WORLD:
		{
			_world.maxobst = 9; kissSimCopyString(_world.title,20,"ARENAWORLD");
			_world.obstacles[1][0]=0;
			_world.obstacles[1][1]=0;
			_world.obstacles[1][2]=239;
			_world.obstacles[1][3]=0;
			
			_world.obstacles[2][0]=239;
			_world.obstacles[2][1]=0;
			_world.obstacles[2][2]=239;
			_world.obstacles[2][3]=159;
			
			_world.obstacles[3][0]=0;
			_world.obstacles[3][1]=159;
			_world.obstacles[3][2]=239;
			_world.obstacles[3][3]=159;
			
			_world.obstacles[4][0]=0;
			_world.obstacles[4][1]=80;
			_world.obstacles[4][2]=50;
			_world.obstacles[4][3]=80;
			
			_world.obstacles[5][0]=50;
			_world.obstacles[5][1]=80;
			_world.obstacles[5][2]=50;
			_world.obstacles[5][3]=120;
			
			_world.obstacles[6][0]=189;
			_world.obstacles[6][1]=80;
			_world.obstacles[6][2]=239;
			_world.obstacles[6][3]=80;
			
			_world.obstacles[7][0]=189;
			_world.obstacles[7][1]=40;
			_world.obstacles[7][2]=189;
			_world.obstacles[7][3]=80;
			
			_world.obstacles[8][0]=0;
			_world.obstacles[8][1]=0;
			_world.obstacles[8][2]=0;
			_world.obstacles[8][3]=159;
		}
		break;
		case EMPTYWORLD:
		{
			_world.maxobst=5; kissSimCopyString(_world.title,20,"EMPTYWORLD");
			_world.obstacles[1][0]=0;
			_world.obstacles[1][1]=0;
			_world.obstacles[1][2]=239;
			_world.obstacles[1][3]=0;
			
			_world.obstacles[2][0]=239;
			_world.obstacles[2][1]=0;
			_world.obstacles[2][2]=239;
			_world.obstacles[2][3]=159;
			
			_world.obstacles[3][0]=0;
			_world.obstacles[3][1]=159;
			_world.obstacles[3][2]=239;
			_world.obstacles[3][3]=159;
			
			_world.obstacles[4][0]=0;
			_world.obstacles[4][1]=0;
			_world.obstacles[4][2]=0;
			_world.obstacles[4][3]=159;
		}
		break;
		case RULERWORLD:
		{
			_world.maxobst=12; kissSimCopyString(_world.title,20,"RULERWORLD");
			_world.obstacles[1][0]=0;
			_world.obstacles[1][1]=0;
			_world.obstacles[1][2]=239;
			_world.obstacles[1][3]=0;
			
			_world.obstacles[2][0]=239;
			_world.obstacles[2][1]=0;
			_world.obstacles[2][2]=239;
			_world.obstacles[2][3]=159;
			
			_world.obstacles[3][0]=0;
			_world.obstacles[3][1]=159;
			_world.obstacles[3][2]=239;
			_world.obstacles[3][3]=159;
			
			_world.obstacles[4][0]=0;
			_world.obstacles[4][1]=0;
			_world.obstacles[4][2]=0;
			_world.obstacles[4][3]=159;
			
			_world.obstacles[5][0]=30;
			_world.obstacles[5][1]=150;
			_world.obstacles[5][2]=30;
			_world.obstacles[5][3]=159;
			
			_world.obstacles[6][0]=60;
			_world.obstacles[6][1]=150;
			_world.obstacles[6][2]=60;
			_world.obstacles[6][3]=159;
			
			_world.obstacles[7][0]=90;
			_world.obstacles[7][1]=150;
			_world.obstacles[7][2]=90;
			_world.obstacles[7][3]=159;
			
			_world.obstacles[8][0]=120;
			_world.obstacles[8][1]=150;
			_world.obstacles[8][2]=120;
			_world.obstacles[8][3]=159;
			
			_world.obstacles[9][0]=150;
			_world.obstacles[9][1]=150;
			_world.obstacles[9][2]=150;
			_world.obstacles[9][3]=159;
			
			_world.obstacles[10][0]=180;
			_world.obstacles[10][1]=150;
			_world.obstacles[10][2]=180;
			_world.obstacles[10][3]=159;
			
			_world.obstacles[11][0]=210;
			_world.obstacles[11][1]=150;
			_world.obstacles[11][2]=210;
			_world.obstacles[11][3]=159;
		}
		break;
	}
}

/************************************************/
/*
void iROBOdrawObst()
{
	int i;
	if(!glfwGetWindowParam(GLFW_OPENED)) return; // exit if no graphics window
	for(i=1;i<_world.maxobst;i++) {
		graphics_line(obstacles[i][0], obstacles[i][1], obstacles[i][2], obstacles[i][3], COBST);
	}
}
*/
