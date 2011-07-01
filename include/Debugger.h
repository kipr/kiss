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

#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include "ui_Debugger.h"
#include "DebuggerInterface.h"

#include <QWidget>
#include <QCloseEvent>

class SourceFile;

class Debugger : public QWidget, private Ui::Debugger, public Responder
{
	Q_OBJECT
public:
	Debugger(QWidget* parent = 0);
	~Debugger();
	
	void startDebug(DebuggerInterface* interface);

	void writeStdout(const QString& str);
	void writeDebugState(const QString& str);
	void writeStderr(const QString& str);
	void update();
	
	void programStopped();
	void programStarted();
	void programPaused();
	void programStepped();
	
	void stack(const QList<Frame>& frames);
	void variables(const QList<Variable>& vars);
	void breakpoints(const QList<Breakpoint>& bkpts);
	
private slots:
	void on_ui_run_clicked();
	void on_ui_pause_clicked();
	void on_ui_step_clicked();
	void on_ui_stop_clicked();
	void on_ui_consoleEnter_clicked();
protected:
	void closeEvent(QCloseEvent *event);
private:
	
	DebuggerInterface* m_interface;
};

#endif