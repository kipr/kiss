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

#include "thread_interface.h"
#include "kiss.h"

int start_process(void (*func)())
{
	if(!kiss_init())
		return 0;

	return glfwCreateThread(kiss_run_process, (void *)func);
}

void kill_process(int pid)
{
	if(!kiss_init())
		return;

	glfwDestroyThread(pid);
}

void defer()
{
	if(!kiss_init())
		return;
	
	glfwSleep(0);
}

kiss_mutex create_mutex()
{
	if(!kiss_init())
		return 0;

	return glfwCreateMutex();
}

void destroy_mutex(kiss_mutex m)
{
	if(!kiss_init())
		return;

	return glfwDestroyMutex(m);
}

void lock_mutex(kiss_mutex m)
{
	if(!kiss_init())
		return;

	glfwLockMutex(m);
}

void unlock_mutex(kiss_mutex m)
{
	if(!kiss_init())
		return;

	glfwUnlockMutex(m);
}

void GLFWCALL kiss_run_process(void *ptr)
{
	void (*func)() = ptr;
	func();
}
