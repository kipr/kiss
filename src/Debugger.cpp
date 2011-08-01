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

#include "Debugger.h"

#include <QDebug>

Debugger::Debugger(QWidget* parent) : QWidget(parent, Qt::Tool), m_interface(0) { setupUi(this); }

void Debugger::on_ui_run_clicked() { m_interface->run(); }
void Debugger::on_ui_pause_clicked() { m_interface->pause(); }
void Debugger::on_ui_step_clicked() { m_interface->step(); }
void Debugger::on_ui_stop_clicked() { m_interface->stop(); }

void Debugger::on_ui_consoleEnter_clicked()
{
	m_interface->send(ui_consoleIn->text());
	ui_consoleIn->clear();
}

void Debugger::startDebug(DebuggerInterface* interface)
{
	if(m_interface) {
		qWarning() << "Deleted during start.";
		m_interface->stop();
		delete m_interface;
		m_interface = 0;
	}
	
	m_interface = interface;
	m_interface->setResponder(this);
	qWarning() << m_interface;
	
	programStopped();
	
	ui_consoleOut->clear();
	ui_consoleIn->clear();
	
	show();
}

void Debugger::writeStdout(const QString& str) { ui_consoleOut->append(str); }

void Debugger::writeDebugState(const QString& str)
{
	QColor color = ui_consoleOut->textColor();
	ui_consoleOut->setTextColor(QColor("darkGreen"));
	ui_consoleOut->append(str);
	ui_consoleOut->setTextColor(color);	
}

void Debugger::writeStderr(const QString& str)
{
	QColor color = ui_consoleOut->textColor();
	ui_consoleOut->setTextColor(QColor("red"));
	ui_consoleOut->append(str);
	ui_consoleOut->setTextColor(color);
}

void Debugger::update() {}

void Debugger::programStopped()
{
	ui_run->setEnabled(true);
	ui_step->setEnabled(false);
	ui_pause->setEnabled(false);
	ui_stop->setEnabled(false);
	
	ui_stack->clear();
	ui_variables->clear();
}

void Debugger::programStarted()
{
	ui_run->setEnabled(false);
	ui_step->setEnabled(false);
	ui_pause->setEnabled(true);
	ui_stop->setEnabled(true);
	
	ui_stack->clear();
	ui_variables->clear();
}

void Debugger::programPaused()
{
	ui_run->setEnabled(true);
	ui_step->setEnabled(true);
	ui_pause->setEnabled(false);
	ui_stop->setEnabled(true);
	
	m_interface->backtrace();
	m_interface->variables();
	m_interface->breakpoints();
}

void Debugger::programStepped()
{
	ui_run->setEnabled(true);
	ui_step->setEnabled(true);
	ui_pause->setEnabled(false);
	ui_stop->setEnabled(true);
}

void Debugger::stack(const QList<Frame>& frames)
{
	ui_stack->clear();
	
	foreach(const Frame& frame, frames) {
		QStringList vars;
		foreach(const Variable& var, frame.variables) {
			vars.append(var.name + " = " + var.value);
		}
		
		ui_stack->addItem(frame.file + ":" + 
			QString::number(frame.line) + "::" + 
			frame.function + "(" + vars.join(", ") + ")");
	}
}

void Debugger::variables(const QList<Variable>& vars)
{
	ui_variables->clear();
	
	foreach(const Variable& var, vars) {
		ui_variables->addItem(var.name + " = " + var.value);
	}
}

void Debugger::breakpoints(const QList<Breakpoint>& bkpts) {}

void Debugger::closeEvent(QCloseEvent* event)
{
	qWarning() << m_interface;
	if(m_interface) {
		qWarning() << "Deleted during close.";
		m_interface->setResponder(0);
		m_interface->stop();
		delete m_interface;
		m_interface = 0;
	}
	event->accept();
}