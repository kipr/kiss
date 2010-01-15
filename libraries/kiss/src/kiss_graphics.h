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

#ifndef __KISS_GRAPHICS_H__
#define __KISS_GRAPHICS_H__

#include <GL/GLee.h>
#include <GL/glfw.h>

#include <stdint.h>

#ifndef GL_TEXTURE_RECTANGLE_EXT
#define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_RECTANGLE_NV
#endif

struct graphics_info {
	int width, height;
	
	int texture_target;
	int texture_wrap;
	int texture_format;
	int texture_type;
	

	uint32_t *data;
	int buffer_width;
	int buffer_height;
	float texture_width;
	float texture_height;
	
	short **edge_buffer;
	short *edge_count;

	GLuint texture;
	GLFWmutex mutex;
};

void kiss_graphics_open_window(int width, int height);
void kiss_graphics_close_window();
void kiss_graphics_update();

void kiss_graphics_create_buffer();
void kiss_graphics_destroy_buffer();

void kiss_graphics_setup_gl();

void kiss_graphics_lock();
void kiss_graphics_unlock();

extern volatile struct graphics_info kiss_g_graphics_info;

#endif
