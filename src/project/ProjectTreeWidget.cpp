#include "ProjectTreeWidget.h"

#include "ResourceHelper.h"

#include <QDebug>
#include <QFileInfo>

class ProjectTreeWidgetItem : public QTreeWidgetItem
{
public:
	ProjectTreeWidgetItem();
	ProjectTreeWidgetItem(const QStringList& strings);
	
	void setAssociatedPath(const QString& path);
	const QString& associatedPath() const;
	
	void setAssociatedProject(Project* associatedProject);
	Project* associatedProject() const;
	
	void setIgnoreFlag(bool ignoreFlag);
	bool ignoreFlag() const;
	
	static ProjectTreeWidgetItem* from(QTreeWidgetItem* item);
private:
	QString m_path;
	Project* m_associatedProject;
	bool m_ignoreFlag;
};

ProjectTreeWidgetItem::ProjectTreeWidgetItem() : QTreeWidgetItem(QTreeWidgetItem::UserType), m_associatedProject(0), m_ignoreFlag(false) {}
ProjectTreeWidgetItem::ProjectTreeWidgetItem(const QStringList& strings)  : QTreeWidgetItem(strings, QTreeWidgetItem::UserType), m_associatedProject(0), m_ignoreFlag(false) {}

void ProjectTreeWidgetItem::setAssociatedPath(const QString& path)
{
	m_path = path;
}

const QString& ProjectTreeWidgetItem::associatedPath() const
{
	return m_path;
}

void ProjectTreeWidgetItem::setAssociatedProject(Project* associatedProject)
{
	m_associatedProject = associatedProject;
}

Project* ProjectTreeWidgetItem::associatedProject() const
{
	return m_associatedProject;
}

void ProjectTreeWidgetItem::setIgnoreFlag(bool ignoreFlag)
{
	m_ignoreFlag = ignoreFlag;
}

bool ProjectTreeWidgetItem::ignoreFlag() const
{
	return m_ignoreFlag;
}

ProjectTreeWidgetItem* ProjectTreeWidgetItem::from(QTreeWidgetItem* item)
{
	return static_cast<ProjectTreeWidgetItem*>(item);
}

ProjectTreeWidget::ProjectTreeWidget(QWidget* parent) : QTreeWidget(parent), m_manager(0)
{
	setColumnCount(2);
	
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(itemClicked(QTreeWidgetItem*)));
}

void ProjectTreeWidget::setManager(ProjectManager* manager)
{
	m_manager = manager;
	
	connect(m_manager, SIGNAL(projectChanged(Project*)), SLOT(projectChanged(Project*)));
	connect(m_manager, SIGNAL(projectOpened(Project*)), SLOT(projectOpened(Project*)));
	connect(m_manager, SIGNAL(projectClosed(Project*)), SLOT(projectClosed(Project*)));
}

ProjectManager* ProjectTreeWidget::manager() const
{
	return m_manager;
}

void ProjectTreeWidget::addUnassociatedFile(const QString& path)
{
	m_unassociatedFiles.removeAll(path);
	m_unassociatedFiles.append(path);
	refreshUnassoicatedFiles();
}

void ProjectTreeWidget::removeUnassociatedFile(const QString& path)
{
	m_unassociatedFiles.removeAll(path);
	refreshUnassoicatedFiles();
}

Project* ProjectTreeWidget::activeProject() const
{
	ProjectTreeWidgetItem* currentItem = ProjectTreeWidgetItem::from(QTreeWidget::currentItem());
	return currentItem ? currentItem->associatedProject() : 0;
}

void ProjectTreeWidget::projectChanged(Project* project)
{
	ProjectTreeWidgetItem* item = lookupProjectItem(project);
	reloadProject(project, item);
}

void ProjectTreeWidget::projectOpened(Project* project)
{
	ProjectTreeWidgetItem* item = new ProjectTreeWidgetItem(QStringList() << project->name());
	item->setAssociatedProject(project);
	addTopLevelItem(item);
	reloadProject(project, item);
}

void ProjectTreeWidget::projectClosed(Project* project)
{
	int i = lookupProjectItemIndex(project);
	if(i >= 0) delete takeTopLevelItem(i);
}

void ProjectTreeWidget::itemClicked(QTreeWidgetItem* item)
{
	ProjectTreeWidgetItem* projectItem = ProjectTreeWidgetItem::from(item);
	if(projectItem->ignoreFlag()) return;
	
	Project* project = projectItem->associatedProject();
	QString path = projectItem->associatedPath();
	
	if(project && path.isEmpty()) {
		qWarning() << project->name() << "clicked";
		emit projectClicked(project);
		return;
	}
	
	qWarning() << path << "clicked";
	
	emit fileClicked(project, path);
}

void ProjectTreeWidget::refreshUnassoicatedFiles()
{
	for(int i = 0; i < topLevelItemCount(); ++i) {
		QTreeWidgetItem* item = topLevelItem(i);
		ProjectTreeWidgetItem* pItem = ProjectTreeWidgetItem::from(item);
		if(!pItem->associatedProject()) {
			delete pItem;
		}
	}
	
	foreach(const QString& path, m_unassociatedFiles) {
		ProjectTreeWidgetItem* item = new ProjectTreeWidgetItem(QStringList() << QFileInfo(path).fileName());
		item->setData(0, Qt::DecorationRole, ResourceHelper::ref().icon("page_white.png"));
		item->setAssociatedPath(path);
		addTopLevelItem(item);
		
		qWarning() << path;
	}
}

void ProjectTreeWidget::addDirectory(const QString& path, const QStringList& paths, ProjectTreeWidgetItem* parent, Project* project)
{
	QMap<QString, QStringList> merged = merge(paths, '/');
	qWarning() << merged;
	foreach(const QString& key, merged.keys()) {
		ProjectTreeWidgetItem* item = new ProjectTreeWidgetItem(QStringList() << key);
		if(merged[key].size() == 0) item->setData(0, Qt::DecorationRole, ResourceHelper::ref().icon("page_white.png"));
		else {
			item->setIgnoreFlag(true);
			item->setData(0, Qt::DecorationRole, ResourceHelper::ref().icon("folder"));
		}
		item->setAssociatedPath(path + (!path.isEmpty() ? "/" : "") + key);
		item->setAssociatedProject(project);
		parent->addChild(item);

		addDirectory(path + "/" + key, merged[key], item, project);
		
	}
}

void ProjectTreeWidget::reloadProject(Project* project, ProjectTreeWidgetItem* parent)
{
	parent->setData(0, Qt::DecorationRole, ResourceHelper::ref().icon("brick.png"));
	QList<QTreeWidgetItem*> items = parent->takeChildren();
	foreach(QTreeWidgetItem* item, items) {
		delete item;
	}
	
	QStringList files = project->files();
	addDirectory("", files, parent, project);
}

ProjectTreeWidgetItem* ProjectTreeWidget::lookupProjectItem(Project* project) const
{
	for(int i = 0; i < topLevelItemCount(); ++i) {
		QTreeWidgetItem* item = topLevelItem(i);
		ProjectTreeWidgetItem* pItem = ProjectTreeWidgetItem::from(item);
		if(pItem->associatedProject() == project) return pItem;
	}
	return 0;
}

int ProjectTreeWidget::lookupProjectItemIndex(Project* project) const
{
	for(int i = 0; i < topLevelItemCount(); ++i) {
		QTreeWidgetItem* item = topLevelItem(i);
		if(ProjectTreeWidgetItem::from(item)->associatedProject() == project) return i;
	}
	return -1;
}

QMap<QString, QStringList> ProjectTreeWidget::merge(const QStringList& list, const char delim)
{
	QMap<QString, QStringList> ret;
	
	foreach(const QString& item, list) {
		QString first = item.section(delim, 0, 0);

		if(item.split(delim).size() == 1) ret[first] = QStringList();
		else ret[first].append(item.section(delim, 1));
	}
	
	return ret;
}