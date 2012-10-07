#include "project.hpp"

#include <QFileInfo>
#include <QDir>

#define PROJECT_SETTINGS_FILE "kiss.settings"

using namespace Kiss::Project;

bool Project::addFile(const QString& path)
{
	if(m_singleFileMode) return false;

	return false;
}

bool Project::removeFile(const QString& path)
{
	if(m_singleFileMode) return false;
	
	return QFile::remove(m_location + "/" + path);
}

QStringList Project::files() const
{
	return m_files;
}

bool Project::settingsEnabled() const
{
	return !m_singleFileMode;
}

void Project::setSettings(const Project::Settings& settings)
{
	m_settings = settings;
}

const Project::Settings& Project::settings() const
{
	return m_settings;
}

void Project::setSetting(const QString& key, const QString& value)
{
	m_settings[key] = value;
}

void Project::removeSetting(const QString& key)
{
	m_settings.remove(key);
}

const QString& Project::location() const
{
	return m_location;
}

bool Project::save()
{
	QFile settingsFile(m_location + "/" + PROJECT_SETTINGS_FILE);
	if(!settingsFile.open(QIODevice::WriteOnly)) return false;
	QDataStream settingsStream(&settingsFile);
	settingsStream << m_settings; // TODO: Make plain text
	settingsFile.close();
	return true;
}

Project *Project::create(const QString& location)
{
	if(!QDir().mkpath(location)) return 0;
	return load(location);
}

Project *Project::load(const QString& location)
{
	if(!QFileInfo(location).isDir()) return 0;
	return new Project(location, false);
}

Project *Project::loadSingleFile(const QString& location)
{
	return new Project(location, true);
}

Project::Project(const QString& location, bool singleFileMode)
	: m_location(location), m_singleFileMode(singleFileMode)
{
	if(!m_singleFileMode) refresh();
	else {
		m_files << m_location;
		m_location = QFileInfo(m_location).path();
	}
	
	setName(QFileInfo(m_location).fileName());
}

void Project::refresh()
{
	refresh(m_location);
}

void Project::refresh(const QString& location)
{
	QFileInfoList files = QDir(location).entryInfoList(QDir::NoDot | QDir::NoDotDot);
	foreach(const QFileInfo& file, files) {
		if(file.filePath() == PROJECT_SETTINGS_FILE) continue;
		
		if(file.isDir()) refresh(file.filePath());
		else m_files << file.filePath();
	}
}