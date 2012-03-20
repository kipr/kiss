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

#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <QString>
#include <QIODevice>
#include <QObject>

class Project : public QObject
{
Q_OBJECT
public:	
	bool addFile(const QString& path);
	bool createFile(const QString& path);
	const QString& setName(const QString& name);
	
	void refresh();
	
	QStringList files() const;
	
	QStringList resources() const;
	QStringList sources() const;
	
	QString outputPath() const;
	
	const QString& name();
	
	static Project* load(const QString& path);
	static void save(Project* project, const QString& path);
	static Project* create(const QString& path);
	
signals:
	void updated();
	
protected:
	Project(const QString& name);
	
private:
	QString m_path;
};

#endif
