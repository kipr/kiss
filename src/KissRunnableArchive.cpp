#include "KissRunnableArchive.h"

#include <QFile>
#include <QDebug>

#define COMMAND_KEY "run"

KissRunnableArchiveWriter::KissRunnableArchiveWriter()
{
	setCommand("echo \"call setCommand in your code\"");
}

void KissRunnableArchiveWriter::setCommand(const QString& command)
{
	m_tinyArchive.update(COMMAND_KEY, command.toStdString().c_str(), command.length());
}

bool KissRunnableArchiveWriter::addFile(const QString& path)
{
	QFile file(path);
	if(file.open(QIODevice::ReadOnly)) return false;
	
	QTextStream stream(&file);
	addVirtualFile(path, QByteArray(stream.readAll().toAscii()));
	
	file.close();
	
	return true;
}

void KissRunnableArchiveWriter::addVirtualFile(const QString& file, const QByteArray& contents)
{
	m_tinyArchive.update(file.toStdString(), contents.constData(), contents.size());
}

const TinyArchive& KissRunnableArchiveWriter::tinyArchive() const
{
	return m_tinyArchive;
}

KissRunnableArchiveReader::KissRunnableArchiveReader(TinyArchive* tinyArchive) : m_tinyArchive(tinyArchive) {}

bool KissRunnableArchiveReader::extractTo(const QString& path)
{
	TinyArchive::const_iterator it = m_tinyArchive->begin();
	bool errors = false;
	for(; it != m_tinyArchive->end(); ++it) {
		ArchiveEntry* entry = *it;
		QString path(entry->path.c_str());
		QFile file(path);
		if(!file.open(QIODevice::WriteOnly)) {
			qWarning() << "Failed to extract" << path;
			errors |= true;
		}
		file.write(entry->data, entry->length);
		file.close();
	}
	return !errors;
}

QString KissRunnableArchiveReader::command() const
{
	m_tinyArchive->at(m_tinyArchive->indexOf(COMMAND_KEY));
}