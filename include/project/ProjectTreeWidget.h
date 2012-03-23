#ifndef _PROJECTTREEWIDGET_H_
#define _PROJECTTREEWIDGET_H_

#include <QTreeWidget>

#include "ProjectManager.h"
#include "Project.h"

class ProjectTreeWidgetItem;

class ProjectTreeWidget : public QTreeWidget
{
Q_OBJECT
public:
	ProjectTreeWidget(QWidget* parent = 0);
	
	void setManager(ProjectManager* manager);
	ProjectManager* manager() const;
	
	void addUnassociatedFile(const QString& path);
	void removeUnassociatedFile(const QString& path);
	
	Project* activeProject() const;
	
signals:
	void projectClicked(Project* project);
	void fileClicked(Project* project, const QString& path);
	
private slots:
	void projectChanged(Project* project);
	void projectOpened(Project* project);
	void projectClosed(Project* project);
	void itemClicked(QTreeWidgetItem* item);
	
private:
	void refreshUnassoicatedFiles();
	void addDirectory(const QString& path, const QStringList& paths, ProjectTreeWidgetItem* parent, Project* project);
	void reloadProject(Project* project, ProjectTreeWidgetItem* parent);
	ProjectTreeWidgetItem* lookupProjectItem(Project* project) const;
	int lookupProjectItemIndex(Project* project) const;
	
	QMap<QString, QStringList> merge(const QStringList& list, const char delim);
	
	ProjectManager* m_manager;
	
	QList<QString> m_unassociatedFiles;
};

#endif
