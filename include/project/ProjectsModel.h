#ifndef _PROJECTSMODEL_H_
#define _PROJECTSMODEL_H_

#include <QStandardItemModel>
#include <QMap>
#include <tinyarchive.hpp>

class ProjectManager;
class Project;
class TinyNode;

class ProjectsModel : public QStandardItemModel, private TinyArchiveListener
{
Q_OBJECT
public:
	ProjectsModel();
	~ProjectsModel();
	void setProject(Project* project);
	void setProjectManager(ProjectManager* manager);
	
	Project* indexToProject(const QModelIndex& index) const;
	const TinyNode* indexToNode(const QModelIndex& index) const;
	
	QModelIndex rootIndexForProject(Project* project) const;
	
	enum Type {
		ProjectType,
		FileType,
		FolderType,
		UnknownType
	};
	
	int indexType(const QModelIndex& index) const;
	
private slots:
	void projectOpened(Project* project);
	void projectChanged(Project* project);
	void projectClosed(Project* project);
	
	void itemChanged(QStandardItem* item);
	
private:
	void addDirectory(const TinyNode* node, QStandardItem* parent, Project* project);
	void reloadProject(Project* project, QStandardItem* parent);
	QStandardItem* lookupRoot(Project* project);
	
	void nodeAdded(const TinyNode* node);
	void nodeRemoved(const TinyNode* node);
	void nodeUpdated(const TinyNode* node);
	
	ProjectManager* m_manager;
	Project* m_project;
	
	QMap<const TinyNode*, QStandardItem*> m_nodeLookup;
};

#endif
