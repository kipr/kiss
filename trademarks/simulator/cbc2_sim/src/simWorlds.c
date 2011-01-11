/* Includes the Standard IO Library  */
#include <stdio.h>
#include <kiss-compat.h>
//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
//#include <kiss-input.h>


#define DEBUG 0

struct __world{
	char *title; // title of world
	struct vg_assembly *obstacles;//the graphics and coordinates of the obstacles
	struct vg_assembly *markings;// graphics and coordinates of non-movable non-obstacles
	int simPaused; // 1 if sim paused
}_world;

void kissSimLoadWorld(struct __world *world);


#include "bb2010arena.c"


void kissSimLoadWorld(struct __world *w)
{
	w->simPaused=0;
	kissSimChooseWorld(w);
	printf("Selected %s\n",w->title);
	glfwSetWindowTitle(w->title);
	vg_draw_assembly(w->obstacles,ERASE);
	graphics_update();
}
