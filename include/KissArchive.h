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

#ifndef __KISSARCHIVE_H__
#define __KISSARCHIVE_H__

#include <QStringList>
#include <QIODevice>

#define KISS_ARCHIVE_VERSION 	1
#define KISS_ARCHIVE_FILE 	"installed"

struct KissReturn 
{
	KissReturn(bool error, const QString& message = "") : error(error), message(message) {}
	
	bool error;
	QString message;
};

class KissArchive 
{
public:
	static KissReturn create(const QString& name, unsigned version, const QStringList& platforms, const QStringList& files, QIODevice* out);
	static KissReturn install(QIODevice* in);
	static KissReturn uninstall(const QString& name);
	static const unsigned version(const QString& name);
	static QStringList installed();
	static QString osName();
private:
};

#endif