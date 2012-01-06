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

#ifndef _JAVASCRIPT_H_
#define _JAVASCRIPT_H_

#include <QFile>

#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

class JavaScript;

struct JavaScriptEnvironment
{
	virtual void prepare(JavaScript* engine) = 0;
};

class JavaScript
{
public:
	JavaScript(JavaScriptEnvironment* env);
	
	QScriptValue evaluate(QFile& file);
	QScriptValue evaluate(const QString& str);
	
	QScriptEngine* engine();
	
	static QMap<QString, QScriptValue> objectMap(QScriptValue object);
	static QMap<QString, QString> objectStringMap(QScriptValue object);
private:
	QScriptEngine m_engine;
	JavaScriptEnvironment* m_env;
};

#endif
