#include "communication_manager.hpp"

#include <QThreadPool>
#include <QDebug>

#include <QObject>

using namespace kiss;
using namespace kiss::target;

// TODO: Try QThread instead of QRunnable for weird time issues

CommunicationWorker::CommunicationWorker(const CommunicationEntryPtr &entry)
	: m_entry(entry)
{
}

void CommunicationWorker::run()
{
	fprintf(stderr, "Entry %llu executing...\n", m_entry->id());
	const Target::ReturnCode success = m_entry->execute();
	fprintf(stderr, "Entry %llu finished!\n", m_entry->id());
	// qDebug() << "Entry" << m_entry->id() << (!success ? "failed." : "finished!");
	emit finished(m_entry, success);
}

CommunicationManager::CommunicationManager()
	: m_paused(false)
	, m_id(0)
{
	qRegisterMetaType<CommunicationEntryPtr>("CommunicationEntryPtr");
}

CommunicationManager::~CommunicationManager()
{
}

bool CommunicationManager::isIdle() const
{
	m_queueMutex.lock();
	bool ret = m_running.isEmpty() && m_queue.isEmpty();
	m_queueMutex.unlock();
	return ret;
}

bool CommunicationManager::isPaused() const
{
	return m_paused;
}

void CommunicationManager::setPaused(const bool pause)
{
	m_paused = pause;
	saturate();
}

void CommunicationManager::clearQueue()
{
	m_queueMutex.lock();
	m_queue.clear();
	m_queueMutex.unlock();
	emit queueFinished();
}

void CommunicationManager::clearQueue(const TargetPtr &target)
{
	if(target.isNull()) return;
	m_queueMutex.lock();
	CommunicationQueue::iterator it = m_queue.begin();
	for(; it != m_queue.end(); ++it) {
		if((*it)->target().isNull()) continue;
		if((*it)->target() == target) {
			emit finished(*it, Target::Error);
			it = m_queue.erase(it);
		}
	}
	m_queueMutex.unlock();
}

quint64 CommunicationManager::admit(const CommunicationEntryPtr &entry)
{
	entry->setId(++m_id);
	m_queueMutex.lock();
	m_queue.enqueue(entry);
	m_queueMutex.unlock();
	emit admitted(entry);
	saturate();
	return m_id;
}

void CommunicationManager::saturate()
{
	if(m_paused) return;

	while(m_running.size() < 1 && !m_queue.isEmpty()) {
		m_queueMutex.lock();
		CommunicationEntryPtr entry = m_queue.dequeue();
		m_queueMutex.unlock();
		m_running.append(entry);
		qDebug() << "Running" << entry->id();
		CommunicationWorker *worker = new CommunicationWorker(entry);
		worker->setAutoDelete(true);
		connect(worker, SIGNAL(finished(kiss::target::CommunicationEntryPtr, kiss::target::Target::ReturnCode)),
			SLOT(workerFinished(kiss::target::CommunicationEntryPtr, kiss::target::Target::ReturnCode)));
		QThreadPool::globalInstance()->start(worker);
		emit began(entry);
	}
}

void CommunicationManager::workerFinished(CommunicationEntryPtr entry, const Target::ReturnCode success)
{
	m_running.removeAll(entry);
	if(success == Target::AuthenticationFailed) {
		// If authentication fails, we pause the work queue,
		// re-add the failed entry to the front of the queue,
		// and ask for authentication of that target.
		setPaused(true);
		m_queueMutex.lock();
		m_queue.prepend(entry);
		m_queueMutex.unlock();
		emit targetNeedsAuthentication(entry->target(), this);
	} else if(success == Target::OldDeviceSoftware) {
		// We can't talk to this target, so clear
		// all pending entries
		clearQueue();
		emit oldDeviceSoftware(entry->target());
	} else if(success == Target::OldHostSoftware) {
		// We can't talk to this target, so clear
		// all pending entries
		clearQueue();
		emit oldHostSoftware(entry->target());
	} else if(success != Target::Success) {
		
		// If one comm entry fails for a target, we
		// clear the queue of all entries for said
		// target.
		clearQueue();
	} else {
		emit finished(entry, success);
		saturate();
	}
	if(m_running.isEmpty() && m_queue.isEmpty()) emit queueFinished();
}