#include "communication_queue.hpp"

#include <QDebug>

using namespace Kiss;
using namespace Kiss::Target;

CommunicationEntry::CommunicationEntry(const TargetPtr &target, const CommunicationEntry::Type& type)
	: m_target(target),
	m_type(type),
	m_id(0)
{
	
}

CommunicationEntry::CommunicationEntry(const TargetPtr &target, const CommunicationEntry::Type& type, const QString& name, const KarPtr& archive)
	: m_target(target),
	m_type(type),
	m_name(name),
	m_archive(archive),
	m_id(0)
{
}

CommunicationEntry::CommunicationEntry(const TargetPtr &target, const QString& custom, const QByteArray& payload)
	: m_target(target),
	m_type(Custom),
	m_name(""),
	m_custom(custom),
	m_payload(payload),
	m_id(0)
{
}

const TargetPtr &CommunicationEntry::target() const
{
	return m_target;
}

const CommunicationEntry::Type& CommunicationEntry::type() const
{
	return m_type;
}

QString CommunicationEntry::typeString() const
{
	switch(m_type) {
	case CommunicationEntry::Custom: return QObject::tr("Custom");
	case CommunicationEntry::Authenticate: return QObject::tr("Authenticate");
	case CommunicationEntry::Unknown: return QObject::tr("Unknown");
	case CommunicationEntry::Download: return QObject::tr("Download");
	case CommunicationEntry::Compile: return QObject::tr("Compile");
	case CommunicationEntry::Run: return QObject::tr("Run");
	case CommunicationEntry::List: return QObject::tr("List");
	case CommunicationEntry::Delete: return QObject::tr("Delete Program");
	case CommunicationEntry::Interaction: return QObject::tr("Interaction");
	case CommunicationEntry::Disconnect: return QObject::tr("Disconnect");
	}
	return QObject::tr("Unknown");
}

const QString& CommunicationEntry::custom() const
{
	return m_custom;
}

const QByteArray& CommunicationEntry::payload() const
{
	return m_payload;
}

const QString& CommunicationEntry::name() const
{
	return m_name;
}

const KarPtr& CommunicationEntry::archive() const
{
	return m_archive;
}

void CommunicationEntry::setId(const quint64 &id)
{
	m_id = id;
}

const quint64 &CommunicationEntry::id() const
{
	return m_id;
}

bool CommunicationEntry::execute() const
{
	switch(m_type) {
	case CommunicationEntry::Custom: return m_target->sendCustom(m_id, m_custom, m_payload);
	case CommunicationEntry::Download: return m_target->download(m_id, m_name, m_archive);
	case CommunicationEntry::Compile: return m_target->compile(m_id, m_name);
	case CommunicationEntry::Run: return m_target->run(m_id, m_name);
	case CommunicationEntry::List: return m_target->list(m_id);
	case CommunicationEntry::Delete: return m_target->deleteProgram(m_id, m_name);
	case CommunicationEntry::Interaction: return m_target->interaction(m_id, m_name);
	case CommunicationEntry::Disconnect: return m_target->disconnect();
	default: qCritical() << "Target does not know how to execute queue entry of type" << m_type;
	}
	
	return false;
}