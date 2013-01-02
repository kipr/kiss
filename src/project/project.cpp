#include "project.hpp"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#define PROJECT_SETTINGS_FILE "kiss.settings"

using namespace Kiss;
using namespace Kiss::Project;

bool Kiss::Project::Project::addFile(const QString& path)
{
	return false;
}

bool Kiss::Project::Project::removeFile(const QString& path)
{
	return QFile::remove(m_location + "/" + path);
}

QStringList Kiss::Project::Project::files() const
{
	return m_files;
}

void Kiss::Project::Project::setSettings(const Kiss::Project::Project::Settings& settings)
{
	m_settings = settings;
}

const Kiss::Project::Project::Settings& Kiss::Project::Project::settings() const
{
	return m_settings;
}

void Kiss::Project::Project::setSetting(const QString& key, const QString& value)
{
	m_settings[key] = value;
}

void Kiss::Project::Project::removeSetting(const QString& key)
{
	m_settings.remove(key);
}

const QString& Kiss::Project::Project::location() const
{
	return m_location;
}

bool Kiss::Project::Project::save()
{
	QFile settingsFile(m_location + "/" + PROJECT_SETTINGS_FILE);
	if(!settingsFile.open(QIODevice::WriteOnly)) return false;
	QDataStream settingsStream(&settingsFile);
	settingsStream << m_settings; // TODO: Make plain text
	settingsFile.close();
	return true;
}

ProjectPtr Kiss::Project::Project::create(const QString& location)
{
	if(!QDir().mkpath(location)) return ProjectPtr();
	return load(location);
}

ProjectPtr Kiss::Project::Project::load(const QString& location)
{
	if(!QFileInfo(location).isDir()) return ProjectPtr();
	return ProjectPtr(new Project(location));
}

Kiss::Project::Project::Project(const QString& location)
	: m_location(location)
{
	refresh();
	
	setName(QFileInfo(m_location).fileName());
}

void Kiss::Project::Project::refresh()
{
	refresh(m_location);
}

void Kiss::Project::Project::refresh(const QString& location)
{
	QFileInfoList files = QDir(location).entryInfoList(QDir::NoDot | QDir::NoDotDot);
	foreach(const QFileInfo& file, files) {
		if(file.fileName() == PROJECT_SETTINGS_FILE) continue;
		
		if(file.isDir()) refresh(file.filePath());
		else m_files << file.filePath();
	}
}

Kiss::KarPtr Kiss::Project::Project::archive() const
{
	return Kar::create(m_location);
}