#include "DocumentationMenu.h"

#include "TargetMenu.h"
#include "MainWindow.h"
#include "Documentation.h"

DocumentationMenu::DocumentationMenu(MainWindow* mainWindow) : ConcreteMenuable(menuName()), m_mainWindow(mainWindow)
{
	TargetMenu::ref().additional()->children.append(node(action("report", "Documentation")));
	finish();
}

void DocumentationMenu::triggered()
{
	QList<Documentation*> docs = m_mainWindow->tabs<Documentation>();
	if(docs.size() >= 1) m_mainWindow->moveToTab(docs[0]);
	else m_mainWindow->addTab(new Documentation(m_mainWindow));
}

QString DocumentationMenu::menuName()
{
	return "Documentation";
}