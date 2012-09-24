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

#ifndef _MACROSTRING_HPP_
#define _MACROSTRING_HPP_

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDate>

struct Macro
{
	virtual ~Macro() {}
	virtual QString macro(const QString& with) const = 0;
};

struct StringMacro : Macro
{
	StringMacro(const QString& string) : m_string(string) {}
	
	QString macro(const QString&) const
	{
		return m_string;
	}
private:
	QString m_string;
};

struct DateMacro : Macro
{
	QString macro(const QString&) const
	{
		return QDate::currentDate().toString(Qt::TextDate);
	}
};

struct MacroString : QMap<QString, Macro*>
{
	virtual ~MacroString()
	{
		while (!isEmpty()) { delete begin().value(); erase(begin()); }
	}
	
	QString apply(const QString& original) const
	{
		QString ret = original;
		foreach(const QString& key, keys())
			ret = ret.replace(key, value(key)->macro(original));
		return ret;
	}
	
	bool macroExists(const QString source, const QString& key, const QString& stop = "") const
	{
		if(!stop.isEmpty() && !source.contains(stop)) return false;
		QString meta = stop.isEmpty() ? source : source.section(stop, 0, 0);
		foreach(const QString& metaLine, meta.split("\n")) {
			QStringList parts;
			foreach(const QString& part, metaLine.split(" ")) parts << part.trimmed();
			if(parts[0] == key) return true;
		}
		return false;
	}
	
	QStringList macroArguments(const QString& source, const QString& key, const QString& stop = "") const
	{
		if(!stop.isEmpty() && !source.contains(stop)) return QStringList();
		QString meta = stop.isEmpty() ? source : source.section(stop, 0, 0);
		foreach(const QString& metaLine, meta.split("\n")) {
			QStringList parts;
			foreach(const QString& part, metaLine.split(" ")) parts << part.trimmed();
			
			if(parts[0] == key) return parts.mid(1);
		}
		return QStringList();
	}
	
	QString nonMeta(const QString& source, const QString& stop) const
	{
		return source.contains(stop) ? source.section(stop, 1, 1) : source;
	}
};

#endif
