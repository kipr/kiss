#include "Debugger.h"

#include <QDebug>

Debugger::Debugger(QWidget* parent) : QWidget(parent, Qt::Tool), m_interface(0)
{
	setupUi(this);
}

Debugger::~Debugger()
{
	if(m_interface) delete m_interface;
}

void Debugger::on_ui_run_clicked()
{
	m_interface->run();
}

void Debugger::on_ui_pause_clicked()
{
	m_interface->pause();
}

void Debugger::on_ui_step_clicked()
{
	m_interface->step();
}

void Debugger::on_ui_stop_clicked()
{
	m_interface->stop();
}

void Debugger::on_ui_consoleEnter_clicked()
{
	m_interface->send(ui_consoleIn->text());
	ui_consoleIn->clear();
}

void Debugger::startDebug(DebuggerInterface* interface, SourceFile* starting)
{
	m_interface = interface;
	m_interface->setResponder(this);
	
	programStopped();
	
	show();
}

void Debugger::writeStdout(const QString& str)
{
	ui_consoleOut->append(str);
}

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

void Debugger::update()
{
	// qWarning() << m_interface << ui_variables << ui_libs;
	
	// ui_libs->clear();
	
	// ui_libs->addItems(m_interface->libs());
}

void Debugger::programStopped()
{
	ui_run->setEnabled(true);
	ui_step->setEnabled(false);
	ui_pause->setEnabled(false);
	ui_stop->setEnabled(false);
	
	ui_stack->clear();
}

void Debugger::programStarted()
{
	ui_run->setEnabled(false);
	ui_step->setEnabled(false);
	ui_pause->setEnabled(true);
	ui_stop->setEnabled(true);
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
			frame.function + "(" + vars.join(",") + ")");
	}
}

void Debugger::variables(const QList<Variable>& vars)
{
	ui_variables->clear();
	
	foreach(const Variable& var, vars) {
		ui_variables->addItem(var.name + " = " + var.value);
	}
}

void Debugger::breakpoints(const QList<Breakpoint>& bkpts)
{
	ui_breakpoints->clear();
	
	foreach(const Breakpoint& bkpt, bkpts) {
		ui_breakpoints->addItem(QString((bkpt.enabled ? tr("Enabled:") : tr("Disabled:"))) + " " + 
			bkpt.file + ":" + bkpt.line + "::" + bkpt.function);
	}
}
