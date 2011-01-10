/* Includes the Standard IO Library  */
//#include <kiss-graphics.h>
//#include <kiss-serial.h>
//#include <kiss-create.h>
//#include <kiss-input.h>

void wait_for_light(int light_port_);
void shut_down_in(float delay);
void _shut_down_task();
void run_for( float howLong, void (*funky));


// Botball utility routines
// (c) D.P.Miller, 2002, KISS Institute for Practical Robotics
// (c) Randy Sargent, 2003, KISS Institute for Practical Robotics
// (c) D.P.Miller, 2004, KISS Institute for Practical Robotics
// 
// Routines to wait for the light to turn on in Botball, and to 
// stop the robot after the contest.

/* History
   1/21/04 -dm modified shut_down_in() to turn off servos as well as motors.
/*
// Copyright(c) KIPR, 2010
// Update of wait_for_light function for the CBC2
//    (uses print in place on the CBC and analog10 to match simulator)
// History: ported and modified 1/27/2010 - cnw
//          replaced tone loops with beep loops 1/30/2010 - cnw

// 			Added run_for as alternative to shutdown_in.  Note that 
//			kill_process is broken on Mac and kills the entire program.
//			code works fine under windows, linux and on CBC

// Update of wait_for_light function for the CBC2
// History: made wait for light loop faster in case of bad calibration 2/3/2010 - cnw


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
int __wfl_keep_beeping=1;//global used as semaphore between processes so kill process not needed
void beeper1();
void wait_for_light(int light_port_)
{
	int l_on_, l_off_, l_mid_, t, OK=0;
	int p; // temp until tone is implemented
	float s;
	
	while (!OK) {
		cbc_display_clear();
		cbc_printf (0,0,"CALIBRATE: sensor port #%d", light_port_);
		sleep(1.); beep(); sleep(1.);
		
		cbc_printf(0,1,"  press <-- when light on");
		while(!left_button()){/*sensor value when light is on*/
			cbc_printf(0,2,"  value is %d, bright = low   ",l_on_=analog10 (light_port_)); 
			msleep(50);
		}
		beep();
		
		cbc_printf(0,1,"  light on value is = %d        ", l_on_);
		sleep(1.);
		beep();
		
		cbc_printf(0,2,"  press --> when light off             ");
		while(!right_button()){	/*sensor value when light is off*/
			cbc_printf(0,3,"   value is %d, dark = high   ",l_off_=analog10 (light_port_)); 
			msleep(50);
		}
		beep();
		
		cbc_printf(0,2,"  light off value is = %d         ", l_off_);
		sleep(1.);
		beep();
		
		cbc_printf(0,3,"                              ");
		
		if((l_off_-l_on_)>=120){ /*bright = small values */
			OK=1;
			l_mid_=(l_on_+l_off_)/2;
			cbc_printf(0,4,"Good Calibration!");
			cbc_printf(0,6,"Diff = %d:  WAITING",l_off_-l_on_);
			p=start_process(beeper1);
			while(analog10(light_port_)>l_mid_){
				cbc_printf(0,7,"Value = %d; Threshold = %d   ",analog10(light_port_),l_mid_);
				msleep(25);
			}
			cbc_printf(0,6,"Going!                      ");
			cbc_printf(0,7,"Value = %d; Threshold = %d   ",analog10(light_port_),l_mid_);
			//kill_process(p); //works on cbc
			__wfl_keep_beeping=0;//avoids kill process which has problems on Mac
		}
		else{
			s=seconds();
			cbc_printf(0,6,"BAD CALIBRATION");
			if(l_off_<512){
				cbc_printf(0,7,"   Add Shielding!!");
				for(t=0; t<4; t++) {beep(); sleep(.2); beep(); sleep(0.4);}
				beep();
			}
			else{
				cbc_printf(0,7,"   Aim sensor!!");
				for(t=0; t<4; t++) {beep(); sleep(.2); beep(); sleep(0.4);}
				beep();
			}
		}
	}
}
void beeper1() {
	while(__wfl_keep_beeping) 
	{sleep(1); beep();}
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
	
	
	// run_for takes a function and runs it for a certain amount of time
	// run_for will return within 1 second of your function exiting, if it 
	// exits before the specified time.
	void run_for(float howLong, void (*funky))
	{
	int pid=start_process(funky);
	while(howLong >= 1.0){
	if(GL_TRUE==glfwWaitThread(pid,GLFW_NOWAIT)) return;// exit now if funky has died
	sleep(1.0);
	howLong=howLong-1.0;
	}
	sleep(howLong);
	kill_process(pid);
	return;
	}
	
