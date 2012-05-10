#include "ArchiveWriter.h"

#include "DeltaArchiveListener.h"
#include "Log.h"
#include "FileSystemUtils.h"
#include "QTinyArchive.h"

#include <QDebug>

ArchiveWriter::ArchiveWriter(TinyArchive* archive, const QDir& root)
	: m_archive(archive), m_listener(new DeltaArchiveListener(archive)), m_root(root)
{
	Log::ref().info(QString("Archive writer created with path %1").arg(m_root.path()));
	write(ArchiveWriter::Full);
}

ArchiveWriter::~ArchiveWriter()
{
	delete m_listener;
}

QStringList ArchiveWriter::files() const
{
	QStringList ret;
	const QStringList& archiveFiles = QTinyArchive::files(m_archive);
	foreach(const QString& file, archiveFiles) ret << m_root.path() + "/" + file;
	return ret;
}

const QDir& ArchiveWriter::root() const
{
	return m_root;
}

const TinyArchive* ArchiveWriter::archive() const
{
	return m_archive;
}

const bool ArchiveWriter::write(const int& mode)
{
	if(mode == ArchiveWriter::Delta) {
		Log::ref().info("Writing out archive in ArchiveWriter::Delta mode");
		if(writeDelta()) return true;
		Log::ref().error("Delta write FAILED! Falling back on full write.");
	}

	Log::ref().info("Writing out archive in ArchiveWriter::Full mode");
	FileSystemUtils::removeDirectory(m_root);
	m_listener->drain();
	return writeFull(m_archive->root());
}

const bool ArchiveWriter::writeFull(const TinyNode* node)
{
	const QString& path = m_root.path() + "/" + QTinyNode::path(node);
	if(node->hasChildren()) {
		QDir().rmdir(path);
		if(!QDir().mkdir(path)) {
			Log::ref().error(QString("Unable to create directory at %1").arg(path));
			return false;
		}
		std::vector<TinyNode*>::const_iterator it = node->children().begin();
		for(; it != node->children().end(); ++it) writeFull(*it);
	} else {
		if(node->id() != 0) return true;
		QFile file(path);
		if(!file.open(QIODevice::WriteOnly)) {
			Log::ref().error(QString("Could not open %1 for writing").arg(path));
			return false;
		}
		file.write(QTinyNode::data(node));
		file.close();
	}
	return true;
}

const bool ArchiveWriter::writeDelta()
{
	while(m_listener->hasEvent()) {
		const ArchiveEvent& event = m_listener->nextEvent();
		qDebug() << "Event at path \"" << event.path() << "\"";
		switch(event.type) {
			case ArchiveEvent::NodeAdded: deltaAddEvent(event); break;
			case ArchiveEvent::NodeUpdated: deltaUpdateEvent(event); break;
			case ArchiveEvent::NodeRemoved: deltaRemoveEvent(event); break;
			default: Log::ref().warning(QString("Unknown event type %1").arg(event.type)); break;
		}
	}
	return true;
}

const bool ArchiveWriter::deltaAddEvent(const ArchiveEvent& event)
{
	if(event.node->id() != 0) return true;
	
	const QString& path = m_root.path() + "/" + event.path();
	QFileInfo info(path);
	if(info.exists()) {
		Log::ref().error(QString("%1 already exists").arg(event.path()));
		return false;
	}
	Log::ref().debug(QString("Add Event at %1").arg(path));
	
	if(!event.node->hasChildren()) {
		QFile file(path);
		if(!file.open(QIODevice::WriteOnly)) {
			Log::ref().error(QString("Could not open %1 for writing").arg(path));
			return false;
		}
		file.write(QTinyNode::data(event.node));
		file.close();
	} else return QDir().mkdir(path);
	return true;
}

const bool ArchiveWriter::deltaUpdateEvent(const ArchiveEvent& event)
{
	if(event.node->id() != 0) return true;
	
	const QString& path = m_root.path() + "/" + event.path();
	QFileInfo info(path);
	if(!info.exists()) {
		Log::ref().error(QString("%1 DOES NOT already exist at %2").arg(event.path()).arg(info.filePath()));
		return false;
	}
	
	Log::ref().debug(QString("Update Event at %1").arg(path));
	
	if(!event.node->hasChildren()) {
		QFile file(path);
		if(!file.open(QIODevice::WriteOnly)) {
			Log::ref().error(QString("Could not open %1 for writing").arg(path));
			return false;
		}
		file.write(QTinyNode::data(event.node));
		file.close();
	} else return QDir().mkdir(path);
	
	return true;
}

const bool ArchiveWriter::deltaRemoveEvent(const ArchiveEvent& event)
{
	const QString& path = m_root.path() + "/" + event.path();
	Log::ref().debug(QString("Remove Event at %1").arg(path));
	return QFileInfo(path).isDir() ? QDir().rmdir(path) : QFile::remove(path);
}