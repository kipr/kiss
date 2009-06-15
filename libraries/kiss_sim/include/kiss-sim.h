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

/* Simulator prototypes anf globals for user accesible function */

#ifndef KISSSIMH
#define KISSSIMH


#define SIMPLEWORLD 0
#define BB08WORLD 1
#define EMPTYWORLD 2
#define RULERWORLD 3
#define BIGEMPTYWORLD 4
#define BIGARENA 5
#define BIGARENA2 6
#define BB09WORLD 7


void kissSimPause();
void kissSimEnablePause();
void kissSim_init(int world, int rx, int ry, float rt);
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
void cbc_printf(int col, int row, char *t, ...); /*Starts printing at col, row then normal printf control string and args*/                                       void wait_for_light(int light_port_);
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
//globals are updated by the functions below to give access to many key Create sensors
extern int gc_lbump, gc_rbump, gc_ldrop, gc_rdrop, gc_fdrop, gc_rcliff, gc_rfcliff, gc_lcliff, gc_lfcliff;
extern int gc_rcliff_amt, gc_rfcliff_amt, gc_lcliff_amt, gc_lfcliff_amt;
extern int gc_distance, gc_angle, gc_total_angle, gc_advance_button, gc_play_button;
extern int gc_wall, gc_wall_amt, gc_wall_hb, gc_IR;
extern int gc_vel, gc_radius, gc_rvel, gc_lvel;
extern int gc_overcurrents;
extern int gc_charge_state, gc_batt_voltage, gc_current_flow, gc_batt_temp, gc_batt_charge, gc_batt_capacity;
extern int gc_digital_in, gc_analog_in, gc_charge_source;

#endif


