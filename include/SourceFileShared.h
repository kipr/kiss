/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
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

#ifndef _SOURCEFILESHARED_H_
#define _SOURCEFILESHARED_H_

#include "Debugger.h"
#include "Singleton.h"

#include <QPixmap>

class SourceFileShared : public Singleton<SourceFileShared>
{
public:	
	SourceFileShared();
	
	const QPixmap& blackBullet() const;
	const QPixmap& blueBullet() const;
	const QPixmap& redBullet() const; 
	const QPixmap& yellowBullet() const;
	Debugger* debugger();
private:
	QPixmap m_blackBullet;
	QPixmap m_blueBullet;
	QPixmap m_redBullet;
	QPixmap m_yellowBullet;
	
	Debugger m_debugger;
};

#endif
