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
 **************************************************************************

	Botball utility routines
	(c) D.P.Miller, 2002, KISS Institute for Practical Robotics
	(c) Randy Sargent, 2003, KISS Institute for Practical Robotics
	(c) D.P.Miller, 2004, KISS Institute for Practical Robotics

	Routines to wait for the light to turn on in Botball, and to 
	stop the robot after the contest.

	History
   1/21/04 -dm modified shut_down_in() to turn off servos as well as motors.

   Using the routines in this library, you should insert the following
   two lines at the beginning of your "main":

      wait_for_light(2);  // Light sensor in channel 2
      shut_down_in(89.0); // Shut down the robot in 89 seconds

   Be sure to only call "shut_down_in" once.

   The expected for "wait_for_light" is:
   1) Turn on start light and call this function
   2) Press start button
   3) Turn off start light
   4) press stop button

   At this point the routine will either give a happy message, play a constant
   buzz sound and wait for the light to come back on (whereupon the function
   returns) OR it will give a sad message with a recommendation, play a klaxon
   tone sequence and hang -- forcing the HB to be reset.
*/

void wait_for_light(int light_port_);
void shut_down_in(float delay);
void _shut_down_task();

void wait_for_light(int light_port_)
{
	int l_on_, l_off_, l_mid_, t;
	
	/* print statements may look funny but are spaced for LCD*/
	printf ("Cal. with sensoron port #%d\n", light_port_);
	sleep(2.);
	
	printf("Cal: press Left when light on\n");
	while(!left_button());
	l_on_=analog(light_port_); /*sensor value when light is on*/
	beep();
	
	printf("Cal: light on   value is=%d\n", l_on_);
	sleep(1.);
	beep();
	
	printf("Cal: press Right when light off\n");
	while(!right_button());
	l_off_=analog(light_port_); /*sensor value when light is off*/
	beep();
	
	printf("Cal: light off  value is=%d\n", l_off_);
	sleep(1.);
	beep();
	
	if((l_off_-l_on_)>=15){ /*bright = small values */
		l_mid_=(l_on_+l_off_)/2;
		printf("Good CalibrationDiff=%d Waiting\n",(l_off_-l_on_));
		while(analog(light_port_)>l_mid_) 
		tone(440.,0.1);
	}
	else{
		if(l_off_<128){
			printf("Bad Calibration Add Shielding!!\n");
			while(1) /*never terminate */
			for(t=100; t<10000; t=t+200) tone((float)t,0.05);
		}
		else{
			printf("Bad Calibration Aim sensor!!\n");
			while(1) /*never terminate */
			for(t=10000; t>200; t=t-200) tone((float)t,0.05);
		}
	}
}

//  
//  Function:  shut_down_in(float: delay)
//  
//  This function will start a background process which sleeps for "delay",
//  then stops the motors and kills all other processes.
//  
//  This function cannot work if another process continually calls
//  "hog_processor", since the background task will never gain control.
//  
//  This function may not work if more than 1000 processes have been
//  created between the original call to "shut_down_in" and when the delay
//  is finished.
//  

int Bb_Game_Over=0; /* global flag*/

int _shut_down_pid;
int _shut_down_delay;

void shut_down_in(float delay)
{
	Bb_Game_Over= 0;
	printf("shut_down_in%f\n", delay);
	_shut_down_delay=1000*delay;
	_shut_down_pid= start_process(_shut_down_task);
}

void _shut_down_task()
{
	int i,j;
	msleep(_shut_down_delay);
	create_stop();
	ao();
    //disable_servos(); // delete this line if you want your servos to freeze but remain powered at the end
    Bb_Game_Over= 1;
    printf("Game over");
    i= 0;
    for (j= 256; j <= 1024; j+=256) {
//        hog_processor();
        while (i < j) {
            kill_process(_shut_down_pid+(++i));
            kill_process(_shut_down_pid-i);
        }
        ao();
        disable_servos(); // delete this line if you want your servos to freeze but remain powered at the end
    }
    printf(".\n");
}

