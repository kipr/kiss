/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
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

#ifndef __KISS_H__
#define __KISS_H__

#define LEXER_FOLDER "lexers"
#define TARGET_FOLDER "targets"
#define TARGET_EXT "target"

#ifdef Q_OS_WIN
#define OS_NAME "win"
#define OS_LIB_EXT "dll"
#elif defined(Q_OS_MAC)
#define OS_NAME "osx"
#define OS_LIB_EXT "dylib"
#else
#define OS_NAME "nix"
#define OS_LIB_EXT "so"
#endif

#define KISS_C_VERSION_MAJOR 3
#define KISS_C_VERSION_MINOR 0
#define KISS_C_VERSION_BUILD 0

#endif