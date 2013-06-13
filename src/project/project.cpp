#include "project.hpp"

#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Project;

bool Kiss::Project::Project::addAsCopy(const QString& path)
{
	QFileInfo info(path);
	if(!info.isFile()) return false;

	return QFile::copy(path, m_location + "/" + info.fileName());
}

bool Kiss::Project::Project::removeFile(const QString& path)
{
	QFileInfo info(path);
	if(!info.isFile()) return false;

	return QFile::remove(path);
}

QStringList Kiss::Project::Project::files() const
{
	QStringList ret;
	QFileInfoList infoList = QDir(m_location).entryInfoList(QDir::Files);
	foreach(QFileInfo file, infoList) ret << file.absoluteFilePath();
	ret << links();

	return ret;
}

bool Kiss::Project::Project::addAsLink(const QString &path)
{
	if(!QFileInfo(path).isFile()) return false;

	QStringList list = links();
	if(list.contains(path)) return false;

	QFile linkFile(m_location + "/" + LINKS_FILE);
    if (!linkFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
    	qWarning() << "Failed to open links file for project!";
        return false;
    }
    QTextStream out(&linkFile);
    out << path << "\n";
    linkFile.close();

    return true;
}

bool Kiss::Project::Project::removeLink(const QString &path)
{
	if(!QFileInfo(path).isFile()) return false;

	QStringList list = links();
	if(!list.removeOne(path)) return false;

	QFile linkFile(m_location + "/" + LINKS_FILE);
    if (!linkFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    	qWarning() << "Failed to open links file for project!";
        return false;
    }
    QTextStream out(&linkFile);
    foreach(const QString& entry, list) out << entry << "\n";
    linkFile.close();

    return true;
}

QStringList Kiss::Project::Project::links() const
{
	QStringList list;
	
	QFile linkFile(m_location + "/" + LINKS_FILE);
    if(linkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    	QTextStream in(&linkFile);
	    while(!in.atEnd()) list << in.readLine();
	    linkFile.close();
    }
    else qWarning() << "Failed to open links file for project!";

    return list;
}

void Kiss::Project::Project::setSetting(const QString& key, const QString& value)
{
	m_settings[key] = value;
	save();
}

void Kiss::Project::Project::setSettings(const Compiler::Options& settings)
{
	m_settings = settings;
	save();
}

void Kiss::Project::Project::removeSetting(const QString& key)
{
	m_settings.remove(key);
	save();
}

const Compiler::Options& Kiss::Project::Project::settings() const
{
	return m_settings;
}

const QString& Kiss::Project::Project::location() const
{
	return m_location;
}

bool Kiss::Project::Project::save()
{
	QDir dir(m_location);
	return m_settings.save(dir.absoluteFilePath(dir.dirName() + ".kissproj"));
}

ProjectPtr Kiss::Project::Project::create(const QString& location)
{
	QFile linkFile(location + "/" + LINKS_FILE);
    if (!linkFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    	qWarning() << "Failed to create links file for project!";
        return ProjectPtr();
    }
    linkFile.close();

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
	QDir dir(m_location);
	m_settings = Compiler::Options::load(dir.absoluteFilePath(dir.dirName() + ".kissproj"));
	m_settings.insert("", "");
	save();
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
	Kiss::KarPtr archive = Kiss::Kar::create();
	QStringList paths = files();
	QString linkExt = QFileInfo(LINKS_FILE).suffix();
	foreach(QString path, paths) {
		QFile file(path);
		if(QFileInfo(file).suffix() == linkExt) continue;
		if(file.open(QIODevice::ReadOnly)) {
			archive->addFile(path, file.readAll());
			file.close();
		}
	}
	return archive;
}
