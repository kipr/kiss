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

#include "graphics_interface.h"
#include "kiss.h"
#include "kiss_graphics.h"
#include "kiss_graphics_draw.h"

#include "kiss_ops.h"

int graphics_init(int width, int height)
{
	if(!kiss_init())
		return 0;

	kiss_lock();
	kiss_exec_op(KISS_OPEN_WINDOW, width, height);
	kiss_unlock();
	return 1;
}

void graphics_quit()
{
	if(!kiss_init())
		return;

	kiss_lock();
	kiss_exec_op(KISS_CLOSE_WINDOW, 0, 0);
	kiss_unlock();
}

void graphics_update()
{
	if(!kiss_init())
		return;

	kiss_lock();
	kiss_graphics_lock();
	kiss_exec_op(KISS_COPY, 0, 0);
	kiss_graphics_unlock();
	kiss_unlock();
}

void graphics_fill(int r, int g, int b)
{
	if(!kiss_init())
		return;

	kiss_lock();
	kiss_graphics_lock();
	if(kiss_g_graphics_enabled)
		kiss_graphics_fill(r, g, b);
	kiss_graphics_unlock();
	kiss_unlock();

}

void graphics_pixel(int x, int y, int r, int g, int b)
{
	if(!kiss_init())
		return;

	kiss_lock();
	kiss_graphics_lock();
	if(kiss_g_graphics_enabled)
		kiss_graphics_pixel(x, y, r, g, b);
	kiss_graphics_unlock();
	kiss_unlock();
}

void graphics_line(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	if(!kiss_init())
		return;

	kiss_lock();
	kiss_graphics_lock();
	if(kiss_g_graphics_enabled)
		kiss_graphics_line(x1, y1, x2, y2, r, g, b);
	kiss_graphics_unlock();
	kiss_unlock();
}

void graphics_circle(int cx, int cy, int radius, int r, int g, int b)
{
	if(!kiss_init())
		return;

	kiss_lock();
	kiss_graphics_lock();
	if(kiss_g_graphics_enabled)
		kiss_graphics_circle(cx, cy, radius, r, g, b);
	kiss_graphics_unlock();
	kiss_unlock();
}

void graphics_circle_fill(int cx, int cy, int radius, int r, int g, int b)
{
	if(!kiss_init())
		return;

	kiss_lock();
	kiss_graphics_lock();
	if(kiss_g_graphics_enabled)
		kiss_graphics_circle_fill(cx, cy, radius, r, g, b);
	kiss_graphics_unlock();
	kiss_unlock();

}

void graphics_rectangle(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	if(!kiss_init())
		return;

	kiss_lock();
	kiss_graphics_lock();
	if(kiss_g_graphics_enabled)
		kiss_graphics_rectangle(x1, y1, x2, y2, r, g, b);
	kiss_graphics_unlock();
	kiss_unlock();

}

void graphics_rectangle_fill(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	if(!kiss_init())
		return;

	kiss_lock();
	kiss_graphics_lock();
	if(kiss_g_graphics_enabled)
		kiss_graphics_rectangle_fill(x1, y1, x2, y2, r, g, b);
	kiss_graphics_unlock();
	kiss_unlock();
}

void graphics_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b)
{
	if(!kiss_init())
		return;
		
	kiss_lock();
	kiss_graphics_lock();
	if(kiss_g_graphics_enabled)
		kiss_graphics_triangle(x1, y1, x2, y2, x3, y3, r, g, b);
	kiss_graphics_unlock();
	kiss_unlock();
}

void graphics_triangle_fill(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b)
{
	if(!kiss_init())
		return;
		
	kiss_lock();
	kiss_graphics_lock();
	if(kiss_g_graphics_enabled)
		kiss_graphics_triangle_fill(x1, y1, x2, y2, x3, y3, r, g, b);
	kiss_graphics_unlock();
	kiss_unlock();
}
