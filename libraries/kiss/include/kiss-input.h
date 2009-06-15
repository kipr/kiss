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

#ifndef __KISS_INPUT_H__
#define __KISS_INPUT_H__


//These are just re-named from the glfw.h file for uniformity

#define KISS_KEY_UNKNOWN      -1
#define KISS_KEY_SPACE        32
#define GLFW_KEY_SPECIAL      256
#define KISS_KEY_ESC          (GLFW_KEY_SPECIAL+1)
#define KISS_KEY_F1           (GLFW_KEY_SPECIAL+2)
#define KISS_KEY_F2           (GLFW_KEY_SPECIAL+3)
#define KISS_KEY_F3           (GLFW_KEY_SPECIAL+4)
#define KISS_KEY_F4           (GLFW_KEY_SPECIAL+5)
#define KISS_KEY_F5           (GLFW_KEY_SPECIAL+6)
#define KISS_KEY_F6           (GLFW_KEY_SPECIAL+7)
#define KISS_KEY_F7           (GLFW_KEY_SPECIAL+8)
#define KISS_KEY_F8           (GLFW_KEY_SPECIAL+9)
#define KISS_KEY_F9           (GLFW_KEY_SPECIAL+10)
#define KISS_KEY_F10          (GLFW_KEY_SPECIAL+11)
#define KISS_KEY_F11          (GLFW_KEY_SPECIAL+12)
#define KISS_KEY_F12          (GLFW_KEY_SPECIAL+13)
#define KISS_KEY_F13          (GLFW_KEY_SPECIAL+14)
#define KISS_KEY_F14          (GLFW_KEY_SPECIAL+15)
#define KISS_KEY_F15          (GLFW_KEY_SPECIAL+16)
#define KISS_KEY_F16          (GLFW_KEY_SPECIAL+17)
#define KISS_KEY_F17          (GLFW_KEY_SPECIAL+18)
#define KISS_KEY_F18          (GLFW_KEY_SPECIAL+19)
#define KISS_KEY_F19          (GLFW_KEY_SPECIAL+20)
#define KISS_KEY_F20          (GLFW_KEY_SPECIAL+21)
#define KISS_KEY_F21          (GLFW_KEY_SPECIAL+22)
#define KISS_KEY_F22          (GLFW_KEY_SPECIAL+23)
#define KISS_KEY_F23          (GLFW_KEY_SPECIAL+24)
#define KISS_KEY_F24          (GLFW_KEY_SPECIAL+25)
#define KISS_KEY_F25          (GLFW_KEY_SPECIAL+26)
#define KISS_KEY_UP           (GLFW_KEY_SPECIAL+27)
#define KISS_KEY_DOWN         (GLFW_KEY_SPECIAL+28)
#define KISS_KEY_LEFT         (GLFW_KEY_SPECIAL+29)
#define KISS_KEY_RIGHT        (GLFW_KEY_SPECIAL+30)
#define KISS_KEY_LSHIFT       (GLFW_KEY_SPECIAL+31)
#define KISS_KEY_RSHIFT       (GLFW_KEY_SPECIAL+32)
#define KISS_KEY_LCTRL        (GLFW_KEY_SPECIAL+33)
#define KISS_KEY_RCTRL        (GLFW_KEY_SPECIAL+34)
#define KISS_KEY_LALT         (GLFW_KEY_SPECIAL+35)
#define KISS_KEY_RALT         (GLFW_KEY_SPECIAL+36)
#define KISS_KEY_TAB          (GLFW_KEY_SPECIAL+37)
#define KISS_KEY_ENTER        (GLFW_KEY_SPECIAL+38)
#define KISS_KEY_BACKSPACE    (GLFW_KEY_SPECIAL+39)
#define KISS_KEY_INSERT       (GLFW_KEY_SPECIAL+40)
#define KISS_KEY_DEL          (GLFW_KEY_SPECIAL+41)
#define KISS_KEY_PAGEUP       (GLFW_KEY_SPECIAL+42)
#define KISS_KEY_PAGEDOWN     (GLFW_KEY_SPECIAL+43)
#define KISS_KEY_HOME         (GLFW_KEY_SPECIAL+44)
#define KISS_KEY_END          (GLFW_KEY_SPECIAL+45)
#define KISS_KEY_KP_0         (GLFW_KEY_SPECIAL+46)
#define KISS_KEY_KP_1         (GLFW_KEY_SPECIAL+47)
#define KISS_KEY_KP_2         (GLFW_KEY_SPECIAL+48)
#define KISS_KEY_KP_3         (GLFW_KEY_SPECIAL+49)
#define KISS_KEY_KP_4         (GLFW_KEY_SPECIAL+50)
#define KISS_KEY_KP_5         (GLFW_KEY_SPECIAL+51)
#define KISS_KEY_KP_6         (GLFW_KEY_SPECIAL+52)
#define KISS_KEY_KP_7         (GLFW_KEY_SPECIAL+53)
#define KISS_KEY_KP_8         (GLFW_KEY_SPECIAL+54)
#define KISS_KEY_KP_9         (GLFW_KEY_SPECIAL+55)
#define KISS_KEY_KP_DIVIDE    (GLFW_KEY_SPECIAL+56)
#define KISS_KEY_KP_MULTIPLY  (GLFW_KEY_SPECIAL+57)
#define KISS_KEY_KP_SUBTRACT  (GLFW_KEY_SPECIAL+58)
#define KISS_KEY_KP_ADD       (GLFW_KEY_SPECIAL+59)
#define KISS_KEY_KP_DECIMAL   (GLFW_KEY_SPECIAL+60)
#define KISS_KEY_KP_EQUAL     (GLFW_KEY_SPECIAL+61)
#define KISS_KEY_KP_ENTER     (GLFW_KEY_SPECIAL+62)

#define KISS_MOUSE_BUTTON_1 0
#define KISS_MOUSE_BUTTON_2 1
#define KISS_MOUSE_BUTTON_3 2

#define a_button() !!kiss_get_key_bit('A')
#define b_button() !!kiss_get_key_bit('B')
#define l_button() !!kiss_get_key_bit('L')
#define r_button() !!kiss_get_key_bit('R')

#define up_button() !!kiss_get_key_bit(KISS_KEY_UP)
#define down_button() !!kiss_get_key_bit(KISS_KEY_DOWN)
#define left_button() !!kiss_get_key_bit(KISS_KEY_LEFT)
#define right_button() !!kiss_get_key_bit(KISS_KEY_RIGHT)

#define start_button() a_button()
#define stop_button() b_button()

#define kiss_get_key(x) kiss_get_key_bit(x)

int kiss_get_key_bit(int key);

void kiss_get_mouse_position(int *x, int *y);
int  kiss_get_mouse_button(int button);
int  kiss_get_mouse_wheel();
void kiss_grab_mouse();
void kiss_release_mouse();

#endif
