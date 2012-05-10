#include "DeltaArchiveListener.h"

#include <QDebug>

ArchiveEvent::ArchiveEvent(const int& type, const char* path, const TinyNode* node)
	: type(type), node(node), m_path(0)
{
	setPath(path);
}

ArchiveEvent::~ArchiveEvent()
{
	if(m_path) delete[] m_path;
}

ArchiveEvent::ArchiveEvent(const ArchiveEvent& rhs)
	: type(rhs.type), node(rhs.node), m_path(0)
{
	setPath(rhs.path());
}

const char* ArchiveEvent::path() const
{
	return m_path;
}

void ArchiveEvent::setPath(const char* path)
{
	if(m_path) delete[] m_path;
	size_t len = strlen(path);
	m_path = new char[len + 1];
	memcpy(m_path, path, len);
	m_path[len] = 0;
	qDebug() << "Set path to" << m_path;
}

DeltaArchiveListener::DeltaArchiveListener(TinyArchive* archive, uint32_t id)
	: m_archive(archive), m_id(id)
{
	m_archive->addListener(this);
}

DeltaArchiveListener::~DeltaArchiveListener()
{
	m_archive->removeListener(this);
}

TinyArchive* DeltaArchiveListener::archive() const
{
	return m_archive;
}

void DeltaArchiveListener::nodeAdded(const TinyNode* node)
{
	if(node->id() != m_id) return;
	push(ArchiveEvent(ArchiveEvent::NodeAdded, node->path().c_str(), node));
}

void DeltaArchiveListener::nodeRemoved(const TinyNode* node)
{
	if(node->id() != m_id) return;
	push(ArchiveEvent(ArchiveEvent::NodeRemoved, node->path().c_str(), 0));
}

void DeltaArchiveListener::nodeUpdated(const TinyNode* node)
{
	if(node->id() != m_id) return;
	push(ArchiveEvent(ArchiveEvent::NodeUpdated, node->path().c_str(), node));
}

void DeltaArchiveListener::drain()
{
	m_history.clear();
}

const bool DeltaArchiveListener::hasEvent() const
{
	return m_history.size();
}

ArchiveEvent DeltaArchiveListener::nextEvent()
{
	return pop();
}

ArchiveEvent DeltaArchiveListener::pop()
{
	ArchiveEvent front = m_history.front();
	m_history.pop_front();
	return front;
}

void DeltaArchiveListener::push(const ArchiveEvent& event)
{
	m_history.push_back(event);
}