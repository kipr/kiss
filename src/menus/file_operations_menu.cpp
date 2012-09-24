#include "file_operations_menu.hpp"

using namespace Kiss::Menu;

FileOperationsMenu::FileOperationsMenu()
	: ConcreteMenuable(menuName())
{
	Node* newFile = node(activeAction(ResourceHelper::ref().icon("page_white.png"), "New File", QKeySequence("Ctrl+Shift+N"), this, "newFile"));
	Node* newProject = node(activeAction(ResourceHelper::ref().icon("brick.png"), "New Project", QKeySequence::New, this, "newProject"));
	Node* openFile = node(activeAction(ResourceHelper::ref().icon("folder"), "Open...", QKeySequence::Open, this, "open"));

	Node* newSubmenu = node(action("New"));
	newSubmenu->children.append(newFile);
	newSubmenu->children.append(newProject);
	
	m_file.append(newSubmenu);
	m_file.append(openFile);
	
	m_toolbar.append(newFile);
	m_toolbar.append(newProject);
	m_toolbar.append(openFile);
	m_toolbar.append(Node::separator());
}

QString FileOperationsMenu::menuName()
{
	return "File Operations";
}