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

#include "ScriptEnvironment.h"

#include "KissScript.h"
#include "MainWindow.h"
#include "MessageDialog.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QScriptEngine>


#define MAIN_WINDOW_PROPERTY "window"

ScriptEnvironment::ScriptEnvironment(MainWindow* mainWindow) : m_engine(new QScriptEngine), m_kissScript(new KissScript)
{
	m_engine->globalObject().setProperty(MAIN_WINDOW_PROPERTY, m_engine->newQObject(mainWindow));
	m_engine->globalObject().setProperty("kiss", m_engine->newQObject(m_kissScript));
}

ScriptEnvironment::~ScriptEnvironment()
{
	delete m_kissScript;
	delete m_engine;
}

bool ScriptEnvironment::execute(const QString& path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly)) return false;
	
	QTextStream str(&file);
	m_engine->pushContext();
	m_engine->evaluate(str.readAll());
	m_engine->popContext();
	file.close();
	
	if(m_engine->hasUncaughtException()) {
		QScriptValue author = m_engine->globalObject().property("author").toString();
		QScriptValue contact = m_engine->globalObject().property("contact").toString();
		MessageDialog::showError(qobject_cast<MainWindow*>(m_engine->globalObject().property(MAIN_WINDOW_PROPERTY).toQObject()), 
			(author.toString() == "kipr" ? "kipr_script_exception" : "script_exception"),
			QStringList() << QFileInfo(path).fileName() << m_engine->uncaughtException().toString()
			<< QString::number(m_engine->uncaughtExceptionLineNumber()) << (!author.isValid() ? QObject::tr("No Author Given") : author.toString())
			<< (!contact.isValid() ? QObject::tr("No Contact Information Given") : contact.toString()));
		return false;
	}
	
	return true;
}