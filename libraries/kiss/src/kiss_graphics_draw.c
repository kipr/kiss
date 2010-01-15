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

#include "kiss_graphics_draw.h"
#include <stdlib.h>
#include <string.h>
#include <GL/GLee.h>

inline void kiss_graphics_fill(int r, int g, int b)
{
	int i, j;

	for(i = 0;i < kiss_g_graphics_info.width;i++)
		for(j = 0;j < kiss_g_graphics_info.height;j++)
			kiss_graphics_pixel(i, j, r, g, b);
}

inline void kiss_graphics_pixel(int x, int y, int r, int g, int b)
{
	if(x < 0) return;
	if(y < 0) return;
	if(x >= kiss_g_graphics_info.width) return;
	if(y >= kiss_g_graphics_info.height) return;
	
	y = kiss_g_graphics_info.height - 1 - y;

	if(kiss_g_graphics_info.texture_format == GL_BGRA)
	  kiss_g_graphics_info.data[x + y*kiss_g_graphics_info.buffer_width] = (r<<16) | (g<<8) | b;
	else
	  kiss_g_graphics_info.data[x + y*kiss_g_graphics_info.buffer_width] = (b<<16) | (g<<8) | r;
}

/* Bressenham algorithm drawn from: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */
inline void kiss_graphics_line(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	int steep, deltax, deltay, error, ystep, x, y;

	if(abs(y2 - y1) > abs(x2 - x1))
		steep = 1;
	else steep = 0;

	if(steep) {
		kiss_graphics_swap(&x1, &y1);
		kiss_graphics_swap(&x2, &y2);
	}
	if(x1 > x2) {
		kiss_graphics_swap(&x1, &x2);
		kiss_graphics_swap(&y1, &y2);
	}

	deltax = x2 - x1;
	deltay = abs(y2 - y1);
	error = -(deltax + 1) >> 1;
	y = y1;

	if(y1 < y2) ystep = 1;
	else		ystep = -1;

	for(x = x1;x <= x2;x++) {
		if(steep) kiss_graphics_pixel(y, x, r, g, b);
		else	  kiss_graphics_pixel(x, y, r, g, b);
		error = error+deltay;
		if(error >= 0) {
			y = y+ystep;
			error = error - deltax;
		}
	}
}

/* Midpoint circle algorithm taken from http://en.wikipedia.org/wiki/Midpoint_circle_algorithm */
inline void kiss_graphics_circle(int cx, int cy, int radius, int r, int g, int b)
{
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	kiss_graphics_pixel(cx, cy + radius, r, g, b);
	kiss_graphics_pixel(cx, cy - radius, r, g, b);
	kiss_graphics_pixel(cx + radius, cy, r, g, b);
	kiss_graphics_pixel(cx - radius, cy, r, g, b);

	while(x < y) {
		if(f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		kiss_graphics_pixel(cx + x, cy + y, r, g, b);
		kiss_graphics_pixel(cx - x, cy + y, r, g, b);
		kiss_graphics_pixel(cx + x, cy - y, r, g, b);
		kiss_graphics_pixel(cx - x, cy - y, r, g, b);
		kiss_graphics_pixel(cx + y, cy + x, r, g, b);
		kiss_graphics_pixel(cx - y, cy + x, r, g, b);
		kiss_graphics_pixel(cx + y, cy - x, r, g, b);
		kiss_graphics_pixel(cx - y, cy - x, r, g, b);
	}
}

inline void kiss_graphics_circle_fill(int cx, int cy, int radius, int r, int g, int b)
{
	long dx, dy, c_squared;
	int i, j;

	c_squared = (long)radius * (long)radius;

	for(i = cx-radius;i<cx+radius;i++) {
		for(j = cy-radius;j<cy+radius;j++) {
			dx = cx-i; dx *= dx;
			dy = cy-j; dy *= dy;

			if(dx + dy <= c_squared)
				kiss_graphics_pixel(i, j, r, g, b);
		}
	}
}

inline void kiss_graphics_rectangle(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	kiss_graphics_line(x1, y1, x1, y2, r, g, b);
	kiss_graphics_line(x1, y1, x2, y1, r, g, b);
	kiss_graphics_line(x2, y2, x2, y1, r, g, b);
	kiss_graphics_line(x2, y2, x1, y2, r, g, b);
}

inline void kiss_graphics_rectangle_fill(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	int i, j;

	if(x1 > x2)
		kiss_graphics_swap(&x1, &x2);
	if(y1 > y2)
		kiss_graphics_swap(&y1, &y2);

	for(i = x1;i <= x2;i++)
		for(j = y1;j <= y2;j++)
			kiss_graphics_pixel(i, j, r, g, b);
}

inline void kiss_graphics_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b)
{
	kiss_graphics_line(x1, y1, x2, y2, r, g, b);
	kiss_graphics_line(x2, y2, x3, y3, r, g, b);
	kiss_graphics_line(x3, y3, x1, y1, r, g, b);
}

inline void kiss_graphics_triangle_fill(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b)
{
	kiss_graphics_edge_buffer_clear();

	kiss_graphics_edge_buffer_line(x1, y1, x2, y2);
	kiss_graphics_edge_buffer_line(x2, y2, x3, y3);
	kiss_graphics_edge_buffer_line(x1, y1, x3, y3);

	kiss_graphics_edge_buffer_paint(r, g, b);
}

inline void kiss_graphics_edge_buffer_clear()
{
	int i, j;

	for(i = 0;i < kiss_g_graphics_info.height;i++)
		for(j = 0;j < 2;j++)
			kiss_g_graphics_info.edge_buffer[i][j] = -1;

	memset(kiss_g_graphics_info.edge_count, 0, kiss_g_graphics_info.height * 2);
}

inline void kiss_graphics_edge_buffer_paint(int r, int g, int b)
{
	int i, j, pos, min, max;

	pos = kiss_g_graphics_info.buffer_width * (kiss_g_graphics_info.height-1);

	for(i = 0;i < kiss_g_graphics_info.height;i++,pos-=kiss_g_graphics_info.buffer_width) {

		if(kiss_g_graphics_info.edge_buffer[i][0] < 0 && kiss_g_graphics_info.edge_buffer[i][1] < 0)
			continue;
		if(kiss_g_graphics_info.edge_buffer[i][0] >= kiss_g_graphics_info.width &&
		   kiss_g_graphics_info.edge_buffer[i][1] >= kiss_g_graphics_info.width)
		   continue;

		min = kiss_g_graphics_info.edge_buffer[i][0];
		max = kiss_g_graphics_info.edge_buffer[i][1];

		if(min > max)
			kiss_graphics_swap(&min, &max);

		if(min < 0) min = 0;
		if(max >= kiss_g_graphics_info.width) max = kiss_g_graphics_info.width;

		for(j = min;j <= max;j++)
			kiss_g_graphics_info.data[pos+j] = (r<<16) | (g<<8) | b;
	}
}

inline void kiss_graphics_edge_buffer_pixel(int x, int y)
{
	if(y < 0) return;
	if(y >= kiss_g_graphics_info.height) return;

	if(kiss_g_graphics_info.edge_count[y] == 2) {
		if(kiss_g_graphics_info.edge_buffer[y][0] > kiss_g_graphics_info.edge_buffer[y][1]) {
			if(x < kiss_g_graphics_info.edge_buffer[y][1]) {
				kiss_g_graphics_info.edge_buffer[y][1] = x;
				return;
			}
			if(x > kiss_g_graphics_info.edge_buffer[y][0]) {
				kiss_g_graphics_info.edge_buffer[y][0] = x;
				return;
			}
		}
		else {
			if(x < kiss_g_graphics_info.edge_buffer[y][0]) {
				kiss_g_graphics_info.edge_buffer[y][0] = x;
				return;
			}
			if(x > kiss_g_graphics_info.edge_buffer[y][1]) {
				kiss_g_graphics_info.edge_buffer[y][1] = x;
				return;
			}
		}
		return;
	}
	kiss_g_graphics_info.edge_buffer[y][kiss_g_graphics_info.edge_count[y]++] = x;
}

inline void kiss_graphics_edge_buffer_line(int x1, int y1, int x2, int y2)
{
	int steep, deltax, deltay, error, ystep, x, y;

	if(abs(y2 - y1) > abs(x2 - x1))
		steep = 1;
	else steep = 0;

	if(steep) {
		kiss_graphics_swap(&x1, &y1);
		kiss_graphics_swap(&x2, &y2);
	}
	if(x1 > x2) {
		kiss_graphics_swap(&x1, &x2);
		kiss_graphics_swap(&y1, &y2);
	}

	deltax = x2 - x1;
	deltay = abs(y2 - y1);
	error = -(deltax + 1) >> 1;
	y = y1;

	if(y1 < y2) ystep = 1;
	else		ystep = -1;

	for(x = x1;x <= x2;x++) {
		if(steep) kiss_graphics_edge_buffer_pixel(y, x);
		else	  kiss_graphics_edge_buffer_pixel(x, y);
		error = error+deltay;
		if(error >= 0) {
			y = y+ystep;
			error = error - deltax;
		}
	}
}

/**********
 * Extras *
 **********/

inline void kiss_graphics_swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
