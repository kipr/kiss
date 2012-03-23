#include "ProjectFile.h"

#include <QStringList>

ProjectFile::~ProjectFile()
{
	// sync();
}

void ProjectFile::moveFile(const QString& oldPath, const QString& newPath)
{
	QByteArray contents = fileContents(oldPath);
	removeFile(oldPath);
	addFile(newPath, contents);
}

bool ProjectFile::containsFile(const QString& path) const
{
	return list().contains(path);
}

bool ProjectFile::containsSetting(const QString& key) const
{
	return projectSettings().contains(key);
}

QString ProjectFile::valueForSetting(const QString& key) const
{
	return projectSettings().value(key);
}

void ProjectFile::addProjectSetting(const QString& key, const QString& value)
{
	QMap<QString, QString> settings = projectSettings();
	settings[key] = value;
	setProjectSettings(settings);
}

void ProjectFile::removeProjectSetting(const QString& key, const QString& value)
{
	QMap<QString, QString> settings = projectSettings();
	settings.remove(key);
	setProjectSettings(settings);
}

void ProjectFile::settingUpdated(const QString& key, const QString& value)
{
	QMap<QString, QString> settings = projectSettings();
	settings[key] = value;
	setProjectSettings(settings);
}