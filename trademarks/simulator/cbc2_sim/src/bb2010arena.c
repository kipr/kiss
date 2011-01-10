/* Includes the Standard IO Library  */
//#include <stdio.h>
//#include <kiss-compat.h>
//#include "vg-library.h"

//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
//#include <kiss-input.h>


struct vg_assembly *kissSimChooseWorld(struct __world *w)// display worlds and click on the one you want
{
	int x,y,i=0;
	char *arenaTitle="2010 Arena", *emptyTitle ="Empty World";
	
	//*****BB2010 Arena World********************
	struct vg_object *arenaObj00=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj01=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj02=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj03=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj04=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj05=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj06=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj07=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj08=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj09=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj10=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj11=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj12=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj13=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj14=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj15=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj16=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj17=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj18=malloc(sizeof(struct vg_object));
	struct vg_object *arenaObj19=malloc(sizeof(struct vg_object));
	struct vg_assembly *arena=malloc(sizeof(struct vg_assembly));
	
	//*****Empty World********************
	struct vg_object *emptyObj00=malloc(sizeof(struct vg_object));
	struct vg_object *emptyObj01=malloc(sizeof(struct vg_object));
	struct vg_object *emptyObj02=malloc(sizeof(struct vg_object));
	struct vg_object *emptyObj03=malloc(sizeof(struct vg_object));
	struct vg_assembly *empty=malloc(sizeof(struct vg_assembly));
	
	//*****Tape and oil slicks********************
	struct vg_object *arena2010tapeObj00=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj01=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj02=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj03=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj04=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj05=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj06=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj07=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj08=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj09=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj10=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj11=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj12=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj13=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj14=malloc(sizeof(struct vg_object));
	struct vg_object *arena2010tapeObj15=malloc(sizeof(struct vg_object));
	struct vg_assembly *arena2010tape=malloc(sizeof(struct vg_assembly));
	
	
	
	
	//*****BB2010 Arena World Coordinates********************
	vg_make_line(arenaObj00,0.000000,0.000000,240.000000,0.000000,255,255,255,200,200,200);
	vg_make_line(arenaObj01,0.000000,0.000000,0.000000,90.000000,255,255,255,200,200,200);
	arenaObj00->next=arenaObj01;
	vg_make_line(arenaObj02,240.000000,0.000000,240.000000,120.000000,255,255,255,200,200,200);
	arenaObj01->next=arenaObj02;
	vg_make_line(arenaObj03,0.000000,90.000000,90.000000,90.000000,255,255,255,200,200,200);
	arenaObj02->next=arenaObj03;
	vg_make_line(arenaObj04,30.000000,90.000000,30.000000,120.000000,255,255,255,200,200,200);
	arenaObj03->next=arenaObj04;
	vg_make_line(arenaObj05,90.000000,90.000000,90.000000,120.000000,255,255,255,200,200,200);
	arenaObj04->next=arenaObj05;
	vg_make_line(arenaObj06,60.000000,90.000000,60.000000,240.000000,255,255,255,200,200,200);
	arenaObj05->next=arenaObj06;
	vg_make_line(arenaObj07,240.000000,120.000000,180.000000,120.000000,255,255,255,200,200,200);
	arenaObj06->next=arenaObj07;
	vg_make_line(arenaObj08,60.000000,210.000000,150.000000,210.000000,255,255,255,200,200,200);
	arenaObj07->next=arenaObj08;
	vg_make_line(arenaObj09,90.000000,150.000000,180.000000,150.000000,255,255,255,200,200,200);
	arenaObj08->next=arenaObj09;
	vg_make_line(arenaObj10,180.000000,120.000000,180.000000,270.000000,255,255,255,200,200,200);
	arenaObj09->next=arenaObj10;
	vg_make_line(arenaObj11,0.000000,240.000000,60.000000,240.000000,255,255,255,200,200,200);
	arenaObj10->next=arenaObj11;
	vg_make_line(arenaObj12,0.000000,240.000000,0.000000,360.000000,255,255,255,200,200,200);
	arenaObj11->next=arenaObj12;
	vg_make_line(arenaObj13,0.000000,360.000000,240.000000,360.000000,255,255,255,200,200,200);
	arenaObj12->next=arenaObj13;
	vg_make_line(arenaObj14,240.000000,360.000000,240.000000,270.000000,255,255,255,200,200,200);
	arenaObj13->next=arenaObj14;
	vg_make_line(arenaObj15,240.000000,270.000000,150.000000,270.000000,255,255,255,200,200,200);
	arenaObj14->next=arenaObj15;
	vg_make_line(arenaObj16,150.000000,270.000000,150.000000,240.000000,255,255,255,200,200,200);
	arenaObj15->next=arenaObj16;
	vg_make_line(arenaObj17,150.000000,240.000000,210.000000,240.000000,255,255,255,200,200,200);
	arenaObj16->next=arenaObj17;
	vg_make_line(arenaObj18,210.000000,240.000000,210.000000,270.000000,255,255,255,200,200,200);
	arenaObj17->next=arenaObj18;
	vg_make_line(arenaObj19,30.000000,120.000000,90.000000,120.000000,255,255,255,200,200,200);
	arenaObj18->next=arenaObj19;
	vg_make_assembly(arena, 20, 0.000000, 0.000000, 0.000000, arenaObj00);
	vg_calculate_assembly_collision_parameters(arena);
	arena->partsCollision=1;
	arena->x=80;
	arena->y=20;
	
	
	//*****Empty World Coordinates********************
	vg_make_line(emptyObj00,0.000000,0.000000,360.000000,0.000000,255,255,255,200,200,200);
	vg_make_line(emptyObj01,0.000000,0.000000,0.000000,360.000000,255,255,255,200,200,200);
	emptyObj00->next=emptyObj01;
	vg_make_line(emptyObj02,360.000000,0.000000,360.000000,360.000000,255,255,255,200,200,200);
	emptyObj01->next=emptyObj02;
	vg_make_line(emptyObj03,0.000000,360.000000,360.000000,360.000000,255,255,255,200,200,200);
	emptyObj02->next=emptyObj03;
	vg_make_assembly(empty, 20, 0.000000, 0.000000, 0.000000, emptyObj00);
	vg_calculate_assembly_collision_parameters(empty);
	empty->partsCollision=1;
	empty->x=20;
	empty->y=20;
	
	//******Coordinates for tape and slicks*******************
	vg_make_triangle_fill(arena2010tapeObj00,58.000000,0.000000,58.000000,90.000000,62.000000,90.000000,0,255,0,200,200,200);
	vg_make_triangle_fill(arena2010tapeObj01,58.000000,0.000000,62.000000,0.000000,62.000000,90.000000,0,255,0,200,200,200);
	arena2010tapeObj00->next=arena2010tapeObj01;
	vg_make_triangle_fill(arena2010tapeObj02,88.000000,0.000000,88.000000,90.000000,92.000000,90.000000,255,0,0,200,200,200);
	arena2010tapeObj01->next=arena2010tapeObj02;
	vg_make_triangle_fill(arena2010tapeObj03,88.000000,0.000000,92.000000,0.000000,92.000000,90.000000,255,0,0,200,200,200);
	arena2010tapeObj02->next=arena2010tapeObj03;
	vg_make_triangle_fill(arena2010tapeObj04,148.000000,270.000000,148.000000,360.000000,152.000000,360.000000,255,0,0,200,200,200);
	arena2010tapeObj03->next=arena2010tapeObj04;
	vg_make_triangle_fill(arena2010tapeObj05,148.000000,270.000000,152.000000,270.000000,152.000000,360.000000,255,0,0,200,200,200);
	arena2010tapeObj04->next=arena2010tapeObj05;
	vg_make_triangle_fill(arena2010tapeObj06,178.000000,270.000000,178.000000,360.000000,182.000000,360.000000,0,255,0,200,200,200);
	arena2010tapeObj05->next=arena2010tapeObj06;
	vg_make_triangle_fill(arena2010tapeObj07,178.000000,270.000000,182.000000,270.000000,182.000000,360.000000,0,255,0,200,200,200);
	arena2010tapeObj06->next=arena2010tapeObj07;
	vg_make_circle_fill(arena2010tapeObj08,90.000000,30.000000,5.000000,0,0,0,200,200,200);
	arena2010tapeObj07->next=arena2010tapeObj08;
	vg_make_circle_fill(arena2010tapeObj09,150.000000,30.000000,12.000000,0,0,0,200,200,200);
	arena2010tapeObj08->next=arena2010tapeObj09;
	vg_make_circle_fill(arena2010tapeObj10,210.000000,30.000000,8.000000,0,0,0,200,200,200);
	arena2010tapeObj09->next=arena2010tapeObj10;
	vg_make_circle_fill(arena2010tapeObj11,210.000000,90.000000,12.000000,0,0,0,200,200,200);
	arena2010tapeObj10->next=arena2010tapeObj11;
	vg_make_circle_fill(arena2010tapeObj12,150.000000,330.000000,5.000000,0,0,0,200,200,200);
	arena2010tapeObj11->next=arena2010tapeObj12;
	vg_make_circle_fill(arena2010tapeObj13,90.000000,330.000000,12.000000,0,0,0,200,200,200);
	arena2010tapeObj12->next=arena2010tapeObj13;
	vg_make_circle_fill(arena2010tapeObj14,30.000000,330.000000,8.000000,0,0,0,200,200,200);
	arena2010tapeObj13->next=arena2010tapeObj14;
	vg_make_circle_fill(arena2010tapeObj15,30.000000,270.000000,12.000000,0,0,0,200,200,200);
	arena2010tapeObj14->next=arena2010tapeObj15;
	vg_make_assembly(arena2010tape, 16, 0.000000, 0.000000, 0.000000, arena2010tapeObj00);
	vg_calculate_assembly_collision_parameters(arena2010tape);
	arena2010tape->partsCollision=1;
	arena2010tape->x=80;
	arena2010tape->y=20;
	//*****End of Your model code:********************
	
	
	while(kiss_get_mouse_button(0));
	while(!kiss_get_mouse_button(0)){
		//		graphics_fill(200,200,200);		
		NOBOLD g_printString("SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,TEAL,1.0);
		//NOBOLD g_printString(arenaTitle,20, +20,TEAL,3.0);
		vg_draw_assembly(arena,DRAW);
		graphics_update();
		while(!right_button()){
			if(kiss_get_mouse_button(0)){
				w->simPaused = 0;
				w->title=arenaTitle;
				w->obstacles= arena;
				w->markings=arena2010tape;
				NOBOLD g_printString("SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,200,200,200,1.0);
				return;
			}
		}	
		vg_draw_assembly(arena,ERASE);
		while(right_button());
		//graphics_fill(200,200,200);		
		//NOBOLD g_printString("SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,TEAL,1.0);
		//NOBOLD g_printString(emptyTitle,20, +20,TEAL,3.0);
		vg_draw_assembly(empty,DRAW);
		graphics_update();
		while(!right_button()){
			if(kiss_get_mouse_button(0)){
				w->simPaused =0;
				w->title=emptyTitle;
				w->obstacles= empty;	
				w->markings=arena2010tape;
				NOBOLD g_printString("SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,200,200,200,1.0);
				return;
			}
		}	
		vg_draw_assembly(empty,ERASE); 
		while(right_button());
	}
}
