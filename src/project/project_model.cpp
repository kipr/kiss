#include "project_model.hpp"

#include "project_manager.hpp"
#include "file_utils.hpp"
#include "resource_helper.hpp"
#include "extension_helper.hpp"

#include <QFileInfo>
#include <QDir>
#include <QFileIconProvider>
#include <QDebug>
#include <QUrl>
#include <QMessageBox>

using namespace kiss;
using namespace kiss::project;

class PathItem : public QStandardItem
{
public:
	PathItem(const QString &path)
			: QStandardItem(QFileInfo(path).fileName()),
			m_path(path)
	{
		setEditable(false);
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
  
	virtual void setActive(bool active)
	{
		setForeground(active ? Qt::black : Qt::lightGray);
    for(int i = 0; i < rowCount(); ++i) {
      PathItem *pathItem = PathItem::pathitem_cast(child(i));
      if(!pathItem) continue;
      pathItem->setActive(active);
    }
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
		setIcon(ResourceHelper::ref().icon(ExtensionHelper::icon(path)));
		setEditable(true);
		refresh();
	}

	virtual void refresh()
	{
	}

	bool isFileEditable()
	{
		return ExtensionHelper::isFileEditable(m_path);
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

class ProjectItem : public FolderItem
{
public:
	ProjectItem(const QString &path, ProjectPtr project)
			: FolderItem(path),
			m_project(project)
	{
		refresh();
	}

	template<typename T>
	static ProjectItem *projectitem_cast(T *item)
	{
		return dynamic_cast<ProjectItem *>(item);
	}

  virtual void setActive(bool active)
  {
    FolderItem::setActive(active);
    if(active) setIcon(ResourceHelper::ref().icon("folder_heart.png"));
    else setIcon(ResourceHelper::ref().icon("folder.png"));
  }

	virtual void refresh()
	{
		FolderItem::refresh();

		const QStringList &list = m_project->links();
		foreach(const QString &entry, list) {
			PathItem *item = (PathItem*)new FileItem(FileUtils::absolutePath(entry, QDir(m_path)));
			item->setForeground(Qt::gray);
			appendRow(item);
		}

		/*const QStringList &deps = m_project->deps();
		foreach(const QString &dep, deps) {
			appendRow(new FileItem(dep));
		}*/
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
	
	m_watcher.addPath(project->projectFilename());
}

void Model::removeProject(ProjectPtr project)
{
	const QString &path = project->location();
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

const QStringList &Model::projects() const
{
	return m_paths;
}

bool Model::isProject(const QModelIndex &index) const
{
	return ProjectItem::projectitem_cast(itemFromIndex(index));
}

bool Model::isLink(const QModelIndex &index) const
{
	ProjectPtr proj = project(index);
	QStringList list = proj->links();
	FileItem *file = FileItem::fileitem_cast(itemFromIndex(index));
	if(!file) return false;
	const QString &path = file->path();
	const QString &absPath = FileUtils::absolutePath(path, QDir(proj->location()));
	const QString &relPath = FileUtils::relativePath(path, QDir(proj->location()));
	
	return list.contains(absPath) || list.contains(relPath);
}

bool Model::isFile(const QModelIndex &index) const
{
	return FileItem::fileitem_cast(itemFromIndex(index));
}

bool Model::isFileEditable(const QModelIndex &index) const
{
	FileItem *fileItem = FileItem::fileitem_cast(itemFromIndex(index));
	return fileItem && fileItem->isFileEditable();
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

Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if(isLink(index)) return defaultFlags;
	if(isFile(index)) return defaultFlags | Qt::ItemIsDragEnabled | Qt::ItemIsEditable;
	if(isProject(index)) return defaultFlags | Qt::ItemIsDropEnabled;
	return defaultFlags;
}

QStringList Model::mimeTypes() const
{
	return QStringList() << "application/vnd.text.list";
}

QMimeData *Model::mimeData(const QModelIndexList &indexes) const
{
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	foreach(const QModelIndex &index, indexes) {
		if(index.isValid()) stream << filePath(index);
	}

	QMimeData *mimeData = new QMimeData();
	mimeData->setData("application/vnd.text.list", encodedData);
	return mimeData;
}

bool Model::dropMimeData(const QMimeData *data, Qt::DropAction action,
	int row, int column, const QModelIndex &parent)
{
	if(action == Qt::IgnoreAction) return true;

	if(data->hasUrls()) {
		QStringList files;
		foreach(QUrl url, data->urls()) {
			if(url.isLocalFile()) files << url.toLocalFile();
		}

		emit filesDropped(files);
	}

	else if(data->hasFormat("application/vnd.text.list")) {
		QByteArray encodedData = data->data("application/vnd.text.list");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);

		QStringList files;
		while(!stream.atEnd()) {
			QString text;
			stream >> text;
			files << text;
		}

		emit filesDropped(files);
	}

	else return false;

	return true;
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
	if(suffix == PROJECT_EXT) directoryChanged(pathInfo.absolutePath());
}

void Model::itemChanged(QStandardItem *item)
{
	PathItem *pathItem = PathItem::pathitem_cast(item);
	if(!pathItem) return;
	pathItem->rename(pathItem->text());
}
