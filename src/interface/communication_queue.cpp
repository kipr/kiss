#include "communication_queue.hpp"

using namespace Kiss::Target;

CommunicationEntry::CommunicationEntry(const Type& type)
	: m_type(type)
{
	
}

CommunicationEntry::CommunicationEntry(const CommunicationEntry::Type& type, const QString& name)
	: m_type(type), m_name(name)
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

CommunicationEntry::CommunicationEntry(const CommunicationEntry&) {}
CommunicationEntry& CommunicationEntry::operator=(const CommunicationEntry&) { return *this; }