#include "communication_manager.hpp"

#include <QThreadPool>
#include <QDebug>

#include <QObject>

using namespace Kiss;
using namespace Kiss::Target;

// TODO: Try QThread instead of QRunnable for weird time issues

CommunicationWorker::CommunicationWorker(const CommunicationEntryPtr &entry)
	: m_entry(entry)
{
}

void CommunicationWorker::run()
{
	fprintf(stderr, "Entry %llu executing...\n", m_entry->id());
	const bool success = m_entry->execute();
	fprintf(stderr, "Entry %llu finished!\n", m_entry->id());
	// qDebug() << "Entry" << m_entry->id() << (!success ? "failed." : "finished!");
	emit finished(m_entry, success);
}

CommunicationManager::CommunicationManager()
	: m_id(0)
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
	// qDebug() << "Saturating work queue";
	while(m_running.size() < 1 && !m_queue.isEmpty()) {
		m_queueMutex.lock();
		CommunicationEntryPtr entry = m_queue.dequeue();
		m_queueMutex.unlock();
		m_running.append(entry);
		CommunicationWorker *worker = new CommunicationWorker(entry);
		worker->setAutoDelete(true);
		connect(worker, SIGNAL(finished(CommunicationEntryPtr, bool)),
			SLOT(workerFinished(CommunicationEntryPtr, bool)));
		QThreadPool::globalInstance()->start(worker);
		emit began(entry);
	}
	
}

void CommunicationManager::workerFinished(CommunicationEntryPtr entry, const bool &success)
{
	// If one comm entry fails for a target, we
	// clear the queue of all entries for said
	// target.
	m_queueMutex.lock();
	if(!success) m_queue.clear(); // TODO: Make it clear by target
	m_queueMutex.unlock();
	emit finished(entry, success);
	m_running.removeAll(entry);
	if(m_running.isEmpty() && m_queue.isEmpty()) emit queueFinished();
	saturate();
}