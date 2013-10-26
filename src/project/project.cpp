#include "project.hpp"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "file_utils.hpp"
#include "communication_manager.hpp"
#include "system_utils.hpp"

using namespace kiss;
using namespace kiss::project;

Project::~Project()
{
  
}

ProjectPtr kiss::project::Project::load(const QString &location)
{
	if(!QFileInfo(location).isDir()) return ProjectPtr();
	return ProjectPtr(new Project(location));
}

bool kiss::project::Project::save()
{
	QDir dir(m_location);
	return m_settings.save(dir.absoluteFilePath(dir.dirName() + "." + PROJECT_EXT));
}

const bool kiss::project::Project::download() const
{
	using namespace kiss::target;

	const QStringList &depsList = dependencies();
	foreach(const QString &dep, depsList) {
		const ProjectPtr &depProject = project::Project::load(dep);
		if(!depProject) {
			qDebug() << "ERROR: failed to open dependency" << dep << "for download";
			return false;
		}
		depProject->setTarget(m_target);
		depProject->download();
	}

	kiss::KarPtr package = archive();
	if(package.isNull()) return false;

	CommunicationManager::ref().admit(CommunicationEntryPtr(
		new CommunicationEntry(m_target, CommunicationEntry::Download,
		m_name, package)));

	return true;
}

const bool kiss::project::Project::compile() const
{
	using namespace kiss::target;

	const QStringList &depsList = dependencies();
	foreach(const QString &dep, depsList) {
		const ProjectPtr &depProject = project::Project::load(dep);
		if(!depProject) {
			qDebug() << "ERROR: failed to open dependency" << dep << "for compilation";
			return false;
		}
		depProject->setSetting("TERMINAL_TYPE", "LIBRARY");
		depProject->setSetting("LIBRARY_NAME", depProject->name());
		depProject->setTarget(m_target);
		depProject->compile();
	}

	CommunicationManager::ref().admit(CommunicationEntryPtr(
			new CommunicationEntry(m_target, CommunicationEntry::Compile, m_name)));
		return true;
}

const bool kiss::project::Project::run() const
{
	using namespace kiss::target;
	
	CommunicationManager::ref().admit(CommunicationEntryPtr(
			new CommunicationEntry(m_target, CommunicationEntry::Run, m_name)));
		return true;
}

bool kiss::project::Project::addAsCopy(const QString &path)
{
	QFileInfo info(path);
	if(!info.isFile()) return false;

	return QFile::copy(path, m_location + "/" + info.fileName());
}

bool kiss::project::Project::addAsMovedCopy(const QString &path)
{
	return (addAsCopy(path) && QFile::remove(path));
}

bool kiss::project::Project::removeFile(const QString &path)
{
	if(!QFileInfo(path).isFile()) return false;
	if(SystemUtils::supportsMoveToTrash()) return SystemUtils::moveToTrash(path);
	return QFile::remove(path);
}

QStringList kiss::project::Project::files() const
{
	QStringList ret;
	QFileInfoList infoList = QDir(m_location).entryInfoList(QDir::Files);
	foreach(QFileInfo file, infoList) ret << file.absoluteFilePath();
	ret << links();

	return ret;
}

bool kiss::project::Project::addAsLink(const QString &path)
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

bool kiss::project::Project::addAsRelativeLink(const QString &path)
{
	return addAsLink(FileUtils::relativePath(path, QDir(m_location)));
}

bool kiss::project::Project::removeLink(const QString &path)
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

QStringList kiss::project::Project::links() const
{
	QStringList list;
	
	QFile linkFile(linksFilePath());
	if(linkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&linkFile);
		while(!in.atEnd()) list << in.readLine();
		linkFile.close();
	}

	return list;
}

const QString kiss::project::Project::linksFilePath() const
{
	const QDir dir(m_location);
	return dir.absoluteFilePath(dir.dirName() + "." + LINKS_EXT);
}

void kiss::project::Project::setSetting(const QString &key, const QString &value)
{
	m_settings[key] = value;
	save();
}

void kiss::project::Project::setSettings(const Compiler::Options &settings)
{
	m_settings = settings;
	save();
}

void kiss::project::Project::removeSetting(const QString &key)
{
	m_settings.remove(key);
	save();
}

const Compiler::Options &kiss::project::Project::settings() const
{
	return m_settings;
}

void kiss::project::Project::setDependencies(const QStringList &deps)
{
	QFile depsFile(dependenciesFilePath());
	if (!depsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qWarning() << "Failed to open deps file for project!";
		return;
	}
	QTextStream out(&depsFile);
	foreach(const QString &dep, deps) out << dep << endl;
	depsFile.close();
}

QStringList kiss::project::Project::dependencies() const
{
	QStringList list;
	QFile depsFile(dependenciesFilePath());
	if (!depsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Failed to open deps file for project!";
		return QStringList();
	}
	QTextStream in(&depsFile);
	while(!in.atEnd()) list << in.readLine();
	depsFile.close();

	return list;
}

const QString kiss::project::Project::dependenciesFilePath() const
{
	const QDir dir(m_location);
	return dir.absoluteFilePath(dir.dirName() + "." + DEPS_EXT);
}

void kiss::project::Project::setTarget(const target::TargetPtr &target)
{
	m_target = target;
}

target::TargetPtr kiss::project::Project::target() const
{
	return m_target;
}

void kiss::project::Project::setName(const QString &name)
{
	m_name = name;
}

const QString &kiss::project::Project::name() const
{
	return m_name;
}

const QString &kiss::project::Project::location() const
{
	return m_location;
}

kiss::KarPtr kiss::project::Project::archive() const
{
	kiss::KarPtr archive = kiss::Kar::create();
	QStringList paths = files();
	foreach(const QString &path, paths) {
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

kiss::project::Project::Project(const QString &location)
	: m_location(location)
{
	m_name = QFileInfo(m_location).fileName();

	QDir dir(m_location);
	m_settings = Compiler::Options::load(dir.absoluteFilePath(dir.dirName() + "." + PROJECT_EXT));
	m_settings.insert("", "");
	save();

	QFile linksFile(linksFilePath());
	if(!linksFile.exists()) {
		if(linksFile.open(QIODevice::WriteOnly)) linksFile.close();
		else qDebug() << "Failed to create" << linksFilePath();
	}

	QFile depsFile(dependenciesFilePath());
	if(!depsFile.exists()) {
		if(depsFile.open(QIODevice::WriteOnly)) depsFile.close();
		else qDebug() << "Failed to create" << dependenciesFilePath();
	}
}
