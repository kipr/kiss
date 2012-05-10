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

#include "Singleton.h"

#include <QString>
#include <QStringList>
#include <QScriptValue>
#include <QDir>
#include <QMap>

class CompileResult
{
public:
	CompileResult(bool success, const QMap<QString, QStringList>& categorizedOutput);
	CompileResult(bool success);
	
	const bool success() const;
	const QStringList output(const QString& category) const;
	const QMap<QString, QStringList>& categorizedOutput() const;
	
	void addCompileResult(const CompileResult& rhs);
	CompileResult& operator+=(const CompileResult& rhs);
	CompileResult operator+(const CompileResult& rhs) const;
private:
	bool m_success;
	QMap<QString, QStringList> m_categorizedOutput;
};

class Compilation;
class Compiler;

class CompilerManager : public Singleton<CompilerManager>
{
public:
	void addCompiler(Compiler* compiler);
	void removeCompiler(Compiler* compiler);
	const QList<Compiler*>& compilers() const;
private:
	QList<Compiler*> m_compilers;
};

class Compiler
{
public:
	Compiler(const QString& name, const QStringList& types);
	
	const QString& name() const;
	const QStringList& types() const;
	
	virtual CompileResult compile(Compilation* compilation, const QStringList& files) = 0;
	
	static QDir rootOutputDirectory();
	QDir outputDirectory();
	
private:
	QString m_name;
	QStringList m_types;
};

class CompilerPlugin : public Compiler
{
public:
	CompilerPlugin(const QScriptValue& plugin);

	virtual CompileResult compile(Compilation* compilation, const QStringList& files);
	
private:
	QScriptValue m_plugin;
};

#endif
