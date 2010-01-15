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

#ifndef __SERIAL_INTERFACE_H__
#define __SERIAL_INTERFACE_H__

#include "kiss_serial.h"

#ifndef BAUD_9600
#define BAUD_9600 2604
#endif
#define BAUD_19K 1302
#define BAUD_38K 651
#define BAUD_57K 434
#define SERIAL_OFF 0
#define IC_SERIAL SERIAL_OFF

#define MAX_CONNECTIONS 17

struct serial_connection
{
	OS_HANDLE os_handle;
	int is_connected;
	struct serial_buffer *buffer;
};

int  serial_open(char *portname, int baud);
int  serial_reopen(int handle, char * portname, int baud);
void serial_close(int handle);
int  serial_set_baud(int handle, int baud);
int  serial_update_buffer(int handle);
int  serial_write(int handle, unsigned char *data, int count);
int  serial_read(int handle, unsigned char *data, int count);
int  serial_iflush(int handle);
int  serial_oflush(int handle);
int  serial_ioflush(int handle);
void serial_writeb(int handle, unsigned char byte);
unsigned char serial_readb(int handle);



#endif
