#include "ProjectMenu.h"

ProjectMenu::ProjectMenu() : ConcreteMenuable(menuName())
{
	MenuNode* project = node(action("Project"));
	project->children.append(node(action("Add Existing Files...", QKeySequence::UnknownKey)));
	project->children.append(node(action("Add New File...", QKeySequence::UnknownKey)));
	project->children.append(node(action("Remove Selected Files", QKeySequence::UnknownKey)));
	project->children.append(node(action("Extract to...", QKeySequence::UnknownKey)));
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
	
}
