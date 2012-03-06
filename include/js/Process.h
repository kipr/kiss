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

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <QObject>
#include <QScriptable>
#include <QProcess>
#include <QScriptValue>

class Process : public QObject, public QScriptable
{
Q_OBJECT
Q_PROPERTY(QString program READ program WRITE setProgram)
Q_PROPERTY(QScriptValue environment READ environment WRITE setEnvironment)
Q_PROPERTY(QScriptValue systemEnvironment READ systemEnvironment)
Q_PROPERTY(int exitCode READ exitCode)
public:
	Process();
	Process(const QString& program);
	
	const QString& program() const;
	void setProgram(const QString& program);
	QScriptValue systemEnvironment() const;
	
public slots:
	void start(const QStringList& args);
	void terminate();
	
	QString stdout() const;
	QString stderr() const;
	
	QScriptValue environment() const;
	void setEnvironment(const QScriptValue& obj);
	
	void waitForFinished(int timeout);
	
	int exitCode() const;
	
signals:
	void started();
	void finished(int code, bool normalExit);
	void error(const QString& message);
	
private slots:
	void processFinished(int code, QProcess::ExitStatus status);
	void processError(QProcess::ProcessError err);
	
private:
	void init();
	
	QString m_program;
	
	QProcess* m_process;
};

#endif
