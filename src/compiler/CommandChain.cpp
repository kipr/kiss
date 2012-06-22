#include "CommandChain.h"
#include <QDebug>
#include <QBuffer>

ChainSession::ChainSession()
	: m_in(new QBuffer()),
	m_out(new QBuffer()),
	m_err(new QBuffer())
{
	m_in->open(QBuffer::ReadWrite);
	m_out->open(QBuffer::ReadWrite);
	m_err->open(QBuffer::ReadWrite);
}

ChainSession::~ChainSession()
{
	m_in->close();
	m_out->close();
	m_err->close();
	delete m_in;
	delete m_out;
	delete m_err;
}

QIODevice* ChainSession::in()
{
	return m_in;
}

QIODevice* ChainSession::out()
{
	return m_out;
}

QIODevice* ChainSession::err()
{
	return m_err;
}

ChainSegment::ChainSegment()
{
	
}

ChainSegment::~ChainSegment()
{
	
}

void ChainSegment::setSession(ChainSession* session)
{
	m_session = session;
}

ChainSession* ChainSegment::session()
{
	return m_session;
}

QThreadSegment::QThreadSegment(QThread* thread, bool parallel, bool transferOwnership)
	: m_thread(thread), m_transferOwnership(transferOwnership), m_parallel(parallel) {}

QThreadSegment::~QThreadSegment()
{
	if(m_transferOwnership) delete m_thread;
}

const bool QThreadSegment::run()
{
	if(running()) return false;
	m_thread->start();
	return true;
}

const bool QThreadSegment::running() const
{
	return m_thread->isRunning();
}

void QThreadSegment::finalize()
{
	
}

void QThreadSegment::join()
{
	m_thread->wait();
}

const bool QThreadSegment::parallel() const
{
	return m_parallel;
}

const bool QThreadSegment::isErrorState() const
{
	const ErrorState* errState = dynamic_cast<const ErrorState*>(m_thread);
	return errState ? errState->isErrorState() : false;
}

CommandChain::CommandChain(quint16 maxConcurrentSegments)
	: m_maxConcurrentSegments(maxConcurrentSegments),
	m_chainSession(0)
{
	
}

CommandChain::~CommandChain()
{
	drainExecuting();
	while(m_chain.size()) {
		ChainSegment* segment = m_chain.front();
		m_chain.pop_front();
		delete segment;
	}
	if(m_chainSession) delete m_chainSession;
}

quint16 CommandChain::maxConcurrentSegments()
{
	return m_maxConcurrentSegments;
}

void CommandChain::add(ChainSegment* segment)
{
	m_chain.push_back(segment);
}

bool CommandChain::execute()
{
	if(m_chainSession) delete m_chainSession;
	m_chainSession = new ChainSession();
	while(m_chain.size()) {
		if(!executeNextSegment()) {
			drainExecuting();
			return false;
		}
	}
	if(drainExecuting()) return false;
	return true;
}

const Chain& CommandChain::chain() const
{
	return m_chain;
}

const bool CommandChain::executeNextSegment()
{
	while(m_executing.size() >= m_maxConcurrentSegments) {
		if(updateExecuting()) return false;
		QThread::yieldCurrentThread();
	}
	ChainSegment* segment = m_chain.front();
	m_chain.pop_front();
	if(!segment->parallel()) if(drainExecuting()) return false;
	segment->setSession(m_chainSession);
	m_executing.push_back(segment);
	return segment->run();
}

const bool CommandChain::updateExecuting()
{
	if(!m_executing.size()) return true;
	
	bool err = false;
	Chain::iterator it = m_executing.begin();
	for(; it != m_executing.end(); ++it) {
		ChainSegment* segment = *it;
		if(!segment->running()) {
			segment->finalize();
			err |= segment->isErrorState();
			it = m_executing.erase(it); --it;
			delete segment;
		}
	}
	return err;
}

const bool CommandChain::drainExecuting()
{
	bool err = false;
	while(m_executing.size()) {
		ChainSegment* segment = m_executing.front();
		segment->join();
		segment->finalize();
		err |= segment->isErrorState();
		m_executing.pop_front();
		delete segment;
	}
	return err;
}

ChainSession* CommandChain::chainSession()
{
	return m_chainSession;
}

QProcessSegment::QProcessSegment(const QString& program, const QStringList& args, bool parallel)
	: m_program(program), m_args(args), m_parallel(parallel) {}

const bool QProcessSegment::isErrorState() const
{
	return m_process.exitStatus() != QProcess::NormalExit || m_process.exitCode() != 0;
}
	
const bool QProcessSegment::run()
{
	qDebug() << "Executing command" << m_program << "with" << m_args;
	m_process.start(m_program, m_args);
	return true;
}

void QProcessSegment::join()
{
	m_process.waitForStarted();
	m_process.waitForFinished();
}

const bool QProcessSegment::running() const
{
	return m_process.state() != QProcess::NotRunning;
}

const bool QProcessSegment::parallel() const
{
	return m_parallel;
}

void QProcessSegment::finalize()
{
	ChainSession* s = session();
	s->out()->write(m_process.readAllStandardOutput());
	s->err()->write(m_process.readAllStandardError());
}

QProcess* QProcessSegment::process()
{
	return &m_process;
}

TimeSegment::TimeSegment(long time) : m_time(time) {}

const bool TimeSegment::isErrorState() const
{
	return false;
}

const bool TimeSegment::run()
{
	timer.start();
	return true;
}

void TimeSegment::finalize()
{
	timer.invalidate();
}

void TimeSegment::join()
{
	while(running()) QThread::yieldCurrentThread();
}

const bool TimeSegment::running() const
{
	return !timer.hasExpired(m_time);
}

const bool TimeSegment::parallel() const
{
	return true;
}