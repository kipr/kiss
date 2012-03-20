#include "FileOperationsMenu.h"

FileOperationsMenu::FileOperationsMenu() : ConcreteMenuable(menuName())
{
	MenuNode* newFile = node(activeAction(ResourceHelper::ref().icon("page_white.png"), "New File", QKeySequence::New, this, "newFile"));
	MenuNode* openFile = node(activeAction(ResourceHelper::ref().icon("folder"), "Open...", QKeySequence::Open, this, "open"));
	m_file.append(newFile);
	m_file.append(openFile);
	
	m_toolbar.append(m_file);
	m_toolbar.append(MenuNode::separator());
}

QString FileOperationsMenu::menuName()
{
	return "File Operations";
}