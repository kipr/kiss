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

#include "kiss_graphics.h"

#include <stdlib.h>

volatile struct graphics_info kiss_g_graphics_info;

void kiss_graphics_open_window(int width, int height)
{
	int major, minor, revision;
  
	kiss_g_graphics_info.width=width;
	kiss_g_graphics_info.height=height;	

	glfwOpenWindow(width, height, 5, 6, 5, 0, 0, 0, GLFW_WINDOW);
  
	glfwGetGLVersion(&major, &minor, &revision);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
  
	if(glfwExtensionSupported("GL_EXT_texture_rectangle") || glfwExtensionSupported("GL_NV_texture_rectangle"))
		kiss_g_graphics_info.texture_target=GL_TEXTURE_RECTANGLE_EXT;
	else
		kiss_g_graphics_info.texture_target=GL_TEXTURE_2D;
	
	if(major == 1 && minor == 1) {
		printf("OpenGL Compatibility mode\n");
		kiss_g_graphics_info.texture_format = GL_RGBA;
		kiss_g_graphics_info.texture_type = GL_UNSIGNED_BYTE;
		kiss_g_graphics_info.texture_wrap = GL_CLAMP;
	} 
	else {
		kiss_g_graphics_info.texture_format = GL_BGRA;
		kiss_g_graphics_info.texture_type = GL_UNSIGNED_INT_8_8_8_8_REV;
		kiss_g_graphics_info.texture_wrap = GL_CLAMP_TO_EDGE;
	}
	  
	
	kiss_graphics_create_buffer();
	kiss_graphics_setup_gl();

	kiss_g_graphics_info.mutex = glfwCreateMutex();
	
}

void kiss_graphics_close_window()
{
	glfwCloseWindow();
	kiss_graphics_destroy_buffer();
	glfwDestroyMutex(kiss_g_graphics_info.mutex);
}

void kiss_graphics_update()
{
	GLenum target = kiss_g_graphics_info.texture_target;
	
	glTexSubImage2D(target, 0, 0, 0, 
					kiss_g_graphics_info.buffer_width, 
					kiss_g_graphics_info.buffer_height, 
					kiss_g_graphics_info.texture_format, 
					kiss_g_graphics_info.texture_type, 
					(void *)kiss_g_graphics_info.data);
					
	glBegin(GL_QUADS);
	  glTexCoord2f(0,0);
	  glVertex2f(0, kiss_g_graphics_info.height);
	  
	  
	  glTexCoord2f(kiss_g_graphics_info.texture_width, 0);
	  glVertex2f(kiss_g_graphics_info.width,
               kiss_g_graphics_info.height);
	  
	  
	  glTexCoord2f(kiss_g_graphics_info.texture_width,
                 kiss_g_graphics_info.texture_height);
	  glVertex2f(kiss_g_graphics_info.width, 0);
    
    
      glTexCoord2f(0, kiss_g_graphics_info.texture_height);
      glVertex2f(0,0);
	glEnd();
	
}

void kiss_graphics_create_buffer()
{
	int width, height;
	long bwidth, bheight;
	int i;

	width = kiss_g_graphics_info.width;
	height = kiss_g_graphics_info.height;

	if(kiss_g_graphics_info.texture_target != GL_TEXTURE_2D) {
		bwidth = width;
		bheight = height;
	}
	else {
		bwidth=2;
		bheight=2;
	
		while(width > bwidth) bwidth *= 2;
		while(height > bheight) bheight *= 2;
	}
	
	kiss_g_graphics_info.data = malloc(bwidth*bheight*4);
	
	kiss_g_graphics_info.edge_buffer = (short**)malloc(height*sizeof(short*));
	for(i = 0;i < height; i++)
		kiss_g_graphics_info.edge_buffer[i] = (short*)malloc(2*sizeof(short));
	
	kiss_g_graphics_info.edge_count = (int*)malloc(height*sizeof(int));
	
	kiss_g_graphics_info.buffer_width = bwidth;
	kiss_g_graphics_info.buffer_height = bheight;
	
	if(bwidth != width || bheight != height) {
	  kiss_g_graphics_info.texture_width = ((float)width)/((float)bwidth);
	  kiss_g_graphics_info.texture_height = ((float)height)/((float)bheight);
	}
	else {
	  kiss_g_graphics_info.texture_width = width;
	  kiss_g_graphics_info.texture_height = height;
	}
}

void kiss_graphics_destroy_buffer()
{
	int i;
	if(kiss_g_graphics_info.data)
		free((void *)kiss_g_graphics_info.data);
	if(kiss_g_graphics_info.edge_count)
		free((void *)kiss_g_graphics_info.edge_count);
		
	for(i = 0;i < kiss_g_graphics_info.height;i++)
		free((void*)kiss_g_graphics_info.edge_buffer[i]);
	free((void*)kiss_g_graphics_info.edge_buffer);
		
	kiss_g_graphics_info.edge_count = 0;
	kiss_g_graphics_info.edge_buffer = 0;
	kiss_g_graphics_info.data = 0;
}

void kiss_graphics_setup_gl()
{
	GLenum target;
	
	target = kiss_g_graphics_info.texture_target;
	
	glGenTextures(1, (GLuint *)&kiss_g_graphics_info.texture);
	glBindTexture(target, kiss_g_graphics_info.texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(target, GL_TEXTURE_PRIORITY, 0);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, kiss_g_graphics_info.texture_wrap);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, kiss_g_graphics_info.texture_wrap);
	glTexImage2D(target, 0, GL_RGBA, kiss_g_graphics_info.buffer_width, kiss_g_graphics_info.buffer_height,
							0, kiss_g_graphics_info.texture_format, kiss_g_graphics_info.texture_type, (void*)kiss_g_graphics_info.data);
	glEnable(target);
	glBindTexture(target, kiss_g_graphics_info.texture);
	
	return;
}

void kiss_graphics_lock()
{
	glfwLockMutex(kiss_g_graphics_info.mutex);
}

void kiss_graphics_unlock()
{
	glfwUnlockMutex(kiss_g_graphics_info.mutex);
}

