#include "project_model.hpp"

#include "linked_kar.hpp"

#include <QFileInfo>
#include <QDir>
#include <QFileIconProvider>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Project;

class PathItem : public QStandardItem
{
public:
	PathItem(const QString& path)
			: QStandardItem(QFileInfo(path).fileName()),
			m_path(path)
	{
		refresh();
	}

	const QString& path()
	{
		return m_path;
	}

	bool rename(const QString& name)
	{
		qDebug() << "Renaming to " << name;
		QFileInfo info(m_path);
		if(!QDir(info.absolutePath()).rename(info.fileName(), name)) return false;
		m_path = info.absolutePath() + "/" + name;
		return true;
	}

	void setPath(const QString& path)
	{
		m_path = path;
		refresh();
	}

	virtual void refresh()
	{
		QFileIconProvider ip;
		setIcon(ip.icon(QFileInfo(m_path)));
	}

	template<typename T>
	static PathItem *pathitem_cast(T *item)
	{
		return dynamic_cast<PathItem *>(item);
	}

private:
	QString m_path;
};

class FileItem : public PathItem
{
public:
	FileItem(const QString& path)
			: PathItem(path)
	{
		refresh();
	}

	virtual void refresh()
	{
		PathItem::refresh();
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
	FolderItem(const QString& path)
			: PathItem(path)

	{
		refresh();
	}

	QDir dir()
	{
		return QDir(path());
	}

	virtual void refresh()
	{
		PathItem::refresh();
		for(int i = 0; i < rowCount(); ++i) removeRow(i--);
		QFileInfoList entries = dir().entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDot | QDir::NoDotDot);
		foreach(const QFileInfo& entry, entries) {
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

class RootItem : public FolderItem
{
public:
	RootItem(const QString& path, ProjectPtr project)
			: FolderItem(path),
			m_project(project)
	{
		setEditable(false);
		refresh();
	}

	template<typename T>
	static RootItem *rootitem_cast(T *item)
	{
		return dynamic_cast<RootItem *>(item);
	}

	void refresh()
	{
		FolderItem::refresh();

		QStringList list = m_project->links();
		foreach(const QString& entry, list) {
			PathItem* item = (PathItem*)new FileItem(entry);
			item->setForeground(Qt::gray);
			appendRow(item);
		}
	}

	ProjectPtr project() const
	{
		return m_project;
	}

private:
	ProjectPtr m_project;
};

/*MultiRootFilesystemModel::MultiRootFilesystemModel(QObject *parent)
: QStandardItemModel(parent)
{
	connect(&m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(pathChanged(QString)));
	connect(&m_watcher, SIGNAL(fileChanged(QString)), this, SLOT(pathChanged(QString)));
	connect(this, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));
}*/

Model::Model(QObject *parent)
			: QStandardItemModel(parent)
{
	connect(&m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(pathChanged(QString)));
	connect(&m_watcher, SIGNAL(fileChanged(QString)), this, SLOT(pathChanged(QString)));
	connect(this, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));
}

Model::~Model()
{
}

void Model::addProject(ProjectPtr project)
{
	//if(m_projects.contains(location)) return;
	addRootPath(project);
}

void Model::removeProject(ProjectPtr project)
{
	removeRootPath(project->location());
}

void Model::addRootPath(ProjectPtr project)
{
	const QString& path = project->location();
	if(m_paths.contains(path)) return;

	m_paths.append(path);
	insertRow(0, new RootItem(QFileInfo(path).absoluteFilePath(), project));
	m_watcher.addPath(path);
	m_watcher.addPath(path + "/" + LINKS_FILE);
}

void Model::removeRootPath(const QString& path)
{
	m_paths.removeAll(path);
	m_watcher.removePath(path);
	m_watcher.removePath(path + "/" + LINKS_FILE);

	QString absolutePath = QFileInfo(path).absoluteFilePath();

	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		QStandardItem *child = root->child(i);
		PathItem *pathItem = PathItem::pathitem_cast(child);
		if(!pathItem) continue;
		if(pathItem->path() == absolutePath || pathItem->path() == path) root->takeRow(i--);
	}
}

const QStringList& Model::rootPaths() const
{
	return m_paths;
}

bool Model::isIndexProject(const QModelIndex& index) const
{
	return RootItem::rootitem_cast(itemFromIndex(index));
}

bool Model::isLink(const QModelIndex &index) const
{
	ProjectPtr proj = project(index);
	FileItem *file = FileItem::fileitem_cast(itemFromIndex(index));
	return proj->links().contains(file->path());
}

bool Model::isFile(const QModelIndex &index) const
{
	return FileItem::fileitem_cast(itemFromIndex(index));
}

bool Model::isProjectRoot(const QModelIndex &index) const
{
	return RootItem::rootitem_cast(itemFromIndex(index));
}

QString Model::filePath(const QModelIndex &index) const
{
	FileItem *projectFile = FileItem::fileitem_cast(itemFromIndex(index));
	if(!projectFile) return QString();
	return projectFile->path();
}

ProjectPtr Model::project(const QModelIndex &index) const
{
	QStandardItem *item = itemFromIndex(index);
	RootItem *projectRoot = RootItem::rootitem_cast(item);
	if(!projectRoot) {
		FileItem *projectFile = FileItem::fileitem_cast(item);
		if(!projectFile) return ProjectPtr();
	// TODO: This assumes a strictly two-level project model
		projectRoot = RootItem::rootitem_cast(projectFile->parent());
	}

	return projectRoot ? projectRoot->project() : ProjectPtr();
}

void Model::pathChanged(const QString& path)
{
	QString absolutePath = QFileInfo(path).absoluteFilePath();

	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		QStandardItem *child = root->child(i);
		PathItem *pathItem = PathItem::pathitem_cast(child);
		if(!pathItem) continue;
		if(pathItem->path() == absolutePath || pathItem->path() == path || pathItem->path() + "/" + LINKS_FILE == absolutePath)
			pathItem->refresh();
	}
}

void Model::itemChanged(QStandardItem *item)
{
	PathItem *pathItem = PathItem::pathitem_cast(item);
	if(!pathItem) return;
	pathItem->rename(pathItem->text());
}
