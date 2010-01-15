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

#ifndef __KISS_GRAPHICS_DRAW_H__
#define __KISS_GRAPHICS_DRAW_H__

#include "kiss_graphics.h"

inline void kiss_graphics_fill(int r, int g, int b);
inline void kiss_graphics_pixel(int x, int y, int r, int g, int b);
inline void kiss_graphics_line(int x1, int y1, int x2, int y2, int r, int g, int b);
inline void kiss_graphics_circle(int cx, int cy, int radius, int r, int g, int b);
inline void kiss_graphics_circle_fill(int cx, int cy, int radius, int r, int g, int b);
inline void kiss_graphics_rectangle(int x1, int y1, int x2, int y2, int r, int g, int b);
inline void kiss_graphics_rectangle_fill(int x1, int y1, int x2, int y2, int r, int g, int b);

inline void kiss_graphics_edge_buffer_clear();
inline void kiss_graphics_edge_buffer_paint(int r, int g, int b);
inline void kiss_graphics_edge_buffer_pixel(int x, int y);
inline void kiss_graphics_edge_buffer_line(int x1, int y1, int x2, int y2);

inline void kiss_graphics_swap(int *a, int *b);

#endif
