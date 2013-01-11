#include "project_model.hpp"

#include "linked_kar.hpp"

#include <QFileInfo>

using namespace Kiss;
using namespace Kiss::Project;

class ProjectRoot : public QStandardItem
{
public:
	ProjectRoot(const ProjectPtr &project)
		: QStandardItem(QFileInfo(project->location()).fileName()),
		m_project(project)
	{
	}
	
	~ProjectRoot()
	{
	}
	
	const ProjectPtr &project() const
	{
		return m_project;
	}
	
	template<typename T>
	static ProjectRoot *cast(T *t)
	{
		return dynamic_cast<ProjectRoot *>(t);
	}
	
private:
	ProjectPtr m_project;
};

class ProjectFile : public QStandardItem
{
public:
	ProjectFile(const QString &path)
		: QStandardItem(QFileInfo(path).fileName()),
		m_path(path)
	{
	}
	
	~ProjectFile()
	{
	}
	
	const QString &path() const
	{
		return m_path;
	}
	
	template<typename T>
	static ProjectFile *cast(T *t)
	{
		return dynamic_cast<ProjectFile *>(t);
	}
	
private:
	QString m_path;
};

Model::Model(QObject *parent)
	: QStandardItemModel(parent)
{
}

Model::~Model()
{
}

bool Model::isLink(const QModelIndex &index) const
{
	QStandardItem *item = itemFromIndex(index);
	ProjectFile *projectFile = ProjectFile::cast(item);
	if(!projectFile) return false;
	return LinkedKar::links(project(index)->archive()).contains(projectFile->path());
}

bool Model::isFile(const QModelIndex &index) const
{
	return ProjectFile::cast(itemFromIndex(index));
}

bool Model::isProjectRoot(const QModelIndex &index) const
{
	return ProjectRoot::cast(itemFromIndex(index));
}

QString Model::filePath(const QModelIndex &index) const
{
	ProjectFile *projectFile = ProjectFile::cast(itemFromIndex(index));
	if(!projectFile) return QString();
	return projectFile->path();
}

ProjectPtr Model::project(const QModelIndex &index) const
{
	QStandardItem *item = itemFromIndex(index);
	ProjectRoot *projectRoot = ProjectRoot::cast(item);
	if(!projectRoot) {
		ProjectFile *projectFile = ProjectFile::cast(item);
		if(!projectFile) return ProjectPtr();
		// TODO: This assumes a strictly two-level project model
		projectRoot = ProjectRoot::cast(projectFile->parent());
	}
	return projectRoot ? projectRoot->project() : ProjectPtr();
}