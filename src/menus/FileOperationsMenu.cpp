#include "FileOperationsMenu.h"

FileOperationsMenu::FileOperationsMenu() : ConcreteMenuable(menuName())
{
	MenuNode* newFile = node(activeAction(ResourceHelper::ref().icon("page_white.png"), "New File", QKeySequence("Ctrl+Shift+N"), this, "newFile"));
	MenuNode* newProject = node(activeAction(ResourceHelper::ref().icon("brick.png"), "New Project", QKeySequence::New, this, "newProject"));
	MenuNode* openFile = node(activeAction(ResourceHelper::ref().icon("folder"), "Open...", QKeySequence::Open, this, "open"));
	MenuNode* openProject = node(activeAction(ResourceHelper::ref().icon("folder"), "Open Project...", QKeySequence("Shift+Ctrl+O"), this, "openProject"));
	m_file.append(newFile);
	m_file.append(newProject);
	m_file.append(openFile);
	m_file.append(openProject);
	
	m_toolbar.append(m_file);
	m_toolbar.append(MenuNode::separator());
}

QString FileOperationsMenu::menuName()
{
	return "File Operations";
}