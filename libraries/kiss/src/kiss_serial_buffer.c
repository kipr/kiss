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
#include "kiss_serial_buffer.h"
#include <stdlib.h>

struct serial_buffer *kiss_serial_buffer_create()
{
	struct serial_buffer *buffer;

	if(!(buffer = malloc(sizeof(struct serial_buffer))))
		return 0;

	buffer->first = 0;
	buffer->last = 0;

	return buffer;
}

void kiss_serial_buffer_destroy(struct serial_buffer *buffer)
{
	free(buffer);
}

int  kiss_serial_buffer_put(struct serial_buffer *buffer, unsigned char *data, int count)
{
	int i;

	for(i = 0;i < count && kiss_serial_buffer_count(buffer) < SERIAL_BUFFER_SIZE-1;i++)
		kiss_serial_buffer_putb(buffer, data[i]);
	return i;
}

int  kiss_serial_buffer_get(struct serial_buffer *buffer, unsigned char *data, int count)
{
	int i;

	for(i = 0;i < count && kiss_serial_buffer_count(buffer);i++)
		data[i] = kiss_serial_buffer_getb(buffer);
	return i;
}

void kiss_serial_buffer_putb(struct serial_buffer *buffer, unsigned char byte)
{
	buffer->data[buffer->last++] = byte;

	if(buffer->last == SERIAL_BUFFER_SIZE)
		buffer->last = 0;

	if(buffer->last == buffer->first) {
		buffer->first++;
		if(buffer->first == SERIAL_BUFFER_SIZE)
			buffer->first = 0;
	}
}

unsigned char kiss_serial_buffer_getb(struct serial_buffer *buffer)
{
	unsigned char byte;

	if(!kiss_serial_buffer_count(buffer))
		return 0;

	byte = buffer->data[buffer->first++];

	if(buffer->first == SERIAL_BUFFER_SIZE)
		buffer->first = 0;

	return byte;
}

int  kiss_serial_buffer_count(struct serial_buffer *buffer)
{
	if(buffer->last < buffer->first)
		return buffer->last+SERIAL_BUFFER_SIZE - buffer->first;
	return buffer->last - buffer->first;
}

