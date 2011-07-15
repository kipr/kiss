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

#ifndef __DEBUGGER_INTERFACE_H__
#define __DEBUGGER_INTERFACE_H__

#include <QString>
#include <QStringList>
#include <QDataStream>
#include <QObject>

struct Location
{
	Location(QString file, int line) : file(file), line(line) {}
	
	QString file;
	int line;
};

struct Variable
{
	Variable(QString name, QString value) : name(name), value(value) {}
	
	QString name;
	QString value;
};

struct Breakpoint 
{
	QString function;
	QString file;
	int line;
	
	bool enabled;
};

struct Frame
{
	QString function;
	QString file;
	int line;
	QString origin;
	
	QList<Variable> variables;
};

struct Responder {
	virtual ~Responder() {}
	
	virtual void writeStdout(const QString& str) = 0;
	virtual void writeDebugState(const QString& str) = 0;
	virtual void writeStderr(const QString& str) = 0;
	virtual void update() = 0;
	
	virtual void programStopped() = 0;
	virtual void programStarted() = 0;
	virtual void programPaused() = 0;
	virtual void programStepped() = 0;
	
	virtual void stack(const QList<Frame>& frames) = 0;
	virtual void variables(const QList<Variable>& vars) = 0;
	virtual void breakpoints(const QList<Breakpoint>& bkpts) = 0;
};

class DebuggerInterface
{
public:
	DebuggerInterface() : m_responder(0) {}
	virtual ~DebuggerInterface() {}
	
	void setResponder(Responder* resp) { m_responder = resp; }
	
	virtual void run() = 0;
	virtual void stop() = 0;
	virtual void step() = 0;
	virtual void pause() = 0;
	
	virtual void send(const QString& str) = 0;
	virtual void addBreakpoint(const QString& filename, const int lineNum) = 0;
	virtual void breakpoints() = 0;
	virtual void backtrace() = 0;
	virtual void value(const QString& variable) = 0;
	virtual void variables() = 0;
	virtual void threads() = 0;
	virtual QStringList libs() = 0;

protected:
	Responder* m_responder;
};

#endif