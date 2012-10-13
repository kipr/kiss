#include "communication_queue.hpp"

using namespace Kiss;
using namespace Kiss::Target;

CommunicationEntry::CommunicationEntry(const CommunicationEntry::Type& type)
	: m_type(type)
{
	
}

CommunicationEntry::CommunicationEntry(const CommunicationEntry::Type& type, const QString& name, const KarPtr& archive)
	: m_type(type), m_name(name), m_archive(archive)
{
}

CommunicationEntry::CommunicationEntry(const QString& custom, const QByteArray& payload)
	: m_type(Custom), m_name(""), m_custom(custom), m_payload(payload)
{
}
	
const CommunicationEntry::Type& CommunicationEntry::type() const
{
	return m_type;
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