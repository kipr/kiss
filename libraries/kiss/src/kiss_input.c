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

#include "kiss_input.h"
#include "kiss.h"

volatile unsigned char kiss_g_key_table[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

GLFWmutex kiss_g_key_mutex = 0;
GLFWmutex kiss_g_mouse_mutex = 0;

void kiss_input_callback(int key, int action)
{
	if(action)
		kiss_set_key_bit(key);
	else
		kiss_clear_key_bit(key);
}

void kiss_set_key_bit(int key)
{
	int byte,bit;
	
	byte = key>>3;
	bit = key-(byte<<3);

	glfwLockMutex(kiss_g_key_mutex);
	kiss_g_key_table[byte] |= 1<<bit;
	glfwUnlockMutex(kiss_g_key_mutex);
}

void kiss_clear_key_bit(int key)
{
	int byte,bit;

	byte = key>>3;
	bit = key-(byte<<3);

	glfwLockMutex(kiss_g_key_mutex);
	kiss_g_key_table[byte] &= ~(1<<bit);
	glfwUnlockMutex(kiss_g_key_mutex);
}

int kiss_get_key_bit(int key)
{
	int byte,bit,ret;

	if(!kiss_init())
		return 0;

	if(!kiss_g_key_mutex)
		kiss_g_key_mutex = glfwCreateMutex();

	byte = key>>3;
	bit = key-(byte<<3);

	glfwLockMutex(kiss_g_key_mutex);
	ret = kiss_g_key_table[byte] & (1<<bit);
	glfwUnlockMutex(kiss_g_key_mutex);

	return ret;
}

void kiss_get_mouse_position(int *x, int *y)
{
	int xpos, ypos;

	glfwLockMutex(kiss_g_mouse_mutex);
	glfwGetMousePos(x,y);
	glfwUnlockMutex(kiss_g_mouse_mutex);
}

int  kiss_get_mouse_button(int button)
{
	int ret;
	glfwLockMutex(kiss_g_mouse_mutex);
	ret = glfwGetMouseButton(button);
	glfwUnlockMutex(kiss_g_mouse_mutex);
	return ret;
}

int  kiss_get_mouse_wheel()
{
	int ret;
	glfwLockMutex(kiss_g_mouse_mutex);
	ret = glfwGetMouseWheel();
	glfwUnlockMutex(kiss_g_mouse_mutex);
	return ret;
}

void kiss_grab_mouse()
{
	kiss_lock();
	glfwDisable(GLFW_MOUSE_CURSOR);
	kiss_unlock();
}

void kiss_release_mouse()
{
	kiss_lock();
	glfwEnable(GLFW_MOUSE_CURSOR);
	kiss_unlock();
}

