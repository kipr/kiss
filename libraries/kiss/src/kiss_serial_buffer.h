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

#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include "kiss_serial.h"

#define SERIAL_BUFFER_SIZE 256

struct serial_buffer
{
	int first;
	int last;
	unsigned char data[SERIAL_BUFFER_SIZE];
};

struct serial_buffer *kiss_serial_buffer_create();
void kiss_serial_buffer_destroy(struct serial_buffer *buffer);
int  kiss_serial_buffer_put(struct serial_buffer *buffer, unsigned char *data, int count);
int  kiss_serial_buffer_get(struct serial_buffer *buffer, unsigned char *data, int count);
void kiss_serial_buffer_putb(struct serial_buffer *buffer, unsigned char byte);
unsigned char kiss_serial_buffer_getb(struct serial_buffer *buffer);
void kiss_serial_buffer_inc(struct serial_buffer *buffer);
void kiss_serial_buffer_dec(struct serial_buffer *buffer);
int  kiss_serial_buffer_count(struct serial_buffer *buffer);

#endif
