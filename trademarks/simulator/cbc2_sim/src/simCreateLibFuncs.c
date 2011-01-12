/* iROBOsim graphics simulator written by David Miller for  IC 
   Do not redistribute this file.  For more copies, contact the author or KIPR.
   (C) 2006-2008 David Miller
   
   Ported for KISS-C by Charles Winton, 6/15/08
   
   The port combines iROBOCreateconstants.ic, iROBO-graphics.ic, iROBOsim-create.ic 
   in iROBOsim.h and iROBOsim.c.  
   
   iROBOsim.h is the user include file and has the necessary
   preprocessor #defines and #includes. Colors updated from 16-bit (555) to 24-bit RGB.
   
   iROBOsim.c has the basic iROBO draw functions 
   
   The file simcreatelib.ic is used in the case the simulated bot
   is to behave like a Create module, providing code compatibility so the program can also
   be used with an actual bot ... it has been ported with only minor adjustment.
   
   Since KISS-C does not have compile and link edit, iROBOsim.h concludes with
   #include for "iROBOsim.c" and "simcreatelib.c"
   
   NOTE: commented out from IC are hog_processor() and defer() since not supported in KISS-C
*/
/* Port note: moved globals added for sim to precede use (_last_angle, _curr_angle)
*/

//#include <kiss-serial.h>

/* This file implements the create functions for moving, getting distance and bumper functions inside
   of the iROBOsim simulator
*/

/* This file has been updated for 2011 to refelect the new Create library which does not have user globals
and implements restraint on serial communications.  This library better reflects communications timing as well.
*/


#ifndef __CREATE_LIB_H__
#define __CREATE_LIB_H__

#include <kiss-serial.h>
#include <stdio.h>
#include <unistd.h>
#include <kiss-compat.h>

//rings the system alert tone (whatever it may be) but also inserts a <CR>
#ifndef _CBCLIB
#include "simCBC.c"
#endif

#define twopi 6.28318531
//CREATE_BUSY keeps any  create commands in other processes from interfering with one another.
#define CREATE_BUSY while(stateOfCreate.createBusy)msleep(10L); stateOfCreate.createBusy = 1;
#define CREATE_FREE stateOfCreate.createBusy = 0;

//checks to make sure create is connected, if not, generates error
#define CREATE_WAITFORBUFFER(bc,err) {if(stateOfCreate.createConnected==0){stateOfCreate.createBusy=0;return(err);}else msleep(bc);}


// The two structures below contain all the info about the Create
struct _sensor_packet{
	float lastUpdate; //cpu clock time
	unsigned int data; // 4 bytes or less of data
};

struct _create_state{
	float songDuration;
	float songStart;
	float oldCtheta;
	int createConnected;
	int createBusy;
	int normalizedAngle;
	int totalAngle;
	int accumulatedDistance;//sum of the increment distances
	int driveDirect;//1 if active command of each wheel
	int drive;//1 if speed and radius command is active
	int lspeed;//command last sent
	int rspeed;
	int radius;
	int speed;
	int leds[3];//The state of the create LEDs
	struct _sensor_packet bumpsWheelDrops;//packet 7, b4 caster, b3 lw, b2 rw, b1 lb, b0 rb
	struct _sensor_packet wall;//packet 8, 0 no wall, 1 wall
	struct _sensor_packet lcliff;//packet 9 left cliff sensor 0 no cliff, 1 cliff
	struct _sensor_packet lfcliff;//packet 10 left front cliff sensor 0 no cliff, 1 cliff
	struct _sensor_packet rfcliff;//packet 11 right front cliff sensor 0 no cliff, 1 cliff
	struct _sensor_packet rcliff;//packet 12 right cliff sensor 0 no cliff, 1 cliff
	struct _sensor_packet vWall;//packet 13 0 no wall seen, 1 there is a wall
	struct _sensor_packet LSDandWheelOvercurrents ;//packet 14;b4 LW, b3 RW, B2 LD2, b1 LD0, b0 LD1
	// packets 15-16 are unused
	struct _sensor_packet infrared;//packet 17; byte received from IR remote. 255 means no byte seen
	struct _sensor_packet buttons;//packet 18; b2 advance button is pressed, b0 play button
	struct _sensor_packet distance; //packet 19; 2 byte signed number in mm since last request
	struct _sensor_packet angle;//packet 20; in degrees since last request - 2 byte signed num
	struct _sensor_packet chargingState;//packet 21; 0=not charging; 1-reconditioning chargeing; 2 full charging; 3 trickle charging; 4 waiting; 5 charge fault
	struct _sensor_packet voltage;//packet 22; unsigned 2 byte in mV
	struct _sensor_packet current;// packet 23; signed 2 byte in mA
	struct _sensor_packet batteryTemp;//packet 24; temp in degrees Celcius (1 byte signed)
	struct _sensor_packet batteryCharge;//packet 25; 2byte no sign in mAh
	struct _sensor_packet batteryCapacity;//packet 26; estimated charge capacity of battery in mAh
	struct _sensor_packet wallSignal;//packet 27; analog value of wall sensor 0-4095
	struct _sensor_packet lcliffSignal;//packet 28 left cliff sensor 0-4095
	struct _sensor_packet lfcliffSignal;//packet 29 left front cliff sensor 0-4095
	struct _sensor_packet rfcliffSignal;//packet 30 right front cliff sensor 0-4095
	struct _sensor_packet rcliffSignal;//packet 31 right cliff sensor 0-4095
	struct _sensor_packet cargoBayDI;//packet 32; b4 baud rate change pin 15, b3 DI3 pin 6, b2 DI2-pin 18, b1 DI1 pin 5, b0 DI0 pin 17
	struct _sensor_packet cargoBayAI;//packet 33; 0-1024
	struct _sensor_packet chargingSource;//packet 34; b1 home base, b0 internal charger
	struct _sensor_packet OIMode;//packet 35;0=off,1=passive,2=safe,3=full
	struct _sensor_packet songNumber;//packet 36; currently selected song number
	struct _sensor_packet songPlaying;//packet 37;1=song is playing, 0 song not playing
	struct _sensor_packet numStreamPackets;//packet-38, number of streamed packets, 0-43, is returned
	struct _sensor_packet requestedVelocity;//packet 39; -500 to 500
	struct _sensor_packet requestedRadius;//packet 40; -32768 to 32767
	struct _sensor_packet requestedRVelocity;//packet 41; -500 to 500
	struct _sensor_packet requestedLVelocity;//packet 42; left wheel v -500 to 500
} ;

// create the Create's state and initialize all data to 0 and all last updates to a billion seconds in the past.
struct _create_state stateOfCreate={0.0,-1.E9,0.0,0,0,0,0,0,0,0,0,0,0,0,{0,0,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0}};//is the one and only instance of this structure


//Function prototypes
int create_connect();//Required (once) before any other Create commands
void create_disconnect() ;//restores Create and serial line to initial state
void create_start();//Puts Create in start mode -- not normally used by when using create_connect
void create_passive();//Puts Create is passive mode -- motors are disabled
void create_safe();//Normal operating mode LED is orange. Create will not go over edges and stops when lifted
void create_full() ;//Safeties are disabled.  Create follows all orders. LED is red.
void create_spot() ;//Thinks its a Roomba doing a spot clean
void create_cover();//Roomba wander program
void create_demo(int d) ;//Executes the built in demos.  See Create manual.
void create_cover_dock();//Wanders looking for a charging dock.
int get_create_mode(float lag);//returns 1 for passive, to for safe and 3 for full.  Data has been gathered within lag seconds

int _get_create_bumpdrop(float lag);//sub function not inteded for general use
int get_create_lbump(float lag);// returns 1 if left bumper is pressed, 0 otherwise.  Data has been gathered within lag seconds
int get_create_rbump(float lag);// returns 1 if right bumper is pressed, 0 otherwise.  Data has been gathered within lag seconds
int get_create_lwdrop(float lag);// returns 1 if left wheel has dropped, 0 otherwise.  Data has been gathered within lag seconds
int get_create_cwdrop(float lag);// returns 1 if caster wheel has dropped, 0 otherwise.  Data has been gathered within lag seconds
int get_create_rwdrop(float lag);// returns 1 if right wheel has dropped, 0 otherwise.  Data has been gathered within lag seconds

int get_create_wall(float lag);//returns 1 if wall is detected, 0 otherwise.  Data has been gathered within lag seconds
int get_create_lcliff(float lag);// returns 1 if nothing or black is under left cliff sensor, 0 otherwise.  Data has been gathered within lag seconds
int get_create_lfcliff(float lag);// returns 1 if nothing or black is under left front cliff sensor, 0 otherwise.  Data has been gathered within lag seconds
int get_create_rfcliff(float lag);// returns 1 if nothing or black is under right front cliff sensor, 0 otherwise.  Data has been gathered within lag seconds
int get_create_rcliff(float lag);// returns 1 if nothing or black is under right cliff sensor, 0 otherwise.  Data has been gathered within lag seconds
int get_create_vwall(float lag);// returns 1 if virtual wall is detected, 0 otherwise.  Data has been gathered within lag seconds
int get_create_overcurrents(float lag);
int get_create_infrared(float lag);// returns byte detected by infrared sensor, 255 otherwise.  Data has been gathered within lag seconds

int _get_create_buttons(float lag);//sub function not inteded for general use
int get_create_advance_button(float lag);// returns 1 if advance button is pressed, 0 otherwise.  Data has been gathered within lag seconds
int get_create_play_button(float lag);// returns 1 if play button is pressed, 0 otherwise.  Data has been gathered within lag seconds


// Note that the create truncates incremental data for distance every time data is requested.
//Therefore SENSING TOO OFTEN REDUCES ACCURACY and will return a number that is less than the actual value.
// A general rule of thumb is to have the lag time be no less than 5/speed seconds and never less than 0.05 seconds.
int get_create_distance(float lag);// returns cumulative distance Create has traveled.  Accurate to within the last lag seconds.
int get_create_incremental_distance(float lag);// not for general use
void set_create_distance(int dist);// sets the value of the Create distance traveled, as of this, call to the value dist.

//Note that the create truncates incremental data for angle every time data is requested.
//Therefore SENSING TOO OFTEN REDUCES ACCURACY and will return a number that is less than the actual value.
//A general rule of thumb is to have the lag time be no less than 10/|(left_speed - right_speed)| seconds and never less than 0.05 seconds.
int get_create_normalized_angle(float lag);// returns normalized angle (0-359 degrees) Create has turned.  Accurate to within the last lag seconds.
int get_create_total_angle(float lag);// returns cumulative angle Create has turned.  Accurate to within the last lag seconds.
int get_create_incremental_angle(float lag);
void set_create_normalized_angle(int a);// sets the value of the Create distance traveled, as of this, call to the value dist.
void set_create_total_angle(int a);// sets the value of the Create distance traveled, as of this, call to the value dist.

int _get_create_battery_charge(float lag);//sub function not inteded for general use

int get_create_wall_amt(float lag);// returns value (0-255) of wall sensor.  Data has been gathered within lag seconds
int get_create_lcliff_amt(float lag);// returns value (0-255) of left cliff sensor.  Data has been gathered within lag seconds
int get_create_lfcliff_amt(float lag);// returns value (0-255) of left front cliff sensor.  Data has been gathered within lag seconds
int get_create_rfcliff_amt(float lag);// returns value (0-255) of right front sensor.  Data has been gathered within lag seconds
int get_create_rcliff_amt(float lag);// returns value (0-255) of right sensor.  Data has been gathered within lag seconds
int get_create_bay_DI(float lag);//byte containing all three digital sensors on lowest 3 bits.  Data has been gathered within lag seconds
int get_create_bay_AI(float lag);//10 bit analog value on pin 4.  Data has been gathered within lag seconds
int get_create_song_number(float lag);// currently selected song 0-15.  Data has been gathered within lag seconds
int get_create_song_playing(float lag);// 0 is song not playing. 1 playing is in progress.  Data has been gathered within lag seconds
int get_create_number_of_stream_packets(float lag);// if data is streaming is being used, returns the size of stream.  Data has been gathered within lag seconds
int get_create_requested_velocity(float lag);//asks Create how fast it was told to be moving -500 to 500mm/s.  Data has been gathered within lag seconds
int get_create_requested_radius(float lag);//asks Create the size of its turning radius.  Data has been gathered within lag seconds
int get_create_requested_right_velocity(float lag);//asks Create how fast it was told to be moving right wheel.  Data has been gathered within lag seconds
int get_create_requested_left_velocity(float lag);//asks Create how fast it was told to be moving left wheel.  Data has been gathered within lag seconds

//With the exception of the create_stop command, all motion commands are only sent to the Create if they are
//different from what the Create has previously been commanded.  They may be placed in a loop without fear
//of overloading the serial buffer.  Anytime a different movement function, or the same function with different
//parameters is called, the command will be sent on to the Create.
//Note all wheel speeds are limited to +/- 500 mm/s
void create_stop();// tells create to stop moving
void create_drive (int speed, int radius);//moves create at speed mm/s on a curve of radius mm.
void create_drive_straight (int speed);// move create at speed mm/s -- range is -500 to 500.
void create_spin_CW (int speed);// spins the Create CW withe the left wheel moving forward at speed mm/s
void create_spin_CCW (int speed);// spins the Create CCW withe the right wheel moving forward at speed mm/s
void create_drive_direct(int r_speed, int l_speed);//Moves the Create with the wheel speeds specified


int create_spin_block(int speed, int angle);//Will accurately spin the Create the requested angle at the requested speed.  However, all commands and sensor requests will be ignored while this command is being executed until the Create has reached the requested angle.
int _create_get_raw_encoders(long *lenc, long *renc);//internal use only

void create_advance_led(int state) ;//sets the LED by the Advance button to the requested state 0 = off; 1 = on 
void create_play_led(int state) ;//sets the LED by the Play button to the requested state 0 = off; 1 = on 
void create_power_led(int color, int brightness) ;//User can set color anywhere along the spectrum from 0 being green and 255 being red, and the brightness where 0 is off and 255 is full brightness.
void create_digital_output(int bits);//User can turn on or off pins 20, 7 and 19 using the lower 3 bits.
void create_pwm_low_side_drivers(int pwm24, int pwm22, int pwm23);//User can set PWM duty cycle for pins 24,22 and 23.
void create_low_side_drivers(int pin24, int pin22, int pin23);//User can set pins 24,22, and 23 on (1) or off (0).


int gc_song_array[16][33]; // This is a global used to load and play songs on the Create.  It is filled by the user
void create_load_song(int num);//loads song from gc_song_array row num.
void create_play_song(int num);//plays the song if it has been loaded

//The following low level functions are not intended for general use, but may be useful for interfacing other TTL serial devices.
unsigned char get_high_byte(int v);
unsigned char get_low_byte(int v);
void clear_serial_buffer();
void s_write_byte(unsigned char byte);
unsigned char s_read_byte() ;


/*****************************************************TEST
int main(){
	create_connect();
	beep();
	while(get_create_total_angle(.1)<180)
	{
		create_spin_CCW(50);
	}
	create_stop();
	printf("Completed at %f angle is %d\n",seconds(),get_create_total_angle(0));
	create_disconnect();
}
/*****************************************************TEST*/




// Create must be turned on and connected.  This routine establishes serial link
// Note that this routine should be called once, and must have been called before 
// any of the other create functions will work.
// Once this function is called, you cannot talk to the XBC over the USB connection
// until the create_disconnect function ahs been called or the GBA on the XBC has been powercycled.
// If Create is connected, power light goes yellow and play led turns on
int create_connect()
{
	sleep(0.1);
	stateOfCreate.createConnected = 1;
	create_start();
	create_safe(); //allows create to be commanded and turns LED orange
	clear_serial_buffer();
	if (get_create_mode(0.0)!=2) {       // if not safe mode, connection not established
		printf("NO CREATE CONNECTION mode is %d\n", get_create_mode(0.5));
		stateOfCreate.createConnected = 0;
		msleep(100); beep(); msleep(100); beep();
		return(-1);//connection failed
	}
	create_advance_led(1); // turn on advance LED
	return(0);//successful connection
}



// returns the serial connections on XBC to normal communications over the USB port.
// Turns of play LED and returns power light to green
void create_disconnect() {
	create_stop();
	create_play_led(0);
	create_power_led(0,255);
	create_passive();
	msleep(100);
	stateOfCreate.createConnected=0; 
}


// if serial communication times out (ie., is lost), attempts to restart it
void create_reconnect()
{
	printf("Create communication lost;\nattempting reconnection.\n");
	create_disconnect();    // reset environment
	create_connect();
}

// Create open interface mode is off=0 (Create powered), passive=1, safe=2, full=3
// From "safe" mode, wheel drop or cliff detect halts motors and puts Create in passive mode
int get_create_mode(float lag) {
	if(seconds()-stateOfCreate.OIMode.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(35);
		CREATE_WAITFORBUFFER(1,100035)
		stateOfCreate.OIMode.lastUpdate=seconds();//update update time
		CREATE_FREE;
	}	
	return(stateOfCreate.OIMode.data);
}



// See the create open interface document for more information on these functions
void create_start() {CREATE_BUSY; s_write_byte(128); stateOfCreate.OIMode.data=0; CREATE_FREE; }
void create_passive() {CREATE_BUSY; s_write_byte(128); stateOfCreate.OIMode.data=1; CREATE_FREE;}//same as start 
void create_safe() {CREATE_BUSY; s_write_byte(131); stateOfCreate.OIMode.data=2; CREATE_FREE;create_power_led(127,255);}
void create_full() {CREATE_BUSY; s_write_byte(132); stateOfCreate.OIMode.data=3; CREATE_FREE; create_power_led(227,255);}

void create_spot() {CREATE_BUSY; s_write_byte(134); CREATE_FREE; create_drive_direct(200,300);}

void create_cover() {CREATE_BUSY; s_write_byte(135); CREATE_FREE;create_drive_direct(400,300);}

void create_demo(int d) {CREATE_BUSY; s_write_byte(136); s_write_byte(d); CREATE_FREE;create_drive_direct(200,200);}

void create_cover_dock() {CREATE_BUSY; s_write_byte(143); CREATE_FREE;create_drive_direct(-200,-300);}


//////////////Create Sensor Routines///////////////
// These work for any Create mode except off


// updates the bumper and wheel drop states with the current values from the Create
int _get_create_bumpdrop(float lag) {
	int b;
	if(seconds()-stateOfCreate.bumpsWheelDrops.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(7);//drops and bumps
		CREATE_WAITFORBUFFER(1,100007)
		stateOfCreate.bumpsWheelDrops.data=2*robot.lcbump+robot.rcbump;;
		stateOfCreate.bumpsWheelDrops.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.bumpsWheelDrops.data);
}

int get_create_lbump(float lag)
{
	int b = _get_create_bumpdrop(lag);
	if(b>100000)return b;
	return((b >> 1) & 0x1);
}
int get_create_rbump(float lag)
{
	int b = _get_create_bumpdrop(lag);
	if(b>100000)return b;
	return((b >> 0) & 0x1);
}
int get_create_cwheel(float lag)
{
	int b = _get_create_bumpdrop(lag);
	if(b>100000)return b;
	return((b >> 4) & 0x1);
}
int get_create_lwheel(float lag)
{
	int b = _get_create_bumpdrop(lag);
	if(b>100000)return b;
	return((b >> 3) & 0x1);
}
int get_create_rwheel(float lag)
{
	int b = _get_create_bumpdrop(lag);
	if(b>100000)return b;
	return((b >> 2) & 0x1);
}


//updates and returns wall sensor in digital mode
int get_create_wall(float lag) {
	if(seconds()-stateOfCreate.wall.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(8);  //  wall seen or not (1 byte)
		CREATE_WAITFORBUFFER(1,100008)
		stateOfCreate.wall.data=0;//*********************Should update this so it works in simulator
		stateOfCreate.wall.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.wall.data);
}


//updates and returns cliff sensor in digital mode
int get_create_lcliff(float lag) {
	if(seconds()-stateOfCreate.lcliff.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(9);  //  wall seen or not (1 byte)
		CREATE_WAITFORBUFFER(1,100009)
		stateOfCreate.lcliff.data=(robot.cliffs[3]<20)?1:0;
		stateOfCreate.lcliff.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.lcliff.data);
}
//updates and returns cliff sensor in digital mode
int get_create_lfcliff(float lag) {
	if(seconds()-stateOfCreate.lfcliff.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(10);  //  wall seen or not (1 byte)
		CREATE_WAITFORBUFFER(1,100010)
		stateOfCreate.lfcliff.data=(robot.cliffs[2]<20)?1:0;
		stateOfCreate.lfcliff.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.lfcliff.data);
}
//updates and returns cliff sensor in digital mode
int get_create_rfcliff(float lag) {
	if(seconds()-stateOfCreate.rfcliff.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(11);  //  wall seen or not (1 byte)
		CREATE_WAITFORBUFFER(1,100011)
		stateOfCreate.rfcliff.data=(robot.cliffs[1]<20)?1:0;
		stateOfCreate.rfcliff.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.rfcliff.data);
}
//updates and returns cliff sensor in digital mode
int get_create_rcliff(float lag) {
	if(seconds()-stateOfCreate.rcliff.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(12);  //  wall seen or not (1 byte)
		CREATE_WAITFORBUFFER(1,100012)
		stateOfCreate.rcliff.data=(robot.cliffs[0]<20)?1:0;
		stateOfCreate.rcliff.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.rcliff.data);
}

//updates and returns virtual wall sensor in digital mode
int get_create_vwall(float lag) {
	if(seconds()-stateOfCreate.vWall.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(13);  //  wall seen or not (1 byte)
		CREATE_WAITFORBUFFER(1,100013)
		stateOfCreate.vWall.data=0;
		stateOfCreate.vWall.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.vWall.data);
}

//updates and returns overcurrents b4 lw, b3 rw, b2 ld2, b1, ld0, b0 ld1
int get_create_overcurrents(float lag) {
	if(seconds()-stateOfCreate.LSDandWheelOvercurrents.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(14);  //  wall seen or not (1 byte)
		CREATE_WAITFORBUFFER(1,100014)
		stateOfCreate.LSDandWheelOvercurrents.data=0;
		stateOfCreate.LSDandWheelOvercurrents.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.LSDandWheelOvercurrents.data);
}


//updates and returns infrared byte. 255 if no signal
int get_create_infrared(float lag) {
	if(seconds()-stateOfCreate.infrared.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(17);  //  infrared
		CREATE_WAITFORBUFFER(1,100017)
		stateOfCreate.infrared.data=0;
		stateOfCreate.infrared.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.infrared.data);
}


// updates the button states with the current values from the Create
int _get_create_buttons(float lag) {
	if(seconds()-stateOfCreate.buttons.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(18);//buttons
		CREATE_WAITFORBUFFER(1,100018)
		stateOfCreate.buttons.data=4*robot.adv+robot.play;
		stateOfCreate.buttons.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.buttons.data);
}

int get_create_advance_button(float lag){int b=_get_create_buttons(lag); if(b>100000) return(b); return((b >>2) & 0x1);}
int get_create_play_button(float lag){int b=_get_create_buttons(lag); if(b>100000) return(b); return(b & 0x1);}

// these functions updates the distance which has the average distance traveled of the two
// wheels (vehicle odometry) in mm.  Turning in place does not change this value.  
// Forward increments, backwards decrements
int get_create_incremental_distance(float lag) {
	if(seconds()-stateOfCreate.distance.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(19);//buttons
		CREATE_WAITFORBUFFER(1,100019)
		stateOfCreate.distance.data=(int)((robot.lenc+robot.renc)/2.0);
		robot.lenc=robot.renc=0;//zero out simulated encoders since that is what Create does whenever data is read
		stateOfCreate.distance.lastUpdate=seconds();
		CREATE_FREE;
		stateOfCreate.accumulatedDistance=stateOfCreate.accumulatedDistance+stateOfCreate.distance.data;
	}
	return(stateOfCreate.distance.data);
}

int get_create_distance(float lag) {get_create_incremental_distance(lag); return stateOfCreate.accumulatedDistance;}
void set_create_distance(int d) {stateOfCreate.distance.lastUpdate=seconds(); stateOfCreate.accumulatedDistance=d;}

// these functions updates angle which stores a normalized angle between 0 and 359 degrees
// and the global gc_total_angle which is not normalized and can be larger than 360 and less than 0.
// CCW angles are positive and CW turns decrement the angle value.
int get_create_incremental_angle(float lag) {
	if(seconds()-stateOfCreate.angle.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(20);//buttons
		CREATE_WAITFORBUFFER(1,100020)
		stateOfCreate.angle.data=(int)(360.0*robot.ctheta/twopi-stateOfCreate.oldCtheta);
		stateOfCreate.oldCtheta=360.0*robot.ctheta/twopi;		
		stateOfCreate.angle.lastUpdate=seconds();
		CREATE_FREE;
		stateOfCreate.totalAngle=stateOfCreate.totalAngle+stateOfCreate.angle.data;
		stateOfCreate.normalizedAngle=stateOfCreate.normalizedAngle+stateOfCreate.angle.data;
		if(stateOfCreate.normalizedAngle>359)stateOfCreate.normalizedAngle=stateOfCreate.normalizedAngle-360;
		if(stateOfCreate.normalizedAngle<0)stateOfCreate.normalizedAngle=stateOfCreate.normalizedAngle+360;
		
	}
	return(stateOfCreate.angle.data);
}

int get_create_total_angle(float lag) {get_create_incremental_angle(lag); return stateOfCreate.totalAngle;}
int get_create_normalized_angle(float lag) {get_create_incremental_angle(lag); return stateOfCreate.normalizedAngle;}
void set_create_total_angle(int a) {stateOfCreate.angle.lastUpdate=seconds(); stateOfCreate.totalAngle=a;}
void set_create_normalized_angle(int a) {
	stateOfCreate.angle.lastUpdate=seconds(); 
	stateOfCreate.normalizedAngle=a;
	if(a>359)set_create_normalized_angle(a-360);
	if(a<0)set_create_normalized_angle(a+360);
}

int get_create_battery_charging_state(float lag)
{
	if(seconds()-stateOfCreate.chargingState.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(21);
		CREATE_WAITFORBUFFER(1,100021)
		stateOfCreate.chargingState.data=0;
		stateOfCreate.chargingState.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.chargingState.data);
}

int get_create_battery_voltage(float lag)
{
	if(seconds()-stateOfCreate.voltage.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(22);
		CREATE_WAITFORBUFFER(2,100022)
		stateOfCreate.voltage.data=15000;
		stateOfCreate.voltage.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.voltage.data);
}

int get_create_battery_current(float lag)
{
	if(seconds()-stateOfCreate.current.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(23);
		CREATE_WAITFORBUFFER(2,100023)
		stateOfCreate.current.data=stateOfCreate.lspeed+stateOfCreate.rspeed;
		stateOfCreate.current.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.current.data);
}


int get_create_battery_temp(float lag)
{
	if(seconds()-stateOfCreate.batteryTemp.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(24);
		CREATE_WAITFORBUFFER(1,100024)
		stateOfCreate.batteryTemp.data=35;
		stateOfCreate.batteryTemp.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.batteryTemp.data);
}

int get_create_battery_charge(float lag)
{
	if(seconds()-stateOfCreate.batteryCharge.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(25);
		CREATE_WAITFORBUFFER(2,100025)
		stateOfCreate.batteryCharge.data=1000;
		stateOfCreate.batteryCharge.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.batteryCharge.data);
}

int get_create_battery_capacity(float lag)
{
	if(seconds()-stateOfCreate.batteryCapacity.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(26);
		CREATE_WAITFORBUFFER(2,100026)
		stateOfCreate.batteryCapacity.data=2500;
		stateOfCreate.batteryCapacity.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.batteryCapacity.data);
}


//updates and returns wall sensor in analog mode
int get_create_wall_amt(float lag) {
	if(seconds()-stateOfCreate.wallSignal.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(27);  //  wall seen or not (1 byte)
		CREATE_WAITFORBUFFER(2,100027)
		stateOfCreate.wallSignal.data=0;//****************update to make this work
		stateOfCreate.wallSignal.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.wallSignal.data);
}


//updates and returns cliff sensor in analog mode
int get_create_lcliff_amt(float lag) {
	if(seconds()-stateOfCreate.lcliffSignal.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(28);  //  wall seen or not (1 byte)
		CREATE_WAITFORBUFFER(2,100028)
		stateOfCreate.lcliffSignal.data=robot.cliffs[3];
		stateOfCreate.lcliffSignal.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.lcliffSignal.data);
}


//updates and returns cliff sensor in analog mode
int get_create_lfcliff_amt(float lag) {
	if(seconds()-stateOfCreate.lfcliffSignal.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(29);  // lf cliff
		CREATE_WAITFORBUFFER(2,100029)
		stateOfCreate.lfcliffSignal.data=robot.cliffs[2];
		stateOfCreate.lfcliffSignal.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.lfcliffSignal.data);
}


//updates and returns cliff sensor in analog mode
int get_create_rfcliff_amt(float lag) {
	if(seconds()-stateOfCreate.rfcliffSignal.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(30);  //  rf
		CREATE_WAITFORBUFFER(2,100030)
		stateOfCreate.rfcliffSignal.data=robot.cliffs[1];
		stateOfCreate.rfcliffSignal.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.rfcliffSignal.data);
}


//updates and returns cliff sensor in analog mode
int get_create_rcliff_amt(float lag) {
	if(seconds()-stateOfCreate.rcliffSignal.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(31);  //  right cliff
		CREATE_WAITFORBUFFER(2,100031)
		stateOfCreate.rcliffSignal.data=robot.cliffs[0];
		stateOfCreate.rcliffSignal.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.rcliffSignal.data);
}


//updates and returns cargo bay digitals b4 is device detect; b3 is pin 6; b2 pin 18; b0 pin 17
int get_create_cargo_bay_digitals(float lag) {
	if(seconds()-stateOfCreate.cargoBayDI.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(32);  //  
		CREATE_WAITFORBUFFER(1,100032)
		stateOfCreate.cargoBayDI.data=0;
		stateOfCreate.cargoBayDI.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.cargoBayDI.data);
}

//updates and returns CB analog sensor pin 4
int get_create_cargo_bay_analog(float lag) {
	if(seconds()-stateOfCreate.cargoBayAI.lastUpdate > lag){
		CREATE_BUSY;
		s_write_byte(142);
		s_write_byte(33);  //  CB analog 0 -1023
		CREATE_WAITFORBUFFER(2,100033)
		stateOfCreate.cargoBayAI.data=127;
		stateOfCreate.cargoBayAI.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.cargoBayAI.data);
}


int get_create_battery_charging_source(float lag)
{
	if(seconds()-stateOfCreate.chargingSource.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(34);
		CREATE_WAITFORBUFFER(1,100034)
		stateOfCreate.chargingSource.data=0;//1 if charger 2 if home base 0 if no charge source
		stateOfCreate.chargingSource.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.chargingSource.data);
}



//packet 36; currently selected song number
int get_create_song_number(float lag)
{
	if(seconds()-stateOfCreate.songNumber.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(36);
		CREATE_WAITFORBUFFER(1,100036)
		stateOfCreate.songNumber.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.songNumber.data);
}

//packet 37;1=song is playing, 0 song not playing
int get_create_song_playing(float lag)
{
	if(seconds()-stateOfCreate.songPlaying.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(37);
		CREATE_WAITFORBUFFER(1,100037)
		stateOfCreate.songPlaying.data=((seconds()-stateOfCreate.songStart) < stateOfCreate.songDuration)?1:0;
		stateOfCreate.songPlaying.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.songPlaying.data);
}

//packet-38, number of streamed packets, 0-43, is returned
int get_create_number_of_stream_packets(float lag)
{
	if(seconds()-stateOfCreate.numStreamPackets.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(38);
		CREATE_WAITFORBUFFER(1,100038)
		stateOfCreate.numStreamPackets.data=0;//song number 0-43
		stateOfCreate.numStreamPackets.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.numStreamPackets.data);
}


//packet 39; -500 to 500
int get_create_requested_velocity(float lag)
{
	if(seconds()-stateOfCreate.requestedVelocity.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(39);
		CREATE_WAITFORBUFFER(2,100039)
		stateOfCreate.requestedVelocity.data=stateOfCreate.speed;
		stateOfCreate.requestedVelocity.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.requestedVelocity.data);
}

//packet 40; -32768 to 32767
int get_create_requested_radius(float lag)
{
	if(seconds()-stateOfCreate.requestedRadius.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(40);
		CREATE_WAITFORBUFFER(2,100040)
		stateOfCreate.requestedRadius.data=stateOfCreate.radius;
		stateOfCreate.requestedRadius.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.requestedRadius.data);
}

//packet 41; -500 to 500
int get_create_requested_right_velocity(float lag)
{
	if(seconds()-stateOfCreate.requestedRVelocity.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(41);
		CREATE_WAITFORBUFFER(2,100041)
		stateOfCreate.requestedRVelocity.data=stateOfCreate.rspeed;
		stateOfCreate.requestedRVelocity.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.requestedRVelocity.data);
}

//packet 42; -500 to 500
int get_create_requested_left_velocity(float lag)
{
	if(seconds()-stateOfCreate.requestedLVelocity.lastUpdate > lag){
		CREATE_BUSY;
		clear_serial_buffer();
		s_write_byte(142);
		s_write_byte(42);
		CREATE_WAITFORBUFFER(2,100042)
		stateOfCreate.requestedLVelocity.data=stateOfCreate.lspeed;
		stateOfCreate.requestedLVelocity.lastUpdate=seconds();
		CREATE_FREE;
	}
	return(stateOfCreate.requestedLVelocity.data);
}



////////////////////CREATE MOVEMENT FUNCTIONS///////////////////////////////////////////////

// This command drives the robot along a curve with radius (in mm) radius; and at a speed (mm/sec) of speed
// a radius of 32767 will drive the robot straight
// a radius of 1 will spin the robot CCW
// a radius of -1 will spin the robot CW
// Negative radii will be right turns, positive radii left turns
void create_drive (int speed, int radius)
{
	float circtime;
	if(stateOfCreate.drive==1 && stateOfCreate.radius==radius && stateOfCreate.speed==speed)return;//if create is already doing this, return
	CREATE_BUSY;
	s_write_byte(137);
	s_write_byte(get_high_byte(speed));
	s_write_byte(get_low_byte(speed));
	s_write_byte(get_high_byte(radius));
	s_write_byte(get_low_byte(radius));
	stateOfCreate.driveDirect=0;
	stateOfCreate.drive=1;
	stateOfCreate.radius=radius;
	stateOfCreate.speed=speed;
	stateOfCreate.lspeed=0;
	stateOfCreate.rspeed=0;
	switch (radius){
		case 1:{
			robot.lcspeed=-speed;
			robot.rcspeed=speed;
			break;
		}
		case -1:{
			robot.lcspeed=speed;
			robot.rcspeed=-speed;
			break;
		}
		case 32767:{
			robot.lcspeed=speed;
			robot.rcspeed=speed;
			break;
		}
		default:{
			circtime=(PI2*(float)radius)/(float)speed;
			robot.rcspeed=(int)((PI2*(iROBOWHEELSEP/.2+(float)radius))/circtime);
			robot.lcspeed=(int)((PI2*(-iROBOWHEELSEP/.2+(float)radius))/circtime);
		}
	}
	CREATE_FREE;
}

// special version of command above drives robot at speed 0, stopping it. 
void create_stop()
{
	CREATE_BUSY;
	s_write_byte(137);
	s_write_byte(0);
	s_write_byte(0);
	s_write_byte(0);  
	s_write_byte(0);    
	stateOfCreate.driveDirect=0;
	stateOfCreate.drive=0;
	stateOfCreate.radius=0;
	stateOfCreate.speed=0;
	stateOfCreate.lspeed=0;
	stateOfCreate.rspeed=0;
			robot.lcspeed=0;
			robot.rcspeed=0;
	CREATE_FREE;
}

// special version of command above drives robot at speed speed.  Negative speed will drive robot backwards
void create_drive_straight (int speed)
{
	if(stateOfCreate.drive==1 && stateOfCreate.radius==32767 && stateOfCreate.speed==speed)return;//if create is already doing this, return
	CREATE_BUSY;
	s_write_byte(137);
	s_write_byte(get_high_byte(speed));
	s_write_byte(get_low_byte(speed));
	s_write_byte(get_high_byte(32767));
	s_write_byte(get_low_byte(32767));
	stateOfCreate.driveDirect=0;
	stateOfCreate.drive=1;
	stateOfCreate.radius=32767;
	stateOfCreate.speed=speed;
	stateOfCreate.lspeed=speed;
	stateOfCreate.rspeed=speed;
			robot.lcspeed=speed;
			robot.rcspeed=speed;
	CREATE_FREE;
}

// special version of command spins robot CW with the wheels turning at speed speed
void create_spin_CW (int speed)
{
	if(stateOfCreate.drive==1 && stateOfCreate.radius==-1 && stateOfCreate.speed==speed)return;//if create is already doing this, return
	CREATE_BUSY;
	s_write_byte(137);
	s_write_byte(get_high_byte(speed));
	s_write_byte(get_low_byte(speed));
	s_write_byte(get_high_byte(-1));
	s_write_byte(get_low_byte(-1));
	stateOfCreate.driveDirect=0;
	stateOfCreate.drive=1;
	stateOfCreate.radius=-1;
	stateOfCreate.speed=speed;
	stateOfCreate.lspeed=speed;
	stateOfCreate.rspeed=-speed;
			robot.lcspeed=speed;
			robot.rcspeed=-speed;
	CREATE_FREE;
}

// special version of command spins robot CCW with the wheels turning at speed speed
void create_spin_CCW (int speed)
{
	if(stateOfCreate.drive==1 && stateOfCreate.radius==1 && stateOfCreate.speed==speed)return;//if create is already doing this, return
	CREATE_BUSY;
	s_write_byte(137);
	s_write_byte(get_high_byte(speed));
	s_write_byte(get_low_byte(speed));
	s_write_byte(get_high_byte(1));
	s_write_byte(get_low_byte(1));
	stateOfCreate.driveDirect=0;
	stateOfCreate.drive=1;
	stateOfCreate.radius=1;
	stateOfCreate.speed=speed;
	stateOfCreate.lspeed=-speed;
	stateOfCreate.rspeed=speed;
				robot.lcspeed=-speed;
			robot.rcspeed=speed;
	CREATE_FREE;
}

// this function drives the right wheel at r_speed and the left wheel at l_speed
// speeds for all of these functions are +/-500mm/sec.
void create_drive_direct(int r_speed, int l_speed)
{
	if(stateOfCreate.driveDirect==1 && stateOfCreate.lspeed==l_speed && stateOfCreate.rspeed==r_speed)return;//if create is already doing this, return
	CREATE_BUSY;
	s_write_byte(145);
	s_write_byte(get_high_byte(r_speed));
	s_write_byte(get_low_byte(r_speed));
	s_write_byte(get_high_byte(l_speed));
	s_write_byte(get_low_byte(l_speed));
	CREATE_FREE;
	stateOfCreate.driveDirect=1;
	stateOfCreate.drive=0;
	stateOfCreate.radius=0;
	stateOfCreate.speed=(l_speed+r_speed)/2;
	stateOfCreate.lspeed=l_speed;
	stateOfCreate.rspeed=r_speed;
				robot.lcspeed=l_speed;
			robot.rcspeed=r_speed;

}    


//This function blocks and does a pretty accurate spin.  Note that the function will
//not return until the spin is complete
//CAUTION, requesting the robot to spin more than about 3600 degrees may never terminate
//the 9/07 firmware update is required for this works. Returns -1 if error
int create_spin_block(int speed, int angle)
{
	int i,b;
	long lenc=0L, renc=0L,slenc,srenc,flenc,frenc;
	float offset, ticsPerMM=7.8324283, rad=129.0, pi=3.1415926;
	
	slenc=robot.lenc;
	srenc=robot.renc;
	offset=(float)angle*pi*rad/180.0;
	if(angle>0){create_drive_direct(speed,-speed);}
	else {offset=-offset; create_drive_direct(-speed,speed);}
	flenc=slenc+(long)offset;
	frenc=srenc+(long)offset;
	while(lenc < flenc && renc < frenc){
		lenc=robot.lenc;
		renc=robot.renc;
	}
	create_stop();
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
		if(!(stateOfCreate.leds[0] & 8)) stateOfCreate.leds[0]=stateOfCreate.leds[0]+ 8;
	}
	else{
		if(stateOfCreate.leds[0] & 8) stateOfCreate.leds[0]=stateOfCreate.leds[0]- 8;
	}
	CREATE_BUSY;
	s_write_byte(139);
	s_write_byte(stateOfCreate.leds[0]);            
	s_write_byte(stateOfCreate.leds[1]);            
	s_write_byte(stateOfCreate.leds[2]);  
		robot.led_advance=on;
	CREATE_FREE;
}

// turns on and off the play LED (0 is off, 1 is on)
void create_play_led(int on) 
{
	if(on){
		if(!(stateOfCreate.leds[0] & 2)) stateOfCreate.leds[0]=stateOfCreate.leds[0]+ 2;
	}
	else{
		if(stateOfCreate.leds[0] & 2) stateOfCreate.leds[0]=stateOfCreate.leds[0]- 2;
	}
	CREATE_BUSY;
	s_write_byte(139);
	s_write_byte(stateOfCreate.leds[0]);            
	s_write_byte(stateOfCreate.leds[1]);            
	s_write_byte(stateOfCreate.leds[2]);  
		robot.led_play=on;
	CREATE_FREE;
}


//sets create power led. Color =0 is green and color = 255 is red -- with intermediate colors
//brightness of 0 is off and 255 is fully bright
void create_power_led(int color, int brightness) 
{
	float red = (color > 127) ? 1.0 : (float)color/127.0;
	float green = ((255 - color) > 127) ? 1.0 : (255.0-(float)color)/127.0;
	robot.led_power_r=(int)((float)brightness*red);
	robot.led_power_g=(int)((float)brightness*green);
	stateOfCreate.leds[1]=color;
	stateOfCreate.leds[2]=brightness;
	CREATE_BUSY;
	s_write_byte(139);
	s_write_byte(stateOfCreate.leds[0]);            
	s_write_byte(stateOfCreate.leds[1]);            
	s_write_byte(stateOfCreate.leds[2]);      
	CREATE_FREE;
}


/////////// The functions below are for writing to the 25 pin connector

//This function sets the three digital out put pins 20,7,19 where 20 is the high bit and 19 is the low. bits should have a value 0 to 7.
void create_digital_output(int bits)
{
	CREATE_BUSY;
	s_write_byte(147);
	s_write_byte(bits);            
	CREATE_FREE;
}

//Sets the PWM signal for the three low side drivers (128 = 100%).  pwm2 is pin 24, pwm1 pin 23 and pwm0 pin 22
void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
	CREATE_BUSY;
	s_write_byte(144);
	s_write_byte(pwm2);            
	s_write_byte(pwm1);            
	s_write_byte(pwm0);            
	CREATE_FREE;
}

//Turns on and off the signal for the three low side drivers (128 = 100%).  pwm2 is pin 24, pwm1 pin 23 and pwm0 pin 22
//A 0 or 1 should be given for each of the drivers to turn them off or on.
void create_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
	CREATE_BUSY;
	s_write_byte(138);
	s_write_byte(4*pwm2+2*pwm1+pwm0);            
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
	int i, numnotes, duration=0;
	numnotes=gc_song_array[num][0];
	if(num >= 0 && num <=15 && numnotes > 0 && numnotes <= 16){
		CREATE_BUSY;
		s_write_byte(140);
		s_write_byte(num);
		s_write_byte(numnotes);
		for(i=1; i< 2*numnotes+1; i++)
		s_write_byte(gc_song_array[num][i]);
		CREATE_FREE;
		stateOfCreate.songNumber.data=num;
		for(i=1;i<numnotes+1;i++)duration=duration+gc_song_array[num][2*i];
		stateOfCreate.songDuration=duration/64.0;
	}
	else printf("illegal song #%d is %d notes long being written to memory\n", num, gc_song_array[num][0]);
}

// see the roomba SCI manual for note codes.
void create_play_song(int num)
{
	int i;
	if(num >= 0 && num <=15 ){
		CREATE_BUSY;
		s_write_byte(141);
		s_write_byte(num);
		CREATE_FREE;
		stateOfCreate.songStart=seconds();
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
	s_write_byte(151);
	s_write_byte(byte); 
	CREATE_FREE;
}


////////////////////CREATE MOVEMENT USING SCRIPTS//////////////////

// WARNING: During script operation, serial commands are ignored, 
// potentially hanging the control computer in cases such as
// waiting for a Create sensor update.
// Each movement command signals on pin 20 when it is done
// If this signal is not read, then a timing estimate will 
// need to be used instead.
// The most recent script may be repeated by s_write_byte(153)

// script to move dist mm at speed mm/sec
void create_script_move(int dist, int speed) { 
	CREATE_BUSY;
	s_write_byte(152); // start script
	s_write_byte(17);  // script length
	s_write_byte(147); // on pin 20
	s_write_byte(0);   //   output 0
	s_write_byte(137); // move 
	s_write_byte(get_high_byte(speed));
	s_write_byte(get_low_byte(speed));
	s_write_byte(128); // no angle (go straight)
	s_write_byte(0);
	s_write_byte(156); // wait for distance done
	s_write_byte(get_high_byte(dist));
	s_write_byte(get_low_byte(dist));
	s_write_byte(137); // stop move 
	s_write_byte(0);   // no speed
	s_write_byte(0);
	s_write_byte(0);   // no angle
	s_write_byte(0);   
	s_write_byte(147); // on pin 20
	s_write_byte(0);   //   output 1
	// end of script
	s_write_byte(153); // run script
	CREATE_FREE;
}

// script to move with individual motor control
// dist mm with r_speed mm/sec on right wheel and l_speed on left
void create_script_move_direct(int dist, int r_speed, int l_speed) { 
	CREATE_BUSY;
	s_write_byte(152); // start script
	s_write_byte(17);  // script length
	s_write_byte(147); // on pin 20
	s_write_byte(0);   //   output 0
	s_write_byte(145); // move 
	s_write_byte(get_high_byte(r_speed));
	s_write_byte(get_low_byte(r_speed));
	s_write_byte(get_high_byte(l_speed));
	s_write_byte(get_low_byte(l_speed));
	s_write_byte(156); // wait for distance done
	s_write_byte(get_high_byte(dist));
	s_write_byte(get_low_byte(dist));
	s_write_byte(137); // stop move 
	s_write_byte(0);   // no speed
	s_write_byte(0);
	s_write_byte(0);   // no angle
	s_write_byte(0);   
	s_write_byte(147); // on pin 20
	s_write_byte(0);   //   output 1
	// end of script
	s_write_byte(153); // run script
	CREATE_FREE;
}


// script to rotate in place through deg degrees
// deg > 0 turn CCW; deg < 0 turn CW
void create_script_turn(int deg, int speed) { // degrees, vel in mm/sec
	CREATE_BUSY;
	s_write_byte(152); // start script
	s_write_byte(17);  // script length
	s_write_byte(147); // on pin 20
	s_write_byte(0);   //   output 0
	s_write_byte(137); // move 
	s_write_byte(get_high_byte(speed));
	s_write_byte(get_low_byte(speed));
	if (deg > 0){ // CCW case 
		s_write_byte(0); 
	s_write_byte(1); }
	else { // CW case
		s_write_byte(255);
	s_write_byte(255); }
	s_write_byte(157); // wait for angle done
	s_write_byte(get_high_byte(deg));
	s_write_byte(get_low_byte(deg));
	s_write_byte(137); // stop move 
	s_write_byte(0);   // no speed
	s_write_byte(0);
	s_write_byte(0);   // no angle
	s_write_byte(0);   
	s_write_byte(147); // on pin 20
	s_write_byte(0);   //   output 1
	// end of script
	s_write_byte(153); // run script
	CREATE_FREE;
}

// script to trace an arc of radius rad until deg is reach
// NOTE: if the turn is not in the direction of deg, the arc won't end
void create_script_arc(int rad, int deg, int speed) { // rad in mm, degrees, vel in mm/sec
	CREATE_BUSY;
	s_write_byte(152); // start script
	s_write_byte(17);  // script length
	s_write_byte(147); // on pin 20
	s_write_byte(0);   //   output 0
	s_write_byte(137); // move 
	s_write_byte(get_high_byte(speed));
	s_write_byte(get_low_byte(speed));
	s_write_byte(get_high_byte(rad)); 
	s_write_byte(get_high_byte(rad)); 
	s_write_byte(157); // wait for angle done
	s_write_byte(get_high_byte(deg));
	s_write_byte(get_low_byte(deg));
	s_write_byte(137); // stop move 
	s_write_byte(0);   // no speed
	s_write_byte(0);
	s_write_byte(0);   // no angle
	s_write_byte(0);   
	s_write_byte(147); // on pin 20
	s_write_byte(0);   //   output 1
	// end of script
	s_write_byte(153); // run script
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
	s_write_byte(_create_script[i]);
	CREATE_FREE;
}

void create_script_run() {
	s_write_byte(153); // run script
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
	msleep(1);
}

void s_write_byte(unsigned char byte)
{
	msleep(1);
}

//Returns 0 if there is nothing to be read
unsigned char s_read_byte() 
{
	msleep(1);
	return stateOfCreate.createConnected;
}



/* END CreateLib.c */

#endif
