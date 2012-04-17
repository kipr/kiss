#include "ProjectsModel.h"

#include "Project.h"
#include "ProjectManager.h"

#include "ResourceHelper.h"

#include <QFileInfo>
#include <QDebug>

#define PATH_ROLE Qt::UserRole + 1
#define PROJECT_ROLE Qt::UserRole + 2

struct ProjectItem : QStandardItem
{
	ProjectItem(Project* project) : QStandardItem(project->name()), m_project(project) {
		setData(ResourceHelper::ref().icon("brick.png"), Qt::DecorationRole);
		setData(QVariant::fromValue(project), PROJECT_ROLE);
		setEditable(false);
	}
	
	Project* project() const { return m_project; }
	
private:
	Project* m_project;
};

struct FileItem : QStandardItem
{
	FileItem(const QString& path, const QString& name, Project* project) : QStandardItem(name), m_path(""), m_project(project) {
		setData(ResourceHelper::ref().icon("page_white.png"), Qt::DecorationRole);
		m_path = path + (!path.isEmpty() ? "/" : "") + name;
		setData(QVariant::fromValue(project), PROJECT_ROLE);
		setEditable(false);
	}
	
	const QString& path() { return m_path; }
	Project* project() const { return m_project; }
	
private:
	QString m_path;
	Project* m_project;
};

ProjectsModel::ProjectsModel()
{
	connect(this, SIGNAL(itemChanged(QStandardItem*)), SLOT(itemChanged(QStandardItem*)));
}

void ProjectsModel::setProjectManager(ProjectManager* projectManager)
{
	connect(projectManager, SIGNAL(projectChanged(Project*)), SLOT(projectChanged(Project*)));
	connect(projectManager, SIGNAL(projectOpened(Project*)), SLOT(projectOpened(Project*)));
	connect(projectManager, SIGNAL(projectClosed(Project*)), SLOT(projectClosed(Project*)));
}

void ProjectsModel::addDirectory(const QString& path, const QStringList& paths, QStandardItem* parent, Project* project)
{
	QMap<QString, QStringList> merged = merge(paths, '/');
	qWarning() << merged;
	parent->setColumnCount(1);
	parent->setRowCount(merged.size());
	quint16 i = 0;
	foreach(const QString& key, merged.keys()) {
		FileItem* item = new FileItem(path, key, project);
		if(merged[key].size() != 0) item->setData(ResourceHelper::ref().icon("folder"), Qt::DecorationRole);
		
		qWarning() << "Added a child at" << i;
		parent->setChild(i++, item);

		addDirectory(path + "/" + key, merged[key], item, project);
	}
}

void ProjectsModel::reloadProject(Project* project, QStandardItem* parent)
{
	if(!project) return;
	
	parent->setData(ResourceHelper::ref().icon("brick.png"), Qt::DecorationRole);
	QList<QStandardItem*> items = parent->takeColumn(0);
	foreach(QStandardItem* item, items) delete item;
	
	
	QStringList files = project->projectFile()->list();
	qWarning() << "Detected" << files;
	addDirectory("", files, parent, project);
}

Project* ProjectsModel::indexToProject(const QModelIndex& index) const
{
	ProjectItem* p = dynamic_cast<ProjectItem*>(itemFromIndex(index));
	if(p) return p->project();
	FileItem* f = dynamic_cast<FileItem*>(itemFromIndex(index));
	if(f) return f->project();
	return 0;
}

QString ProjectsModel::indexToPath(const QModelIndex& index) const
{
	FileItem* p = dynamic_cast<FileItem*>(itemFromIndex(index));
	return p ? p->path() : QString();
}

int ProjectsModel::indexType(const QModelIndex& index) const
{
	QStandardItem* item = itemFromIndex(index);
	
	if(dynamic_cast<ProjectItem*>(item)) return ProjectType;
	else if(dynamic_cast<FileItem*>(item)) return FileType;
	
	return UnknownType;
}

void ProjectsModel::projectOpened(Project* project)
{
	ProjectItem* item = new ProjectItem(project);
	reloadProject(project, item);
	appendRow(item);
}

void ProjectsModel::projectChanged(Project* project)
{
	qWarning() << "Project changed";
	reloadProject(project, lookupRoot(project));
	
	//emit dataChanged(QModelIndex(), QModelIndex());
}

void ProjectsModel::projectClosed(Project* project)
{
	removeRow(lookupRoot(project)->row());
	//emit dataChanged(QModelIndex(), QModelIndex());
}

void ProjectsModel::itemChanged(QStandardItem* item)
{
	/* FileItem* fileItem = dynamic_cast<FileItem*>(item);
	if(fileItem) {
		const QString& path = fileItem->path();
		fileItem->project()->projectFile()->moveFile(path, QFileInfo(path).path() + "/" + item->text());
	} */
}

QMap<QString, QStringList> ProjectsModel::merge(const QStringList& list, const char delim)
{
	QMap<QString, QStringList> ret;
	
	foreach(const QString& item, list) {
		QString first = item.section(delim, 0, 0);

		if(item.split(delim).size() == 1) ret[first] = QStringList();
		else ret[first].append(item.section(delim, 1));
	}
	
	return ret;
}

QStandardItem* ProjectsModel::lookupRoot(Project* project)
{
	quint16 i = 0;
	for(; i < rowCount(); ++i) {
		ProjectItem* p = dynamic_cast<ProjectItem*>(item(i));
		if(!p) continue;
		
		if(p->project() == project) return p;
	}
	return 0;
}