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

#ifndef __KISS_PLATFORM_SERIAL_H__
#define __KISS_PLATFORM_SERIAL_H__

#ifdef WIN32
#include <windows.h>
#define OS_HANDLE HANDLE
#else
#define OS_HANDLE int
#endif

OS_HANDLE kiss_platform_serial_open(char *portname);
void kiss_platform_serial_close(OS_HANDLE handle);
int  kiss_platform_serial_set_baud(OS_HANDLE handle, int baud);
int  kiss_platform_serial_read(OS_HANDLE handle, unsigned char *data, int count);
int  kiss_platform_serial_write(OS_HANDLE handle, unsigned char *data, int count);
int  kiss_platform_serial_check_handle(OS_HANDLE handle);
int  kiss_platform_serial_iflush(OS_HANDLE handle);
int  kiss_platform_serial_oflush(OS_HANDLE handle);

#endif
