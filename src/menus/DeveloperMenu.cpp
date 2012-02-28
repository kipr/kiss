#include "DeveloperMenu.h"

#include "KissArchive.h"

#include <QFileDialog>

DeveloperMenu::DeveloperMenu() : ConcreteMenuable(menuName())
{
	MenuNode* developer = new MenuNode("Developer");
	developer->children.append(node(injectScript = action("Inject Script")));
	developer->children.append(MenuNode::separator());
	developer->children.append(node(uninstallAll = action("Uninstall All Packages")));
	m_actions.append(developer);
	
	finish();
}

void DeveloperMenu::triggered()
{
	QAction* _ = (QAction*)sender();
	if(_ == uninstallAll) {
		QStringList installed = KissArchive::installed();
		foreach(const QString& install, installed) KissArchive::uninstall(install);
		qWarning() << "Uninstalled all packages.";
	} else if(_ == injectScript) {
		QStringList filePaths = QFileDialog::getOpenFileNames(0, tr("Inject Scripts"), QDir::homePath(), "Javascript (*.js)");
		
		foreach(const QString& path, filePaths) {
			qWarning() << path;
		}
	}
}

QString DeveloperMenu::menuName()
{
	return "Developer";
}