//CBC user library functions
//--DPM 12/27/08

//Note that button functions and display function have not been updated yet (see end of file)
//Note that voltage and sonar scaling probably need adjustment

////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes
/* Includes the Standard IO Library */
#include <stdio.h>
#include <kiss-compat.h>
//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
//#include <kiss-input.h>

/////////////////////////////////////////////////////////////
// CBC Function prototypes
void tone(int frequency, int duration); /* makes a sound at frequency for duration ms */
void beep(); /* make a beep */
int digital(int port); /* returns a 1 or 0 reflecting the state of port (0 to 7) */
int set_digital_output_value(int port, int value); /*sets port (0 to 7)to value (0 or 1) */
int analog10(int port); /* returns 10-bit value from analog port (ports 8 to 15) */
int analog(int port); /* returns 8-bit value from analog port (ports 8 to 15) */
int accel_x(); /* returns x accelleration (-2047 to 2047, +/- 1.5 gee) */
int accel_y(); /* returns y accelleration (-2047 to 2047, +/- 1.5 gee) */
int accel_z(); /* returns z accelleration (-2047 to 2047, +/- 1.5 gee) */
int sonar(int port); /* returns range in mm for sonar plugged into port (13-15)*/
int sonar_inches(int port); /* returns range in whole inches for sonar plugged into port (13-15)*/
float power_level(); /* returns a float battery voltage */
void enable_servos(); /* powers up the servos */
void disable_servos(); /* powers down the servos */
int set_servo_position(int servo, int pos); /* sets servo (1 to 4) to pos (0 to 2047) */
int get_servo_position(int servo); /* returns int of last setting for servo (1 to 4) */
int get_motor_position_counter(int motor); /* returns int of motor (0 to 3) position +/-2147483647 */
int clear_motor_position_counter(int motor); /* sets motor (0 to 3) counter to 0 */
int move_at_velocity(int motor, int velocity); /* PID control of motor (0 to 3) at velocity tick per second */
int mav(int motor, int velocity); /* PID control of motor (0 to 3) at velocity tick per second */
int move_to_position(int motor, int speed, long goal_pos);/* move motor (0 to 3) at speed to goal_pos */
int mtp(int motor, int speed, long goal_pos);/* move motor (0 to 3) at speed to goal_pos */
int move_relative_position(int motor, int speed, long delta_pos);/* move motor (0 to 3) at speed by delta_pos */
int mrp(int motor, int speed, long delta_pos);/* move motor (0 to 3) at speed by delta_pos */
int freeze(int motor);/* keep motor (0 to 3) at current position */
int get_motor_done(int motor); /* returns 1 if motor (0 to 3) is moving to a goal and 0 otherwise */
void block_motor_done(int motor); /* returns when motor (0 to 3) has reached goal */
void bmd(int motor); /* returns when motor (0 to 3) has reached goal */
int setpwm(int motor, int pwm); /* turns on motor (0 to 3) at pwm (-255 to 255)*/
void fd(int motor); /* motor (0 to 3) at full forward */
void bk(int motor); /* motor (0 to 3) at full reverse */
void motor (int motor, int percent);/* motor (0 to 3) at percent % of full (-100 to 100)*/
void off(int motor); /* turns motor (0 to 3) off */
void ao(); /* turns all motors off */
int black_button(); /* returns 1 when black button is pressed*/
void cbc_display_clear(); /* Clears CBC console display (7 blanks lines in simulator)*/
void cbc_printf(int col, int row, char *t, ...); /*Starts printing at col, row then normal printf control string and args*/                                                                

#ifndef _CBCLIB
#define _CBCLIB 1
/////////////////////////////////////////////////////////////
// Simulation function noops for use on actual hardware
#ifndef _KISSSIMLOADED
void kissSim_init(int world, int rx, int ry, int rt){}
void kissSim_end(){}
void kissSimEnablePause(){}
void kissSimPause(){}
int kissSimActive(){return 1;}
#endif

/////////////////////////////////////////////////////////////
// Tone Functions
void tone(int frequency, int duration)
{
	if(frequency <5 || frequency > 6000)frequency=0;
	_bob.tone_freq=frequency;
	msleep(duration);
	_bob.tone_freq=0;
}

void beep()
{
	tone(440, 100);
}


/////////////////////////////////////////////////////////////
// Sensor Functions

// retruns 1 or 0 unless port is out of bounds, then -1
int digital(int port)
{
	if(port < 8 && port >= 0) {
		// first clear the output enable bit, if it was set
		_bob.enable_digital_outputs = (255 ^ (1 << port)) & _bob.enable_digital_outputs;
		return (1 & (_bob.digitals >> port));
		printf("Digital must be between 8 and 15\n");
		return -1;
	}
}


int set_digital_output_value(int port, int value)
{
	if(port < 8 && port >= 0) {
		// first set the output enable bit
		_bob.enable_digital_outputs = (1 << port) | _bob.enable_digital_outputs;
		// now set the output value
		_bob.digital_output_values = ((!(!value)) << port) | _bob.digital_output_values;
		return (0);
	}
	printf("Digital must be between 8 and 15\n");
	return -1;
}

int analog10(int port)
{
	if(port > 7 && port < 16) return (_bob.analogs[port-8]);
	printf("Analog sensors must be between 8 and 16\n");
	return -1;
}

// 8-bit analog for HB compatibility
int analog(int port)
{
	if(port > 7 && port < 16) return (_bob.analogs[port-8]/4);
	printf("Analog sensors must be between 8 and 16\n");
	return -1;
}

/////////////////////////////////////////////////////////////
// Accelerometer Functions

int accel_x() { return (_bob.acc_x-2048); }
int accel_y() { return (_bob.acc_y-2048); }
int accel_z() { return (_bob.acc_z-2048); }


//////////////////////////
// Sonar function: returns distance in mm

int sonar(int port)
{
	if(port > 12 && port < 16) {
		return (analog10(port)*13);
	}        
	printf("Sonar port must be between 13 and 15\n");
	return -1;
}

// returns distance in inches
int sonar_inches(int port)
{
	if(port > 12 && port < 16) {
		return (analog10(port)/2);
	}        
	printf("Sonar port must be between 13 and 15\n");
	return -1;
}

//////////////////////////
// Battery power functions

float power_level()
{
	float p = _bob.volts/4095.0;
	float scale = 10.0; //$$$$$$$$ This needs to be corrected $$$$$$$
	return (p*scale);
}



/////////////////////////////////////////////////////////////
// Servo Functions
void enable_servos()
{
	_bob.enable_servos=1;
}

void disable_servos()
{
	_bob.enable_servos=0;
}

int set_servo_position(int servo, int pos)
{
	if(servo < 1 || servo > 4)
	{
		printf("Servo index must be between 1 and 4\n");
		return -1;
	}
	if (pos < 0 || pos > 2047)
	{
		printf("Servo position must be between 0 and 2047\n");
		return -1;
	}
	_bob.servo_targets[servo-1]=pos;
	return 0;
}

int get_servo_position(int servo)
{
	if(servo < 1 || servo > 4)
	{
		printf("Servo index must be between 1 and 4\n");
		return -1;
	}
	return (_bob.servo_targets[servo-1]);
}

/////////////////////////////////////////////////////////////
// Motor Functions

//////////////////////////
// CBC-specific motor functions


#define MAX_VEL   1000
#define MAX_POS_POS 2147483647
#define MAX_NEG_POS -2147483647


int get_motor_position_counter(int motor)
{
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
		return 0;
	}
	return (_bob.motor_counter[motor]);// for actual CBC, divide by 160
}


int clear_motor_position_counter(int motor)
{
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
		return -1;
	}
	_bob.clear_motor_counters= _bob.clear_motor_counters | (1 << motor); // set the bit to clear that counter
	_bob.motor_counter_targets[motor]=_bob.motor_tps[motor]=0;//if the motor is moving to a position, zero that position & stop motor
	while ((_bob.motor_counter[motor] > 10 || _bob.motor_counter[motor] < -10) && kissSimActive())msleep(10);// wait until counter has been zeroed
	_bob.clear_motor_counters=_bob.clear_motor_counters ^ (1 << motor); // clear the bit so the motor can be counted
	
	return 0;
}


int move_at_velocity(int motor, int velocity)
{
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
		return -1;
	}
	_bob.motor_pwm[motor]=0;
	if(velocity < 0){_bob.motor_counter_targets[motor]=MAX_NEG_POS; velocity=-velocity;}
	else _bob.motor_counter_targets[motor]=MAX_POS_POS;
	_bob.motor_tps[motor]=velocity;// for actual CBC, multiply by 160
	return 0;
}

int mav(int motor, int velocity)
{ return move_at_velocity(motor, velocity); }

// Move motor to goal_pos at given velocity.  The amount actually
// moved will be goal_pos - get_motor_position().
int move_to_position(int motor, int speed, long goal_pos)
{ 
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
		return -1;
	}
	_bob.motor_pwm[motor]=0;
	_bob.motor_counter_targets[motor]=goal_pos;// for actual CBC, multiply by 160
	//if(goal_pos < _bob.motor_counter[motor]) velocity=-velocity;
	_bob.motor_tps[motor]=speed;// for actual CBC, multiply by 160
	_bob.motor_in_motion=_bob.motor_in_motion | (1 << motor);//solves bmd sync issue where this is not set by BoB b4 bmd command is executed
	return 0;
}

int mtp(int motor, int velocity, long goal_pos)
{ move_to_position(motor, velocity, goal_pos); }


// Move delta_pos relative to the current position of the motor.  The
// final motor tick will be get_motor_position() at the time of the
// call + delta_pos.
int move_relative_position(int motor, int speed, long delta_pos)
{ 
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
		return -1;
	}
	_bob.motor_pwm[motor]=0;
	_bob.motor_counter_targets[motor]=_bob.motor_counter[motor]+delta_pos;// for actual CBC, multiply delta_pos by 160
	//if(delta_pos < 0)velocity=-velocity;
	_bob.motor_tps[motor]=speed;// for actual CBC, multiply by 160
	_bob.motor_in_motion=_bob.motor_in_motion | (1 << motor);//solves bmd sync issue where this is not set by BoB b4 bmd command is executed
	return 0;
}

int mrp(int motor, int velocity, long delta_pos)
{
	return(move_relative_position(motor, velocity, delta_pos));
}

//Turns off or actively holds the motor in position depending  on the situation -- but it may drift
int freeze(int motor)
{
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
		return -1;
	}
	_bob.motor_pwm[motor]=0;
	_bob.motor_counter_targets[motor]=_bob.motor_counter[motor];
	_bob.motor_tps[motor]=MAX_VEL/2;// for actual CBC, multiply by 160
	return 0;
}

void set_pid_gains(int motor, int p, int i, int d)
{
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
	}
	_bob.pid_gains[motor][0]=p;
	_bob.pid_gains[motor][1]=i;
	_bob.pid_gains[motor][2]=d;
}


//returns 0 if motor is in motion and 1 if it has reached its target position
int get_motor_done(int motor)
{
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
		return -1;
	}
	return(!(1 & (_bob.motor_in_motion >> motor)));
}

void bmd(int motor)
{
	//loop doing nothing while motor position move is in progress
	while(!get_motor_done(motor) && kissSimActive()){msleep(10);}
}

void block_motor_done(int motor)
{
	//loop doing nothing while motor position move is in progress
	while(!get_motor_done(motor) && kissSimActive()){msleep(10);}
}

int setpwm(int motor, int pwm)
{
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
		return -1;
	}
	_bob.motor_tps[motor]=0;
	_bob.motor_pwm[motor]=pwm;
	return 0;
}

//////////////////////////
// Classic pwm motor functions, scaled for XBC and Blk gear motors
/***************************************************************** */
/*                                                                */
/* MOTOR PRIMITIVES                                               */
/*                                                                */
/*   fd(n) sets motor n to full on in the green direction    */
/*   bk(n) sets motor n to full on in the red direction      */
/*   motor(n, s) sets motor n on at speed s;               */
/*     s= 100 is full on green,                  */
/*     s= -100 is full on red,                   */
/*     s= 0 is off                               */
/*   off(n)  turns off motor n      */
/*                                                                */
/*   alloff() turns off all motors                      */
/*   ao()  turns off all motors                      */
/*                                                                */
/*                                                                */
/*   motors are numbered 0 through 3.                          */
/***************************************************************** */

void fd(int motor)
{
	//    move_at_velocity(motor, MAX_VEL);
	setpwm(motor,255);
}

void bk(int motor)
{ 
	setpwm(motor,-255);
	//    move_at_velocity(motor, -MAX_VEL);
}

void motor(int port, int speed)
{
	setpwm(port,(255*speed)/100);
}



void off(int motor)
{
	setpwm(motor, 0);
}

void alloff()
{
	setpwm(0, 0);
	setpwm(1, 0);
	setpwm(2, 0);
	setpwm(3, 0);
}

void ao()
{
	setpwm(0, 0);
	setpwm(1, 0);
	setpwm(2, 0);
	setpwm(3, 0);
}




/////////////////////////////////////////////////////////////
// Button Functions
int black_button(){return _bob.button;}

/////////////////////////////////////////////////////////////
// Display Functions
// Copyright(c) KIPR, 2009
// Full screen management functions for the display window provided with the CBC console screen
//   1. cbc_display_clear()
//   2. cbc_display_printf(int column, int row, <printf arguments>)
//
// Console screen display window size is 7 rows and limited to 28 columns, indexed from 0
// (variable pitch font used by CBC necessitates using a shortened width and prevents using
//  the slicker strategy of treating the display as one very long string)
//
// Initial version: 1/27/2009 - cnw
//
// stdarg.h provides macros for accessing a function's argument list ... see K&R
#include <stdarg.h>
#include <string.h>

#ifndef BB09WORLD       // temporary workaround
   #define BB09WORLD 0  
#endif                  //

#define _MAPy 7
#define _MAPx 29  // last column is for \0
char _display_map[_MAPy][_MAPx]; 

void cbc_display_clear() {
  int x,y;
  printf("\n\n\n\n\n\n\n");  // blow it away and reset map
  for(y=0;y<_MAPy;y++)       
    for(x=0;x<_MAPx; x++) _display_map[y][x]=' ';   // blank out the map and
  for(y=0;y<_MAPy;y++) _display_map[y][_MAPx-1]='\0'; // make each row a string
  msleep(100);   // needs a brief pause to empty display
}

void cbc_printf(int col, int row, char *t, ...) { // column, row, string with format phrases, args
  va_list argp;    // argp is typed for traversing the variable part of the arg list
  int i; char *c; double d;  // working variables to receive arg values
  char *cp, *fmte, sc;  // cp traverses format string t, fmte marks end of each format phrase, sc is switch control
  
  int y;           // row index
  char *dp;        // pointer into display
  int maxw;        // available room on line
  char fws[_MAPx]; // formatted phrase work area
  char fmt[_MAPx]; int fl; // fmt is a working string for each format extracted
  
  va_start (argp,t);  // t is last named argument in cbc_printf's function header;
  // this initializes argp to point to first variable arg
  
  dp = &_display_map[row][col]; // starting point for printf
  maxw=_MAPx-col;               // space remaining on line
  for (cp = t; *cp; cp++)       // process printf string; stop when *cp = '\0'
    {
      if (*cp != '%')           // if not a format phrase 
	{
	  if(strspn(cp,"\n\t\v\b\r\f")>0) {  // is it a spec char? if so treat as if \n
	    for (i=0;i<maxw;i++) { // clear balance of line
	      *dp=' '; dp++;
	    }
	    if (row==_MAPy) break; // out of rows so proceed to display refresh
	    row++;
	    dp = &_display_map[row][0]; maxw=_MAPx; // set up for new line
	  }
	  else { // nothing special about this one so insert it          
	    *dp=*cp; dp++;
	    maxw--; if (maxw==0) break; // no more room on line so proceed to display refresh
	  }
	  continue;             // return to top
	}
      // OK, if we're here we may have hit a format phrase
      fmte = strpbrk(cp+1, "dioxXucsfeEgG%"); // look for format end        
      // strpbrk returns the location of 1st character of its argument that is in the scan string
      if (fmte == NULL)        // what's left is not a format phrase so insert % and return to top       
	{                        
	  *dp='%'; dp++; 
	  maxw--; if (maxw==0) break; // no more room on line so proceed to display refresh
	  continue;            // return to top
	}
      // OK, there looks to be a format phrase
      sc = *fmte;              // set switch control for the case 
      fl = 1+fmte-cp;          // pick off phrase (pointed to by cp)
      strncpy(fmt,cp,fl);      // capture the format phrase
      fmt[fl] = '\0';          // make it a string
      switch (sc)              // process the kind of format specified
	{
	case 'd': case 'i': case 'o': case 'x': case 'X': case 'u': case 'c':
	  i = va_arg(argp, int);    // return next parm as type int and step argp
	  sprintf(fws,fmt,i);       // use sprintf to do the formatting to fws
	  break;
	case 's':
	  c = va_arg(argp, char *); // return next parm as type char * and step argp
	  sprintf(fws,fmt,c);       // use sprintf to do the formatting to fws
	  break;
	case 'f': case 'e': case 'E': case 'g': case 'G':
	  d = va_arg(argp, double); // return next parm as type double and step argp
	  sprintf(fws,fmt,d);       // use sprintf to do the formatting to fws
	  break;
	case '%':                 // no format specified between %'s
	  sprintf(fws,fmt);
	  break;
	}
      for(i=0; i<strlen(fws); i++) {// insert formatted phrase in display map
	if (maxw==0) break;       // if no more room get out of this
	*dp = fws[i];             // insert next character from formatted phrase
	dp++; maxw--;
      }
      if (maxw==0) break;           // if no more room proceed to display refresh
      cp = fmte;                    // set cp for next pass
    }
  va_end(argp);                     // clean up
  for(y=0;y<_MAPy; y++) _display_map[y][_MAPx-1]='\0';  // make each row a string
  for(y=0;y<_MAPy; y++) printf("\n%s",_display_map[y]); // refresh the display
}





#endif
