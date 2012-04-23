#include "ProjectSaveAs.h"

#include "MainWindow.h"
#include "Project.h"

#include "ProjectManager.h"
#include "TargetManager.h"

ProjectSaveAs::ProjectSaveAs(MainWindow* mainWindow)
	: QDialog(mainWindow), m_mainWindow(mainWindow),
	m_active(0), m_parent(0)
{
	setupUi(this);
	
	ui_buttons->button(QDialogButtonBox::Save)->setEnabled(false);
	
	ui_projects->setModel(&m_model);
	connect(ui_projects, SIGNAL(pressed(const QModelIndex&)), SLOT(selectionChanged(const QModelIndex&)));
	
	ProjectManager::ref().projects();
	
	on_ui_filename_textChanged("");
}

void ProjectSaveAs::setProjectManager(ProjectManager* projectManager)
{
	m_model.setProjectManager(projectManager);
	ui_projects->expandAll();
}

void ProjectSaveAs::setDefaultProject(Project* project)
{
	ui_projects->selectionModel()->select(m_model.rootIndexForProject(project), QItemSelectionModel::Select);
	setActiveProject(project);
}

Project* ProjectSaveAs::activeProject() const
{
	return m_active;
}

const TinyNode* ProjectSaveAs::parent() const
{
	return m_parent;
}

QString ProjectSaveAs::fileName() const
{
	return ui_filename->text();
}

void ProjectSaveAs::on_ui_filename_textChanged(const QString& text)
{
	bool invalid = text.isEmpty();
	bool c = false;
	bool invalidText = false;
	
	c = text.contains(':');
	invalidText |= c;
	
	c = text.contains('/');
	invalidText |= c;
	
	ui_error->setText("");
	if(invalidText) ui_error->setText(tr("Invalid file name"));
	if(!text.contains('.')) {
		ui_error->setText(tr("File does not have an extension"));
		invalid |= true;
	}
	
	invalid |= invalidText;
	
	ui_buttons->button(QDialogButtonBox::Save)->setEnabled(!invalid);
}

void ProjectSaveAs::selectionChanged(const QModelIndex& index)
{
	setActiveProject(m_model.indexToProject(index));
	int type = m_model.indexType(index);
	if(type == ProjectsModel::ProjectType) {
		
	}
}

void ProjectSaveAs::setActiveProject(Project* project)
{
	m_active = project;
	updateActiveLabel();
}

void ProjectSaveAs::setParentNode(const TinyNode* parent)
{
	m_parent = parent;
	updateActiveLabel();
}

void ProjectSaveAs::updateActiveLabel()
{
	ui_savinginto->setText(tr("Saving into project ") + m_active->name() + tr(" under ") + QTinyNode::path(m_parent));
}