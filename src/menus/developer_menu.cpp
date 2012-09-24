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

#include "developer_menu.hpp"

#ifdef BUILD_DEVELOPER_TOOLS

#include "project_manager.hpp"
#include "declarative_tab.hpp"
#include "main_window.hpp"

#include <QFileDialog>

using namespace Kiss;
using namespace Kiss::Menu;

DeveloperMenu::DeveloperMenu(Widget::MainWindow* mainWindow)
	: ConcreteMenuable(menuName()),
	m_mainWindow(mainWindow)
{
	Node *developer = new Node("Developer");
#ifdef BUILD_DECLARATIVE_TAB
	developer->children.append(node(declTab = action("Create Declarative Tab")));
#endif
	m_actions.append(developer);
}

void DeveloperMenu::triggered()
{
	QAction *_ = (QAction*)sender();
#ifdef BUILD_DECLARATIVE_TAB
	if(_ == declTab) {
		QStringList filePaths = QFileDialog::getOpenFileNames(0, tr("Load QML"), QDir::homePath(), "QML (*.qml)");
		
		foreach(const QString& path, filePaths) {
			qWarning() << path;
			m_mainWindow->addTab(new Widget::DeclarativeTab(QUrl::fromLocalFile(path), m_mainWindow));
		}
	}
#endif
}

QString DeveloperMenu::menuName()
{
	return "Developer";
}

#endif