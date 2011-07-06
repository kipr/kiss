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

#include "SourceFileShared.h"
#include "MainWindow.h"

SourceFileShared& SourceFileShared::ref()
{
	static SourceFileShared instance;
	return instance;
}

const QPixmap& SourceFileShared::blackBullet() const 	{ return m_blackBullet; }
const QPixmap& SourceFileShared::blueBullet() const 	{ return m_blueBullet; }
const QPixmap& SourceFileShared::redBullet() const 	{ return m_redBullet; }
const QPixmap& SourceFileShared::yellowBullet() const 	{ return m_yellowBullet; }

Debugger* SourceFileShared::debugger() { return &m_debugger; }

SourceFileShared::SourceFileShared() :
	m_blackBullet(":/sourcefile/icon_set/icons/bullet_black.png"), 
	m_blueBullet(":/sourcefile/icon_set/icons/bullet_blue.png"), 
	m_redBullet(":/sourcefile/icon_set/icons/bullet_red.png"), 
	m_yellowBullet(":/sourcefile/icon_set/icons/bullet_yellow.png"),
	m_debugger(&MainWindow::ref())
{
	m_debugger.hide();
}

SourceFileShared::SourceFileShared(const SourceFileShared& rhs) { Q_UNUSED(rhs); }
