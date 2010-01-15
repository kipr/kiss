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

#include "kiss.h"
#include "kiss_ops.h"

#include "kiss_input.h"

#include <stdlib.h>

#ifndef WIN32
#include <pthread.h>
#endif

//Core variables
GLFWmutex kiss_g_glfw_mutex;
volatile int kiss_g_glfw_enabled = 0;
volatile int kiss_g_glfw_initting = 0;
volatile int kiss_g_glfw_quit = 0;

volatile int kiss_g_op = 0;
volatile int kiss_g_op_arg1 = 0;
volatile int kiss_g_op_arg2 = 0;

//Graphics variables
volatile int kiss_g_graphics_enabled = 0;

#ifndef WIN32
pthread_t kiss_g_glfw_pthread;
#endif

int kiss_init()
{
	if(kiss_g_glfw_enabled)
		return 1;

	kiss_g_glfw_initting = 1;

#ifndef WIN32
	pthread_create(&kiss_g_glfw_pthread, 0, kiss_main_thread, 0);
#else
 	CreateThread(NULL, 0, kiss_main_thread, 0, 0, 0);
#endif

	while(kiss_g_glfw_initting);

	if(kiss_g_glfw_enabled) {
		atexit(kiss_terminate);
		return 1;
	}

	return 0;
}

void kiss_terminate()
{
	kiss_lock();
	kiss_g_glfw_quit = 1;
	while(kiss_g_glfw_quit);
}

#ifndef WIN32
void *kiss_main_thread(void *arg)
#else
DWORD WINAPI kiss_main_thread( LPVOID lpParam )
#endif
{
	glfwInit();

	kiss_g_glfw_mutex = glfwCreateMutex();
	kiss_g_key_mutex = glfwCreateMutex();
	kiss_g_mouse_mutex = glfwCreateMutex();

	if(!kiss_g_glfw_mutex) {
		kiss_g_glfw_initting = 0;
		glfwTerminate();
		return 0;
	}

	glfwDisable(GLFW_AUTO_POLL_EVENTS);

	kiss_g_glfw_enabled = 1;
	kiss_g_glfw_initting = 0;

	while(!kiss_g_glfw_quit) {
		if(kiss_g_op) {
			switch(kiss_g_op) {
				case KISS_OPEN_WINDOW:
					kiss_graphics_open_window(kiss_g_op_arg1, kiss_g_op_arg2);
					glfwSetKeyCallback(kiss_input_callback);
					kiss_g_graphics_enabled=1;
					break;
				case KISS_CLOSE_WINDOW:
					kiss_graphics_close_window();
					kiss_g_graphics_enabled=0;
					break;
				case KISS_COPY:
					if(!kiss_g_graphics_enabled)
						break;
					kiss_graphics_update();
					glfwSwapBuffers();
					break;
			}
			kiss_g_op = 0;
		}
		if(kiss_g_graphics_enabled)
			glfwPollEvents();
	}

	if(kiss_g_graphics_enabled)
		kiss_graphics_close_window();

	glfwTerminate();

	kiss_g_glfw_enabled = 0;
	kiss_g_glfw_quit = 0;

	return 0;
}

void kiss_exec_op(int op, int arg1, int arg2)
{
	kiss_g_op_arg1 = arg1;
	kiss_g_op_arg2 = arg2;
	kiss_g_op = op;

	while(kiss_g_op);
}

void kiss_lock()
{
	glfwLockMutex(kiss_g_glfw_mutex);
}

void kiss_unlock()
{
	glfwUnlockMutex(kiss_g_glfw_mutex);
}


