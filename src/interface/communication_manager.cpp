#include "action_manager.hpp"

#include <QThreadPool>

using namespace Kiss;
using namespace Kiss::Target;

CommunicationWorker::CommunicationWorker(const CommunicationEntryPtr &entry)
	: m_entry(entry)
{
}

void CommunicationWorker::run()
{
	m_entry->execute();
	emit finished(m_entry);
}

CommunicationManager::CommunicationManager()
	: m_id(0)
{
}

CommunicationManager::~CommunicationManager()
{
}

quint64 CommunicationManager::admit(const CommunicationEntryPtr &entry)
{
	entry->setId(++m_id);
	m_queue.enqueue(entry);
	emit admitted(entry);
	saturate();
}

void CommunicationManager::saturate()
{
	while(m_running.size() < 1 && !m_queue.isEmpty()) {
		CommunicationEntryPtr entry = m_queue.head();
		m_running.append(entry);
		CommunicationWorker *worker = new CommunicationWorker(entry);
		worker->setAutoDelete(true);
		connect(worker, SIGNAL(finished(CommunicationEntryPtr)), SLOT(workerFinished(CommunicationEntryPtr)));
		QThreadPool::globalInstance()->start(worker);
		emit began(entry);
	}
}

void CommunicationManager::workerFinished(const CommunicationEntryPtr &entry)
{
	emit finished(entry);
	m_running.removeAll(entry);
	saturate();
}