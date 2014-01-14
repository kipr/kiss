#include "file_operations_menu.hpp"

using namespace kiss::menu;

FileOperationsMenu::FileOperationsMenu()
	: ConcreteMenuable(menuName())
{
	Node *newFile = node(activeAction(ResourceHelper::ref().icon("page_white.png"), tr("New File"),
		QKeySequence::New, this, "newFile"));
	Node *newProject = node(activeAction(ResourceHelper::ref().icon("brick.png"), tr("New Project"),
		QKeySequence("Ctrl+Shift+N"), this, "newProject"));
	Node *newTemplatePack = node(activeAction(ResourceHelper::ref().icon("package"), tr("New Template Pack"),
		QKeySequence::UnknownKey, this, "newTemplatePack"));
	Node *openFile = node(activeAction(ResourceHelper::ref().icon("folder.png"), tr("Open..."),
		QKeySequence::Open, this, "open"));

	Node *newSubmenu = node(action(ResourceHelper::ref().icon("page_white.png"), tr("New")));
	// newSubmenu->children.append(newFile);
	newSubmenu->children.append(newProject);
	newSubmenu->children.append(Node::separator());
	newSubmenu->children.append(newTemplatePack);
	
	Node *importSubmenu = node(action(ResourceHelper::ref().icon("bullet_go.png"), tr("Import")));
	Node *importTemplatePack = node(activeAction(ResourceHelper::ref().icon("package"), tr("Template Pack"),
		QKeySequence::UnknownKey, this, "importTemplatePack"));
	importSubmenu->children.append(importTemplatePack);
	
	m_file.append(newSubmenu);
	m_file.append(importSubmenu);
	m_file.append(openFile);
	
	
	// m_toolbar.append(newFile);
	m_toolbar.append(newProject);
	m_toolbar.append(openFile);
	m_toolbar.append(Node::separator());
}

QString FileOperationsMenu::menuName()
{
	return "File Operations";
}