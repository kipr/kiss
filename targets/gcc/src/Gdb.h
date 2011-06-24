#ifndef __GDB_H__
#define __GDB_H__

#include <QObject>
#include <QThread>
#include <QProcess>
#include <QList>
#include "DebuggerInterface.h"

class Gdb : public QObject, public DebuggerInterface 
{
	Q_OBJECT
public:
	Gdb(QString filename);
	~Gdb();

	void run();
	void stop();
	void step();
	void pause();

	void send(const QString& str);
	void addBreakpoint(const QString& filename, const int lineNum);
	void breakpoints();
	void where();
	void backtrace();
	void value(const QString& variable);
	void variables();
 	void threads();

	QStringList libs();
signals:
	void error(QString error);
	
private slots:
	void gdbError(QProcess::ProcessError err);
	void readyRead();
	
private:
	void parse(const QByteArray& input);
	QString shlibsAdded(const QString& data);
	void stopped(const QString& data);
	void stack(const QString& data);
	void stackArgs(const QString& data);
	void locals(const QString& data);
	void bkpt(const QString& data);
	void breakpointTable(const QString& data);
	
	QString cString(const QString& data, int starting);
	int after(const QString& data, const QString& key);
	
	QString m_filename;
	QProcess m_process;
	bool m_active;
	bool m_run;
	QStringList m_libs;
	
	QList<Frame> m_frames;
};

#endif