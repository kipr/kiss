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

#include "ScriptUtils.h"

#include <QScriptValueIterator>

QMap<QString, QScriptValue> ScriptUtils::objectMap(QScriptValue object)
{
	QMap<QString, QScriptValue> ret;
	QScriptValueIterator it(object);
	while(it.hasNext()) {
		it.next();
		ret[it.name()] = it.value();
	}
	return ret;
}

QMap<QString, QString> ScriptUtils::objectStringMap(QScriptValue object)
{
	QMap<QString, QString> ret;
	QScriptValueIterator it(object);
	while(it.hasNext()) {
		it.next();
		ret[it.name()] = it.value().toString();
	}
	return ret;
}

QScriptValue ScriptUtils::createArray(QScriptEngine* engine, QScriptValueList list)
{
	QScriptValue ret = engine->newObject();
	int i = 0;
	foreach(QScriptValue val, list) ret.setProperty(QString::number(i++), val);
	ret.setProperty("length", i);
	return ret;
}