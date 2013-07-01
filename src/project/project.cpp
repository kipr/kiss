#include "project.hpp"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "file_utils.hpp"
#include "communication_manager.hpp"

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
	QStringList linksList = links();
	QDir projectDir(m_location);
	const QString &absPath = FileUtils::absolutePath(path, projectDir);
	const QString &relPath = FileUtils::relativePath(path, projectDir);
	if(!projectDir.exists(path) || linksList.contains(absPath) ||
		linksList.contains(relPath)) return false;

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
	return addAsLink(FileUtils::relativePath(path, QDir(m_location)));
}

bool Kiss::Project::Project::removeLink(const QString &path)
{
	QStringList linksList = links();
	QDir projectDir(m_location);
	if(!projectDir.exists(path)) return false;

	const QString &absPath = FileUtils::absolutePath(path, projectDir);
	const QString &relPath = FileUtils::relativePath(path, projectDir);
	if(!linksList.removeOne(absPath) && !linksList.removeOne(relPath)) return false;

	QFile linkFile(linksFilePath());
    if (!linkFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    	qWarning() << "Failed to open links file for project!";
        return false;
    }
    QTextStream out(&linkFile);
    foreach(const QString &entry, linksList) out << entry << "\n";
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

void Kiss::Project::Project::setName(const QString &name)
{
	m_name = name;
}

const QString &Kiss::Project::Project::name() const
{
	return m_name;
}

void Kiss::Project::Project::setTarget(const Target::TargetPtr &target)
{
	m_target = target;
}

Target::TargetPtr Kiss::Project::Project::target() const
{
	return m_target;
}

const bool Kiss::Project::Project::download() const
{
	using namespace Kiss::Target;

	Kiss::KarPtr package = archive();
	if(package.isNull()) return false;

	CommunicationManager::ref().admit(CommunicationEntryPtr(
		new CommunicationEntry(m_target, CommunicationEntry::Download,
		m_name, package)));

	return true;
}

const bool Kiss::Project::Project::compile() const
{
	using namespace Kiss::Target;

	CommunicationManager::ref().admit(CommunicationEntryPtr(
			new CommunicationEntry(m_target, CommunicationEntry::Compile, m_name)));
		return true;
}

const bool Kiss::Project::Project::run() const
{
	using namespace Kiss::Target;
	
	CommunicationManager::ref().admit(CommunicationEntryPtr(
			new CommunicationEntry(m_target, CommunicationEntry::Run, m_name)));
		return true;
}

Kiss::Project::Project::Project(const QString &location)
	: m_location(location)
{
	m_name = QFileInfo(m_location).fileName();
	QDir dir(m_location);
	m_settings = Compiler::Options::load(dir.absoluteFilePath(dir.dirName() + "." + PROJECT_EXT));
	m_settings.insert("", "");
	save();
}
