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
	return m_options.save(m_projectFilename);
}

const bool kiss::project::Project::download() const
{
	using namespace kiss::target;

	if(autoCompileDeps()) {
		const QStringList &depsList = deps();
		foreach(const QString &dep, depsList) {
			const ProjectPtr &depProject = project::Project::load(dep);
			if(!depProject) {
				qDebug() << "ERROR: failed to open dependency" << dep << "for download";
				return false;
			}
			depProject->setTarget(m_target);
			depProject->download();
		}
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

	if(autoCompileDeps()) {
		const QStringList &depsList = deps();
		foreach(const QString &dep, depsList) {
			const ProjectPtr &depProject = project::Project::load(dep);
			if(!depProject) {
				qDebug() << "ERROR: failed to open dependency" << dep << "for compilation";
				return false;
			}
			depProject->setCompileLib(true);
			depProject->setCompilerFlag("LIBRARY_NAME", depProject->name());
			depProject->setTarget(m_target);
			depProject->compile();
		}
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

bool kiss::project::Project::addFileAsCopy(const QString &path, const QString &dest)
{
	QFileInfo info(path);
	if(!info.isFile()) return false;

	return QFile::copy(path, dest + "/" + info.fileName());
}

bool kiss::project::Project::addFileAsMovedCopy(const QString &path, const QString &dest)
{
	return (addFileAsCopy(path, dest) && QFile::remove(path));
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

bool kiss::project::Project::addFileAsLink(const QString &path)
{
	QStringList linksList = links();
	QDir projectDir(m_location);
	const QString &absPath = FileUtils::absolutePath(path, projectDir);
	const QString &relPath = FileUtils::relativePath(path, projectDir);
	if(!projectDir.exists(path) || linksList.contains(absPath) ||
		linksList.contains(relPath)) return false;
	
	QStringList links = m_options.value(KEY_LINKS).toStringList();
	links << path;
	m_options.insert(KEY_LINKS, links);
	save();

	return true;
}

bool kiss::project::Project::addFileAsRelativeLink(const QString &path)
{
	return addFileAsLink(FileUtils::relativePath(path, QDir(m_location)));
}

bool kiss::project::Project::removeLink(const QString &path)
{
	QDir projectDir(m_location);
	if(!projectDir.exists(path)) return false;

	const QString &absPath = FileUtils::absolutePath(path, projectDir);
	const QString &relPath = FileUtils::relativePath(path, projectDir);
	
	QStringList links = m_options.value(KEY_LINKS).toStringList();
	const bool ret1 = links.removeOne(absPath);
	const bool ret2 = links.removeOne(relPath);
	m_options.insert(KEY_LINKS, links);
	
	save();

	return (ret1 || ret2);
}

QStringList kiss::project::Project::links() const
{	
	return m_options.value(KEY_LINKS).toStringList();
}

void kiss::project::Project::setDeps(const QStringList &deps)
{
	m_options.insert(KEY_DEPS, deps);
	save();
}

QStringList kiss::project::Project::deps() const
{	
	return m_options.value(KEY_DEPS).toStringList();
}

void kiss::project::Project::setAutoCompileDeps(bool autoCompileDeps)
{
	m_options.insert(KEY_AUTO_COMPILE_DEPS, autoCompileDeps);
	save();
}

bool kiss::project::Project::autoCompileDeps() const
{
	return m_options.value(KEY_AUTO_COMPILE_DEPS).toBool();
}

void kiss::project::Project::setCompilerFlag(const QString &flag, const QString &value)
{
	m_options.insert(flag, value);
	save();
}

void kiss::project::Project::setCompilerFlags(const Compiler::Options &flags)
{
	const QStringList reserved = reservedKeys();
	foreach(const QString &key, m_options.keys()) {
		if(reserved.contains(key)) continue;
		m_options.remove(key);
	}
	
	foreach(const QString &key, flags.keys()) m_options.insert(key, flags.value(key));
	
	save();
}

void kiss::project::Project::removeCompilerFlag(const QString &flag)
{
	m_options.remove(flag);
	save();
}

const Compiler::Options kiss::project::Project::compilerFlags() const
{
	Compiler::Options ret = m_options;
	const QStringList reserved = reservedKeys();
	foreach(const QString &key, ret.keys()) if(reserved.contains(key)) ret.remove(key);
	
	return ret;
}

void kiss::project::Project::setCompileLib(bool lib)
{
	m_options.insert(KEY_COMPILE_LIB, lib);
	save();
}

bool kiss::project::Project::compileLib() const
{
	return m_options.value(KEY_COMPILE_LIB).toBool();
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

const QString &kiss::project::Project::projectFilename() const
{
	return m_projectFilename;
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
		if(!file.open(QIODevice::ReadOnly)) continue;
    QString fileName = fileInfo.fileName();
    if(fileInfo.suffix() == PROJECT_EXT) {
      fileName.chop(QString(PROJECT_EXT).length());
      fileName += "ops";
    }
    archive->addFile(fileName, file.readAll());
    file.close();
	}
	return archive;
}

kiss::project::Project::Project(const QString &location)
	: m_location(location)
{
	m_name = QFileInfo(m_location).fileName();

	QDir dir(location);
	m_projectFilename = dir.absoluteFilePath(dir.dirName() + "." + PROJECT_EXT);
	m_options = Compiler::Options::load(m_projectFilename);
	m_options.insert("", "");
	save();
	m_options.remove("");
	save();
}

QStringList kiss::project::Project::reservedKeys() const
{
	return QStringList() << KEY_DEPS << KEY_LINKS << KEY_AUTO_COMPILE_DEPS << KEY_COMPILE_LIB;
}
