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

#include "Temporary.h"

#include <QVariant>
#include <QScriptEngine>
#include <QDebug>

CompileResult::CompileResult(bool success, const QMap<QString, QStringList>& categorizedOutput, const QString& raw)
	: m_success(success), m_categorizedOutput(categorizedOutput), m_raw(raw)
{
	
}

CompileResult::CompileResult(bool success) : m_success(success)
{
	
}

const bool CompileResult::success() const
{
	return m_success;
}

const QString& CompileResult::raw() const
{
	return m_raw;
}

const QStringList CompileResult::output(const QString& category) const
{
	return m_categorizedOutput.value(category);
}

const QMap<QString, QStringList>& CompileResult::categorizedOutput() const
{
	return m_categorizedOutput;
}

void CompileResult::clear()
{
	m_categorizedOutput.clear();
}

void CompileResult::addCompileResult(const CompileResult& rhs)
{
	const QMap<QString, QStringList>& other = rhs.categorizedOutput();
	m_categorizedOutput.unite(other);
	foreach(const QString& category, m_categorizedOutput.uniqueKeys()) {
		const QList<QStringList>& values = m_categorizedOutput.values(category);
		qDebug() << "Uniting" << values;
		QStringList res;
		foreach(const QStringList& list, values) res << list;
		m_categorizedOutput.remove(category);
		qDebug() << "Done" << res;
		m_categorizedOutput[category] = res;
	}
	m_success &= rhs.success();
	m_raw += rhs.raw();
}

CompileResult& CompileResult::operator+=(const CompileResult& rhs)
{
	addCompileResult(rhs);
	return *this;
}

CompileResult CompileResult::operator+(const CompileResult& rhs) const
{
	CompileResult ret(m_success);
	ret += *this;
	ret += rhs;
	return ret;
}

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

QDir Compiler::rootOutputDirectory()
{
	QDir d = Temporary::subdir("__compiler_output");
	if(!d.exists()) QDir().mkpath(d.path());
	return d;
}

QDir Compiler::outputDirectory()
{
	QDir d = rootOutputDirectory();
	d.mkdir(m_name);
	d.cd(m_name);
	return d;
}

CompilerPlugin::CompilerPlugin(const QScriptValue& plugin)
	: Compiler(plugin.property("name").toString(), plugin.property("types").toVariant().toStringList()), m_plugin(plugin)
{
	if(m_plugin.property("compile").isUndefined()) {
		qCritical() << "Compiler plugin" << name() << "does not have \"compile\" property.";
	}
}

CompileResult CompilerPlugin::compile(Compilation* compilation, const QStringList& files)
{
	QScriptValue ret = m_plugin.property("compile").call(QScriptValue(), QScriptValueList() << m_plugin.engine()->toScriptValue(files));
	
	QMap<QString, QStringList> a;
	return CompileResult(false, a);
}