#include "ProjectSaveAs.h"

#include "MainWindow.h"
#include "Project.h"

#include "ProjectManager.h"

ProjectSaveAs::ProjectSaveAs(MainWindow* mainWindow)
	: QDialog(mainWindow), m_mainWindow(mainWindow),
	m_active(0), m_parent(0)
{
	setupUi(this);
	
	ui_buttons->button(QDialogButtonBox::Save)->setEnabled(false);
	
	ui_projects->setModel(m_mainWindow->projectsModel());
	connect(ui_projects, SIGNAL(pressed(const QModelIndex&)), SLOT(selectionChanged(const QModelIndex&)));
	
	on_ui_filename_textChanged("");
}

void ProjectSaveAs::setDefaultProject(Project* project)
{
	ui_projects->selectionModel()->select(m_mainWindow->projectsModel()->rootIndexForProject(project), QItemSelectionModel::Select);
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
	if(!invalidText && text.section('.', 1).isEmpty()) {
		ui_error->setText(tr("File does not have an extension"));
		invalid |= true;
	}
	
	invalid |= invalidText;
	
	ui_buttons->button(QDialogButtonBox::Save)->setEnabled(!invalid);
}

void ProjectSaveAs::selectionChanged(const QModelIndex& index)
{
	setActiveProject(m_mainWindow->projectsModel()->indexToProject(index));
	int type = m_mainWindow->projectsModel()->indexType(index);
	const TinyNode* node = m_mainWindow->projectsModel()->indexToNode(index);
	if(type == ProjectsModel::FileType) {
		ui_filename->setText(QTinyNode::name(node));
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