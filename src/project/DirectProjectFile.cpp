#include "DirectProjectFile.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <QDebug>

DirectProjectFile::DirectProjectFile(const QString& path) : m_path(path)
{
	QFile file(m_path + "/" + projectSettingsFile());
	if(!file.open(QIODevice::ReadOnly)) return;
		
	QTextStream stream(&file);
	
	qWarning() << "Reading settings...";
	while(!stream.atEnd()) {
		QString key;
		QString value;
		stream >> key >> value;
		m_projectSettings[key] = value;
		qWarning() << key << value;
	}
	
	file.close();
}

QStringList DirectProjectFile::list() const
{
	return list(QDir(m_path));
}

const QString& DirectProjectFile::path() const
{
	return m_path;
}

#define BUFFER_SIZE 256

void DirectProjectFile::addFile(const QString& path, QDataStream& file)
{
	QByteArray data;
	
	const int len = BUFFER_SIZE;
	char buffer[len];
	int read = 0;
	while((read = file.readRawData(buffer, len)) > 0) data.append(buffer, read);
	
	addFile(path, data);
}

void DirectProjectFile::addFile(const QString& path, const QByteArray& data)
{
	m_io.enqueue(new WriteFileQueuedJob(path, data));
}

#define BUFFER_SIZE 256

QByteArray DirectProjectFile::fileContents(const QString& path) const
{
	QFile file(m_path + "/" + path);
	if(!file.open(QIODevice::ReadOnly)) return QByteArray();
	
	QDataStream fileStream(&file);
	
	QByteArray ret;
	
	const int len = BUFFER_SIZE;
	char buffer[len];
	int read = 0;
	while((read = fileStream.readRawData(buffer, len)) > 0) ret.append(buffer, read);
	
	file.close();
	
	return ret;
}

bool DirectProjectFile::updateFileContents(const QString& path, const QByteArray& ba)
{
	m_io.enqueue(new WriteFileQueuedJob(this->path() + "/" + path, ba));
	
	return true;
}

void DirectProjectFile::removeFile(const QString& path)
{
	m_io.enqueue(new RemoveFileQueuedJob(this->path() + "/" + path));
}

void DirectProjectFile::removeIndex(const quint32& i)
{
	removeFile(list()[i]);
}

bool DirectProjectFile::sync()
{
	bool success = true;
	success &= m_io.execute();
	
	QFile file(m_path + "/" + projectSettingsFile());
	if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		
		QTextStream stream(&file);
		foreach(const QString& key, m_projectSettings.keys())
			stream << key << " " << m_projectSettings[key];
		
		file.close();
	} else success &= false;
	
	return success;
}

QString DirectProjectFile::projectSettingsFile() const
{
	return "project.settings";
}

void DirectProjectFile::setProjectSettings(const QMap<QString, QString>& projectSettings)
{
	m_projectSettings = projectSettings;
}

QMap<QString, QString> DirectProjectFile::projectSettings() const
{
	return m_projectSettings;
}

QStringList DirectProjectFile::list(QDir dir) const
{
	qWarning() << "Iterating over" << dir.path();
	QStringList ret;
	QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs | QDir::NoSymLinks);
	foreach(QFileInfo file, files) {
		if(file.fileName() == projectSettingsFile()) continue;
		
		qWarning() << file.absoluteFilePath().remove(m_path + "/");
		if(file.isDir()) ret << list(QDir(file.absoluteFilePath()));
		else ret << file.absoluteFilePath().remove(m_path + "/");
	}
	return ret;
}