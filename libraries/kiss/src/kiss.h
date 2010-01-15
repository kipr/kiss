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

#ifndef __KISS_H__
#define __KISS_H__

#include <GL/GLee.h>
#include <GL/glfw.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "kiss_graphics.h"

int kiss_init();
void kiss_terminate();
void kiss_exec_op(int op, int arg1, int arg2);

void kiss_lock();
void kiss_unlock();

#ifndef WIN32
void *kiss_main_thread(void *arg);
#else
DWORD WINAPI kiss_main_thread( LPVOID lpParam );
#endif

extern volatile int kiss_g_graphics_enabled;
extern volatile int kiss_g_glfw_enabled;

#endif
