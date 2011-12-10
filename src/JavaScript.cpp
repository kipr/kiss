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

#include "JavaScript.h"

#include <QTextStream>

JavaScript::JavaScript(JavaScriptEnvironment* env) : m_env(env) { env->prepare(this); }

QScriptValue JavaScript::evaluate(QFile& file)
{
	if(!file.open(QIODevice::ReadOnly)) return 0;
	QString str = QTextStream(&file).readAll();
	file.close();
	return evaluate(str);
}

QScriptValue JavaScript::evaluate(const QString& str)
{
	QScriptValue ret = m_engine.evaluate(str);
	QScriptValue exception = m_engine.uncaughtException();
	return exception.isValid() ? exception : QScriptValue();
}

QScriptEngine* JavaScript::engine() { return &m_engine; }

QMap<QString, QScriptValue> JavaScript::objectMap(QScriptValue object)
{
	QMap<QString, QScriptValue> ret;
	QScriptValueIterator it(object);
	while(it.hasNext()) {
		it.next();
		ret[it.name()] = it.value();
	}
	return ret;
}

QMap<QString, QString> JavaScript::objectStringMap(QScriptValue object)
{
	QMap<QString, QString> ret;
	QScriptValueIterator it(object);
	while(it.hasNext()) {
		it.next();
		ret[it.name()] = it.value().toString();
	}
	return ret;
}