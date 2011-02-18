/* iROBOsim graphics simulator written by David Miller for IC 
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

//CREATE_BUSY keeps any  create commands in other processes from interfering with one another.
#define CREATE_BUSY while(g_create_busy)msleep(1L); g_create_busy = 1;
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
int gc_mode=0;

// additional globals needed only for sim
float _last_angle=0.0, _curr_angle=0.0;

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


// Create must be turned on and connected.  This routine establishes serial link
// Note that this routine should be called once, and must have been called before 
// any of the other create functions will work.  Puts Create in "safe" mode (see documentation).
// Once this function is called, you cannot talk to the XBC over the USB connection
// until the create_disconnect function has been called or the GBA on the XBC has been power cycled.
// If Create is connected, power light goes yellow and play led turns on
int create_connect()
{
    gc_mode=2;
 	create_start();
	create_safe();
	create_advance_led(1); // turn on advance LED
   return(0);//successful connection
}


// Same as above but for serial XBCs (versions 1 & 2)
int create_connect_s() 
{
 	create_start();
	create_safe();
	create_advance_led(1); // turn on advance LED
    gc_mode=2;
    return(0);//successful connection
}

// returns the serial connections on XBC to normal communications over the USB port.
// Turns of play LED and returns power light to green
void create_disconnect() {
	create_play_led(0);
	create_power_led(0,255);
}

// Same as above but for serial XBCs
void create_disconnect_s() {
	create_play_led(0);
	create_power_led(0,255);
}


// if serial communication times out (ie., is lost), attempts to restart it
void create_reconnect()
{
		create_disconnect();    // reset environment
		create_connect();
}

// Create open interface mode is off=0 (Create powered), passive=1, safe=2, full=3
// From "safe" mode, wheel drop or cliff detect halts motors and puts Create in passive mode
int create_mode() {
    return(0);
}


// See the create open interface document for more information on these functions
void create_start() {gc_mode=1;}
void create_passive() {gc_mode=1;}//same as start 
void create_safe() {gc_mode=2; create_power_led(140,255);}
void create_full() {gc_mode=3; create_power_led(220,255);}

void create_spot() {}

void create_cover() {}

void create_demo(int d) {}

void create_cover_dock() {}

//////////////Create Sensor Routines///////////////
// These work for any Create mode except off

//globals are updated by the functions below to give access to many key Create sensors
int gc_lbump, gc_rbump, gc_ldrop, gc_rdrop, gc_fdrop, gc_rcliff, gc_rfcliff, gc_lcliff, gc_lfcliff;
int gc_rcliff_amt, gc_rfcliff_amt, gc_lcliff_amt, gc_lfcliff_amt;
int gc_distance, gc_angle, gc_total_angle, gc_advance_button, gc_play_button;
int gc_wall, gc_wall_amt, gc_wall_hb, gc_IR;
int gc_vel, gc_radius, gc_rvel, gc_lvel;
int gc_overcurrents;
int gc_charge_state, gc_batt_voltage, gc_current_flow, gc_batt_temp, gc_batt_charge, gc_batt_capacity;
int gc_digital_in, gc_analog_in, gc_charge_source;


//this function will update the values for all of the sensor globals above
//serial connection failure codes are
// -1:create_mode, -2:create_wall, -3:create_buttons, -4:create_bumpdrop, -5:create_cliffs,
// -6:create_angle, -7:create_distance, -8:create_velocity, -9:create_read_IR 
int create_sensor_update()
{
    float diff_angle;
    //defer();
    create_angle();
     //create_bumpdrop();
    gc_lbump=robot.lcbump;
    gc_rbump=robot.rcbump;
    //create_distance();
    gc_distance=gc_distance+(int)((robot.lenc+robot.renc)/2L);
    robot.lenc=0;
    robot.renc=0;
	gc_play_button=robot.play;
	gc_advance_button = robot.adv;
    return(0);
}

// updates the virtual wall and home base sensors (seen or not seen) 
// and virtual wall signal strength
int create_wall() {
    return(0);
}

// updates the play and advance buttons
int create_buttons() {
	gc_advance_button=robot.adv;
	gc_play_button=robot.play;
    return(0);
}

// updates the bumper and wheel drop globals with the current values from the Create
int create_bumpdrop() {
    //defer();
    gc_lbump=robot.lcbump;
    gc_rbump=robot.rcbump;
    return(0);
}

// updates the cliff sensor globals with the current sensor values from the Create
int create_cliffs() {
    return(0);
}

// this function updates gc_angle which stores a normalized angle between 0 and 359 degrees
// and the global gc_total_angle which is not normalized and can be larger than 360 and less than 0.
// CCW angles are positive and CW turns decrement the angle value.

int create_angle() {
    float diff_angle;
	static float fgc_total_angle=0.0, fgc_angle=0.0;
    //defer();
	if(gc_angle==0)fgc_angle=0.0;
	if(gc_total_angle==0)fgc_total_angle=0.0;
    _curr_angle=robot.ctheta;
    diff_angle=_curr_angle-_last_angle;//reversed because sim works in other direction
    fgc_total_angle =  fgc_total_angle+(180.0*diff_angle/PI);
    fgc_angle=(fgc_angle+(180.0*diff_angle/PI));
    gc_total_angle =  (int)fgc_total_angle;
    gc_angle=(int)fgc_angle % 360;
    if(gc_angle < 0) gc_angle = gc_angle + 360;
    _last_angle=_curr_angle;
    return(0);
}

// this function updates the global gc_distance which has the average distance traveled of the two
// wheels (vehicle odometry) in mm.  Turning in place does not change this value.  
// Forward increments, backwards decrements
int create_distance() {
    //defer();
    gc_distance=gc_distance+(int)((robot.lenc+robot.renc)/2L);
	robot.lenc=robot.renc=0;
    return(0);
}

// this function updates the global gc_vel, gc_radius, gc_rvel, and gc_lvel variables
// under create_drive, returned values for gc_rvel and gc_lvel are 0
// under create_drive_direct, returned values for gc_vel and gc_radius are 0
int create_velocity() {
    gc_vel=(robot.lcspeed+robot.rcspeed)/2;
    gc_rvel=robot.rcspeed;
    gc_lvel=robot.lcspeed;
    return(0);
}    

//sets gc_overcurrents to have a state where 16s'bit is left wheel; 8's is right wheel, 4's id LD2, 2's is LD0 and 1's is LD1
int create_motor_overcurrents(){
    return(0);
}

//updates values for charging and battery state
int create_battery_charge()
{
    return(0);
}

//reads the values off of the cargo bay inputs, and also the charge and
int create_cargo_bay_inputs()
{
    return(0);
}


////////////////////CREATE MOVEMENT FUNCTIONS///////////////////////////////////////////////

// This command drives the robot along a curve with radius (in mm) radius; and at a speed (mm/sec) of speed
// a radius of 32767 will drive the robot straight
// a radius of 1 will spin the robot CCW
// a radius of -1 will spin the robot CW
// Negative radii will be right turns, positive radii left turns
// speed range is -500 to +500 mm/sec; radius is -2000 to +2000 mm
// These functions do not work if Create is in passive mode
void create_drive (int speed, int radius)
{
    float circtime;
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
    //defer();
}

// special version of command above drives robot at speed 0, stopping it. 
void create_stop()
{
    robot.lcspeed=0;
    robot.rcspeed=0;
    //defer();
}

// special version of command above drives robot at speed speed.  Negative speed will drive robot backwards
void create_drive_straight (int speed)
{
    robot.lcspeed=speed;
    robot.rcspeed=speed;
    //defer();
}

// special version of command spins robot CW with the wheels turning at speed speed
void create_spin_CCW (int speed)  // increments gc_angle
{
    robot.lcspeed=-speed;
    robot.rcspeed=speed;
    //defer();
}

// special version of command spins robot CCW with the wheels turning at speed speed
void create_spin_CW (int speed)  // decrements gc_angle
{
    robot.lcspeed=speed;
    robot.rcspeed=-speed;
    //defer();
}

// this function drives the right wheel at r_speed and the left wheel at l_speed
// speeds for all of these functions are +/-500mm/sec.
void create_drive_direct(int r_speed, int l_speed)
{
    robot.lcspeed=l_speed;
    robot.rcspeed=r_speed;
    //defer();
}    

//This function blocks and does a pretty accurate spin.  Note that the function will
//not return until the spin is complete
//CAUTION, requesting the robot to spin more than about 3600 degrees may never terminate
//the 9/07 firmware update is required for this works.
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


/////////////////////////CREATE LEDs///////////////////

int gc_leds[3]={0,0,0};

// turns on and off the advance (>>) LED (0 is off, 1 is on)
void create_advance_led(int on) 
{
	robot.led_advance=on;
}

// turns on and off the play LED (0 is off, 1 is on)
void create_play_led(int on) 
{
	robot.led_play=on;
}


//sets create power led. Color =0 is green and color = 255 is red -- with intermediate colors
//brightness of 0 is off and 255 is fully bright
void create_power_led(int color, int brightness) 
{
	float red = (color > 127) ? 1.0 : (float)color/127.0;
	float green = ((255 - color) > 127) ? 1.0 : (255.0-(float)color)/127.0;
	robot.led_power_r=(int)((float)brightness*red);
	robot.led_power_g=(int)((float)brightness*green);
}

/////////// The functions below are for writing to the 25 pin connector

//This function sets the three digital out put pins 20,7,19 where 20 is the high bit and 19 is the low. bits should have a value 0 to 7.
void create_digital_output(int bits)
{
}

//Sets the PWM signal for the three low side drivers (128 = 100%).  pwm2 is pin 24, pwm1 pin 23 and pwm0 pin 22
void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
}

//Turns on and off the signal for the three low side drivers (128 = 100%).  pwm2 is pin 24, pwm1 pin 23 and pwm0 pin 22
//A 0 or 1 should be given for each of the drivers to turn them off or on.
void create_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
}


/////////// The functions below are communication and byte packing utitilities
int get_high_byte(int v)
{
    return (v >>8);
}

int get_low_byte(int v)
{
    return (v & 255);
}

void clear_serial_buffer()
{
}

void s_write_byte(int byte)
{
}

//Returns 0 if there is nothing to be read
int s_read_byte() 
{
}

////////////////////CREATE IR FUNCTIONS///////////////////////////////////////////

// The send_IR command assumes an IR transmitter is connected per the Create Open 
// interface spec - uses the switched +5V source (pin 8 of cargo bay adapter) 
// and low side driver 1 (pin 23).  An IR data byte is sent in the form expected
// by the Create IR receiver.

//Commands IR transmitter to send a byte of data
//129-143 are sent by the remote control, 240-254 by the base station
void create_send_IR(int byte) {  
}

// gc_IR=255 indicates no IR byte is being received
int create_read_IR() {  
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
    int sdist, ddist;
    create_drive_straight(speed);
    create_distance();
    sdist=gc_distance;
    ddist=sdist+dist;
    if(dist>0)
      while(gc_distance<ddist)create_distance();
    else
      while(gc_distance>ddist)create_distance();
    create_stop();
}

// script to move with individual motor control
// dist mm with r_speed mm/sec on right wheel and l_speed on left
void create_script_move_direct(int dist, int r_speed, int l_speed) { 
    int sdist, ddist;
    create_drive_direct(r_speed, l_speed);
    create_distance();
    sdist=gc_distance;
    ddist=sdist+dist;
    if(dist>0)
      while(gc_distance<ddist)create_distance();
    else
      while(gc_distance>ddist)create_distance();
    create_stop();
}


// script to rotate in place through deg degrees
// deg > 0 turn CCW; deg < 0 turn CW
void create_script_turn(int deg, int speed) { // degrees, vel in mm/sec
    int sangle, dangle;
    create_angle();
    sangle=gc_total_angle;
    dangle=sangle+deg;
    if(deg>0){
        create_spin_CCW(speed);
        while(gc_total_angle<dangle)create_angle();
    }
    else{
        create_spin_CW(speed);
        while(gc_total_angle>dangle)create_angle();
    }
    create_stop();
}


// script to trace an arc of radius rad until deg is reach
// NOTE: if the turn is not in the direction of deg, the arc won't end
void create_script_arc(int rad, int deg, int speed) { // rad in mm, degrees, vel in mm/sec
    int sangle, dangle;
    create_angle();
    sangle=gc_total_angle;
    dangle=sangle+deg;
    create_drive(rad,speed);
    if(deg>0){
        while(gc_total_angle<dangle)create_angle();
    }
    else{
        while(gc_total_angle>dangle)create_angle();
    }
    create_stop();
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
}
int create_do_move(int dist, int speed) {
}
int create_do_move_direct(int dist, int r_speed, int l_speed) {  // individual motor control move
}
int create_do_arc(int rad, int deg, int speed) {
}

int create_do_turn(int deg, int speed) {  // speed > 0
}

void create_script_end() {
}

void create_script_run() {
}
