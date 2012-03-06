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

#ifndef _KISSSCRIPT_H_
#define _KISSSCRIPT_H_

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QStringList>

class KissScript : public QObject, public QScriptable
{
Q_OBJECT
Q_PROPERTY(QString os READ os)
Q_PROPERTY(QString cwd READ cwd)
public:
	KissScript();
	
	QString os() const;
	QString cwd() const;
	
public slots:
	QScriptValue plugin(const QString& type, const QScriptValue& obj);
	void log(const QString& from, const QString& message);
	void log(const QString& message);
	
	void message(const QString& label, const QString& _template, const QStringList& args) const;
	void error(const QString& _template, const QStringList& args) const;
	
	QStringList availableImports() const;
	QStringList imported() const;
	QScriptValue use(const QString& package);
	
	QScriptValue tabs(const QString& name) const;
	
	QScriptValue associatedWindow() const;
	
	QScriptValue process();
	
private:
	
};

#endif
