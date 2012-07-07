#include "Device.h"
#include "DeviceResponder.h"

#include <QDebug>

Device::Device(Interface* interface, const QString& name) : m_interface(interface)
{
	
}

Device::~Device()
{
	
}

Interface* Device::interface() const
{
	return m_interface;
}

const QString Device::displayName() const
{
	return information().value(DISPLAY_NAME);
}

const QString Device::commPort() const
{
	return information().value(COMM_PORT);
}

const QString Device::type() const
{
	return information().value(DEVICE_TYPE);
}

const QString Device::serial() const
{
	return information().value(SERIAL);
}

const QString Device::version() const {
	return information().value(VERSION);
}

void Device::setResponder(DeviceResponder *responder)
{
	m_responder = responder;
}

DeviceResponder *Device::responder()
{
	return m_responder;
}

const bool Device::retryLastQueue()
{
	if(!m_queue.size()) return false;
	return executeQueue(m_queue);
}

const bool Device::executeQueue(const CommunicationQueue& queue)
{
	if(m_workingQueue.size()) return false;
	m_workingQueue = m_queue = queue;
	notifyQueue(true);
	return true;
}

const bool Device::isQueueExecuting() const
{
	return m_workingQueue.size();
}

void Device::notifyQueue(const bool success)
{
	if(!m_workingQueue.size()) return;
	if(success) {
		CommunicationEntry *entry = m_workingQueue.dequeue();
		if(!executeEntry(entry)) clearEntries();
		delete entry;
	} else clearEntries();
}

const bool Device::executeEntry(const CommunicationEntry *entry)
{
	switch(entry->type()) {
		case CommunicationEntry::Custom: return sendCustom(entry->custom(), entry->payload());
		case CommunicationEntry::Download: return download(entry->name(), entry->archive());
		case CommunicationEntry::Compile: return compile(entry->name());
		case CommunicationEntry::Run: return run(entry->name());
		case CommunicationEntry::Disconnect: return disconnect();
		default: qCritical() << "Device does not know how to execute queue entry of type" << entry->type();
	}
	return false;
}

void Device::clearEntries()
{
	while(m_workingQueue.size()) delete m_workingQueue.dequeue();
}
