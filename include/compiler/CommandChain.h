#ifndef _COMMANDCHAIN_H_
#define _COMMANDCHAIN_H_

#include <QThread>
#include <QIODevice>
#include <QProcess>
#include <QElapsedTimer>

class ChainSession
{
public:
	ChainSession();
	~ChainSession();
	
	QIODevice* in();
	QIODevice* out();
	QIODevice* err();
private:
	QIODevice* m_in;
	QIODevice* m_out;
	QIODevice* m_err;
};

class ErrorState
{
public:
	virtual const bool isErrorState() const = 0;
};

class CommandChain;

class ChainSegment : public ErrorState
{
public:
	ChainSegment();
	
	virtual ~ChainSegment();
	
	virtual const bool run() = 0;
	virtual const bool running() const = 0;
	virtual void join() = 0;
	virtual void finalize() = 0;
	
	virtual const bool parallel() const = 0;
	
	void setSession(ChainSession* session);
	ChainSession* session();
private:
	ChainSession* m_session;
};

class QThreadSegment : public ChainSegment
{
public:
	QThreadSegment(QThread* thread, bool parallel = true, bool transferOwnership = true);
	~QThreadSegment();
	
	virtual const bool run();
	virtual const bool running() const;
	virtual void join();
	virtual void finalize();
	
	virtual const bool parallel() const;
	virtual const bool isErrorState() const;
private:
	QThread* m_thread;
	bool m_transferOwnership;
	bool m_parallel;
};

typedef QList<ChainSegment*> Chain;

class CommandChain
{
public:
	CommandChain(quint16 maxConcurrentSegments);
	~CommandChain();
	
	quint16 maxConcurrentSegments();
	
	void add(ChainSegment* segment);
	
	bool execute();
	
	const Chain& chain() const;
	
	ChainSession* chainSession();
private:
	const bool executeNextSegment();
	const bool updateExecuting();
	const bool drainExecuting();
	
	quint16 m_maxConcurrentSegments;
	
	Chain m_chain;
	Chain m_executing;
	
	ChainSession* m_chainSession;
};

class QProcessSegment : public ChainSegment
{

public:
	QProcessSegment(const QString& program, const QStringList& args = QStringList(), bool parallel = true);
	virtual const bool isErrorState() const;
	virtual const bool run();
	virtual void join();
	virtual const bool running() const;
	virtual const bool parallel() const;
	virtual void finalize();
	
	QProcess* process();
private:
	const QString m_program;
	const QStringList m_args;
	bool m_parallel;
	QProcess m_process;
};

class TimeSegment : public ChainSegment
{
public:
	TimeSegment(long time);
	virtual const bool isErrorState() const;
	virtual const bool run();
	virtual void finalize();
	virtual void join();
	virtual const bool running() const;
	virtual const bool parallel() const;
private:
	long m_time;
	QElapsedTimer timer;
};

#endif
