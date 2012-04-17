#ifndef _PROJECTSMODEL_H_
#define _PROJECTSMODEL_H_

#include <QStandardItemModel>

class ProjectManager;
class Project;

class ProjectsModel : public QStandardItemModel
{
Q_OBJECT
public:
	ProjectsModel();
	void setProjectManager(ProjectManager* manager);
	
	Project* indexToProject(const QModelIndex& index) const;
	QString indexToPath(const QModelIndex& index) const;
	
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
	void addDirectory(const QString& path, const QStringList& paths, QStandardItem* parent, Project* project);
	void reloadProject(Project* project, QStandardItem* parent);
	QStandardItem* lookupRoot(Project* project);
	QMap<QString, QStringList> merge(const QStringList& list, const char delim);
};

#endif
