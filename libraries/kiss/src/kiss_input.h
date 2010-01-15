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

#ifndef __KISS_INPUT_H__
#define __KISS_INPUT_H__

#include <GL/GLee.h>
#include <GL/glfw.h>

void kiss_input_callback(int key, int action);

void kiss_set_key_bit(int key);
void kiss_clear_key_bit(int key);
int kiss_get_key_bit(int key);

void kiss_get_mouse_position(int *x, int *y);
int  kiss_get_mouse_button(int button);
int  kiss_get_mouse_wheel();
void kiss_grab_mouse();
void kiss_release_mouse();

extern GLFWmutex kiss_g_key_mutex;
extern GLFWmutex kiss_g_mouse_mutex;

#endif
