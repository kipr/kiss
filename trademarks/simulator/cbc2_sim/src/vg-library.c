/* Includes the Standard IO Library  */
#include <stdio.h>
#include <kiss-compat.h>
//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
//#include <kiss-input.h>

void vg_draw_line(struct vg_object *l, float xOff, float yOff, int r, int g, int b)
{
	graphics_line(l->l1.p1.x + xOff, l->l1.p1.y + yOff, l->l1.p2.x + xOff, l->l1.p2.y + yOff, r, g, b);
}

void vg_draw_circle_fill(struct vg_object *c, float xOff, float yOff, int r, int g, int b)
{
	graphics_circle_fill(c->l1.p1.x + xOff, c->l1.p1.y + yOff, c->radius, r, g, b);
}

void vg_draw_circle(struct vg_object *c, float xOff, float yOff, int r, int g, int b)
{
	graphics_circle(c->l1.p1.x + xOff, c->l1.p1.y + yOff, c->radius, r, g, b);
}


void vg_draw_triangle_fill(struct vg_object *t, float xOff, float yOff, int r, int g, int b)
{
	graphics_triangle_fill(t->l1.p1.x + xOff, t->l1.p1.y + yOff,
	t->l2.p1.x + xOff, t->l2.p1.y + yOff,
	t->l3.p1.x + xOff, t->l3.p1.y + yOff, r, g, b);
}

void vg_draw_triangle(struct vg_object *t, float xOff, float yOff, int r, int g, int b)
{
	graphics_triangle(t->l1.p1.x + xOff, t->l1.p1.y + yOff,
	t->l2.p1.x + xOff, t->l2.p1.y + yOff,
	t->l3.p1.x + xOff, t->l3.p1.y + yOff, r, g, b);
}


void vg_draw_assembly(struct vg_assembly *a, int draw)
{
	struct vg_object *p;
	int i;
	p=a->part;
	for(i=0;i<(a->numParts);i++){
		p->x = a->x;
		p->y = a->y;
		p->theta = a->theta+a->thetaOffset;
		vg_draw_object(p, draw);
		if(p->next == NULL) break;
		p=p->next;
	}
}

void vg_special_colors(int *r, int *g, int *b, int draw){
	*r=_vgSpecialColors[draw][0];
	*g=_vgSpecialColors[draw][1];
	*b=_vgSpecialColors[draw][2];
}

void vg_draw_object(struct vg_object *o, int draw)
{
	int r,g,b;
	float s=sin(o->theta), c=cos(o->theta);
	if(draw==DRAW){ // if you want to draw (rather than erase)
		r=o->r;
		g=o->g;
		b=o->b;
	}
	else {
		if(draw==ERASE){ //erase colors
			r=o->er;
			g=o->eg;
			b=o->eb;
		}
		else vg_special_colors(&r,&g,&b,draw);
	}
	
	switch (o->type){
		case CIRCLEFILL:{
			o->l1.p1.x=c*o->ap1.x  - s*o->ap1.y;
			o->l1.p1.y=s*o->ap1.x  + c*o->ap1.y;
			vg_draw_circle_fill(o,o->x,o->y,r,g,b);
			break;
		}
		case CIRCLE:{
			o->l1.p1.x=c*o->ap1.x  - s*o->ap1.y;
			o->l1.p1.y=s*o->ap1.x  + c*o->ap1.y;
			vg_draw_circle(o,o->x,o->y,r,g,b);
			break;
		}
		case LINE:{
			o->l1.p1.x=c*o->ap1.x  - s*o->ap1.y;
			o->l1.p1.y=s*o->ap1.x  + c*o->ap1.y;
			o->l1.p2.x=c*o->ap2.x  - s*o->ap2.y;
			o->l1.p2.y=s*o->ap2.x  + c*o->ap2.y;
			vg_draw_line(o,o->x,o->y,r,g,b);
			break;
		}
		case TRIANGLEFILL:{
			o->l1.p1.x=c*o->ap1.x  - s*o->ap1.y;
			o->l1.p1.y=s*o->ap1.x  + c*o->ap1.y;
			o->l1.p2.x=c*o->ap2.x  - s*o->ap2.y;
			o->l1.p2.y=s*o->ap2.x  + c*o->ap2.y;
			o->l2.p1.x=o->l1.p2.x;
			o->l2.p1.y=o->l1.p2.y;
			o->l2.p2.x=c*o->ap3.x  - s*o->ap3.y;
			o->l2.p2.y=s*o->ap3.x  + c*o->ap3.y;
			o->l3.p1.x=o->l2.p2.x;
			o->l3.p1.y=o->l2.p2.y;
			o->l3.p2.x=o->l1.p1.x;
			o->l3.p2.y=o->l1.p1.y;
			vg_draw_triangle_fill(o,o->x,o->y,r,g,b);
			break;
		}
		case TRIANGLE:{
			o->l1.p1.x=c*o->ap1.x  - s*o->ap1.y;
			o->l1.p1.y=s*o->ap1.x  + c*o->ap1.y;
			o->l1.p2.x=c*o->ap2.x  - s*o->ap2.y;
			o->l1.p2.y=s*o->ap2.x  + c*o->ap2.y;
			o->l2.p1.x=o->l1.p2.x;
			o->l2.p1.y=o->l1.p2.y;
			o->l2.p2.x=c*o->ap3.x  - s*o->ap3.y;
			o->l2.p2.y=s*o->ap3.x  + c*o->ap3.y;
			o->l3.p1.x=o->l2.p2.x;
			o->l3.p1.y=o->l2.p2.y;
			o->l3.p2.x=o->l1.p1.x;
			o->l3.p2.y=o->l1.p1.y;
			vg_draw_triangle(o,o->x,o->y,r,g,b);
			break;
		}
		default: {
			printf("vgERR: Illegal vg_object type given to vg_draw_object\n");
		}
	}
}




void vg_make_line(struct vg_object *o, float x1, float y1, float x2, float y2, int r, int g, int b, int er, int eg, int eb)
{
	o->ap1.x=x1;
	o->ap1.y=y1;
	o->ap2.x=x2;
	o->ap2.y=y2;
	o->type=LINE;
	o->x=0.;
	o->y=0.;
	o->theta=0.;
	o->r=r;
	o->g=g;
	o->b=b;
	o->er=er;
	o->eg=eg;
	o->eb=eb;
	o->next=NULL;
}

void vg_make_circle(struct vg_object *o, float x1, float y1, float radius, int r, int g, int b, int er, int eg, int eb)
{
	o->radius=radius;
	o->ap1.x=x1;
	o->ap1.y=y1;
	o->type=CIRCLE;
	o->x=0.;
	o->y=0.;
	o->theta=0.;
	o->r=r;
	o->g=g;
	o->b=b;
	o->er=er;
	o->eg=eg;
	o->eb=eb;
	o->next=NULL;
}

void vg_make_circle_fill(struct vg_object *o, float x1, float y1, float radius, int r, int g, int b, int er, int eg, int eb)
{
	o->radius=radius;
	o->ap1.x=x1;
	o->ap1.y=y1;
	o->type=CIRCLEFILL;
	o->x=0.;
	o->y=0.;
	o->theta=0.;
	o->r=r;
	o->g=g;
	o->b=b;
	o->er=er;
	o->eg=eg;
	o->next=NULL;
	o->eb=eb;
}


void vg_make_triangle_fill(struct vg_object *o, float x1, float y1, float x2, float y2, float x3, float y3, 
int r, int g, int b, int er, int eg, int eb)
{
	o->ap1.x=x1;
	o->ap1.y=y1;
	o->ap2.x=x2;
	o->ap2.y=y2;
	o->ap3.x=x3;
	o->ap3.y=y3;
	o->type=TRIANGLEFILL;
	o->x=0.;
	o->y=0.;
	o->theta=0.;
	o->r=r;
	o->g=g;
	o->b=b;
	o->er=er;
	o->eg=eg;
	o->eb=eb;
	o->next=NULL;
}

void vg_make_triangle(struct vg_object *o, float x1, float y1, float x2, float y2, float x3, float y3, 
int r, int g, int b, int er, int eg, int eb)
{
	o->ap1.x=x1;
	o->ap1.y=y1;
	o->ap2.x=x2;
	o->ap2.y=y2;
	o->ap3.x=x3;
	o->ap3.y=y3;
	o->type=TRIANGLE;
	o->x=0.;
	o->y=0.;
	o->theta=0.;
	o->r=r;
	o->g=g;
	o->b=b;
	o->er=er;
	o->eg=eg;
	o->eb=eb;
	o->next=NULL;
}

void  vg_make_assembly(struct vg_assembly *a, int numParts, float x, float y, float theta, struct vg_object *part)
{
	a->numParts=numParts;
	a->x=x;
	a->y=y;
	a->theta=theta;
	a->part=part;
	a->partsCollision = 0;//default to radius calculation
	a->thetaOffset = 0.0;//default as drawn
}

float vg_max(float a, float b){return(a<b?b:a);}
float vg_min(float a, float b){return(a>b?b:a);}

void vg_calculate_assembly_collision_parameters(struct vg_assembly *a){
	int i;
	struct vg_object *tmp;
	float xmax=-10000.0, xmin=10000.0,ymax=-10000.0,ymin=10000.0,x,y;
	tmp=a->part;
	for(i=0;i<a->numParts;i++){
		if(tmp->type<LINE){
			xmax=vg_max(xmax, tmp->ap1.x+tmp->radius);
			ymax=vg_max(ymax, tmp->ap1.y+tmp->radius);
			xmin=vg_min(xmin, tmp->ap1.x-tmp->radius);
			ymin=vg_min(ymin, tmp->ap1.y-tmp->radius);
		}
		if(tmp->type==LINE){
			xmax=vg_max(xmax, tmp->ap1.x);
			ymax=vg_max(ymax, tmp->ap1.y);
			xmin=vg_min(xmin, tmp->ap1.x);
			ymin=vg_min(ymin, tmp->ap1.y);
			xmax=vg_max(xmax, tmp->ap2.x);
			ymax=vg_max(ymax, tmp->ap2.y);
			xmin=vg_min(xmin, tmp->ap2.x);
			ymin=vg_min(ymin, tmp->ap2.y);
		}
		if(tmp->type>LINE){
			xmax=vg_max(xmax, tmp->ap1.x);
			ymax=vg_max(ymax, tmp->ap1.y);
			xmin=vg_min(xmin, tmp->ap1.x);
			ymin=vg_min(ymin, tmp->ap1.y);
			xmax=vg_max(xmax, tmp->ap2.x);
			ymax=vg_max(ymax, tmp->ap2.y);
			xmin=vg_min(xmin, tmp->ap2.x);
			ymin=vg_min(ymin, tmp->ap2.y);
			xmax=vg_max(xmax, tmp->ap3.x);
			ymax=vg_max(ymax, tmp->ap3.y);
			xmin=vg_min(xmin, tmp->ap3.x);
			ymin=vg_min(ymin, tmp->ap3.y);
		}
		if(tmp->next==NULL)break;
		tmp=tmp->next;
		
	}
	x=xmin+(xmax-xmin)/2.0;
	y=ymin+(ymax-ymin)/2.0;
	a->collisionR=vg_max((xmax-xmin)/2.0,(ymax-ymin)/2.0);
	a->collisionX = (xmax+xmin)/2.0;
	a->collisionY = (ymax+ymin)/2.0;
//	a->collisionX = x*cos(a->thetaOffset) + y*sin(a->thetaOffset);
//	a->collisionY = y*cos(a->thetaOffset) + x*sin(a->thetaOffset);
}

// returns angle from a's center to point of collision
int vg_assembly_collision(struct vg_assembly *a, struct vg_assembly *b, float ca[]){
	float ax,ay,bx,by,dist,ac=cos(a->theta),as=sin(a->theta),bc=cos(b->theta),bs=sin(b->theta), collisionAngle;
	struct vg_object *p; struct vg_assembly *pAssem, *wAssem;
	int i,collisionCount=0;
	if(!a->partsCollision && !b->partsCollision){
		ax=a->x +(a->collisionX*ac + a->collisionY*as);
		ay=a->y +(-a->collisionX*as + a->collisionY*ac);
		bx=b->x +(b->collisionX*bc + b->collisionY*bs);
		by=b->y +(-b->collisionX*bs + b->collisionY*bc);
		dist=sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by));
		//printf("*******a %f,%f b %f,%f dist %f\n",ax,ay, bx,by,dist);
		if(dist > (a->collisionR + b->collisionR))return(0);
		else {
			ca[0] = vg_normalize_angle(a->theta + atan2(b->y - a->y, b->x - a->x));
			return(1);
		}
	}
	pAssem = b; wAssem=a; //check assembly a against each of the parts in b
	p=pAssem->part;
	for(i=0;i<(pAssem->numParts);i++){
		//p->x = pAssem->x;
		//p->y = pAssem->y;
		p->theta = pAssem->theta;
		if(vg_collision_assembly_part(wAssem, p,pAssem->x,pAssem->y,bs,bc, &collisionAngle)){
			ca[collisionCount] = vg_normalize_angle(2*PI-collisionAngle);
			collisionCount++;
		}
		if(p->next == NULL || collisionCount > 1) break;
		p=p->next;
	}
	return(collisionCount);
}

// returns 1 if there is a collision between the assembly and the part, and returns 0 otherwise
// if there is a collision, then the parameter collisionAngle is set of the angle, normalized in the coordinate system of the a
// assembly to the point of collision.
int vg_collision_assembly_part(struct vg_assembly *wAssem, struct vg_object *part, float x, float y, float pSin, float pCos, float *collisionAngle)
{
	int numOfPoints;
	float d1,d2,d3;
	struct point *a=malloc(sizeof(struct point)), *b=malloc(sizeof(struct point)), 
	*p=malloc(sizeof(struct point)), *cp=malloc(sizeof(struct point));
	p->x = cos(wAssem->theta /*+ wAssem->thetaOffset*/)*wAssem->collisionX + sin(wAssem->theta /*+ wAssem->thetaOffset*/)*wAssem->collisionY + wAssem->x;
	p->y = cos(wAssem->theta /*+ wAssem->thetaOffset*/)*wAssem->collisionY + sin(wAssem->theta /*+ wAssem->thetaOffset*/)*wAssem->collisionX + wAssem->y; // the whole assembly collision distance is determined by its radius
	*collisionAngle =5.0;	
	if(part->type==CIRCLE || part->type==CIRCLEFILL){//is the part a circle?
		a->x = part->ap1.x*pCos + part->ap1.y*pSin + x;
		a->y = part->ap1.y*pCos + part->ap1.x*pSin + y;
		d1=distanceBetweenPoints(p,a);
		if(d1 > (wAssem->collisionR + part->radius))return 0;//no collision
		else {
			*collisionAngle = atan2(a->y - p->y, a->x - p->x) - wAssem->theta;
			return 1;
		}
	}
	if(part->type==LINE || part->type==TRIANGLE || part->type==TRIANGLEFILL){//is the part a line? or first line of triangle?
		a->x = part->ap1.x*pCos + part->ap1.y*pSin + x;
		a->y = part->ap1.y*pCos + part->ap1.x*pSin + y;
		b->x = part->ap2.x*pCos + part->ap2.y*pSin + x;
		b->y = part->ap2.y*pCos + part->ap2.x*pSin + y;
		d1=distanceBetweenPoints(p,cp=getClosestPoint(a, b, p));
		if(d1 < wAssem->collisionR){
		*collisionAngle = atan2(cp->y - p->y, cp->x - p->x) - wAssem->theta;
		return 1;
		}
		}
		if(part->type==TRIANGLE || part->type==TRIANGLEFILL){// 2nd line of triangle
		a->x = part->ap3.x*pCos + part->ap3.y*pSin + x;
		a->y = part->ap3.y*pCos + part->ap3.x*pSin + y;
		b->x = part->ap2.x*pCos + part->ap2.y*pSin + x;
		b->y = part->ap2.y*pCos + part->ap2.x*pSin + y;
		d1=distanceBetweenPoints(p,cp=getClosestPoint(a, b, p));
		if(d1 < wAssem->collisionR){
		*collisionAngle = atan2(cp->y - p->y, cp->x - p->x) - wAssem->theta;
		return 1;
		}
		}
		if(part->type==TRIANGLE || part->type==TRIANGLEFILL){// 3rd line of triangle
		a->x = part->ap1.x*pCos + part->ap1.y*pSin + x;
		a->y = part->ap1.y*pCos + part->ap1.x*pSin + y;
		b->x = part->ap3.x*pCos + part->ap3.y*pSin + x;
		b->y = part->ap3.y*pCos + part->ap3.x*pSin + y;
		d1=distanceBetweenPoints(p,cp=getClosestPoint(a, b, p));
		if(d1 < wAssem->collisionR){
		*collisionAngle = atan2(cp->y - p->y, cp->x - p->x) - wAssem->theta;
		return 1;
		}
		}
		return 0; // no collision
		}
		
		
		void vg_beep(){ start_process(_vg_beep);}
		void _vg_beep(){printf("%c\n",7);}
		
		
		
		//assumes 512 x 768 color map where hue is vertically and x=0 to 255 is saturation while 256-511 is value
		// Hue starts at red goes through rainbow and comes back towards red.
		void vg_get_color_map_values(int i, int j, int *r, int *g, int *b)
		{
		float jrat,lg,crat;
		if(j<128){ // red to yellow
		if(i<256){
		jrat=(float)(j%128)/128.0;
		*r=255; *g=255-(int)((float)i*(1.-jrat)); *b=255-i;
		return;
		}
		else {
		jrat=(float)(j%128)/128.0;
		lg=jrat*255.0;
		*r=255-(i-256); *g=(int)(lg-((float)(i-256)*jrat)); *b=0;
		return;
		}
		}
		if(j<256){ // yellow to green
		if(i<256){
		jrat=(1.0 - (float)(j%128)/128.0);
		*g=255; *r=255-(int)((float)i*(1.-jrat)); *b=255-i;
		return;
		}
		else {
		jrat=(1.-(float)(j%128)/128.0);
		lg=jrat*255.0;
		*r=(int)lg-(int)((float)(i-256)*jrat); *g=255-(i-256); *b=0;
		return;
		}
		}
		if(j<384){ // green to cyan
		if(i<256){
		jrat=(float)(j%128)/128.0;
		*g=255; *b=255-(int)((float)i*(1.-jrat)); *r=255-i;
		return;
		}
		else {
		jrat=(float)(j%128)/128.0;
		lg=jrat*255.0;
		*g=255-(i-256); *b=(int)(lg-((float)(i-256)*jrat)); *r=0;
		return;
		}
		}
		if(j<512){ // cyan to blue
		if(i<256){
		jrat=(1.0 - (float)(j%128)/128.0);
		*b=255; *g=255-(int)((float)i*(1.-jrat)); *r=255-i;
		return;
		}
		else {
		jrat=(1.-(float)(j%128)/128.0);
		lg=jrat*255.0;
		*g=(int)lg-(int)((float)(i-256)*jrat); *b=255-(i-256); *r=0;
		return;
		}
		}
		if(j<640){ // blue to magenta
		if(i<256){
		jrat=(float)(j%128)/128.0;
		*b=255; *r=255-(int)((float)i*(1.-jrat)); *g=255-i;
		return;
		}
		else {
		jrat=(float)(j%128)/128.0;
		lg=jrat*255.0;
		*b=255-(i-256); *r=(int)(lg-((float)(i-256)*jrat)); *g=0;
		return;
		}
		}
		if(j<768){ // magenta to red
		if(i<256){
		jrat=(1.0 - (float)(j%128)/128.0);
		*r=255; *b=255-(int)((float)i*(1.-jrat)); *g=255-i;
		return;
		}
		else {
		jrat=(1.-(float)(j%128)/128.0);
		lg=jrat*255.0;
		*b=(int)lg-(int)((float)(i-256)*jrat); *r=255-(i-256); *g=0;
		return;
		}
		}
		if(j<784){//grey scale
		*r=i/2; *g=i/2; *b=i/2;
		}
		}
		
		
		// based on oliii's solution on http://www.gamedev.net/community/forums/topic.asp?topic_id=444154
		struct point* getClosestPoint(struct point *A, struct point *B, struct point *P)
		{
		struct point *AP = vectorSub(P,A);
		struct point *AB = vectorSub(B,A);
		struct point *Closest=malloc(sizeof(struct point));
		float ab2 = AB->x*AB->x + AB->y*AB->y;
		float ap_ab = AP->x*AB->x + AP->y*AB->y;
		float t = ap_ab / ab2;
		if (t < 0.0f) t = 0.0f;
		else if (t > 1.0f) t = 1.0f;
		Closest->x = A->x + AB->x * t;
		Closest->y = A->y + AB->y * t;
		return Closest;
		}
		
		struct point* vectorSub(struct point *a, struct point *b)
		{
		struct point *diff=malloc(sizeof(struct point));
		diff->x=a->x - b->x;
		diff->y=a->y - b->y;
		return diff;
		}
		
		float distanceBetweenPoints(struct point *a, struct point *b)
		{
		float xD = a->x - b->x;
		float yD = a->y - b->y;
		return sqrt((xD*xD) + (yD*yD));
		}
		
		float vg_normalize_angle(float a)
		{
		float pi2 = 2.0*PI;
		while(a > pi2 || a < 0.0){
		if(a > pi2) a = a - pi2;
		if(a < 0.0) a = a + pi2;
		}
		return a;
		}
	
		

