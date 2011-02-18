/* Includes the Standard IO Library  */
#include <stdio.h>
#include <math.h>
#include <kiss-compat.h>
#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
#include <kiss-input.h>


#ifndef _VGLIB
#define _VGLIB 1


#define PI 3.141592654
#define UNDEFINED 0
#define CIRCLEFILL 2
#define CIRCLE 1
#define LINE 3
#define TRIANGLEFILL 5
#define TRIANGLE 4
#define DRAW 1
#define ERASE 0/*
//#define RED 2
//#define GREEN 3
#define BLUE 4
#define YELLOW 5
#define BLACK 1
#define WHITE 6
*/

int _vgSpecialColors[7][3]={{0,0,0},{0,0,0},{255,0,0},{0,255,0},{0,0,255},{255,255,0},{255,255,255}};
int _xOffset=0, _yOffset=0, _offsetOn=0;


struct point{ // an x,y point
	float x;
	float y;
};



struct line{ // the two points define the ends of a line segment
	struct point p1;	
	struct point p2;
	int propertiesCached; //set to 1 once the properties have been calculated
	float angle; // the angle (in radians) of the line where the X axis is 0
	float sine; // the sine of the angle
	float cosine; // the cosine of the angle
	float rise; // y2 - y1
	float run; // x2 - x1
	float m; // slope of line segment
	float b; // y intercept of line (not necessarily line segment)
};


struct vg_object{
	float radius;
	struct point ap1; //these are the 0 angle coordinates points
	struct point ap2;
	struct point ap3;
	struct line l1; // these are coordinates to draw
	struct line l2;
	struct line l3;
	int type;
	int r;
	int g;
	int b;
	int er; //colors to use when erasing
	int eg;
	int eb;
	float x; // these are used as rotations and offset when drawing
	float y;
	float theta;
	struct vg_object *next;
};

struct vg_assembly{
	int numParts;
	float x;
	float y;
	float theta; //rotation of assembly where X axis is 0
	float thetaOffset;//to designate new front of object
	int partsCollision; // true if collision check should be done with each part.  0 if OK to use collisionR around offset collision X & Y
	float collisionX;
	float collisionY;
	float collisionR;
	struct vg_object *part;
};
	
void vg_draw_line(struct vg_object *l, float xOff, float yOff, int r, int g, int b);

void vg_draw_circle_fill(struct vg_object *c, float xOff, float yOff, int r, int g, int b);

void vg_draw_circle(struct vg_object *c, float xOff, float yOff, int r, int g, int b);

void vg_draw_triangle_fill(struct vg_object *t, float xOff, float yOff, int r, int g, int b);

void vg_draw_triangle(struct vg_object *t, float xOff, float yOff, int r, int g, int b);

void vg_draw_object(struct vg_object *o, int draw);

void vg_draw_assembly(struct vg_assembly *a, int draw);

void vg_special_colors(int *r, int *g, int *b, int draw);


void vg_make_line(struct vg_object *o, float x1, float y1, float x2, float y2, int r, int g, int b, int er, int eg, int eb);
	
void vg_make_circle_fill(struct vg_object *o, float x1, float y1, float radius, int r, int g, int b, int er, int eg, int eb);

void vg_make_circle(struct vg_object *o, float x1, float y1, float radius, int r, int g, int b, int er, int eg, int eb);

void vg_make_triangle_fill(struct vg_object *o, float x1, float y1, float x2, float y2, float x3, float y3, 
		int r, int g, int b, int er, int eg, int eb);

void vg_make_triangle(struct vg_object *o, float x1, float y1, float x2, float y2, float x3, float y3, 
		int r, int g, int b, int er, int eg, int eb);

void vg_make_assembly(struct vg_assembly *a, int numParts, float x, float y, float theta, struct vg_object *part);
void vg_calculate_assembly_collision_parameters(struct vg_assembly *a);
int vg_assembly_collision(struct vg_assembly *a, struct vg_assembly *b, float ca[]);
int vg_collision_assembly_part(struct vg_assembly *wAssem, struct vg_object *part, float x, float y, float pSin, float pCos, float *collisionAngle);
float vg_normalize_angle(float a);

struct point* vectorSub(struct point *a, struct point *b);
struct point* getClosestPoint(struct point *A, struct point *B, struct point *P);
float distanceBetweenPoints(struct point *a, struct point *b);
struct point* vectorSub(struct point *a, struct point *b);

void vg_beep();
void _vg_beep();

float vg_max(float a, float b);
float vg_min(float a, float b);

/* color map functions */

//assumes 512 x 768 color map where hue is vertically and x=0 to 255 is saturation while 256-511 is value
// Hue starts at red goes through rainbow and comes back towards red.
// sets r,g,b values for corresponding color map coordinates
void vg_get_color_map_values(int i, int j, int *r, int *g, int *b);

#include "vg-library.c"
 
#endif 
