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

#include "Compiler.h"

#include <QVariant>
#include <QScriptEngine>

CompileResult::CompileResult(bool success, const QStringList& verboseMessages, const QStringList& errorMessages,
	const QStringList& warningMessages, const QStringList& linkerMessages)
	: success(success), verboseMessages(verboseMessages), errorMessages(errorMessages),
	warningMessages(warningMessages), linkerMessages(linkerMessages) {}

Compiler::Compiler(const QString& name, const QStringList& types) : m_name(name), m_types(types)
{
	
}

const QString& Compiler::name() const
{
	return m_name;
}

const QStringList& Compiler::types() const
{
	return m_types;
}

CompilerPlugin::CompilerPlugin(const QScriptValue& plugin)
	: Compiler(plugin.property("name").toString(), plugin.property("types").toVariant().toStringList()), m_plugin(plugin)
{
}

CompileResult CompilerPlugin::compile(const QStringList& files)
{
	QScriptValue ret = m_plugin.property("compile").call(QScriptValue(), QScriptValueList() << m_plugin.engine()->toScriptValue(files));
	
	return CompileResult(ret.property("success").toBool(), ret.property("verboseMessages").toVariant().toStringList(),
		ret.property("errorMessages").toVariant().toStringList(), ret.property("warningMessages").toVariant().toStringList(),
		ret.property("linkerMessages").toVariant().toStringList());
}