/* Includes the Standard IO Library  */
#include <stdio.h>
#include <kiss-compat.h>
//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
//#include <kiss-input.h>

#ifndef __simCBC_
#define __simCBC_

#define __CSTATMINH 500
#define __CSTATW 200
#define FULLMOTORTICS 15

struct __bob{
	// sensors
	// From Bob to Chumby
	int analogs[8]; // 10 bits resolution
	int acc_x; // 12 bits resolution
	int acc_y;
	int acc_z;
	int digitals; //should be char? bits 0-7
	int button; //should be char? bit 0
	int motor_counter[4]; //32-bit signed integrated motor position counters
	int motor_in_motion; // bits 0-3 are 1 if motor is executing BEMF command
	int volts;//the 12 bit voltage reading
	//effectors
	// From Chumby to BoB
	int motor_pwm[4]; // pwm values for setting motor
	int motor_tps[4]; // tics per second values for motor speeds (for BEMF commands)
	int motor_counter_targets[4]; // absolute position targets (set to +/- 2147483647 if position independent BEMF command is used)
	int clear_motor_counters; // low 4 bits set to 1 by user to reset encoders. Library routine clears the bit after it receives a 0 encoder value
	int servo_targets[4]; // Servos move left 0 to right 2047
	int enable_servos; // 1 for power on to servos, 0 is off, only 1 bit for all 4 servos
	int enable_digital_outputs;
	int digital_output_values;
	int pid_gains[4][3];//to change the motor PID functions for each channel
	float t_update;//last time updated
	
	//int for Chumby only
	int tone_freq; // pitch in Hz for the speaker; value of 0 is speaker off
	int analog_floats;//bit mask of state of analog ports 0=pullup 1 = float
	unsigned char a;
	unsigned char b;
	unsigned char up;
	unsigned char down;
	unsigned char left;
	unsigned char right;
	
	//int for simulator only
	float fmcounters[4];
	float ptime;//previous wall time
} _bob;
#include "cbclib.c"

struct __CBC{
	struct __bob *bob;
} computer;

void kissSimComputerDim(int *w, int *h);
void kissSimComputerUpdateDigitals();
void convertNumToString(int num,int numDigits, char arr[],int numCols, int idx, int offset);
void kissSimComputerUpdateAnalogs(int x0, int y0, int x1, int y1, int yOffset);
void kissSimComputerUpdateMotors();
void kissSimComputerTone(int *r, int *g, int *b);


void kissSimComputerDim(int *w, int *h)
{
	*w = __CSTATW;
	*h = (__CSTATMINH > ksWindow.height) ? __CSTATMINH : ksWindow.height;
	if(*h > ksWindow.height){
		ksWindow.height = *h;
		ksWindow.simHeight = *h - ksWindow.rHeight;
	}
}


void kissSimComputerDraw()
{
	int br,bg,bb;
	int oriX=ksWindow.simWidth, oriY=0, i;
	static char digitals[]="0  0  0  0  0  0  0  0";
	static char buttons[]="0 0 0 0 0 0 0";
	static char analogs[11][10]={"         ","         ","         ","         ","         ","         ","         ","         ","         ","         ","         "};
	static char actuators[4][25]={"                        ","                        ","                        ","                        "};
	static char servos[4][25]={"                        ","                        ","                        ","                        "};
	if(!glfwGetWindowParam(GLFW_OPENED)) return;
	graphics_rectangle_fill(oriX,oriY,oriX+1,ksWindow.height-ksWindow.tHeight,BLACK); // draw left border
	draw_bg(oriX+5,oriY+9,0.36); // Botguy
	kissSimComputerTone(&br,&bg,&bb);
	graphics_circle_fill(ksWindow.width-10,oriY+9,40,br,bg,bb);//draw beep color
	NOBOLD g_printString(" CBC SIMULATOR",oriX+25, oriY+10,TEAL,1.0);
	NOBOLD g_printString("    DIGITALS  ",oriX+25, oriY+40,TEAL,1.0);
	NOBOLD g_printString("8  9  10 11 12 13 14 15",oriX+25, oriY+55,TEAL,1.0);
	NOBOLD g_printString("    BUTTONS  ",oriX+25, oriY+90,TEAL,1.0);
	//NOBOLD g_printString("< ^ V > A B .",oriX+30, oriY+105,TEAL,1.0);
	NOBOLD g_printString("\xb \xd \xe \xc A B .",oriX+30, oriY+105,TEAL,1.0);
	// hex b, c, d, e are left, right, up, and down arrows
	NOBOLD g_printString(digitals,oriX+25, oriY+70,WHITE,1.0); // erase old digital values
	NOBOLD g_printString("    ANALOGS  ",oriX+25, oriY+140,TEAL,1.0);
	for(i=0;i<8;i++){
		digitals[3*i]=48+digital(i+8);
	}
	NOBOLD g_printString(digitals,oriX+25, oriY+70,BLACK,1.0); // write new digital values
	NOBOLD g_printString(buttons,oriX+30, oriY+120,WHITE,1.0); // erase old button values
	buttons[0]=48+!!left_button();
	buttons[2]=48+!!up_button();
	buttons[4]=48+!!down_button();
	buttons[6]=48+!!right_button();
	buttons[8]=48+!!a_button();
	buttons[10]=48+!!b_button();
	_bob.button=!!kiss_get_key('.');
	buttons[12]=48+black_button();
	NOBOLD g_printString(buttons,oriX+30, oriY+120,BLACK,1.0); // erase old button values
	for(i=0;i<8;i++){//print out analogs
		analogs[i][1]=(i<10)?48+i:48+i-10;
		analogs[i][0]=(i<10)?32:49;
		analogs[i][2]=':';
		analogs[i][3]=' ';
		NOBOLD g_printString(analogs[i],oriX+5, oriY+155+(15*(i)),WHITE,1.0);
		convertNumToString(analog10(i),4,*analogs,10,i,3);
		graphics_rectangle_fill(oriX+30,oriY+155+(15*(i)),oriX+157, oriY+165+(15*(i)),GRAY);
		graphics_line(oriX+30+analog(i)/2, oriY+155+(15*(i)),oriX+30+analog(i)/2,oriY+165+(15*(i)),RED);
		NOBOLD g_printString(analogs[i],oriX+5, oriY+155+(15*(i)),BLACK,1.0);
		if(1 & (_bob.analog_floats >> i)) {NOBOLD g_printString("F",oriX+162, oriY+155+(15*(i)),BLACK,1.0);}
		else {NOBOLD g_printString("F",oriX+162, oriY+155+(15*(i)),WHITE,1.0);}
	}
	analogs[8][1]='X';
	analogs[8][0]='A';
	analogs[8][2]=':';
	analogs[8][3]=' ';
	NOBOLD g_printString(analogs[8],oriX+5, oriY+155+(15*(8)),WHITE,1.0);
	convertNumToString(accel_x(),5,*analogs,10,8,3);
	graphics_rectangle_fill(oriX+30,oriY+155+(15*(8)),oriX+157, oriY+165+(15*(8)),GRAY);
	graphics_line(oriX+30+(accel_x()+127)/2, oriY+155+(15*(8)),oriX+30+(accel_x()+127)/2,oriY+165+(15*(8)),RED);
	NOBOLD g_printString(analogs[8],oriX+5, oriY+155+(15*(8)),BLACK,1.0);
	analogs[9][1]='Y';
	analogs[9][0]='A';
	analogs[9][2]=':';
	analogs[9][3]=' ';
	NOBOLD g_printString(analogs[9],oriX+5, oriY+155+(15*(9)),WHITE,1.0);
	convertNumToString(accel_y(),5,*analogs,10,9,3);
	graphics_rectangle_fill(oriX+30,oriY+155+(15*(9)),oriX+157, oriY+165+(15*(9)),GRAY);
	graphics_line(oriX+30+(accel_y()+127)/2, oriY+155+(15*(9)),oriX+30+(accel_y()+127)/2,oriY+165+(15*(9)),RED);
	NOBOLD g_printString(analogs[9],oriX+5, oriY+155+(15*(9)),BLACK,1.0);
	analogs[10][1]='Z';
	analogs[10][0]='A';
	analogs[10][2]=':';
	analogs[10][3]=' ';
	NOBOLD g_printString(analogs[10],oriX+5, oriY+155+(15*(10)),WHITE,1.0);
	convertNumToString(accel_z(),5,*analogs,10,10,3);
	graphics_rectangle_fill(oriX+30,oriY+155+(15*(10)),oriX+157, oriY+165+(15*(10)),GRAY);
	graphics_line(oriX+30+(accel_z()+127)/2, oriY+155+(15*(10)),oriX+30+(accel_z()+127)/2,oriY+165+(15*(10)),RED);
	NOBOLD g_printString(analogs[10],oriX+5, oriY+155+(15*(10)),BLACK,1.0);
	
	NOBOLD g_printString("MOT PWM TPS  ENCODER VAL",oriX+5, oriY+330,TEAL,1.0);
	for(i=0;i<4;i++){
		actuators[i][0]=48+i;
		actuators[i][1]=':';
		actuators[i][2]=' ';
		NOBOLD g_printString(actuators[i],oriX+5, oriY+345+(15*(i)),WHITE,1.0);
		convertNumToString(_bob.motor_pwm[i],4,*actuators,25,i,2);
		convertNumToString(_bob.motor_tps[i],4,*actuators,25,i,7);
		convertNumToString(_bob.motor_counter[i],10,*actuators,25,i,12);
		NOBOLD g_printString(actuators[i],oriX+5, oriY+345+(15*(i)),BLACK,1.0);
	}
	if(_bob.enable_servos){
		NOBOLD g_printString("SERVO TARGET DISABLED",oriX+5, oriY+410,WHITE,1.0);
		NOBOLD g_printString("SERVO TARGET ENABLED",oriX+5, oriY+410,TEAL,1.0);
	}
	else {
		NOBOLD g_printString("SERVO TARGET ENABLED",oriX+5, oriY+410,WHITE,1.0);
		NOBOLD g_printString("SERVO TARGET DISABLED",oriX+5, oriY+410,TEAL,1.0);
	}
	for(i=0;i<4;i++){
		servos[i][2]=48+i;//servos on CBC2 are 0 indexed!!
		servos[i][3]=':';
		servos[i][4]=' ';
		NOBOLD g_printString(servos[i],oriX+5, oriY+425+(15*(i)),WHITE,1.0);
		convertNumToString(_bob.servo_targets[i],4,*servos,25,i,6);
		NOBOLD g_printString(servos[i],oriX+5, oriY+425+(15*(i)),BLACK,1.0);
	}
	
}

void convertNumToString(int num,int numDigits, char arr[],int numCols, int idx, int offset)
{
	int i,dig,neg=0;
	if(num<0){neg=1; num=-num;}
	for(i=0;i<numDigits;i++){
		dig=num%10;//get least significant digit
		num=num/10;
		arr[(numCols*idx)+offset+numDigits-i]=dig+48;//insert ascii digit
	}
	if(neg)arr[(numCols*idx)+offset+1]='-';
}

void kissSimComputerUpdateMotors()
{
	int m;
	float dtime, speed,err;
	dtime=seconds()- _bob.ptime;
	if(!glfwGetWindowParam(GLFW_OPENED)) return;
	if(_world.simPaused)dtime=0.0;
	for(m=0;m<4;m++){
		speed=0.0;
		if(_bob.motor_tps[m]){
			err= (float)_bob.motor_counter_targets[m] - _bob.fmcounters[m];
			if(err < 8.0 && err > -8.0){speed=0.0;}
			else{
				if(err < 0.5*(float)_bob.motor_tps[m] && err > -0.5*(float)_bob.motor_tps[m]){
					speed=(float)_bob.motor_tps[m]*err/(0.5*(float)_bob.motor_tps[m]);
				}
				else {
					if(err<0.0) {speed =0.0 - (float)_bob.motor_tps[m]; }
					else {speed=(float)_bob.motor_tps[m]; }
				}
			}
			if(speed > 0.0 && _bob.motor_counter[m] > _bob.motor_counter_targets[m]){
				speed=0.0;
				_bob.motor_in_motion=_bob.motor_in_motion & ~(1 << m);
			}
			else{
				if(speed < 0.0 && _bob.motor_counter[m] < _bob.motor_counter_targets[m]){
					speed=0.0;
					_bob.motor_in_motion=_bob.motor_in_motion & ~(1 << m);
				}
				else {
					if(speed > -.5 && speed < .5) _bob.motor_in_motion=_bob.motor_in_motion & ~(1 << m);
					else _bob.motor_in_motion=_bob.motor_in_motion | (1 << m);
				}
			}
		}
		else if(_bob.motor_pwm[m])speed=FULLMOTORTICS*_bob.motor_pwm[m];
		_bob.fmcounters[m]=_bob.fmcounters[m]+dtime*speed;
		//printf("Motor %d target %d Pos %d vel %f\n",m,_bob.motor_counter_targets[m],_bob.motor_counter[m],speed);
		if((1 << m) & _bob.clear_motor_counters){// should this motor's encoder be zeroed
			_bob.fmcounters[m]=0.0;
		}
		_bob.motor_counter[m]=(int)_bob.fmcounters[m];
	}
	_bob.ptime=seconds();
}



void kissSimComputerUpdate()
{
	if(!glfwGetWindowParam(GLFW_OPENED)) return;
	kissSimComputerUpdateDigitals();
	kissSimComputerUpdateAnalogs(ksWindow.simWidth+30, 155, ksWindow.simWidth+157 ,  165,  15);
	kissSimComputerUpdateMotors();
	kissSimComputerDraw();
}

void kissSimComputerInit()
{
	int i;
	computer.bob = &_bob;
	_bob.digitals=0;
	_bob.enable_digital_outputs=0;
	_bob.digital_output_values=0;
	_bob.enable_servos=0;
	_bob.motor_in_motion=0;
	_bob.button=0;
	_bob.acc_x=127;
	_bob.acc_y=127;
	_bob.acc_z=191;
	for(i=0;i<8;i++)_bob.analogs[i]=0;
	for(i=0;i<4;i++){
		_bob.motor_counter[i]=0;
		_bob.motor_tps[i]=0;
		_bob.motor_pwm[i]=0;
		_bob.fmcounters[i]=0.0;
	}
	_bob.ptime=seconds();
	kissSimComputerUpdateMotors();
	kissSimComputerUpdate();
}

void kissSimComputerUpdateDigitals()
{
	int i;
	static unsigned char d[8]={0,0,0,0,0,0,0,0};
	static unsigned char p[8]={0,0,0,0,0,0,0,0};
	
	
	for(i=0;i<8;i++) {
		d[i]=!(!kiss_get_key(((8+i)%10)+48));//if a number key is pressed set that d to 1 otherwise set to 0
		//if(d[i])printf("Key %d has been pressed pd=%d\n",i,p[i]);
		if(p[i] && !d[i]) _bob.digitals = (_bob.digitals ^ (1 << i)); // toggle the ith digital if key has been released
		p[i]=d[i]; // set previous to current;
	}
}

void kissSimComputerUpdateAnalogs(int x0, int y0, int x1, int y1, int yOffset)
{
	int i,x,y;
	for (i=0;i<11;i++){
		kiss_get_mouse_position(&x,&y);
		if(kiss_get_mouse_button(0) && x>=x0 && x <=x1 && (y>= (y0 + i*yOffset)) && (y<= (y1 + i*yOffset))){
			if(i<8)_bob.analogs[i]=(x-x0)*8;
			else{
				if(i==8)_bob.acc_x=(x-x0)*2;
				if(i==9)_bob.acc_y=(x-x0)*2;
				if(i==10)_bob.acc_z=(x-x0)*2;
			}
		}
	}
}

int kissSimComputerInstructions(int i)
{
	NOBOLD g_printString("                SIMULATING THE CBC               ",5, 5+i*15,BLACK,1.0);
	i++;
	NOBOLD g_printString("USE THE 8-5 KEYS TO TOGGLE DIGITALS ON AND OFF...",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("...8 FOR 8, 9 FOR 9, 0 for 10, 1 for 11,... & 5 for 15.",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("PRESS THE A,B, OR ARROW KEYS TO SIMULATE",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("PRESSING THE CORRESPONDING TOUCH SCREEN KEYS.",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("USE THE PERIOD KEY FOR THE CBC BLACK BUTTON.",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("USE MOUSE (IN CORRESPONDING GRAY AREA) TO",5, 5+i*15,TEAL,1.0);
	i++;
	NOBOLD g_printString("ADJUST THE ANALOGS TO SIMULATED VALUES.",5, 5+i*15,TEAL,1.0);
	return i;
}

void kissSimComputerTone(int *r, int *g, int *b)
{
	int pitch;
	int hue;
	static float base=1.0595;
	if(_bob.tone_freq==0){*r=*g=*b=255; hue=0; return;}
	pitch=_bob.tone_freq;
	hue=log((float)pitch)/log(base);
	hue=hue-25;//0 value for 5hz, 127 for 6kHz
	if(hue < 26){*r=255; *g=hue*10; *b=0;}
	else {
		if(hue < 52){*r=255-((hue-26)*10); *g=255; *b=0;}
		else {
			if(hue < 78){*r=0; *g=255; *b=(hue-52)*10;}
			else {
				if(hue < 104){*r=0; *g=255-10*(hue-78); *b=255;}
				else{
					if(hue < 130){*r=10*(hue-104); *g=0; *b=255;}
				}
			}
		}
	}
}

#endif


