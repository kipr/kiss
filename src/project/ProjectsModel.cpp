#include "ProjectsModel.h"

#include "Project.h"
#include "ProjectManager.h"

#include "ResourceHelper.h"
#include "QTinyArchive.h"
#include "Log.h"

#include <QFileInfo>
#include <QDebug>

#define PATH_ROLE Qt::UserRole + 1
#define PROJECT_ROLE Qt::UserRole + 2

struct Projectable
{
	Projectable(Project* project) : m_project(project) {}
	Project* project() { return m_project; }
	
	template<typename T>
	static Project* project_cast(T* type)
	{
		if(!type) return 0;
		Projectable* p = dynamic_cast<Projectable*>(type);
		return p ? p->project() : 0;
	}
	
private:
	Project* m_project;
};

struct ProjectItem : QStandardItem, Projectable
{
	ProjectItem(Project* project) : QStandardItem(project->name()), Projectable(project) {
		setData(ResourceHelper::ref().icon("brick.png"), Qt::DecorationRole);
		setData(QVariant::fromValue(project), PROJECT_ROLE);
		setEditable(false);
	}
	
	template<typename T>
	static ProjectItem* cast(T* type)
	{
		if(!type) return 0;
		ProjectItem* p = dynamic_cast<ProjectItem*>(type);
		return p;
	}
};

struct FileItem : QStandardItem, Projectable
{
	FileItem(const TinyNode* node, Project* project)
		: QStandardItem(QTinyNode::name(node)), Projectable(project), m_node(node)
	{
		QIcon icon = ResourceHelper::ref().icon(QString("page_white_") + QFileInfo(text()).completeSuffix() + ".png");
		if(icon.isNull()) icon = ResourceHelper::ref().icon("page_white.png");
		setData(icon, Qt::DecorationRole);
		setData(QVariant::fromValue(project), PROJECT_ROLE);
		setEditable(false);
	}
	
	template<typename T>
	static FileItem* cast(T* type)
	{
		if(!type) return 0;
		FileItem* p = dynamic_cast<FileItem*>(type);
		return p;
	}
	
	const TinyNode* node() { return m_node; }
private:
	const TinyNode* m_node;
};

ProjectsModel::ProjectsModel() : m_manager(0), m_project(0)
{
	connect(this, SIGNAL(itemChanged(QStandardItem*)), SLOT(itemChanged(QStandardItem*)));
}

ProjectsModel::~ProjectsModel()
{
	// Make sure we aren't still listening to anything
	if(m_manager) {
		foreach(Project* project, m_manager->projects())
			project->archive()->removeListener(this);
	}
	if(m_project) {
		m_project->archive()->removeListener(this);
	}
}

void ProjectsModel::setProject(Project* project)
{
	if(m_project) projectClosed(m_project);
	m_project = project;
	projectOpened(project);
}

void ProjectsModel::setProjectManager(ProjectManager* projectManager)
{
	if(m_manager) {
		// TODO: need to disconnect from old signals
	}
	
	m_manager = projectManager;
	// Open existing projects
	foreach(Project* project, m_manager->projects()) projectOpened(project);
	
	connect(m_manager, SIGNAL(projectChanged(Project*)), SLOT(projectChanged(Project*)));
	connect(m_manager, SIGNAL(projectOpened(Project*)), SLOT(projectOpened(Project*)));
	connect(m_manager, SIGNAL(projectClosed(Project*)), SLOT(projectClosed(Project*)));
}

void ProjectsModel::addDirectory(const TinyNode* node, QStandardItem* parent, Project* project)
{
	const std::vector<TinyNode*>& children = node->children();
	std::vector<TinyNode*>::const_iterator it = children.begin();
	
	if(children.size()) parent->setData(ResourceHelper::ref().icon("folder"), Qt::DecorationRole);
	
	parent->setColumnCount(1);
	parent->setRowCount(children.size());
	quint16 i = 0;
	
	for(; it != children.end(); ++it) {
		const TinyNode* child = *it;
		if(child->id() > 0) { // Special ID, ignore
			parent->setRowCount(children.size() - 1);
			return;
		}
		FileItem* item = new FileItem(child, project);
		m_nodeLookup[child] = item;
		
		Log::ref().debug(QString("Added child at %1").arg(i));
		parent->setChild(i++, item);

		addDirectory(child, item, project);
	}
}

void ProjectsModel::reloadProject(Project* project, QStandardItem* parent)
{
	if(!project) return;
	
	QList<QStandardItem*> items = parent->takeColumn(0);
	foreach(QStandardItem* item, items) delete item;
	
	const TinyNode* node = project->archive()->root();
	addDirectory(node, parent, project);
	m_nodeLookup[node] = parent;
	parent->setData(ResourceHelper::ref().icon("brick.png"), Qt::DecorationRole);
}

Project* ProjectsModel::indexToProject(const QModelIndex& index) const
{
	return Projectable::project_cast(itemFromIndex(index));
}

const TinyNode* ProjectsModel::indexToNode(const QModelIndex& index) const
{
	FileItem* p = dynamic_cast<FileItem*>(itemFromIndex(index));
	return p ? p->node() : 0;
}

QModelIndex ProjectsModel::rootIndexForProject(Project* project) const
{
	QStandardItem* root = invisibleRootItem();
	int rows = root->rowCount();
	for(int i = 0; i < rows; ++i) {
		QStandardItem* child = root->child(i);
		if(Projectable::project_cast(child) == project) return child->index();
	}
	return QModelIndex();
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
	project->archive()->addListener(this);
}

void ProjectsModel::projectChanged(Project* project)
{
	Log::ref().debug(QString("Project %1 changed").arg(project->name()));
	reloadProject(project, lookupRoot(project));
}

void ProjectsModel::projectClosed(Project* project)
{
	project->archive()->removeListener(this);
	
	const std::vector<const TinyNode*>& allNodes = project->archive()->allNodes();
	std::vector<const TinyNode*>::const_iterator it = allNodes.begin();
	for(; it != allNodes.end(); ++it) {
		
	}
	
	removeRow(lookupRoot(project)->row());
}

void ProjectsModel::itemChanged(QStandardItem* item)
{
	FileItem* fileItem = dynamic_cast<FileItem*>(item);
	if(fileItem) {
		const TinyNode* node = fileItem->node();
		
		// fileItem->project()->projectFile()->moveFile(path, item->text());
	}
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

void ProjectsModel::nodeAdded(const TinyNode* node)
{
	Log::ref().debug(QString("Node %1 Added").arg(QTinyNode::name(node)));
	if(node->id() > 0) return; // Special ID, ignore
	
	if(!node->parent()) {
		Log::ref().error("Node does not have a parent");
		return;
	}
	
	QMap<const TinyNode*, QStandardItem*>::iterator it = m_nodeLookup.find(node->parent());
	if(it == m_nodeLookup.end()) {
		Log::ref().error(QString("Failed to lookup %1").arg(QTinyNode::name(node->parent())));
		return;
	}
	
	QStandardItem* item = *it;
	
	Project* project = Projectable::project_cast(item);
	if(!project) {
		Log::ref().error("Unable to determine project");
		return;
	}
	
	FileItem* childItem = new FileItem(node, project);
	qDebug() << "Appending" << childItem->text() << "to" << item->text();
	item->appendRow(childItem);
	qDebug() << "Item children" << item->rowCount();
	m_nodeLookup[node] = childItem;
}

void ProjectsModel::nodeRemoved(const TinyNode* node)
{
	Log::ref().debug(QString("Node %1 Removed").arg(QTinyNode::name(node)));
	if(node->id() > 0) return; // Special ID, ignore
	
	QMap<const TinyNode*, QStandardItem*>::iterator it = m_nodeLookup.find(node);
	if(it == m_nodeLookup.end()) {
		qCritical() << "Failed to lookup" << QTinyNode::name(node);
		return;
	}
	
	FileItem* item = FileItem::cast(*it);
	if(!item) {
		qCritical() << "Cannot remove non-file item";
		return;
	}

	item->parent()->removeRow(item->row());
	
	m_nodeLookup.erase(it);
}

void ProjectsModel::nodeUpdated(const TinyNode* node)
{
	Log::ref().debug(QString("Node %1 Updated").arg(QTinyNode::name(node)));
}