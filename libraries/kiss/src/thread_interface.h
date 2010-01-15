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

#ifndef __THREAD_INTERFACE_H__
#define __THREAD_INTERFACE_H__

#include <GL/GLee.h>
#include <GL/glfw.h>

typedef GLFWmutex kiss_mutex;

int start_process(void (*func)());
void kill_process(int pid);

void defer();

kiss_mutex create_mutex();
void destroy_mutex(kiss_mutex m);

void lock_mutex(kiss_mutex m);
void unlock_mutex(kiss_mutex m);

void GLFWCALL kiss_run_process(void *ptr);


#endif
