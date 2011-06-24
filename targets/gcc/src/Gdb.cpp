#include "Gdb.h"
#include <QDebug>

Gdb::Gdb(QString filename) : m_filename(filename), m_active(false), m_run(false)
{
	connect(&m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(gdbError(QProcess::ProcessError)));
	connect(&m_process, SIGNAL(readyRead()), this, SLOT(readyRead()));
	qWarning() << "--interpreter=mi" << filename << "-q";
	m_process.start("/usr/bin/gdb", QStringList() << "--interpreter=mi" << filename << "-q");
	qWarning() << "Started gdb";
}

Gdb::~Gdb()
{
	m_process.kill();
}

void Gdb::run()
{
	if(!m_run) {
		m_process.write("-exec-run\n");
		qWarning() << "run";
		m_run = true;
	} else {
		m_process.write("-exec-continue\n");
		qWarning() << "c";
	}
	m_active = true;
	
	m_responder->programStarted();
}

void Gdb::stop()
{
	pause();
	send("-exec-abort");
	m_run = false;
	
	m_libs.clear();
	
	m_responder->programStopped();
}

void Gdb::step()
{
	m_process.write("-exec-step\n");
	
	m_responder->programStepped();
}

void Gdb::pause()
{
	m_process.write("-exec-interrupt\n");
}

void Gdb::send(const QString& str)
{
	qWarning() << "Sending" << str;
	m_process.write((str + "\n").toAscii());
}

void Gdb::addBreakpoint(const QString& filename, const int lineNum)
{
	send("-break-insert " + filename + ":" + QString::number(lineNum));
}

void Gdb::breakpoints()
{
	send("-break-list");
}

void Gdb::where()
{
	send("where");
}

void Gdb::backtrace()
{
	send("-stack-list-frames");
	
}

void Gdb::value(const QString& variable)
{
	send("p " + variable);
}

void Gdb::variables()
{
	send("-stack-list-locals --all-values");
}

void Gdb::threads()
{

}

void Gdb::gdbError(QProcess::ProcessError err)
{
	qWarning() << "Error: " << err;
}

void Gdb::readyRead()
{
	QByteArray input = m_process.read(100000L);
	qWarning() << "GDB:" << input.data();
	if(	input.startsWith("=") || 
		input.startsWith("-") || 
		input.startsWith("~") || 
		input.startsWith("^") ||
		input.startsWith("*")) 
			parse(input);
	else {
		if(input.contains("(gdb)")) return;
		if(input.startsWith("&")) return;
		m_responder->writeStdout(input.data());
	}
}

void Gdb::parse(const QByteArray& input)
{
	qWarning() << "Parse!";
	if(input.startsWith("~")) m_responder->writeDebugState(cString(input.data(), 1));
	else if(input.startsWith("^error")) m_responder->writeStderr(cString(input.data(), after(input.data(), "msg=")));
	else if(input.startsWith("^done,stack=")) stack(input.data());
	else if(input.startsWith("^done,stack-args=")) stackArgs(input.data());
	else if(input.startsWith("^done,locals=")) locals(input.data());
	else if(input.startsWith("^done,BreakpointTable=")) breakpointTable(input.data());
	else if(input.startsWith("^done,bkpt="));
	else if(input.startsWith("^done")) m_responder->writeDebugState(cString(input.data(), after(input.data(), "reason=")));
	else if(input.startsWith("*stopped")) stopped(input.data());
	else if(input.startsWith("=shlibs-added")) m_libs += shlibsAdded(input.data());
	qWarning() << "Updating";
	m_responder->update();
}

QString Gdb::shlibsAdded(const QString& data)
{
	return cString(data, after(data, "path="));
}

void Gdb::stopped(const QString& data)
{
	qWarning() << "STOPPED!";
	QString runningTime = cString(data, after(data, "wallclock="));
	QString reason = cString(data, after(data, "reason="));
	m_responder->writeStdout("Program Stopped. Running Time: " + runningTime + ", Reason: " + reason + "\n");
	
	if(reason == "exited-normally") {
		m_run = false;
		m_libs.clear();
		m_responder->programStopped();
	} else {
		m_responder->programPaused();
	}
}

void Gdb::stack(const QString& data)
{
	QStringList frames = data.split("frame=");
	frames.removeFirst(); // Just the opening stuff
	foreach(const QString& frame, frames) {
		Frame f;
		qWarning() << frame;
		f.function = cString(frame, after(frame, "func="));
		f.file = cString(frame, after(frame, "file="));
		f.line = cString(frame, after(frame, "line=")).toInt();
		f.origin = cString(frame, after(frame, "shlibname="));
		qWarning() << f.function << f.file << f.line;
		m_frames.append(f);
	}
	
	send("-stack-list-arguments 1"); // Now we send out a query for the arguments
}

void Gdb::stackArgs(const QString& data)
{
	QStringList frames = data.split("frame=");
	frames.removeFirst(); // Just the opening stuff
	int i = 0;
	foreach(const QString& frame, frames) {
		QStringList variables = frame.split("name=");
		variables.removeFirst();
		
		foreach(const QString& variable, variables) {
			Variable v(cString(variable, 0), cString(variable, after(variable, "value=")));
			m_frames[i].variables.append(v);
			
		}
		++i;
	}
	
	m_responder->stack(m_frames);
	m_frames.clear();
}

void Gdb::locals(const QString& data)
{
	QStringList vars = data.split("name=");
	vars.removeFirst();
	QList<Variable> localList;
	foreach(const QString& var, vars) {
		Variable local(cString(var, 0), cString(var, after(var, "value=")));
		localList.append(local);
	}
	m_responder->variables(localList);
}

void Gdb::breakpointTable(const QString& data)
{
	QStringList bkpts = data.split("bkpt=");
	bkpts.removeFirst();
	QList<Breakpoint> bs;
	foreach(const QString& bkpt, bkpts) {
		Breakpoint b;
		b.file = cString(bkpt, after(bkpt, "file="));
		b.function = cString(bkpt, after(bkpt, "func="));
		b.line = cString(bkpt, after(bkpt, "line=")).toInt();
		b.enabled = cString(bkpt, after(bkpt, "enabled=")).contains("y") ? true : false; 
	}
	m_responder->breakpoints(bs);
}

QString Gdb::cString(const QString& data, int starting)
{
	if(data[starting].toAscii() != '\"') return QString();
	int ending = data.indexOf("\"", starting + 1);
	if(ending == -1) return QString();
	QString ret = data.mid(starting + 1, ending - starting - 1);
	int i = -1;
	while((i = ret.indexOf("\\")) != -1) {
		ret.replace(i, 2, "");
	}
	return ret;
}

int Gdb::after(const QString& data, const QString& key)
{
	int i = data.indexOf(key);
	return key < 0 ? -1 : (i + key.length()); 	
}

QStringList Gdb::libs()
{
	return m_libs;
}