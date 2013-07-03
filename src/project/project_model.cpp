#include "project_model.hpp"

#include "project_manager.hpp"
#include "resource_helper.hpp"

#include <QFileInfo>
#include <QDir>
#include <QFileIconProvider>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Project;

class PathItem : public QStandardItem
{
public:
	PathItem(const QString &path)
			: QStandardItem(QFileInfo(path).fileName()),
			m_path(path)
	{
	}

	bool rename(const QString &name)
	{
		qDebug() << "Renaming to " << name;
		QFileInfo info(m_path);
		if(!QDir(info.absolutePath()).rename(info.fileName(), name)) return false;
		m_path = info.absolutePath() + "/" + name;
		return true;
	}

	void setPath(const QString &path)
	{
		m_path = path;
	}

	const QString &path()
	{
		return m_path;
	}

	virtual void refresh()
	{
	}

	template<typename T>
	static PathItem *pathitem_cast(T *item)
	{
		return dynamic_cast<PathItem *>(item);
	}

protected:
	QString m_path;
};

class FileItem : public PathItem
{
public:
	FileItem(const QString &path)
			: PathItem(path)
	{
		setIcon(ResourceHelper::ref().icon("page_white.png"));
		refresh();
	}

	virtual void refresh()
	{
	}

	template<typename T>
	static FileItem *fileitem_cast(T *item)
	{
		return dynamic_cast<FileItem *>(item);
	}
};

class FolderItem : public PathItem
{
public:
	FolderItem(const QString &path)
			: PathItem(path)

	{
		setIcon(ResourceHelper::ref().icon("folder.png"));
		refresh();
	}

	QDir dir()
	{
		return QDir(path());
	}

	virtual void refresh()
	{
		for(int i = 0; i < rowCount(); ++i) removeRow(i--);
		QFileInfoList entries = dir().entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDot | QDir::NoDotDot);
		const QStringList &hidden = Manager::hiddenExtensions();
		foreach(const QFileInfo &entry, entries) {
			if(hidden.contains(entry.suffix())) continue;
			appendRow(entry.isDir() ? (PathItem *)new FolderItem(entry.absoluteFilePath())
				: (PathItem *)new FileItem(entry.absoluteFilePath()));
		}
	}

	template<typename T>
	static FolderItem *folderitem_cast(T *item)
	{
		return dynamic_cast<FolderItem *>(item);
	}
};

class DependencyItem : public PathItem
{
public:
	DependencyItem(const QString &path)
			: PathItem(path)
	{
		setIcon(ResourceHelper::ref().icon("brick.png"));
		refresh();
	}

	virtual void refresh()
	{
	}

	template<typename T>
	static DependencyItem *dependencyitem_cast(T *item)
	{
		return dynamic_cast<DependencyItem *>(item);
	}
};

class ProjectItem : public FolderItem
{
public:
	ProjectItem(const QString &path, ProjectPtr project)
			: FolderItem(path),
			m_project(project)
	{
		setEditable(false);
		refresh();
	}

	template<typename T>
	static ProjectItem *projectitem_cast(T *item)
	{
		return dynamic_cast<ProjectItem *>(item);
	}

	void setActive(bool active)
	{
		if(active) setIcon(ResourceHelper::ref().icon("folder_heart.png"));
		else setIcon(ResourceHelper::ref().icon("folder.png"));
	}

	virtual void refresh()
	{
		FolderItem::refresh();

		const QStringList &list = m_project->links();
		foreach(const QString &entry, list) {
			PathItem* item = (PathItem*)new FileItem(QDir::cleanPath(QDir(m_path).absoluteFilePath(entry)));
			item->setForeground(Qt::gray);
			appendRow(item);
		}

		const QStringList &deps = m_project->dependencies();
		foreach(const QString &dep, deps) {
			appendRow(new DependencyItem(dep));
		}
	}

	ProjectPtr project() const
	{
		return m_project;
	}

private:
	ProjectPtr m_project;
};

Model::Model(QObject *parent)
			: QStandardItemModel(parent)
{
	connect(&m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
	connect(&m_watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
	connect(this, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));
}

Model::~Model()
{
}

void Model::addProject(ProjectPtr project)
{
	const QString &path = project->location();
	if(m_paths.contains(path)) return;

	m_paths.append(path);
	insertRow(0, new ProjectItem(QFileInfo(path).absoluteFilePath(), project));
	m_watcher.addPath(path);
	m_watcher.addPath(project->linksFilePath());
	m_watcher.addPath(project->dependenciesFilePath());
}

void Model::removeProject(ProjectPtr project)
{
	const QString &path = project->location();
	m_paths.removeAll(path);
	m_watcher.removePath(path);
	m_watcher.removePath(project->linksFilePath());
	m_watcher.removePath(project->dependenciesFilePath());

	QString absolutePath = QFileInfo(path).absoluteFilePath();
	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		QStandardItem *child = root->child(i);
		PathItem *pathItem = PathItem::pathitem_cast(child);
		if(!pathItem) continue;
		if(pathItem->path() == absolutePath || pathItem->path() == path) root->takeRow(i--);
	}
}

const QStringList &Model::projects() const
{
	return m_paths;
}

bool Model::isDependency(const QModelIndex &index) const
{
	return DependencyItem::dependencyitem_cast(itemFromIndex(index));
}

bool Model::isProject(const QModelIndex &index) const
{
	return ProjectItem::projectitem_cast(itemFromIndex(index));
}

bool Model::isLink(const QModelIndex &index) const
{
	ProjectPtr proj = project(index);
	QStringList list = proj->links();
	const QString &path = FileItem::fileitem_cast(itemFromIndex(index))->path();
	if(list.contains(path)) return true;

	QString otherPath;
	if(QFileInfo(path).isAbsolute()) otherPath = QDir(proj->location()).relativeFilePath(path);
	else otherPath = QDir::cleanPath(QDir(proj->location()).absoluteFilePath(path));

	return list.contains(otherPath);
}

bool Model::isFile(const QModelIndex &index) const
{
	return FileItem::fileitem_cast(itemFromIndex(index));
}

ProjectPtr Model::project(const QModelIndex &index) const
{
	QStandardItem *item = itemFromIndex(index);
	ProjectItem *projectItem = ProjectItem::projectitem_cast(item);
	if(!projectItem) {
		FileItem *projectFile = FileItem::fileitem_cast(item);
		if(!projectFile) return ProjectPtr();
		// TODO: This assumes a strictly two-level project model
		projectItem = ProjectItem::projectitem_cast(projectFile->parent());
	}

	return projectItem ? projectItem->project() : ProjectPtr();
}

QString Model::filePath(const QModelIndex &index) const
{
	FileItem *projectFile = FileItem::fileitem_cast(itemFromIndex(index));
	if(!projectFile) return QString();
	return projectFile->path();
}

void Model::activeChanged(const ProjectPtr &oldActive, const ProjectPtr &newActive)
{
	const QString &oldPath = oldActive ? QFileInfo(oldActive->location()).absoluteFilePath() : QString();
	const QString &newPath = newActive ? QFileInfo(newActive->location()).absoluteFilePath() : QString();
	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		QStandardItem *child = root->child(i);
		ProjectItem *projectItem = ProjectItem::projectitem_cast(child);
		if(!projectItem) continue;
		const QString &projectPath = projectItem->path();
		if(projectPath == oldPath) projectItem->setActive(false);
		if(projectPath == newPath) projectItem->setActive(true);
	}
}

void Model::directoryChanged(const QString &path)
{
	const QString &absolutePath = QFileInfo(path).absoluteFilePath();
	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		QStandardItem *child = root->child(i);
		ProjectItem *projectItem = ProjectItem::projectitem_cast(child);
		if(!projectItem) continue;

		const QString &itemPath = projectItem->path();
		if(itemPath != absolutePath && itemPath != path) continue;
		projectItem->refresh();
	}
}

void Model::fileChanged(const QString &path)
{
	const QFileInfo pathInfo(path);
	const QString &suffix = pathInfo.suffix();
	if(suffix == LINKS_EXT || suffix == DEPS_EXT) directoryChanged(pathInfo.absolutePath());
}

void Model::itemChanged(QStandardItem *item)
{
	PathItem *pathItem = PathItem::pathitem_cast(item);
	if(!pathItem) return;
	pathItem->rename(pathItem->text());
}
