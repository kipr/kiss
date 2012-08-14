#include "CommunicationQueue.h"
#include <tinyarchive.hpp>

CommunicationEntry::CommunicationEntry(const Type& type)
	: m_type(type)
{
	
}

CommunicationEntry::CommunicationEntry(const CommunicationEntry::Type& type, const QString& name, TinyArchive *archive)
	: m_type(type), m_name(name), m_archive(archive)
{
}

CommunicationEntry::CommunicationEntry(const QString& custom, const QByteArray& payload)
	: m_type(Custom), m_name(""), m_custom(custom), m_payload(payload), m_archive(0)
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

TinyArchive *CommunicationEntry::archive() const
{
	return m_archive;
}

CommunicationEntry::CommunicationEntry(const CommunicationEntry&) {}
CommunicationEntry& CommunicationEntry::operator=(const CommunicationEntry&) { return *this; }