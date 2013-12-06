#include "project_menu.hpp"

using namespace kiss::menu;

ProjectMenu::ProjectMenu()
	: ConcreteMenuable(menuName())
{
  m_projectMenu = new Node(menuName());
  
  m_projectMenu->children.append(node(activeAction(ResourceHelper::ref().icon("page_white.png"), "Add New File",
		QKeySequence::UnknownKey, this, "activeProjectAddNew")));
  m_projectMenu->children.append(node(activeAction(ResourceHelper::ref().icon("page_white.png"), "Add Existing Files",
		QKeySequence::UnknownKey, this, "activeProjectAddExisting")));
  m_projectMenu->children.append(Node::separator());
  m_projectMenu->children.append(node(activeAction(ResourceHelper::ref().icon("folder_wrench.png"), "Project Settings",
		QKeySequence::UnknownKey, this, "activeProjectOpenSettings")));
  m_projectMenu->children.append(Node::separator());
  m_projectMenu->children.append(node(activeAction(ResourceHelper::ref().icon("folder.png"), "Close Project",
  	QKeySequence::UnknownKey, this, "activeProjectClose")));
  m_projectMenu->children.append(node(activeAction(ResourceHelper::ref().icon("folder_delete.png"), "Delete Project",
		QKeySequence::UnknownKey, this, "activeProjectDelete")));
    
    m_actions.append(m_projectMenu);
}

void ProjectMenu::activated()
{
	menuManager()->addActivation(this);
}

void ProjectMenu::deactivated()
{
	menuManager()->removeActivation(this);
}

QString ProjectMenu::menuName()
{
	return "Project";
}