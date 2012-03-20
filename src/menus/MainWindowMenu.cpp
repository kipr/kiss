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

#include "MainWindowMenu.h"
#include "BuildOptions.h"

#include "ResourceHelper.h"

#include "MainWindow.h"

#include <QIcon>
#include <QDebug>

MainWindowMenu::MainWindowMenu(MainWindow* mainWindow) : ConcreteMenuable(menuName())
{	
	MenuNode* packages = new MenuNode("Packages");
#ifdef BUILD_REPOSITORY_TAB
	packages->children.append(node(activeAction("", "Manage Packages", QKeySequence::UnknownKey, this, "managePackages")));
#endif
	packages->children.append(node(activeAction("cog_go", "Install Local Packages...", QKeySequence::UnknownKey, this, "installLocalPackage")));
	m_file.append(packages);
	m_file.append(MenuNode::separator());
	m_file.append(m_nextNode = node(activeAction(QIcon(ResourceHelper::ref().lookup("arrow_right")), "Next", QKeySequence::NextChild, this, "next")));
	m_file.append(m_prevNode = node(activeAction(QIcon(ResourceHelper::ref().lookup("arrow_left")), "Previous", QKeySequence::PreviousChild, this, "previous")));
	m_file.append(m_closeNode = node(activeAction("cross", "Close", QKeySequence::Close, this, "closeTab")));
	m_file.append(MenuNode::separator());
	QAction* quit = activeAction("cross", "Quit", QKeySequence::Quit, this, "close");
	quit->setMenuRole(QAction::QuitRole);
	m_file.append(node(quit));
	
	m_edit.append(MenuNode::separator());
	m_edit.append(node(activeAction("Settings", QKeySequence::Preferences, this, "settings")));
	
	QAction* about = activeAction("About KISS IDE", QKeySequence::UnknownKey, this, "about");
	m_help.append(node(about));
}

MenuNode* MainWindowMenu::closeNode() const
{
	return m_closeNode;
}

void MainWindowMenu::update()
{
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(active());
	if(!mainWindow || !menuManager()->isConstructed()) return;
	
	m_closeNode->rawAction->setEnabled(mainWindow->canClose());
	m_nextNode->rawAction->setEnabled(mainWindow->canGoNext());
	m_prevNode->rawAction->setEnabled(mainWindow->canGoPrevious());
}

QString MainWindowMenu::menuName()
{
	return "General";
}