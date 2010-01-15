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

#include "serial_interface.h"
#include "kiss_serial_buffer.h"
#include "kiss_serial.h"

//this is *not* typically good practice, but will make this file a little easier to read
#define conn_list kiss_g_serial_connection_list

struct serial_connection kiss_g_serial_connection_list[MAX_CONNECTIONS] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},
															 {0,0,0},{0,0,0},{0,0,0},{0,0,0},
															 {0,0,0},{0,0,0},{0,0,0},{0,0,0},
															 {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};


int serial_open(char *portname, int baud)
{
	int handle;

	for(handle = 1;handle < MAX_CONNECTIONS && conn_list[handle].is_connected;handle++);

	if(serial_reopen(handle, portname, baud))
		return handle;

	return -1;
}

int serial_reopen(int handle, char * portname, int baud)
{
	if(conn_list[handle].is_connected)
		serial_close(handle);

	if(baud == SERIAL_OFF)
		return 1;

	conn_list[handle].os_handle = kiss_platform_serial_open(portname);
	if(!kiss_platform_serial_check_handle(conn_list[handle].os_handle))
		return 0;

	if(!kiss_platform_serial_set_baud(conn_list[handle].os_handle, baud))
		return 0;

	if(!(conn_list[handle].buffer = kiss_serial_buffer_create())) {
		kiss_platform_serial_close(conn_list[handle].os_handle);
		return 0;
	}

	conn_list[handle].is_connected = 1;
	return 1;
}

void serial_close(int handle)
{
	if(conn_list[handle].is_connected) {
		kiss_serial_buffer_destroy(conn_list[handle].buffer);
		kiss_platform_serial_close(conn_list[handle].os_handle);
		conn_list[handle].is_connected = 0;
	}
}

int serial_set_baud(int handle, int baud)
{
	if(conn_list[handle].is_connected)
		return kiss_platform_serial_set_baud(conn_list[handle].os_handle, baud);
	return 0;
}

int serial_update_buffer(int handle)
{
	unsigned char data[SERIAL_BUFFER_SIZE];
	int byte_count;

	if(!conn_list[handle].is_connected)
		return 0;

	byte_count = kiss_platform_serial_read(conn_list[handle].os_handle, data, SERIAL_BUFFER_SIZE - 1 -kiss_serial_buffer_count(conn_list[handle].buffer));
	kiss_serial_buffer_put(conn_list[handle].buffer, data, byte_count);
	return kiss_serial_buffer_count(conn_list[handle].buffer);
}

int serial_write(int handle, unsigned char *data, int count)
{
	if(!conn_list[handle].is_connected)
		return 0;

	return kiss_platform_serial_write(conn_list[handle].os_handle, data, count);
}

int serial_read(int handle, unsigned char *buffer, int count)
{
	if(!conn_list[handle].is_connected)
		return 0;

	serial_update_buffer(handle);
	return kiss_serial_buffer_get(conn_list[handle].buffer, buffer, count);
}

int serial_iflush(int handle)
{
	if(!conn_list[handle].is_connected)
		return 0;
	
	while(kiss_serial_buffer_count(conn_list[handle].buffer))
		kiss_serial_buffer_getb(conn_list[handle].buffer);
	
	return kiss_platform_serial_iflush(conn_list[handle].os_handle);
}

int serial_oflush(int handle)
{
	if(!conn_list[handle].is_connected)
		return 0;
	return kiss_platform_serial_oflush(conn_list[handle].os_handle);
}

int serial_ioflush(int handle)
{
	return (serial_iflush(handle) && serial_oflush(handle) ? 1 : 0);
}

void serial_writeb(int handle, unsigned char byte)
{
	if(!conn_list[handle].is_connected)
		return;

	kiss_platform_serial_write(conn_list[handle].os_handle, &byte, 1);
}

unsigned char serial_readb(int handle)
{
	serial_update_buffer(handle);
	if(kiss_serial_buffer_count(conn_list[handle].buffer))
		return kiss_serial_buffer_getb(conn_list[handle].buffer);
	return 0;
}

