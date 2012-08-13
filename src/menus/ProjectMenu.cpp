#include "ProjectMenu.h"
#include "MainWindow.h"

ProjectMenu::ProjectMenu() : ConcreteMenuable(menuName())
{
	MenuNode* project = node(action("Project"));
	project->children.append(addExistingFile = node(activeAction("Add Existing Files...", QKeySequence::UnknownKey, this, "projectAddExisting")));
	project->children.append(addNewFile = node(activeAction("Add New File...", QKeySequence::UnknownKey, this, "projectAddNew")));
	project->children.append(removeExistingFile = node(activeAction("Remove Selected Files", QKeySequence::UnknownKey, this, "projectRemoveExisting")));
	project->children.append(extractTo = node(activeAction("Extract to...", QKeySequence::UnknownKey, this, "projectExtractTo")));
	m_actions.append(project);
}

QString ProjectMenu::menuName()
{
	return "Project";
}

void ProjectMenu::activated()
{
	menuManager()->addActivation(this);
}

void ProjectMenu::deactivated()
{
	menuManager()->removeActivation(this);
}

void ProjectMenu::triggered()
{
	
}

void ProjectMenu::update()
{
	MainWindow *mainWindow = qobject_cast<MainWindow *>(active());
	if(!mainWindow) return;
	Project *active = mainWindow->activeProject();
	addExistingFile->rawAction->setEnabled(active);
	addNewFile->rawAction->setEnabled(active);
	removeExistingFile->rawAction->setEnabled(active);
	extractTo->rawAction->setEnabled(active);
}
