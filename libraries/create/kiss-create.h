/**************************************************************************
 *  Copyright 2007,2008,2009 KISS Institute for Practical Robotics        *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

/* BEGIN CreateLib.h */

/* Library revised 6/27/08 to improve beep (still problematic) and rarrange/correct comments -DPM */

/* Library revised 1/31/08 to fix 2's complement error in calculating gc_distance & gc_angle -DPM */

/* Library revised to  be compatible with IC8 and run under GCC 1/08 -DPM */

/* This file contais a number of useful functions for communicating with an iRobot Create.
This file is similar, but not identical to the CreateLib for IC.  The functions serial_write_byte and
serial_read_byte are assumed to be defined elsewhere.
Written by David Miller, 4/07 
*/


#ifndef __CREATE_LIB_H__
#define __CREATE_LIB_H__

#include <kiss-serial.h>
#include <stdio.h>
#include <unistd.h>
#include <kiss-compat.h>

//rings the system alert tone (whatever it may be) but also inserts a <CR>
void beep()
{
	printf("%c\n",7);
}

#define twopi 6.28318531
//CREATE_BUSY keeps any  create commands in other processes from interfering with one another.
#define CREATE_BUSY while(g_create_busy)msleep(10L); g_create_busy = 1;
#define CREATE_FREE g_create_busy = 0;

//if more than 0.5 seconds elapse without the serial buffer filling, connection is lost
//attempt is made to reconnect and error code err is returned 
#define CREATE_WAITFORBUFFER(bc,err) {int _to=0; while(serial_buffer_count()<(bc)){if((_to)>50){g_create_busy=0;create_reconnect();return(err);}else{msleep(10L);_to++;}}}
// time out error codes
// -1:create_mode, -2:create_wall, -3:create_buttons, -4:create_bumpdrop, -5:create_cliffs,
// -6:create_angle, -7:create_distance, -8:create_velocity, -9:create_read_IR, -10:create_overcurrents, -11:create_battery_charge, -12:create_cargo_bay_inputs
int g_create_busy = 0;
int g_create_connected = 0;
int g_create_USB=0;

//globals are updated by the functions below to give access to many key Create sensors
int gc_lbump, gc_rbump, gc_ldrop, gc_rdrop, gc_fdrop, gc_rcliff, gc_rfcliff, gc_lcliff, gc_lfcliff;
int gc_rcliff_amt, gc_rfcliff_amt, gc_lcliff_amt, gc_lfcliff_amt;
int gc_distance, gc_angle, gc_total_angle, gc_advance_button, gc_play_button;
int gc_wall, gc_wall_amt, gc_wall_hb, gc_IR;
int gc_vel, gc_radius, gc_rvel, gc_lvel;
int gc_overcurrents;
int gc_charge_state, gc_batt_voltage, gc_current_flow, gc_batt_temp, gc_batt_charge, gc_batt_capacity;
int gc_digital_in, gc_analog_in, gc_charge_source;
int gc_leds[3]={0,0,0};
int gc_song_array[16][33]; 
int gc_mode=0;

//Function prototypes
int create_connect();
int create_connect_s();
void create_disconnect() ;
void create_disconnect_s();
void create_start();
void create_passive();
void create_safe();
void create_full() ;
void create_spot() ;
void create_cover();
void create_demo(int d) ;
void create_cover_dock();
int create_mode();
int create_sensor_update();
int create_wall();
int create_buttons() ;
int create_bumpdrop();
int create_cliffs() ;
int create_angle() ;
int create_distance();
int create_velocity();
int create_read_IR();
int create_overcurrents();
int create_battery_charge();
int create_cargo_bay_inputs();
void create_stop();
void create_drive (int speed, int radius);
void create_drive_straight (int speed);
void create_spin_CW (int speed);
void create_spin_CCW (int speed);
void create_drive_direct(int r_speed, int l_speed);
int create_spin_block(int speed, int angle);
int _create_get_raw_encoders(long *lenc, long *renc);
void create_advance_led(int on) ;
void create_play_led(int on) ;
void create_power_led(int color, int brightness) ;
void create_digital_output(int bits);
void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0);
void create_low_side_drivers(int pwm2, int pwm1, int pwm0);
void create_load_song(int num);
void create_play_song(int num);
unsigned char get_high_byte(int v);
unsigned char get_low_byte(int v);
void clear_serial_buffer();
void s_write_byte(unsigned char byte);
unsigned char s_read_byte() ;

// Create must be turned on and connected.  This routine establishes serial link
// Note that this routine should be called once, and must have been called before 
// any of the other create functions will work.
// Once this function is called, you cannot talk to the XBC over the USB connection
// until the create_disconnect function ahs been called or the GBA on the XBC has been powercycled.
// If Create is connected, power light goes yellow and play led turns on
int create_connect()
{
	g_create_USB=1;
	gc_mode=0;    
	serial_set_mode(BAUD_57K);
	serial_set_extra(1);
	sleep(0.1);
	create_start();
	create_safe(); //allows create to be commanded and turns LED orange
	clear_serial_buffer();
	create_mode();//gets mode and sets global gc_mode
	if (gc_mode!=2) {       // if not safe mode, connection not established
		printf("NO CREATE CONNECTION mode is %d\n", gc_mode);
		g_create_connected = 0;
		msleep(100); beep(); msleep(100); beep();
		return(-1);//connection failed
	}
	create_advance_led(1); // turn on advance LED
	g_create_connected = 1;
	return(0);//successful connection
}


// Same as above but for code compatibility with serial XBCs (versions 1 & 2)
int create_connect_s() {
	g_create_USB=0;
	gc_mode=0;    
	serial_set_mode(BAUD_57K);
	sleep(0.1);
	create_start();
	create_safe();
	clear_serial_buffer();
	create_mode();
	if (gc_mode!=2) {       // if not safe mode, connection not established
		printf("NO CREATE CONNECTION (mode %d)\n", gc_mode);
		g_create_connected = 0;
		msleep(100); beep(); msleep(100); beep();
		return(-1);//connection failed
	}
	create_advance_led(1); // turn on advance LED
	g_create_connected = 1;
	return(0);//successful connection
}

// returns the serial connections on XBC to normal communications over the USB port.
// Turns of play LED and returns power light to green
void create_disconnect() {
	create_play_led(0);
	create_power_led(0,255);
	create_stop();
	g_create_connected=0; serial_set_mode(0); 
	serial_set_extra(0);
}

// Same as above but for serial XBCs
void create_disconnect_s() {
	create_play_led(0);
	create_power_led(0,255);
	create_stop();
	g_create_connected=0; serial_set_mode(0); 
}

// if serial communication times out (ie., is lost), attempts to restart it
void create_reconnect()
{
	printf("Create communication lost;\nattempting reconnection.\n");
	if(g_create_USB==1){
		create_disconnect();    // reset environment
		create_connect();
	}
	else{
		create_disconnect_s();  // reset environment
		create_connect_s();
	}
}

// Create open interface mode is off=0 (Create powered), passive=1, safe=2, full=3
// From "safe" mode, wheel drop or cliff detect halts motors and puts Create in passive mode
int create_mode() {
	CREATE_BUSY;
	s_write_byte(142);
	s_write_byte(35);
	CREATE_WAITFORBUFFER(1,-1)
	gc_mode=s_read_byte();
	CREATE_FREE;
	return(0);
}



// See the create open interface document for more information on these functions
void create_start() {CREATE_BUSY; s_write_byte(128); CREATE_FREE;}
void create_passive() {CREATE_BUSY; s_write_byte(128); CREATE_FREE;}//same as start 
void create_safe() {CREATE_BUSY; s_write_byte(131); CREATE_FREE;create_power_led(64,255);}
void create_full() {CREATE_BUSY; s_write_byte(132); CREATE_FREE; create_power_led(227,255);}

void create_spot() {CREATE_BUSY; s_write_byte(134); CREATE_FREE;}

void create_cover() {CREATE_BUSY; s_write_byte(135); CREATE_FREE;}

void create_demo(int d) {CREATE_BUSY; s_write_byte(136); s_write_byte(d); CREATE_FREE;}

void create_cover_dock() {CREATE_BUSY; s_write_byte(143); CREATE_FREE;}


//////////////Create Sensor Routines///////////////
// These work for any Create mode except off


//this function will update the values for many of the sensor globals above
//serial connection failure codes are
// -1:create_mode, -2:create_wall, -3:create_buttons, -4:create_bumpdrop, -5:create_cliffs,
// -6:create_angle, -7:create_distance, -8:create_velocity, -9:create_read_IR 
int create_sensor_update()
{
	int r=0; // if serial connection times out, bit positions give the functions where it occurred
	r=r-2*create_wall();
	r=r-4*create_buttons();
	r=r-8*create_bumpdrop();
	r=r-16*create_cliffs();
	r=r-32*create_angle();
	r=r-64*create_distance();
	r=r-128*create_velocity();
	return(r);
}

// updates the virtual wall and home base sensors (seen or not seen) 
// and virtual wall signal strength
int create_wall() {
	int b;
	CREATE_BUSY;
	s_write_byte(149);
	s_write_byte(3);  // send 3 packets
	s_write_byte(8);  // virtual wall seen or not (1 byte)
	s_write_byte(13); // virtual wall or home base seen or not (1 byte)
	s_write_byte(27); // signal strength (2 bytes)
	CREATE_WAITFORBUFFER(4,-1)
	gc_wall=s_read_byte();
	gc_wall_hb=s_read_byte();
	gc_wall_amt=(256*s_read_byte() + s_read_byte());
	CREATE_FREE;
	return(0);
}

// updates the bumper and wheel drop globals with the current values from the Create
int create_buttons() {
	int b;
	CREATE_BUSY;
	s_write_byte(142);
	s_write_byte(18);//buttons
	CREATE_WAITFORBUFFER(1,-1)
	b=s_read_byte();
	CREATE_FREE;
	gc_play_button=b & 0x1;
	gc_advance_button=(b >> 2) & 0x1;
	return(0);
}

// updates the bumper and wheel drop globals with the current values from the Create
int create_bumpdrop() {
	int b;
	CREATE_BUSY;
	s_write_byte(142);
	s_write_byte(7);//drops and bumps
	CREATE_WAITFORBUFFER(1,-1)
	b=s_read_byte();
	CREATE_FREE;
	gc_fdrop=(b >> 4) & 0x1;
	gc_ldrop=(b >> 3) & 0x1;
	gc_rdrop=(b >> 2) & 0x1;
	gc_lbump=(b >> 1) & 0x1;
	gc_rbump=b & 0x1;
	return(0);
}

// updates the cliff sensor globals with the current sensor values from the Create
int create_cliffs() {
	CREATE_BUSY;
	s_write_byte(149);
	s_write_byte(8);  // send 8 packets
	s_write_byte(9);  // cliff sensor detected or not (single byte packets)
	s_write_byte(10);  
	s_write_byte(11);
	s_write_byte(12);
	s_write_byte(28); // cliff sensor signal strength (two byte packets)
	s_write_byte(29);
	s_write_byte(30);
	s_write_byte(31);
	CREATE_WAITFORBUFFER(12,-1)
	gc_lcliff=s_read_byte(); 
	gc_lfcliff=s_read_byte();
	gc_rfcliff=s_read_byte();
	gc_rcliff=s_read_byte();
	gc_lcliff_amt=(256*s_read_byte() + s_read_byte());
	gc_lfcliff_amt=(256*s_read_byte() + s_read_byte());
	gc_rfcliff_amt=(256*s_read_byte() + s_read_byte());
	gc_rcliff_amt=(256*s_read_byte() + s_read_byte());
	CREATE_FREE;
	return(0);
}

// this function updates gc_angle which stores a normalized angle between 0 and 359 degrees
// and the global gc_total_angle which is not normalized and can be larger than 360 and less than 0.
// CCW angles are positive and CW turns decrement the angle value.
int create_angle() {
	int newangle=0;
	CREATE_BUSY;
	s_write_byte(142);
	s_write_byte(20);
	//    while(serial_buffer_count()<2);
	CREATE_WAITFORBUFFER(2,-1)
	newangle = (256*s_read_byte() + s_read_byte());
	CREATE_FREE;
	if(newangle > 32767){ //if this was meant to be a negative 16 bit int
		newangle=newangle - 65536;//convert from 16 bit 2's complement to 32bit int
	}
	gc_total_angle = gc_total_angle + newangle;
	gc_angle=(gc_angle+ newangle) % 360;
	if(gc_angle < 0) gc_angle = gc_angle + 360;
	return(0);
}

// this function updates the global gc_distance which has the average distance traveled of the two
// wheels (vehicle odometry) in mm.  Turning in place does not change this value.  
// Forward increments, backwards decrements
int create_distance() {
	int incDistance;
	CREATE_BUSY;
	s_write_byte(142);
	s_write_byte(19);
	CREATE_WAITFORBUFFER(2,-1)
	incDistance=(256*s_read_byte() + s_read_byte());
	if(incDistance > 32767){ //if this was meant to be a negative 16 bit int
		incDistance=incDistance - 65536;//convert from 16 bit 2's complement to 32bit int
	}//note that if Create moves more than 32m between create_distance call, there will be overflow		
	gc_distance=gc_distance+incDistance;
	CREATE_FREE;
	return(0);
}

// this function updates the global gc_vel, gc_radius, gc_rvel, and gc_lvel variables
// under create_drive, returned values for gc_rvel and gc_lvel are 0
// under create_drive_direct, returned values for gc_vel and gc_radius are 0
int create_velocity() {
	CREATE_BUSY;
	s_write_byte(149);
	s_write_byte(4);  // send 4 packets
	s_write_byte(39); 
	s_write_byte(40);
	s_write_byte(41);
	s_write_byte(42);
	CREATE_WAITFORBUFFER(8,-1)
	gc_vel=(256*s_read_byte() + s_read_byte());     // curent create_drive velocity in mm/sec (-500 to +500)
	if(gc_vel > 32767) gc_vel = 65536 - gc_vel;
	gc_radius=(256*s_read_byte() + s_read_byte());  // current create_drive radius in mm
	if(gc_radius > 32767) gc_radius = 65536 - gc_radius;
	gc_rvel=(256*s_read_byte() + s_read_byte());    // current drive_direct right wheel velocity in mm/sec
	if(gc_rvel > 32767) gc_rvel = 65536 - gc_rvel;
	gc_lvel=(256*s_read_byte() + s_read_byte());    // current drive_direct left wheel velocity in mm/sec
	if(gc_lvel > 32767) gc_lvel = 65536 - gc_lvel;
	CREATE_FREE;
	return(0);
}    

//sets gc_overcurrents to have a state where 16s'bit is left wheel; 8's is right wheel, 4's id LD2, 2's is LD0 and 1's is LD1
int create_motor_overcurrents(){
	CREATE_BUSY;
	s_write_byte(142);
	s_write_byte(14);
	CREATE_WAITFORBUFFER(1,-10)
	gc_overcurrents=s_read_byte();
	CREATE_FREE;
	return(0);
}

//updates values for charging and battery state
int create_battery_charge()
{
	CREATE_BUSY;
	clear_serial_buffer();
	s_write_byte(149);
	s_write_byte(7);
	s_write_byte(21);
	s_write_byte(22);
	s_write_byte(23);
	s_write_byte(24);
	s_write_byte(25);
	s_write_byte(26);
	s_write_byte(34);
	CREATE_WAITFORBUFFER(11,-11)
	gc_charge_state=s_read_byte();
	gc_batt_voltage=s_read_byte()*256;
	gc_batt_voltage=gc_batt_voltage+s_read_byte();
	gc_current_flow=s_read_byte()*256;
	gc_current_flow=gc_current_flow+s_read_byte();
	if(gc_current_flow>32767)gc_current_flow=gc_current_flow-65536;
	gc_batt_temp==s_read_byte();
	if(gc_batt_temp>127)gc_batt_temp=gc_batt_temp-128;
	gc_batt_charge=s_read_byte()*256;
	gc_batt_charge=gc_batt_charge+s_read_byte();
	gc_batt_capacity=s_read_byte()*256;
	gc_batt_capacity=gc_batt_capacity+s_read_byte();
	gc_charge_source=s_read_byte();//1 if charger 2 if home base 0 if no charge source
	CREATE_FREE;
	return(0);
}

//reads the values off of the cargo bay inputs
int create_cargo_bay_inputs()
{
	CREATE_BUSY;
	s_write_byte(149);
	s_write_byte(2);
	s_write_byte(32);
	s_write_byte(33);
	CREATE_WAITFORBUFFER(3,-12)
	gc_digital_in=s_read_byte();//pins 15,6,18,5,17
	gc_analog_in=s_read_byte()*256;
	gc_analog_in=gc_analog_in+s_read_byte();//analog input is on pin 4
	CREATE_FREE;
	return(0);
}




////////////////////CREATE MOVEMENT FUNCTIONS///////////////////////////////////////////////

// This command drives the robot along a curve with radius (in mm) radius; and at a speed (mm/sec) of speed
// a radius of 32767 will drive the robot straight
// a radius of 1 will spin the robot CCW
// a radius of -1 will spin the robot CW
// Negative radii will be right turns, positive radii left turns
void create_drive (int speed, int radius)
{
	CREATE_BUSY;
	serial_write_byte(137);
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(get_high_byte(radius));
	serial_write_byte(get_low_byte(radius));
	CREATE_FREE;
}

// special version of command above drives robot at speed 0, stopping it. 
void create_stop()
{
	CREATE_BUSY;
	serial_write_byte(137);
	serial_write_byte(0);
	serial_write_byte(0);
	serial_write_byte(0);  
	serial_write_byte(0);    
	CREATE_FREE;
}

// special version of command above drives robot at speed speed.  Negative speed will drive robot backwards
void create_drive_straight (int speed)
{
	CREATE_BUSY;
	serial_write_byte(137);
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(get_high_byte(32767));
	serial_write_byte(get_low_byte(32767));
	CREATE_FREE;
}

// special version of command spins robot CW with the wheels turning at speed speed
void create_spin_CW (int speed)
{
	CREATE_BUSY;
	serial_write_byte(137);
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(get_high_byte(-1));
	serial_write_byte(get_low_byte(-1));
	CREATE_FREE;
}

// special version of command spins robot CCW with the wheels turning at speed speed
void create_spin_CCW (int speed)
{
	CREATE_BUSY;
	serial_write_byte(137);
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(get_high_byte(1));
	serial_write_byte(get_low_byte(1));
	CREATE_FREE;
}

// this function drives the right wheel at r_speed and the left wheel at l_speed
// speeds for all of these functions are +/-500mm/sec.
void create_drive_direct(int r_speed, int l_speed)
{
	CREATE_BUSY;
	serial_write_byte(145);
	serial_write_byte(get_high_byte(r_speed));
	serial_write_byte(get_low_byte(r_speed));
	serial_write_byte(get_high_byte(l_speed));
	serial_write_byte(get_low_byte(l_speed));
	CREATE_FREE;
}    


//This function blocks and does a pretty accurate spin.  Note that the function will
//not return until the spin is complete
//CAUTION, requesting the robot to spin more than about 3600 degrees may never terminate
//the 9/07 firmware update is required for this works. Returns -1 if error
int create_spin_block(int speed, int angle)
{
	int r[7]={18,7,9,24,12,15,21},v[7],i,b;
	long lenc=0L, renc=0L,slenc,srenc,flenc,frenc;
	float offset, ticsPerMM=7.8324283, rad=129.0, pi=3.1415926;
	CREATE_BUSY;
	s_write_byte(8);//version
	CREATE_WAITFORBUFFER(7,-99)
	for(i=0;i<7;i++){
		v[i]=s_read_byte();
	}
	CREATE_FREE;
	if(!(v[1]==r[1] && v[2]==r[2] && v[3]==r[3])){
		printf("create_spin_block\n requires Create Firmware dated\n 9/24/2007\n");
		printf("Yours is:%d/%d/%d\nFunction aborted\n", v[2], v[3], v[1]+2000);
		return(-1);
	}
	_create_get_raw_encoders(&slenc, &srenc);
	offset=(float)angle*pi*rad*ticsPerMM/180.0;
	if(angle>0){create_drive_direct(speed,-speed);}
	else {offset=-offset; create_drive_direct(-speed,speed);}
	flenc=slenc+(long)offset;
	frenc=srenc+(long)offset;
	if(frenc > 65535L && flenc > 65000L){
		while(lenc >= slenc)_create_get_raw_encoders(&lenc, &renc); 
		while(lenc < flenc-65536L)_create_get_raw_encoders(&lenc, &renc);
	create_stop();}
	else{
		while(lenc < flenc && renc < frenc)_create_get_raw_encoders(&lenc, &renc); 
		create_stop();
	}
	return(0);
}
int _create_get_raw_encoders(long *lenc, long *renc)
{
	CREATE_BUSY;
	s_write_byte(149);
	s_write_byte(2);
	s_write_byte(43);
	s_write_byte(44);
	CREATE_WAITFORBUFFER(4,-14)
	*lenc=(long)s_read_byte()*256L;
	*lenc=*lenc+(long)s_read_byte();//left encoder value
	*renc=(long)s_read_byte()*256L;
	*renc=*renc+(long)s_read_byte();//left encoder value
	CREATE_FREE;
}



/////////////////////////CREATE LEDs///////////////////

// turns on and off the advance (>>) LED (0 is off, 1 is on)
void create_advance_led(int on) 
{
	if(on){
		if(!(gc_leds[0] & 8)) gc_leds[0]=gc_leds[0]+ 8;
	}
	else{
		if(gc_leds[0] & 8) gc_leds[0]=gc_leds[0]- 8;
	}
	CREATE_BUSY;
	serial_write_byte(139);
	serial_write_byte(gc_leds[0]);            
	serial_write_byte(gc_leds[1]);            
	serial_write_byte(gc_leds[2]);  
	CREATE_FREE;
}

// turns on and off the play LED (0 is off, 1 is on)
void create_play_led(int on) 
{
	if(on){
		if(!(gc_leds[0] & 2)) gc_leds[0]=gc_leds[0]+ 2;
	}
	else{
		if(gc_leds[0] & 2) gc_leds[0]=gc_leds[0]- 2;
	}
	CREATE_BUSY;
	serial_write_byte(139);
	serial_write_byte(gc_leds[0]);            
	serial_write_byte(gc_leds[1]);            
	serial_write_byte(gc_leds[2]);  
	CREATE_FREE;
}


//sets create power led. Color =0 is green and color = 255 is red -- with intermediate colors
//brightness of 0 is off and 255 is fully bright
void create_power_led(int color, int brightness) 
{
	gc_leds[1]=color;
	gc_leds[2]=brightness;
	CREATE_BUSY;
	serial_write_byte(139);
	serial_write_byte(gc_leds[0]);            
	serial_write_byte(gc_leds[1]);            
	serial_write_byte(gc_leds[2]);      
	CREATE_FREE;
}


/////////// The functions below are for writing to the 25 pin connector

//This function sets the three digital out put pins 20,7,19 where 20 is the high bit and 19 is the low. bits should have a value 0 to 7.
void create_digital_output(int bits)
{
	CREATE_BUSY;
	serial_write_byte(147);
	serial_write_byte(bits);            
	CREATE_FREE;
}

//Sets the PWM signal for the three low side drivers (128 = 100%).  pwm2 is pin 24, pwm1 pin 23 and pwm0 pin 22
void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
	CREATE_BUSY;
	serial_write_byte(144);
	serial_write_byte(pwm2);            
	serial_write_byte(pwm1);            
	serial_write_byte(pwm0);            
	CREATE_FREE;
}

//Turns on and off the signal for the three low side drivers (128 = 100%).  pwm2 is pin 24, pwm1 pin 23 and pwm0 pin 22
//A 0 or 1 should be given for each of the drivers to turn them off or on.
void create_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
	CREATE_BUSY;
	serial_write_byte(138);
	serial_write_byte(4*pwm2+2*pwm1+pwm0);            
	CREATE_FREE;
}


/////////// CREATE MUSIC////////////

// this loads a song into the robot's memory. 
// song can be numbered 0 to 15. the first element in each row of the array should be
// the number of notes (1-16) the subsequent pairs of bytes should be tone and duration
// see the roomba SCI manual for note codes.
// user's program should load the song data into the array before calling this routine
void create_load_song(int num)
{
	int i, numnotes;
	numnotes=gc_song_array[num][0];
	if(num >= 0 && num <=15 && numnotes > 0 && numnotes <= 16){
		CREATE_BUSY;
		serial_write_byte(140);
		serial_write_byte(num);
		serial_write_byte(numnotes);
		for(i=1; i< 2*numnotes+1; i++)
		serial_write_byte(gc_song_array[num][i]);
		CREATE_FREE;
	}
	else printf("illegal song #%d is %d notes long being written to memory\n", num, gc_song_array[num][0]);
}

// see the roomba SCI manual for note codes.
void create_play_song(int num)
{
	int i;
	if(num >= 0 && num <=15 ){
		CREATE_BUSY;
		serial_write_byte(141);
		serial_write_byte(num);
		CREATE_FREE;
	}
	else printf("Song array reference is out of bounds\n");
}


////////////////////CREATE IR FUNCTIONS///////////////////////////////////////////

// The send_IR command assumes an IR transmitter is connected per the Create Open 
// interface spec - uses the switched +5V source (pin 8 of cargo bay adapter) 
// and low side driver 1 (pin 23).  An IR data byte is sent in the form expected
// by the Create IR receiver.

//Commands IR transmitter to send a byte of data
//129-143 are sent by the remote control, 240-254 by the base station
void create_send_IR(int byte) {  
	CREATE_BUSY;
	serial_write_byte(151);
	serial_write_byte(byte); 
	CREATE_FREE;
}

// gc_IR=255 indicates no IR byte is being received
int create_read_IR() {  
	CREATE_BUSY;
	s_write_byte(142);
	s_write_byte(17); // IR packet
	CREATE_WAITFORBUFFER(1,-1)
	gc_IR=s_read_byte(); 
	CREATE_FREE;
	return(0);
}

////////////////////CREATE MOVEMENT USING SCRIPTS//////////////////

// WARNING: During script operation, serial commands are ignored, 
// potentially hanging the control computer in cases such as
// waiting for a Create sensor update.
// Each movement command signals on pin 20 when it is done
// If this signal is not read, then a timing estimate will 
// need to be used instead.
// The most recent script may be repeated by serial_write_byte(153)

// script to move dist mm at speed mm/sec
void create_script_move(int dist, int speed) { 
	CREATE_BUSY;
	serial_write_byte(152); // start script
	serial_write_byte(17);  // script length
	serial_write_byte(147); // on pin 20
	serial_write_byte(0);   //   output 0
	serial_write_byte(137); // move 
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(128); // no angle (go straight)
	serial_write_byte(0);
	serial_write_byte(156); // wait for distance done
	serial_write_byte(get_high_byte(dist));
	serial_write_byte(get_low_byte(dist));
	serial_write_byte(137); // stop move 
	serial_write_byte(0);   // no speed
	serial_write_byte(0);
	serial_write_byte(0);   // no angle
	serial_write_byte(0);   
	serial_write_byte(147); // on pin 20
	serial_write_byte(0);   //   output 1
	// end of script
	serial_write_byte(153); // run script
	CREATE_FREE;
}

// script to move with individual motor control
// dist mm with r_speed mm/sec on right wheel and l_speed on left
void create_script_move_direct(int dist, int r_speed, int l_speed) { 
	CREATE_BUSY;
	serial_write_byte(152); // start script
	serial_write_byte(17);  // script length
	serial_write_byte(147); // on pin 20
	serial_write_byte(0);   //   output 0
	serial_write_byte(145); // move 
	serial_write_byte(get_high_byte(r_speed));
	serial_write_byte(get_low_byte(r_speed));
	serial_write_byte(get_high_byte(l_speed));
	serial_write_byte(get_low_byte(l_speed));
	serial_write_byte(156); // wait for distance done
	serial_write_byte(get_high_byte(dist));
	serial_write_byte(get_low_byte(dist));
	serial_write_byte(137); // stop move 
	serial_write_byte(0);   // no speed
	serial_write_byte(0);
	serial_write_byte(0);   // no angle
	serial_write_byte(0);   
	serial_write_byte(147); // on pin 20
	serial_write_byte(0);   //   output 1
	// end of script
	serial_write_byte(153); // run script
	CREATE_FREE;
}


// script to rotate in place through deg degrees
// deg > 0 turn CCW; deg < 0 turn CW
void create_script_turn(int deg, int speed) { // degrees, vel in mm/sec
	CREATE_BUSY;
	serial_write_byte(152); // start script
	serial_write_byte(17);  // script length
	serial_write_byte(147); // on pin 20
	serial_write_byte(0);   //   output 0
	serial_write_byte(137); // move 
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	if (deg > 0){ // CCW case 
		serial_write_byte(0); 
	serial_write_byte(1); }
	else { // CW case
		serial_write_byte(255);
	serial_write_byte(255); }
	serial_write_byte(157); // wait for angle done
	serial_write_byte(get_high_byte(deg));
	serial_write_byte(get_low_byte(deg));
	serial_write_byte(137); // stop move 
	serial_write_byte(0);   // no speed
	serial_write_byte(0);
	serial_write_byte(0);   // no angle
	serial_write_byte(0);   
	serial_write_byte(147); // on pin 20
	serial_write_byte(0);   //   output 1
	// end of script
	serial_write_byte(153); // run script
	CREATE_FREE;
}

// script to trace an arc of radius rad until deg is reach
// NOTE: if the turn is not in the direction of deg, the arc won't end
void create_script_arc(int rad, int deg, int speed) { // rad in mm, degrees, vel in mm/sec
	CREATE_BUSY;
	serial_write_byte(152); // start script
	serial_write_byte(17);  // script length
	serial_write_byte(147); // on pin 20
	serial_write_byte(0);   //   output 0
	serial_write_byte(137); // move 
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(get_high_byte(rad)); 
	serial_write_byte(get_high_byte(rad)); 
	serial_write_byte(157); // wait for angle done
	serial_write_byte(get_high_byte(deg));
	serial_write_byte(get_low_byte(deg));
	serial_write_byte(137); // stop move 
	serial_write_byte(0);   // no speed
	serial_write_byte(0);
	serial_write_byte(0);   // no angle
	serial_write_byte(0);   
	serial_write_byte(147); // on pin 20
	serial_write_byte(0);   //   output 1
	// end of script
	serial_write_byte(153); // run script
	CREATE_FREE;
}

// Script builder
// cwinton: 11/18/2007
	/* usage:
	create_script_start(); // initiates script build
	create_do_<action>;    // one or more build actions
	create_script_end();   // instantiates script build
	create_script_run();   // runs current build
	*//*
	The script signals on pin 20 when it is done;
if this signal is not read, then a timing estimate will 
need to be used instead to determine when script action ends.

Maximum length from do actions is 91 bytes;
each do action below is 8 bytes, limiting script to no
more than 11 do actions.
*/
int _create_script[102]={152,0,147,0}, // build space
// (script command, length (to be set), pin 20, output value)
_create_script_indx=0;
void create_script_start() { // initialize script
	_create_script_indx=4; 
}

int create_do_move(int dist, int speed) {
	if ((91-_create_script_indx)<8) {
		printf("max script exceeded\n");
	return(_create_script_indx); }  // not enough script space left 
	_create_script[_create_script_indx++]=137; // move command
	_create_script[_create_script_indx++]=get_high_byte(speed);
	_create_script[_create_script_indx++]=get_low_byte(speed);
	_create_script[_create_script_indx++]=128; // no angle (go straight)
	_create_script[_create_script_indx++]=0;
	_create_script[_create_script_indx++]=156; // wait for distance done
	_create_script[_create_script_indx++]=get_high_byte(dist); 
	_create_script[_create_script_indx++]=get_low_byte(dist);
	return(0);
}
int create_do_move_direct(int dist, int r_speed, int l_speed) {  // individual motor control move
	if ((91-_create_script_indx)<8) {
		printf("max script exceeded\n");
	return(_create_script_indx); }  // not enough script space left 
	_create_script[_create_script_indx++]=145; // move command
	_create_script[_create_script_indx++]=get_high_byte(r_speed);
	_create_script[_create_script_indx++]=get_low_byte(r_speed);
	_create_script[_create_script_indx++]=get_high_byte(l_speed);
	_create_script[_create_script_indx++]=get_low_byte(l_speed);
	_create_script[_create_script_indx++]=156; // wait for distance done
	_create_script[_create_script_indx++]=get_high_byte(dist); 
	_create_script[_create_script_indx++]=get_low_byte(dist);
	return(0);
}
int create_do_arc(int rad, int deg, int speed) {
	// warning: CCW movement increases degree, CW decreases
	// opposite signs on rad and vel cause CW rotation
	// if direction is CW, this won't stop if deg > 0
	if ((91-_create_script_indx)<8) {
		printf("max script exceeded\n");
	return(_create_script_indx); }  // not enough script space left 
	_create_script[_create_script_indx++]=137; // move command
	_create_script[_create_script_indx++]=get_high_byte(speed);
	_create_script[_create_script_indx++]=get_low_byte(speed);
	_create_script[_create_script_indx++]=get_high_byte(rad);
	_create_script[_create_script_indx++]=get_low_byte(rad);
	_create_script[_create_script_indx++]=157; // wait for angle done
	_create_script[_create_script_indx++]=get_high_byte(deg); 
	_create_script[_create_script_indx++]=get_low_byte(deg);
	return(0);
}

int create_do_turn(int deg, int speed) {  // speed > 0
	if ((91-_create_script_indx)<8) {
		printf("max script exceeded\n");
	return(_create_script_indx); }  // not enough script space left 
	_create_script[_create_script_indx++]=137; // move command
	_create_script[_create_script_indx++]=get_high_byte(speed);
	_create_script[_create_script_indx++]=get_low_byte(speed);
	if (deg > 0){ // CCW case 
		_create_script[_create_script_indx++]=0; 
	_create_script[_create_script_indx++]=1; }
	else { // CW case
		_create_script[_create_script_indx++]=255;
	_create_script[_create_script_indx++]=255; }    
	_create_script[_create_script_indx++]=157; // wait for angle done
	_create_script[_create_script_indx++]=get_high_byte(deg); 
	_create_script[_create_script_indx++]=get_low_byte(deg);
	return(0);
}

void create_script_end() {
	int i;
	_create_script[_create_script_indx++]=137; // stop move 
	_create_script[_create_script_indx++]=0;   // (no speed)
	_create_script[_create_script_indx++]=0;
	_create_script[_create_script_indx++]=0;   // (no angle)
	_create_script[_create_script_indx++]=0;   
	_create_script[_create_script_indx++]=147; // on pin 20
	_create_script[_create_script_indx++]=1;   //   output 1 
	// set script length
	_create_script[1]=_create_script_indx-2;
	// load script from build space
	CREATE_BUSY;
	for(i=0;i<_create_script_indx;i++)
	serial_write_byte(_create_script[i]);
	CREATE_FREE;
}

void create_script_run() {
	serial_write_byte(153); // run script
}


/////////// The functions below are communication and byte packing utitilities
unsigned char get_high_byte(int v)
{
	return (v >>8);
}

unsigned char get_low_byte(int v)
{
	return (v & 255);
}

void clear_serial_buffer()
{
	while (serial_buffer_count()) serial_read_byte();
}

void s_write_byte(unsigned char byte)
{
	serial_write_byte(byte);
}

//Returns 0 if there is nothing to be read
unsigned char s_read_byte() 
{
	return serial_read_byte();
}



/* END CreateLib.c */

#endif




