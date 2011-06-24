#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include "ui_Debugger.h"
#include "DebuggerInterface.h"
#include <QWidget>

class SourceFile;

class Debugger : public QWidget, private Ui::Debugger, public Responder
{
	Q_OBJECT
public:
	Debugger(QWidget* parent = 0);
	~Debugger();
	
	void startDebug(DebuggerInterface* interface, SourceFile* starting);

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

private:
	DebuggerInterface* m_interface;
};

#endif