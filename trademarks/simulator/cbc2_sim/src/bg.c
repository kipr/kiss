/* Includes the Standard IO Library  */
#include <stdio.h>
#include <string.h>
#include <kiss-compat.h>
#include <kiss-graphics.h>
//#define BLACK 0,0,0
//#define RED 255,0,0
//#define WHITE 255,255,255
#define POINT struct xy
struct xy {
	int x;
	int y;
} ;
int draw_bg(int x, int y, float pct);
int clear_bg(int x, int y, float pct);
void circle(POINT ctr, int rad, int r, int g, int b);
void rectangle(POINT ul, POINT lr, int r, int g, int b);
void triangle(POINT p1, POINT p2, POINT p3, int r, int g, int b);
POINT scale_trans(POINT pt);
float __pct;   // scale fraction
POINT __trans; // (x,y) transfer for upper left corner of BG bbox
void set_scale_trans(int x, int y, float pct);

int draw_bg(int x, int y, float pct) {
	// base size is 100 high, 76 wide
	// displace (t.x,t.y) from upper left of bbox 
	// adjust size by pct
	set_scale_trans(x, y, pct);
	POINT hu={22,17}, hl={53,39};    // coords for head block (upper left, lower right)
	POINT bu={22,44}, bl={53,83};    // coords for body block
	POINT ltu={0,66}, ltl={21,100};  // coords for Left Tire block
	POINT rtu={54,66}, rtl={76,100}; // coords for Right Tire block	
	POINT tk={37,4}; int tkrad=4;    // coords and radius for top knot circle
	POINT p1={36,16}, p2={37,9}, p3={39,16};    // coords of the corners of top knot post
	POINT eoL={24,29}; int eoLrad=9;	// eye outline (2 circles and box)
	POINT eoR={50,29}; int eoRrad=9;    //
	POINT eou={31,23}, eol={43,35};     //
	POINT ebL={28,29}; int ebLrad=6;	// eye balls (2 circles and box)
	POINT ebR={46,29}; int ebRrad=6;    //
	POINT ebu={31,27}, ebl={43,33};     //
	POINT pL={32,28}; int pLrad=4;  // left pupil
	POINT pR={42,28}; int pRrad=4;  // right pupil
	POINT V0u={29,48},V0l={32,51};  // b logo - left vertical
	POINT V1u={29,53},V1l={32,56};  //
	POINT V2u={29,58},V2l={32,61};  //
	POINT V3u={29,63},V3l={32,66};  //
	POINT V4u={29,68},V4l={32,71};  //
	POINT V5u={29,73},V5l={32,76};	//
	POINT V6u={44,63},V6l={47,66};  // right vertical
	POINT V7u={44,68},V7l={47,71};  //
	POINT H0u={34,58},H0l={37,61};  // upper horizontal
	POINT H1u={39,58},H1l={43,61};  //
	POINT H2u={34,73},H2l={37,76};  // lower horizontal
	POINT H3u={39,73},H3l={43,76};  //
	
	rectangle(hu,hl,RED);     // head block 
	rectangle(bu,bl,RED);     // body block
	rectangle(ltu,ltl,BLACK);  // L tire block
	rectangle(rtu,rtl,BLACK);  // R tire block
	circle(tk,tkrad,BLACK);               // top knot
	triangle(p1,p2,p3,RED); // top knot post
	circle(eoL,eoLrad,BLACK);  	      // eye outline (2 circles and box)
	circle(eoR,eoRrad,BLACK);         //
	rectangle(eou,eol,BLACK); //
	circle(ebL,ebLrad,WHITE);  		  // eye balls (2 circles and box)
	circle(ebR,ebRrad,WHITE);         //
	rectangle(ebu,ebl,WHITE); //	
	circle(pL,pLrad,BLACK);      // left pupil
	circle(pR,pRrad,BLACK);      // right pupil
	rectangle(V0u,V0l,WHITE);  // b logo - left vertical
	rectangle(V1u,V1l,WHITE);  //
	rectangle(V2u,V2l,WHITE);  //
	rectangle(V3u,V3l,WHITE);  //
	rectangle(V4u,V4l,WHITE);  //
	rectangle(V5u,V5l,WHITE);  //
	rectangle(V6u,V6l,WHITE);  // right vertical
	rectangle(V7u,V7l,WHITE);  // 
	rectangle(H0u,H0l,WHITE);  // upper horizontal
	rectangle(H1u,H1l,WHITE);  // 	
	rectangle(H2u,H2l,WHITE);  // lower horizontal
	rectangle(H3u,H3l,WHITE);  //
	graphics_update();
}
int clear_bg(int x, int y, float pct) {
	// base size is 100 high, 76 wide
	// displace (t.x,t.y) from upper left of bbox 
	// adjust size by pct
	set_scale_trans(x, y, pct);
	POINT hu={22,17}, hl={53,39};    // coords for head block (upper left, lower right)
	POINT bu={22,44}, bl={53,83};    // coords for body block
	POINT ltu={0,66}, ltl={21,100};  // coords for Left Tire block
	POINT rtu={54,66}, rtl={76,100}; // coords for Right Tire block	
	POINT tk={37,4}; int tkrad=4;    // coords and radius for top knot circle
	POINT p1={36,16}, p2={37,9}, p3={39,16};    // coords of the corners of top knot post
	POINT eoL={24,29}; int eoLrad=9;	// eye outline (2 circles and box)
	POINT eoR={50,29}; int eoRrad=9;    //
	POINT eou={31,23}, eol={43,35};     //
	
	rectangle(hu,hl,WHITE);     // head block 
	rectangle(bu,bl,WHITE);     // body block
	rectangle(ltu,ltl,WHITE);  // L tire block
	rectangle(rtu,rtl,WHITE);  // R tire block
	circle(tk,tkrad,WHITE);               // top knot
	triangle(p1,p2,p3,WHITE); // top knot post
	circle(eoL,eoLrad,WHITE);  	      // eye outline (2 circles and box)
	circle(eoR,eoRrad,WHITE);         //
	rectangle(eou,eol,WHITE); //
}

void circle(POINT ctr, int rad, int r, int g, int b) {
	//graphics_circle(ctr.x,ctr.y,rad,r,g,b);
	ctr=scale_trans(ctr); rad=(int)0.5+__pct*(float)rad;
	graphics_circle_fill(ctr.x,ctr.y,rad,r,g,b);
}

void rectangle(POINT ul, POINT lr, int r, int g, int b) {
	//graphics_rectangle(x1,y1,x2,y2,r,g,b);
	ul=scale_trans(ul);
	lr=scale_trans(lr);
	graphics_rectangle_fill(ul.x,ul.y,lr.x,lr.y,r,g,b);
}

void triangle(POINT p1, POINT p2, POINT p3, int r, int g, int b) {
	//graphics_triangle(x1,y1,x2,y2,x3,y3,r,g,b);
	p1=scale_trans(p1);
	p2=scale_trans(p2);
	p3=scale_trans(p3);
	graphics_triangle_fill(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,r,g,b);
}


void set_scale_trans(int x, int y, float pct) {
	__trans.x=x;
	__trans.y=y;
	__pct=pct;
}

POINT scale_trans(POINT pt) {
	pt.x=__trans.x+(int)(0.5+__pct*(float)pt.x);
	pt.y=__trans.y+(int)(0.5+__pct*(float)pt.y);
	return(pt);
	}
	
	
	
	
