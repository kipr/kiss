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
	
	virtual void stack(QList<Frame> frames) = 0;
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
	virtual void where() = 0;
	virtual void backtrace() = 0;
	virtual void value(const QString& variable) = 0;
	virtual void variables() = 0;
	virtual void threads() = 0;
	virtual QStringList libs() = 0;

protected:
	Responder* m_responder;
};

#endif