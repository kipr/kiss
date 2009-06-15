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

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreturn-type"
void kissSimPause() {}
void kissSimEnablePause() {}
void kissSim_init(int world, int rx, int ry, float rt) {}
void tone(int frequency, int duration) {} 
void beep() {} 
int digital(int port) {} 
int set_digital_output_value(int port, int value) {} 
void set_pid_gains(int motor, int p, int i, int d, int pd, int id, int dd) {} 
int analog10(int port) {} 
int analog(int port) {} 
int accel_x() {} 
int accel_y() {} 
int accel_z() {} 
int sonar(int port) {} 
int sonar_inches(int port) {} 
float power_level() {} 
void enable_servos() {} 
void disable_servos() {} 
int set_servo_position(int servo, int pos) {} 
int get_servo_position(int servo) {} 
int get_motor_position_counter(int motor) {} 
int clear_motor_position_counter(int motor) {} 
int move_at_velocity(int motor, int velocity) {} 
int mav(int motor, int velocity) {} 
int move_to_position(int motor, int speed, long goal_pos) {}
int mtp(int motor, int speed, long goal_pos) {}
int move_relative_position(int motor, int speed, long delta_pos) {}
int mrp(int motor, int speed, long delta_pos) {}
int freeze(int motor) {}
int get_motor_done(int motor) {} 
void block_motor_done(int motor) {} 
void bmd(int motor) {} 
int setpwm(int motor, int pwm) {} 
void fd(int motor) {} 
void bk(int motor) {} 
void motor (int motor, int percent) {}
void off(int motor) {} 
void ao() {} 
int black_button() {} 
void cbc_display_clear() {} 
void cbc_printf(int col, int row, char *t, ...) {}
void wait_for_light(int light_port_) {}
void shut_down_in(float delay) {}
int init_camera() {} 
int track_update() {} 
int track_get_frame() {} 
int track_count(int ch) {} 
int track_size(int ch, int i) {} 
int track_x(int ch, int i) {} 
int track_y(int ch, int i) {}
int track_confidence(int ch, int i) {} 
int track_bbox_left(int ch, int i) {} 
int track_bbox_right(int ch, int i) {} 
int track_bbox_top(int ch, int i) {} 
int track_bbox_bottom(int ch, int i) {} 
int track_bbox_width(int ch, int i) {} 
int track_bbox_height(int ch, int i) {} 
float track_angle(int ch, int i) {} 
int track_major_axis(int ch, int i) {} 
int track_minor_axis(int ch, int i) {} 
int create_connect() {}
int create_connect_s() {}
void create_disconnect() ;
void create_disconnect_s() {}
void create_start() {}
void create_passive() {}
void create_safe() {}
void create_full() ;
void create_spot() ;
void create_cover() {}
void create_demo(int d) ;
void create_cover_dock() {}
int create_mode() {}
int create_sensor_update() {}
int create_wall() {}
int create_buttons() ;
int create_bumpdrop() {}
int create_cliffs() ;
int create_angle() ;
int create_distance() {}
int create_velocity() {}
int create_read_IR() {}
int create_overcurrents() {}
int create_battery_charge() {}
int create_cargo_bay_inputs() {}
void create_stop() {}
void create_drive (int speed, int radius) {}
void create_drive_straight (int speed) {}
void create_spin_CW (int speed) {}
void create_spin_CCW (int speed) {}
void create_drive_direct(int r_speed, int l_speed) {}
int create_spin_block(int speed, int angle) {}
int _create_get_raw_encoders(long *lenc, long *renc) {}
void create_advance_led(int on) ;
void create_play_led(int on) ;
void create_power_led(int color, int brightness) ;
void create_digital_output(int bits) {}
void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0) {}
void create_low_side_drivers(int pwm2, int pwm1, int pwm0) {}
void create_load_song(int num) {}
void create_play_song(int num) {}

extern int gc_lbump, gc_rbump, gc_ldrop, gc_rdrop, gc_fdrop, gc_rcliff, gc_rfcliff, gc_lcliff, gc_lfcliff;
extern int gc_rcliff_amt, gc_rfcliff_amt, gc_lcliff_amt, gc_lfcliff_amt;
extern int gc_distance, gc_angle, gc_total_angle, gc_advance_button, gc_play_button;
extern int gc_wall, gc_wall_amt, gc_wall_hb, gc_IR;
extern int gc_vel, gc_radius, gc_rvel, gc_lvel;
extern int gc_overcurrents;
extern int gc_charge_state, gc_batt_voltage, gc_current_flow, gc_batt_temp, gc_batt_charge, gc_batt_capacity;
extern int gc_digital_in, gc_analog_in, gc_charge_source;

