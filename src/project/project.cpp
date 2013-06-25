#include "project.hpp"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Project;

ProjectPtr Kiss::Project::Project::load(const QString &location)
{
	if(!QFileInfo(location).isDir()) return ProjectPtr();
	return ProjectPtr(new Project(location));
}

bool Kiss::Project::Project::save()
{
	QDir dir(m_location);
	return m_settings.save(dir.absoluteFilePath(dir.dirName() + "." + PROJECT_EXT));
}

bool Kiss::Project::Project::addAsCopy(const QString &path)
{
	QFileInfo info(path);
	if(!info.isFile()) return false;

	return QFile::copy(path, m_location + "/" + info.fileName());
}

bool Kiss::Project::Project::addAsMovedCopy(const QString &path)
{
	return (addAsCopy(path) && QFile::remove(path));
}

bool Kiss::Project::Project::removeFile(const QString &path)
{
	if(!QFileInfo(path).isFile()) return false;
	
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
	QStringList list = links();
	QDir projectDir(m_location);
	if(!projectDir.exists(path) || list.contains(path)) return false;

	QString otherPath;
	if(QFileInfo(path).isAbsolute()) otherPath = projectDir.relativeFilePath(path);
	else otherPath = QDir::cleanPath(projectDir.absoluteFilePath(path));

	if(list.contains(otherPath)) return false;

	QFile linkFile(linksFilePath());
    if (!linkFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
    	qWarning() << "Failed to open links file for project!";
        return false;
    }
    QTextStream out(&linkFile);
    out << path << "\n";
    linkFile.close();

    return true;
}

bool Kiss::Project::Project::addAsRelativeLink(const QString &path)
{
	return addAsLink(QDir(m_location).relativeFilePath(path));
}

bool Kiss::Project::Project::removeLink(const QString &path)
{
	QDir projectDir(m_location);
	if(!projectDir.exists(path)) return false;

	QStringList list = links();
	QString otherPath;
	if(QFileInfo(path).isAbsolute()) otherPath = projectDir.relativeFilePath(path);
	else otherPath = QDir::cleanPath(projectDir.absoluteFilePath(path));

	if(!list.removeOne(path) && !list.removeOne(otherPath)) return false;

	QFile linkFile(linksFilePath());
    if (!linkFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    	qWarning() << "Failed to open links file for project!";
        return false;
    }
    QTextStream out(&linkFile);
    foreach(const QString &entry, list) out << entry << "\n";
    linkFile.close();

    return true;
}

QStringList Kiss::Project::Project::links() const
{
	QStringList list;
	
	QFile linkFile(linksFilePath());
    if(linkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    	QTextStream in(&linkFile);
	    while(!in.atEnd()) list << in.readLine();
	    linkFile.close();
    }
    else qWarning() << "Failed to open links file for project!";

    return list;
}

const QString Kiss::Project::Project::linksFilePath() const
{
	const QDir dir(m_location);
	return dir.absoluteFilePath(dir.dirName() + "." + LINKS_EXT);
}

void Kiss::Project::Project::setSetting(const QString &key, const QString &value)
{
	m_settings[key] = value;
	save();
}

void Kiss::Project::Project::setSettings(const Compiler::Options &settings)
{
	m_settings = settings;
	save();
}

void Kiss::Project::Project::removeSetting(const QString &key)
{
	m_settings.remove(key);
	save();
}

const Compiler::Options &Kiss::Project::Project::settings() const
{
	return m_settings;
}

const QString &Kiss::Project::Project::location() const
{
	return m_location;
}

Kiss::KarPtr Kiss::Project::Project::archive() const
{
	Kiss::KarPtr archive = Kiss::Kar::create();
	QStringList paths = files();
	foreach(QString path, paths) {
		QFile file(path);
		QFileInfo fileInfo(file);
		if(fileInfo.suffix() == LINKS_EXT) continue;
		if(file.open(QIODevice::ReadOnly)) {
			QString fileName = fileInfo.fileName();
			if(fileInfo.suffix() == PROJECT_EXT) {
				fileName.chop(QString(PROJECT_EXT).length());
				fileName += "ops";
			}
			archive->addFile(fileName, file.readAll());
			file.close();
		}
	}
	return archive;
}

Kiss::Project::Project::Project(const QString &location)
	: m_location(location)
{
	setName(QFileInfo(m_location).fileName());
	QDir dir(m_location);
	m_settings = Compiler::Options::load(dir.absoluteFilePath(dir.dirName() + "." + PROJECT_EXT));
	m_settings.insert("", "");
	save();

	qDebug() << "LINKS FILE:" << linksFilePath();
}
