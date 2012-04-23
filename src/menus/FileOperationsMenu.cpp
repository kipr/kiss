#include "FileOperationsMenu.h"

FileOperationsMenu::FileOperationsMenu() : ConcreteMenuable(menuName())
{
	MenuNode* newFile = node(activeAction(ResourceHelper::ref().icon("page_white.png"), "New File", QKeySequence("Ctrl+Shift+N"), this, "newFile"));
	MenuNode* newProject = node(activeAction(ResourceHelper::ref().icon("brick.png"), "New Project", QKeySequence::New, this, "newProject"));
	MenuNode* openFile = node(activeAction(ResourceHelper::ref().icon("folder"), "Open...", QKeySequence::Open, this, "open"));

	MenuNode* newSubmenu = node(action("New"));
	newSubmenu->children.append(newFile);
	newSubmenu->children.append(newProject);
	
	m_file.append(newSubmenu);
	m_file.append(openFile);
	
	m_toolbar.append(newFile);
	m_toolbar.append(newProject);
	m_toolbar.append(openFile);
	m_toolbar.append(MenuNode::separator());
}

QString FileOperationsMenu::menuName()
{
	return "File Operations";
}