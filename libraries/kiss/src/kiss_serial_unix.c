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

#include "kiss_serial.h"
#include "serial_interface.h"

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <strings.h>

int kiss_platform_serial_open(char *portname)
{
	int stream;

	stream = open(portname, O_NDELAY | O_RDWR | O_NOCTTY);
	if(stream < 0)
		return -1;
	fcntl(stream, F_SETFL, ~O_NDELAY & fcntl(stream, F_GETFL, 0));
	return stream;
}

void kiss_platform_serial_close(int handle)
{
	close(handle);
}

int kiss_platform_serial_set_baud(int handle, int baud)
{
	int baud_rate;
	struct termios tio;
	
	switch(baud) {
		case BAUD_9600:
			baud_rate=B9600;
			break;
		case BAUD_19K:
			baud_rate=B19200;
			break;
		case BAUD_38K:
			baud_rate=B38400;
			break;
		case BAUD_57K:
			baud_rate=B57600;
			break;
		default:
			return 0;
	}

	if(tcgetattr(handle, &tio) == -1) {
		return 0;
	}

	tio.c_iflag = IGNBRK;
	tio.c_oflag = 0;
	tio.c_cflag = CREAD | CLOCAL;
	tio.c_cflag |= CS8;
	tio.c_lflag = 0;
	bzero(&tio.c_cc, sizeof(tio.c_cc));
	
	tio.c_cc[VTIME] = 0;
	tio.c_cc[VMIN] = 0;
	cfsetspeed(&tio, baud_rate);
	
	if(tcsetattr(handle, TCSADRAIN, &tio) == -1) {
		return 0;
	}

	return 1;
}

int  kiss_platform_serial_read(int handle, unsigned char *data, int count)
{
	return read(handle, data, count);
}

int  kiss_platform_serial_write(int handle, unsigned char *data, int count)
{
	return write(handle, data, count);
}

int  kiss_platform_serial_check_handle(int handle)
{
	return !(handle < 0);
}

int kiss_platform_serial_iflush(int handle)
{
	if(tcflush(handle, TCIFLUSH))
		return 0;
	return 1;
}

int kiss_platform_serial_oflush(int handle)
{
	if(tcflush(handle, TCOFLUSH))
		return 0;
	return 1;
}

