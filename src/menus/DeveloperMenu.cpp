/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "DeveloperMenu.h"

#ifdef BUILD_DEVELOPER_TOOLS

#include "KissArchive.h"
#include "ArchiveWriter.h"
#include "ProjectManager.h"
#include <QFileDialog>

#include "DeclarativeTab.h"

DeveloperMenu::DeveloperMenu(MainWindow* mainWindow) : ConcreteMenuable(menuName()), m_mainWindow(mainWindow)
{
	MenuNode* developer = new MenuNode("Developer");
	developer->children.append(node(injectScript = action("Inject Script")));
	developer->children.append(node(writeFullProject = action("Write Full Project")));
	developer->children.append(node(writeDeltaProject = action("Write Delta Project")));
#ifdef BUILD_DECLARATIVE_TAB
	developer->children.append(node(declTab = action("Create Declarative Tab")));
#endif
	developer->children.append(MenuNode::separator());
	developer->children.append(node(uninstallAll = action("Uninstall All Packages")));
	m_actions.append(developer);
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
			m_mainWindow->scriptEnvironment()->execute(path);
		}
	} else if(_ == writeFullProject) {
		Project* project = m_mainWindow->activeProject();
		ArchiveWriter* writer = ProjectManager::ref().archiveWriter(project);
		if(!writer) return;
		writer->write(ArchiveWriter::Full);
	} else if(_ == writeDeltaProject) {
		Project* project = m_mainWindow->activeProject();
		ArchiveWriter* writer = ProjectManager::ref().archiveWriter(project);
		if(!writer) return;
		writer->write(ArchiveWriter::Delta);
	}
#ifdef BUILD_DECLARATIVE_TAB
	else if(_ == declTab) {
		QStringList filePaths = QFileDialog::getOpenFileNames(0, tr("Load QML"), QDir::homePath(), "QML (*.qml)");
		
		foreach(const QString& path, filePaths) {
			qWarning() << path;
			m_mainWindow->addTab(new DeclarativeTab(QUrl::fromLocalFile(path), m_mainWindow));
		}
	}
#endif
}

QString DeveloperMenu::menuName()
{
	return "Developer";
}

#endif