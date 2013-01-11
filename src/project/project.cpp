#include "project.hpp"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#define PROJECT_SETTINGS_FILE "kiss.settings"

using namespace Kiss;
using namespace Kiss::Project;

bool Kiss::Project::Project::copyFile(const QString& path)
{
	qWarning() << "NYI: copyFile";
	return false; // m_archive->addFile();
}

bool Kiss::Project::Project::removeFile(const QString& path)
{
	return m_archive->removeFile(path);
}

QStringList Kiss::Project::Project::files() const
{
	return m_archive->files();
}

void Kiss::Project::Project::setSettings(const Kiss::Project::Project::Settings &settings)
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
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << m_settings; // TODO: Make plain text
	m_archive->setFile(PROJECT_SETTINGS_FILE, data);
	return m_archive->save(m_location);
}

ProjectPtr Kiss::Project::Project::create(const QString& location)
{
	if(!QDir().mkpath(location)) return ProjectPtr();
	return load(location);
}

ProjectPtr Kiss::Project::Project::load(const QString& location)
{
	if(!QFileInfo(location).isFile()) return ProjectPtr();
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
}

Kiss::KarPtr Kiss::Project::Project::archive() const
{
	return m_archive;
}