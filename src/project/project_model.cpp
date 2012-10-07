#include "project_model.hpp"
#include "project.hpp"

#include <QFileInfo>
#include <QFileIconProvider>
#include <QDir>

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
		QFileInfoList entries = dir().entryInfoList(QDir::Dirs | QDir::Files
			| QDir::NoDot | QDir::NoDotDot);
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
	RootItem(const QString& path)
		: FolderItem(path)
	{
		setEditable(false);
	}
	
	template<typename T>
	static RootItem *rootitem_cast(T *item)
	{
		return dynamic_cast<RootItem *>(item);
	}
};



MultiRootFilesystemModel::MultiRootFilesystemModel(QObject *parent)
	: QStandardItemModel(parent)
{
	connect(&m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(pathChanged(QString)));
	connect(&m_watcher, SIGNAL(fileChanged(QString)), this, SLOT(pathChanged(QString)));
	connect(this, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemRenamed(QStandardItem *)));
}

void MultiRootFilesystemModel::pathChanged(const QString& path)
{
	QString absolutePath = QFileInfo(path).absoluteFilePath();
	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		QStandardItem *child = root->child(i);
		PathItem *pathItem = PathItem::pathitem_cast(child);
		if(!pathItem) continue;
		if(pathItem->path() == absolutePath || pathItem->path() == path) pathItem->refresh();
	}
}

void MultiRootFilesystemModel::addRootPath(const QString& path)
{
	if(m_paths.contains(path)) return;
	m_paths.append(path);
	insertRow(0, new RootItem(QFileInfo(path).absoluteFilePath()));
	m_watcher.addPath(path);
}

void MultiRootFilesystemModel::removeRootPath(const QString& path)
{
	m_paths.removeAll(path);
	m_watcher.removePath(path);
	
	QString absolutePath = QFileInfo(path).absoluteFilePath();
	
	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		QStandardItem *child = root->child(i);
		PathItem *pathItem = PathItem::pathitem_cast(child);
		if(!pathItem) continue;
		if(pathItem->path() == absolutePath || pathItem->path() == path) root->takeRow(i--);
	}
}

const QStringList& MultiRootFilesystemModel::rootPaths() const
{
	return m_paths;
}

void MultiRootFilesystemModel::itemChanged(QStandardItem *item)
{
	PathItem *pathItem = PathItem::pathitem_cast(item);
	if(!pathItem) return;
	pathItem->rename(pathItem->text());
}

Model::Model()
{
	
}

Model::~Model()
{
	m_projects.clear();
}

Kiss::Project::Project *Model::indexToProject(const QModelIndex& index) const
{
	PathItem *pathItem = PathItem::pathitem_cast(itemFromIndex(index));
	if(!pathItem) return 0;
	return m_projects.value(pathItem->path(), 0);
}

void Model::addProject(Kiss::Project::Project *project)
{
	const QString& location = project->location();
	if(m_projects.contains(location)) return;
	m_projects.insert(location, project);
	addRootPath(location);
}

void Model::removeProject(Kiss::Project::Project *project)
{
	const QString& location = project->location();
	m_projects.remove(location);
	removeRootPath(location);
}