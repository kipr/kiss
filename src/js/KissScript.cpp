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

#include "KissScript.h"

#include "Compilers.h"
#include "Compiler.h"

#include "Process.h"
#include "MessageDialog.h"
#include "MainWindow.h"

#include "ScriptUtils.h"
#include "Kiss.h"

#include <iostream>
#include <QScriptEngine>

#include <QDebug>
#include <QDir>

KissScript::KissScript()
{
	
}

QString KissScript::os() const
{
	return OS_NAME;
}

QString KissScript::cwd() const
{
	return QDir::currentPath();
}

QScriptValue KissScript::plugin(const QString& type, const QScriptValue& obj)
{
	if(type == "compiler") {
		Compilers::ref().addCompiler(new CompilerPlugin(obj));
		return true;
	}
	if(type == "lexer") {
		qWarning() << "Lexer plugins are NYI";
		return false;
	}
	if(type == "targets") {
		
	}
	return false;
}

void KissScript::log(const QString& from, const QString& message)
{
	std::cout << "Message from plugin " << from.toStdString() << ": " << message.toStdString() << std::endl;
}

void KissScript::log(const QString& message)
{
	std::cout << message.toStdString() << std::endl;
}

void KissScript::message(const QString& label, const QString& _template, const QStringList& args) const
{
	MessageDialog::showMessage(0, label, _template, args);
}

void KissScript::error(const QString& _template, const QStringList& args) const
{
	MessageDialog::showError(0, _template, args);
}

QStringList KissScript::availableImports() const
{
	return engine()->availableExtensions();
}

QStringList KissScript::imported() const
{
	return engine()->importedExtensions();
}

QScriptValue KissScript::use(const QString& package)
{
	return engine()->importExtension(package);
}

QScriptValue KissScript::tabs(const QString& name) const
{
	if(!engine()) return QScriptValue();
	QScriptValue ret = engine()->newObject();
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(associatedWindow().toQObject());
	QList<QObject*> tabs = mainWindow->tabs(name);
	QScriptValueList list;
	foreach(QObject* tab, tabs) list << engine()->newQObject(tab, QScriptEngine::QtOwnership);
	
	return ScriptUtils::createArray(engine(), list);
}

QScriptValue KissScript::associatedWindow() const
{
	return engine()->globalObject().property("window");
}

QScriptValue KissScript::process()
{
	return engine() ? engine()->newQObject(new Process, QScriptEngine::ScriptOwnership) : QScriptValue();
}