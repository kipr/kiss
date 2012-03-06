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

#include "ResourceHelper.h"

#include "MainWindow.h"

#include <QIcon>

#include <QDebug>

MainWindowMenu::MainWindowMenu(MainWindow* mainWindow) : ConcreteMenuable(menuName()), m_mainWindow(mainWindow)
{	
	MenuNode* newFileNode = node(newFile = action(QIcon(ResourceHelper::ref().lookup("page_white.png")), "New File", QKeySequence::New));
	MenuNode* openFileNode = node(open = action(QIcon(ResourceHelper::ref().lookup("folder")), "Open...", QKeySequence::Open));
	m_file.append(newFileNode);
	m_file.append(openFileNode);
	m_file.append(m_fileOperations = MenuNode::insertionPoint());
	m_file.append(MenuNode::separator());
	MenuNode* packages = new MenuNode("Packages");
	packages->children.append(node(repo = action("Manage Packages")));
	packages->children.append(node(install = action("cog_go", "Install Local Packages...")));
	m_file.append(packages);
	m_file.append(MenuNode::separator());
	m_file.append(m_nextNode = node(next = action(QIcon(ResourceHelper::ref().lookup("arrow_right")), "Next", QKeySequence::NextChild)));
	m_file.append(m_prevNode = node(prev = action(QIcon(ResourceHelper::ref().lookup("arrow_left")), "Previous", QKeySequence::PreviousChild)));
	m_file.append(m_closeNode = node(close = activeAction("cross", "Close", QKeySequence::Close, this, "closeTab")));
	m_file.append(MenuNode::separator());
	quit = activeAction("cross", "Quit", QKeySequence::Quit, this, "close");
	quit->setMenuRole(QAction::QuitRole);
	m_file.append(node(quit));
	
	m_toolbar.append(newFileNode);
	m_toolbar.append(openFileNode);
	m_toolbar.append(MenuNode::separator());
	
	finish();
}

MenuNode* MainWindowMenu::fileOperations() const
{
	return m_fileOperations;
}

MenuNode* MainWindowMenu::nextNode() const
{
	return m_nextNode;
}

MenuNode* MainWindowMenu::prevNode() const
{
	return m_prevNode;
}

MenuNode* MainWindowMenu::closeNode() const
{
	return m_closeNode;
}

void MainWindowMenu::triggered()
{
	QAction* _ = (QAction*)sender();
	
	if(_ == newFile) m_mainWindow->newFile();
	else if(_ == open) m_mainWindow->open();
	else if(_ == next) m_mainWindow->next();
	else if(_ == prev) m_mainWindow->previous();
	else if(_ == repo) m_mainWindow->managePackages();
	else if(_ == install) m_mainWindow->installLocalPackage();
	else qWarning() << "MainWindowMenu cannot respond to" << _->text();
}

QString MainWindowMenu::menuName()
{
	return "General";
}