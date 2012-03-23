#include "TinyArchiveProjectFile.h"

#include <fstream>
#include <QDebug>

#define SETTINGS_FILE "settings:"

using namespace std;

#define BUFFER_SIZE 256

TinyArchiveProjectFile::TinyArchiveProjectFile(const QString& path) : m_archive(0), m_path(path)
{
	ifstream ifs;
	ifs.open(path.toLocal8Bit().data(), ifstream::in);
	if(ifs.is_open()) m_archive = TinyArchive::read(ifs);
	ifs.close();
	
	if(!m_archive) {
		m_archive = new TinyArchive();
		m_archive->push_back(new ArchiveEntry(SETTINGS_FILE, "", 0));
	}
}

QStringList TinyArchiveProjectFile::list() const
{
	QStringList ret;
	TinyArchive::const_iterator it = m_archive->begin();
	for(; it != m_archive->end(); ++it) ret << QString::fromStdString((*it)->path);
	ret.removeAll(SETTINGS_FILE);
	return ret;
}

const QString& TinyArchiveProjectFile::path() const
{
	return m_path;
}

void TinyArchiveProjectFile::addFile(const QString& path, QDataStream& file)
{
	QByteArray data;
	
	const int len = BUFFER_SIZE;
	char buffer[len];
	int read = 0;
	while((read = file.readRawData(buffer, len)) > 0) data.append(buffer, read);
	
	addFile(path, data);
}

void TinyArchiveProjectFile::addFile(const QString& path, const QByteArray& data)
{
	m_archive->push_back(new ArchiveEntry(path.toStdString(), data.data(), data.size()));
	emit fileCreated(path);
}

QByteArray TinyArchiveProjectFile::fileContents(const QString& path) const
{
	int i = m_archive->indexOf(path.toStdString());
	qWarning() << "Index of" << path << ":" << i;
	if(i < 0) return QByteArray();
	
	return contentsForEntry(m_archive->at(i));
}

bool TinyArchiveProjectFile::updateFileContents(const QString& path, const QByteArray& ba)
{
	qWarning() << "Updating contents of" << QString::fromStdString(path.toStdString()) << "to" << ba;
	bool success = m_archive->update(path.toStdString(), ba.data(), ba.size());
	qWarning() << "Called update";
	if(!success) {
		m_archive->push_back(new ArchiveEntry(path.toStdString(), ba.data(), ba.size()));
		emit fileCreated(path);
	} else emit fileChanged(path);
	return true;
}

void TinyArchiveProjectFile::removeFile(const QString& path)
{
	m_archive->remove(path.toStdString());
	emit fileRemoved(path);
}

void TinyArchiveProjectFile::removeIndex(const quint32& i)
{
	removeFile(QString::fromStdString(m_archive->at(i)->path));
}

void TinyArchiveProjectFile::setProjectSettings(const QMap<QString, QString>& projectSettings)
{
	QByteArray ba;
	QTextStream stream(&ba);
	foreach(const QString& key, projectSettings.keys()) stream << key << " " << projectSettings[key];
	m_archive->update(SETTINGS_FILE, ba.data(), ba.size());
}

QMap<QString, QString> TinyArchiveProjectFile::projectSettings() const
{
	int i = m_archive->indexOf(SETTINGS_FILE);
	if(i < 0) {
		qWarning() << "No " << SETTINGS_FILE << "in tiny archive.";
		return QMap<QString, QString>();
	}
	
	QByteArray ba = contentsForEntry(m_archive->at(i));
	QTextStream stream(&ba);
	
	QMap<QString, QString> projectSettings;
	while(!stream.atEnd()) {
		QString key;
		QString value;
		stream >> key >> value;
		projectSettings[key] = value;
		qWarning() << key << value;
	}
	
	return projectSettings;
}

bool TinyArchiveProjectFile::sync()
{
	ofstream ofs;
	ofs.open(m_path.toLocal8Bit().data(), ifstream::out);
	if(!ofs.is_open()) return false;
	m_archive->write(ofs);
	ofs.close();
	return true;
}

QByteArray TinyArchiveProjectFile::contentsForEntry(const ArchiveEntry* entry) const
{
	return QByteArray(entry->data, entry->length);
}