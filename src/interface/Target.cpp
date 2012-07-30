#include "Target.h"
#include "TargetResponder.h"

#include <QDebug>

Target::Target(Interface* interface, const QString& name) : m_interface(interface)
{
	
}

Target::~Target()
{
	
}

Interface* Target::interface() const
{
	return m_interface;
}

const QString Target::displayName() const
{
	return information().value(DISPLAY_NAME);
}

const QString Target::commPort() const
{
	return information().value(COMM_PORT);
}

const QString Target::type() const
{
	return information().value(DEVICE_TYPE);
}

const QString Target::serial() const
{
	return information().value(SERIAL);
}

const QString Target::version() const {
	return information().value(VERSION);
}

void Target::setResponder(TargetResponder *responder)
{
	m_responder = responder;
}

TargetResponder *Target::responder()
{
	return m_responder;
}

const bool Target::retryLastQueue()
{
	if(!m_queue.size()) return false;
	return executeQueue(m_queue);
}

const bool Target::executeQueue(const CommunicationQueue& queue)
{
	if(m_workingQueue.size()) return false;
	m_workingQueue = m_queue = queue;
	notifyQueue(true);
	return true;
}

const bool Target::isQueueExecuting() const
{
	return m_workingQueue.size();
}

void Target::notifyQueue(const bool success)
{
	if(!m_workingQueue.size()) return;
	if(success) {
		CommunicationEntryPtr entry = m_workingQueue.dequeue();
		if(!executeEntry(entry.data())) clearEntries();
	} else clearEntries();
}

const bool Target::executeEntry(const CommunicationEntry *entry)
{
	switch(entry->type()) {
		case CommunicationEntry::Custom: return sendCustom(entry->custom(), entry->payload());
		case CommunicationEntry::Download: return download(entry->name(), entry->archive());
		case CommunicationEntry::Compile: return compile(entry->name());
		case CommunicationEntry::Run: return run(entry->name());
		case CommunicationEntry::Disconnect: return disconnect();
		default: qCritical() << "Target does not know how to execute queue entry of type" << entry->type();
	}
	return false;
}

void Target::clearEntries()
{
	m_workingQueue.clear();
}
