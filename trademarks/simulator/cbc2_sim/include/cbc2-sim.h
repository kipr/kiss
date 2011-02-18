/* Simulator prototypes anf globals for user accesible function */

#ifndef KISSSIMH
#define KISSSIMH


#ifndef NOGRAPHICSSIMULATOR
extern int __ks_flg;
#define main() fake(); int ___main(); void __main_function(){___main(); __ks_flg=0;} int main() {int p; kissSim_init(); sleep(1.0); p=start_process(__main_function); while(__ks_flg) sleep(.05); kissSimPause(); sleep(.25); kill_process(p);} int ___main()
#define printf ks_printf
#define cbc_printf ks_cbc_printf
#define cbc_display_clear ks_cbc_display_clear
#endif

#define SIMPLEWORLD 0
#define BB08WORLD 1
#define EMPTYWORLD 2
#define RULERWORLD 3
#define BIGEMPTYWORLD 4
#define BIGARENA 5
#define BIGARENA2 6
#define BB09WORLD 7

void ks_cbc_printf(int col, int row, char *t, ...); 
void ks_printf(char *t, ...);
void ks_cbc_display_clear();
void kissSimPause();
void kissSimEnablePause();
void kissSim_init();
void tone(int frequency, int duration); /* makes a sound at frequency for duration ms */
void beep(); /* make a beep */
int digital(int port); /* returns a 1 or 0 reflecting the state of port (0 to 7) */
int set_digital_output_value(int port, int value); /*sets port (0 to 7)to value (0 or 1) */
void set_pid_gains(int motor, int p, int i, int d, int pd, int id, int dd); /* sets the PID gains for a specific motor */
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
void wait_for_light(int light_port_);
void run_for(float howLong, void (*funky));
void shut_down_in(float delay);
int init_camera(); /*initialize the camera before any other camera functions*/
int track_update(); /*tracking data for a new frame and make it available for retrieval by the other vision functions*/
int track_get_frame(); /*return value is the frame number used to generate the tracking data.*/
int track_count(int ch); /*return the number of blobs available for the channel ch*/
int track_size(int ch, int i); /* returns the number of pixels in the blob*/
int track_x(int ch, int i); /*gets the pixel x coordinate of the centroid of the blob*/
int track_y(int ch, int i);/*gets the pixel y coordinate of the centroid of the blob*/
int track_confidence(int ch, int i); /*area/bounding box area (range 0-100, low numbers bad, high numbers good)*/
int track_bbox_left(int ch, int i); /*gets the pixel x coordinate of the leftmost pixel in the blob i*/
int track_bbox_right(int ch, int i); /*gets the pixel x coordinate of the rightmost pixel in the blob i*/
int track_bbox_top(int ch, int i); /*gets the pixel y coordinate of the topmost pixel in the blob i*/
int track_bbox_bottom(int ch, int i); /*gets the pixel y coordinate of the bottommost pixel in the blob i*/
int track_bbox_width(int ch, int i); /*gets the pixel x width of the bounding box of the blob i*/
int track_bbox_height(int ch, int i); /*gets the pixel y height of the bounding box of the blob i*/
float track_angle(int ch, int i); /*gets the angle in radians of the major axis of the blob i*/
int track_major_axis(int ch, int i); /*gets the length in pixels of the major  axis of the bounding ellipse*/
int track_minor_axis(int ch, int i); /*gets the length in pixels of the minor axis of the bounding ellipse*/
int track_capture_time(); /*Time since the last frame was captured*/
int mseconds(); /* returns the time since boot */
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


extern int gc_song_array[16][33]; // This is a global used to load and play songs on the Create.  It is filled by the user
void create_load_song(int num);//loads song from gc_song_array row num.
void create_play_song(int num);//plays the song if it has been loaded

void set_each_analog_state(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);
void set_analog_floats(int mask);

extern int gc_song_array[16][33]; // This is a global used to load and play songs on the Create.  It is filled by the user

#endif


