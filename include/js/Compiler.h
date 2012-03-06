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

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <QString>
#include <QStringList>
#include <QScriptValue>

struct CompileResult
{
	CompileResult(bool success, const QStringList& verboseMessages, const QStringList& errorMessages,
		const QStringList& warningMessages, const QStringList& linkerMessages);
	
	const bool success;
	
	const QStringList verboseMessages;
	const QStringList errorMessages;
	const QStringList warningMessages;
	const QStringList linkerMessages;
};

class Compiler
{
public:
	Compiler(const QString& name, const QStringList& types);
	
	const QString& name() const;
	const QStringList& types() const;
	
	virtual CompileResult compile(const QStringList& files) = 0;
private:
	QString m_name;
	QStringList m_types;
};

class CompilerPlugin : public Compiler
{
public:
	CompilerPlugin(const QScriptValue& plugin);

	virtual CompileResult compile(const QStringList& files);
	
private:
	QScriptValue m_plugin;
};

#endif
