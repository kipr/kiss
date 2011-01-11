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
	char *bb2011Title="Botball 2011", *arenaTitle="2010 Arena", *emptyTitle ="Empty World";
	
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
	
	//*****2011 PVC********************
	//declare pointer variables to the parts of the assembly and the assembly
	struct vg_object *bb2011Obj00=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj01=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj02=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj03=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj04=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj05=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj06=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj07=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj08=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj09=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj10=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj11=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj12=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj13=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj14=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj15=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj16=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj17=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011Obj18=malloc(sizeof(struct vg_object));
	struct vg_assembly *bb2011=malloc(sizeof(struct vg_assembly));
	
	//*****2011 Tape********************
	//declare pointer variables to the parts of the assembly and the assembly
	struct vg_object *bb2011TapeObj00=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj01=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj02=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj03=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj04=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj05=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj06=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj07=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj08=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj09=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj10=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj11=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj12=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj13=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj14=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj15=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj16=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj17=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj18=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj19=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj20=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj21=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj22=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj23=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj24=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj25=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj26=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj27=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj28=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj29=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj30=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj31=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj32=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj33=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj34=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj35=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj36=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj37=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj38=malloc(sizeof(struct vg_object));
	struct vg_object *bb2011TapeObj39=malloc(sizeof(struct vg_object));
	struct vg_assembly *bb2011Tape=malloc(sizeof(struct vg_assembly));
	
	
	//declare background color values to use in graphics_fill
	int fill_r=255, fill_g=255, fill_b=255;
	//end of Declarations...
	
	
	//******BB2011 Arean World Coordiantes
	vg_make_line(bb2011Obj00,0.000000,0.000000,244.000000,0.000000,255,255,255,200,200,200);
	vg_make_line(bb2011Obj01,244.000000,0.000000,244.000000,244.000000,255,255,255,200,200,200);
	bb2011Obj00->next=bb2011Obj01;
	vg_make_line(bb2011Obj02,244.000000,244.000000,0.000000,244.000000,255,255,255,200,200,200);
	bb2011Obj01->next=bb2011Obj02;
	vg_make_line(bb2011Obj03,0.000000,244.000000,0.000000,0.000000,255,255,255,200,200,200);
	bb2011Obj02->next=bb2011Obj03;
	vg_make_line(bb2011Obj04,109.000000,0.000000,109.000000,81.000000,255,255,255,200,200,200);
	bb2011Obj03->next=bb2011Obj04;
	vg_make_line(bb2011Obj05,135.000000,0.000000,135.000000,81.000000,255,255,255,200,200,200);
	bb2011Obj04->next=bb2011Obj05;
	vg_make_line(bb2011Obj06,109.000000,63.000000,135.000000,63.000000,255,255,255,200,200,200);
	bb2011Obj05->next=bb2011Obj06;
	vg_make_line(bb2011Obj07,109.000000,165.000000,109.000000,244.000000,255,255,255,200,200,200);
	bb2011Obj06->next=bb2011Obj07;
	vg_make_line(bb2011Obj08,135.000000,165.000000,135.000000,244.000000,255,255,255,200,200,200);
	bb2011Obj07->next=bb2011Obj08;
	vg_make_line(bb2011Obj09,109.000000,178.000000,135.000000,178.000000,255,255,255,200,200,200);
	bb2011Obj08->next=bb2011Obj09;
	vg_make_line(bb2011Obj10,0.000000,211.000000,109.000000,211.000000,255,255,255,200,200,200);
	bb2011Obj09->next=bb2011Obj10;
	vg_make_line(bb2011Obj11,135.000000,211.000000,244.000000,211.000000,255,255,255,200,200,200);
	bb2011Obj10->next=bb2011Obj11;
	vg_make_line(bb2011Obj12,53.000000,211.000000,53.000000,244.000000,255,255,255,200,200,200);
	bb2011Obj11->next=bb2011Obj12;
	vg_make_line(bb2011Obj13,191.000000,211.000000,191.000000,244.000000,255,255,255,200,200,200);
	bb2011Obj12->next=bb2011Obj13;
	vg_make_line(bb2011Obj14,0.000000,117.000000,10.000000,117.000000,255,255,255,200,200,200);
	bb2011Obj13->next=bb2011Obj14;
	vg_make_line(bb2011Obj15,234.000000,117.000000,244.000000,117.000000,255,255,255,200,200,200);
	bb2011Obj14->next=bb2011Obj15;
	/* vg_make_line(bb2011Obj16,56.000000,308.000000,159.000000,308.000000,255,255,255,200,200,200);
	bb2011Obj15->next=bb2011Obj16;
	vg_make_line(bb2011Obj17,-33.000000,309.000000,-33.000000,331.000000,255,255,255,200,200,200);
	bb2011Obj16->next=bb2011Obj17;
	vg_make_line(bb2011Obj18,109.000000,309.000000,108.000000,334.000000,255,255,255,200,200,200);
	bb2011Obj17->next=bb2011Obj18;*/
	vg_make_assembly(bb2011, 16, 0.000000, 0.000000, 0.000000, bb2011Obj00);
	vg_calculate_assembly_collision_parameters(bb2011);
	bb2011->partsCollision=1;
	bb2011->x=80;
	bb2011->y=80;
	//*****End of Your model code:********************
	
	
	
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
	arena->y=80;
	
	
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
	empty->y=80;
	
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
	arena2010tape->y=80;
	//*****End of Your model code:********************
	
	
	//******** BB 2011 Tape
	vg_make_triangle_fill(bb2011TapeObj00,38.000000,1.000000,38.000000,38.000000,43.000000,38.000000,0,0,0,200,200,200);
	vg_make_triangle_fill(bb2011TapeObj01,43.000000,38.000000,43.000000,1.000000,38.000000,1.000000,0,0,0,200,200,200);//
	bb2011TapeObj00->next=bb2011TapeObj01;
	vg_make_triangle_fill(bb2011TapeObj02,1.000000,43.000000,43.000000,43.000000,1.000000,38.000000,0,0,0,200,200,200);
	bb2011TapeObj01->next=bb2011TapeObj02;
	vg_make_triangle_fill(bb2011TapeObj03,1.000000,38.000000,43.000000,38.000000,43.000000,43.000000,0,0,0,200,200,200);//
	bb2011TapeObj02->next=bb2011TapeObj03;
	vg_make_triangle_fill(bb2011TapeObj04,1.000000,92.000000,15.000000,92.000000,15.000000,97.000000,0,0,0,200,200,200);
	bb2011TapeObj03->next=bb2011TapeObj04;
	vg_make_triangle_fill(bb2011TapeObj05,15.000000,97.000000,1.000000,92.000000,1.000000,97.000000,0,0,0,200,200,200);//
	bb2011TapeObj04->next=bb2011TapeObj05;
	vg_make_triangle_fill(bb2011TapeObj06,15.000000,92.000000,20.000000,92.000000,20.000000,142.000000,0,0,0,200,200,200);
	bb2011TapeObj05->next=bb2011TapeObj06;
	vg_make_triangle_fill(bb2011TapeObj07,20.000000,142.000000,15.000000,142.000000,15.000000,92.000000,0,0,0,200,200,200);//
	bb2011TapeObj06->next=bb2011TapeObj07;
	vg_make_triangle_fill(bb2011TapeObj08,1.000000,137.000000,1.000000,142.000000,20.000000,142.000000,0,0,0,200,200,200);
	bb2011TapeObj07->next=bb2011TapeObj08;
	vg_make_triangle_fill(bb2011TapeObj09,20.000000,142.000000,1.000000,142.000000,20.000000,137.000000,0,0,0,200,200,200);//
	bb2011TapeObj08->next=bb2011TapeObj09;
	vg_make_triangle_fill(bb2011TapeObj10,20.000000,210.000000,25.000000,210.000000,20.000000,178.000000,0,0,0,200,200,200);
	bb2011TapeObj09->next=bb2011TapeObj10;
	vg_make_triangle_fill(bb2011TapeObj11,20.000000,178.000000,25.000000,178.000000,25.000000,210.000000,0,0,0,200,200,200);//
	bb2011TapeObj10->next=bb2011TapeObj11;
	vg_make_triangle_fill(bb2011TapeObj12,20.000000,178.000000,25.000000,179.000000,109.000000,82.000000,0,0,0,200,200,200);
	bb2011TapeObj11->next=bb2011TapeObj12;
	vg_make_triangle_fill(bb2011TapeObj13,109.000000,82.000000,109.000000,87.000000,25.000000,179.000000,0,0,0,200,200,200);//
	bb2011TapeObj12->next=bb2011TapeObj13;
	vg_make_triangle_fill(bb2011TapeObj14,54.000000,205.000000,54.000000,210.000000,108.000000,210.000000,0,0,0,200,200,200);
	bb2011TapeObj13->next=bb2011TapeObj14;
	vg_make_triangle_fill(bb2011TapeObj15,108.000000,210.000000,108.000000,205.000000,54.000000,205.000000,0,0,0,200,200,200);//
	bb2011TapeObj14->next=bb2011TapeObj15;
	vg_make_triangle_fill(bb2011TapeObj16,136.000000,205.000000,190.000000,205.000000,190.000000,210.000000,0,0,0,200,200,200);
	bb2011TapeObj15->next=bb2011TapeObj16;
	vg_make_triangle_fill(bb2011TapeObj17,190.000000,210.000000,136.000000,210.000000,136.000000,205.000000,0,0,0,200,200,200);//
	bb2011TapeObj16->next=bb2011TapeObj17;
	vg_make_triangle_fill(bb2011TapeObj18,219.000000,210.000000,224.000000,210.000000,224.000000,178.000000,0,0,0,200,200,200);
	bb2011TapeObj17->next=bb2011TapeObj18;
	vg_make_triangle_fill(bb2011TapeObj19,224.000000,178.000000,219.000000,178.000000,219.000000,210.000000,0,0,0,200,200,200);//
	bb2011TapeObj18->next=bb2011TapeObj19;
	vg_make_triangle_fill(bb2011TapeObj20,219.000000,178.000000,224.000000,178.000000,135.000000,82.000000,0,0,0,200,200,200);
	bb2011TapeObj19->next=bb2011TapeObj20;
	vg_make_triangle_fill(bb2011TapeObj21,135.000000,82.000000,135.000000,87.000000,219.000000,179.000000,0,0,0,200,200,200);//
	bb2011TapeObj20->next=bb2011TapeObj21;
	vg_make_triangle_fill(bb2011TapeObj22,224.000000,137.000000,224.000000,142.000000,243.000000,137.000000,0,0,0,200,200,200);
	bb2011TapeObj21->next=bb2011TapeObj22;
	vg_make_triangle_fill(bb2011TapeObj23,243.000000,137.000000,243.000000,142.000000,224.000000,142.000000,0,0,0,200,200,200);//
	bb2011TapeObj22->next=bb2011TapeObj23;
	vg_make_triangle_fill(bb2011TapeObj24,224.000000,142.000000,228.000000,142.000000,228.000000,92.000000,0,0,0,200,200,200);
	bb2011TapeObj23->next=bb2011TapeObj24;
	vg_make_triangle_fill(bb2011TapeObj25,228.000000,92.000000,224.000000,92.000000,224.000000,142.000000,0,0,0,200,200,200);//
	bb2011TapeObj24->next=bb2011TapeObj25;
	vg_make_triangle_fill(bb2011TapeObj26,224.000000,92.000000,243.000000,92.000000,243.000000,97.000000,0,0,0,200,200,200);
	bb2011TapeObj25->next=bb2011TapeObj26;
	vg_make_triangle_fill(bb2011TapeObj27,243.000000,97.000000,224.000000,97.000000,224.000000,92.000000,0,0,0,200,200,200);//
	bb2011TapeObj26->next=bb2011TapeObj27;
	vg_make_triangle_fill(bb2011TapeObj28,201.000000,1.000000,206.000000,1.000000,206.000000,42.000000,0,0,0,200,200,200);
	bb2011TapeObj27->next=bb2011TapeObj28;
	vg_make_triangle_fill(bb2011TapeObj29,206.000000,42.000000,201.000000,42.000000,201.000000,1.000000,0,0,0,200,200,200);//
	bb2011TapeObj28->next=bb2011TapeObj29;
	vg_make_triangle_fill(bb2011TapeObj30,201.000000,38.000000,201.000000,42.000000,243.000000,42.000000,0,0,0,200,200,200);
	bb2011TapeObj29->next=bb2011TapeObj30;
	vg_make_triangle_fill(bb2011TapeObj31,243.000000,42.000000,243.000000,38.000000,201.000000,38.000000,0,0,0,200,200,200);//
	bb2011TapeObj30->next=bb2011TapeObj31;
	vg_make_triangle_fill(bb2011TapeObj32,66.000000,1.000000,66.000000,43.000000,71.000000,43.000000,0,0,0,200,200,200);
	bb2011TapeObj31->next=bb2011TapeObj32;
	vg_make_triangle_fill(bb2011TapeObj33,71.000000,43.000000,71.000000,1.000000,66.000000,1.000000,0,0,0,200,200,200);//
	bb2011TapeObj32->next=bb2011TapeObj33;
	vg_make_triangle_fill(bb2011TapeObj34,66.000000,43.000000,66.000000,38.000000,173.000000,38.000000,0,0,0,200,200,200);
	bb2011TapeObj33->next=bb2011TapeObj34;
	vg_make_triangle_fill(bb2011TapeObj35,173.000000,38.000000,173.000000,43.000000,66.000000,43.000000,0,0,0,200,200,200);//
	bb2011TapeObj34->next=bb2011TapeObj35;
	vg_make_triangle_fill(bb2011TapeObj36,173.000000,43.000000,173.000000,1.000000,178.000000,1.000000,0,0,0,200,200,200);
	bb2011TapeObj35->next=bb2011TapeObj36;
	vg_make_triangle_fill(bb2011TapeObj37,178.000000,1.000000,178.000000,43.000000,173.000000,43.000000,0,0,0,200,200,200);//
	bb2011TapeObj36->next=bb2011TapeObj37;
	vg_make_triangle_fill(bb2011TapeObj38,109.000000,64.000000,135.000000,64.000000,135.000000,177.000000,0,0,0,200,200,200);
	bb2011TapeObj37->next=bb2011TapeObj38;
	vg_make_triangle_fill(bb2011TapeObj39,135.000000,177.000000,109.000000,177.000000,109.000000,64.000000,0,0,0,200,200,200);
	bb2011TapeObj38->next=bb2011TapeObj39;
	vg_make_assembly(bb2011Tape, 40, 0.000000, 0.000000, 0.000000, bb2011TapeObj00);
	vg_calculate_assembly_collision_parameters(bb2011Tape);
	bb2011Tape->partsCollision=1;
	bb2011Tape->x=80;
	bb2011Tape->y=80;
	//*****End of Your model code:********************
	
	
	while(kiss_get_mouse_button(0));
	while(!kiss_get_mouse_button(0)){
		graphics_fill(200,200,200);		
		NOBOLD g_printString("2011 Board: SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,TEAL,1.0);
		//NOBOLD g_printString(arenaTitle,20, +20,TEAL,3.0);
		vg_draw_assembly(bb2011,DRAW);
		graphics_update();
		while(!right_button()){
			if(kiss_get_mouse_button(0)){
				w->simPaused = 0;
				w->title=bb2011Title;
				w->obstacles= bb2011;
				w->markings=bb2011Tape;//*************************update
				NOBOLD g_printString("2011 BOARD: SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,200,200,200,1.0);
				return;
			}
		}	
		vg_draw_assembly(bb2011,ERASE);
		while(right_button());
		graphics_fill(200,200,200);		
		//NOBOLD g_printString(emptyTitle,20, +20,TEAL,3.0);
		vg_draw_assembly(empty,DRAW);
		NOBOLD g_printString("Empty with 2011 Tape: SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,TEAL,1.0);
		graphics_update();
		while(!right_button()){
			if(kiss_get_mouse_button(0)){
				w->simPaused =0;
				w->title=emptyTitle;
				w->obstacles= empty;	
				w->markings=bb2011Tape;
				NOBOLD g_printString("EMPTY WITH 2011 TAPE: SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,200,200,200,1.0);
				return;
			}
		}	
		vg_draw_assembly(empty,ERASE); 
		while(right_button());
		graphics_fill(200,200,200);		
		NOBOLD g_printString("2010 Board: SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,TEAL,1.0);
		vg_draw_assembly(arena,DRAW);
		graphics_update();
		while(!right_button()){
			if(kiss_get_mouse_button(0)){
				w->simPaused = 0;
				w->title=arenaTitle;
				w->obstacles= arena;
				w->markings=arena2010tape;
				NOBOLD g_printString("2010 BOARD: SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",5, 10,200,200,200,1.0);
				return;
			}
		}	
		vg_draw_assembly(arena,ERASE);
		while(right_button());
		graphics_fill(200,200,200);		
		NOBOLD g_printString("Empty with 2010 tape: SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,TEAL,1.0);
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
				NOBOLD g_printString("EMPTY WITH 2010 TAPE: SELECT WORLD WITH MOUSE CLICK, USE RIGHT ARROW KEY TO SWITCH",20, 10,200,200,200,1.0);
				return;
			}
		}	
		vg_draw_assembly(empty,ERASE); 
		while(right_button());
	}
}
