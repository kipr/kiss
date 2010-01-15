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

#include "kiss_util.h"
#include "kiss.h"

#include <stdlib.h>

GLFWmutex kiss_g_rand_mutex = 0;

int kiss_random()
{
	int num;

	if(!kiss_init())
		return 0;
	if(!kiss_g_rand_mutex)
		kiss_g_rand_mutex = glfwCreateMutex();

	glfwLockMutex(kiss_g_rand_mutex);
	num = rand();
	glfwUnlockMutex(kiss_g_rand_mutex);

	return num;
}

void kiss_srandom(int seed)
{
	if(!kiss_init())
		return;

	if(!kiss_g_rand_mutex)
		kiss_g_rand_mutex = glfwCreateMutex();

	glfwLockMutex(kiss_g_rand_mutex);
	srand(seed);
	glfwUnlockMutex(kiss_g_rand_mutex);	
}
