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

#include "main_window_menu.hpp"


#include "build_options.hpp"
#include "resource_helper.hpp"
#include "main_window.hpp"
#include "log_window.hpp"

#include <QIcon>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Menu;

MainWindowMenu::MainWindowMenu(Widget::MainWindow* mainWindow)
	: ConcreteMenuable(menuName()),
	m_mainWindow(mainWindow)
{	
	Node* packages = new Node("Packages");
	m_file.append(packages);
	m_file.append(node(m_hideErrors = action("Hide Errors", QKeySequence::UnknownKey)));
	m_file.append(Node::separator());
	m_file.append(m_nextNode = node(activeAction(QIcon(ResourceHelper::ref().lookup("arrow_right")), "Next", QKeySequence::NextChild, this, "next")));
	m_file.append(m_prevNode = node(activeAction(QIcon(ResourceHelper::ref().lookup("arrow_left")), "Previous", QKeySequence::PreviousChild, this, "previous")));
	m_file.append(m_closeNode = node(activeAction("cross", "Close", QKeySequence::Close, this, "closeTab")));
	m_file.append(Node::separator());
	QAction* quit = activeAction("cross", "Quit", QKeySequence::Quit, this, "close");
	quit->setMenuRole(QAction::QuitRole);
	m_file.append(node(quit));
	
	m_edit.append(Node::separator());
	m_edit.append(node(activeAction("cog.png", "Settings", QKeySequence::Preferences, this, "settings")));
	m_edit.append(node(activeAction("theme.png", "Editor Theme", QKeySequence::Preferences, this, "theme")));
	
	QAction* about = activeAction("information", "About KISS IDE", QKeySequence::UnknownKey, this, "about");
	m_help.append(node(about));
#ifdef ENABLE_LOG_WINDOW
	m_help.append(node(m_errorLog = action("Error Log", QKeySequence::UnknownKey)));
#endif
}

Node* MainWindowMenu::closeNode() const
{
	return m_closeNode;
}

void MainWindowMenu::triggered()
{
	QAction* _ = (QAction*)sender();
	if(!_) return;
	if(_ == m_errorLog) Widget::LogWindow::ref().setVisible(true);
	else if(_ == m_hideErrors) m_mainWindow->hideErrors();
	else qWarning() << "MainWindowMenu received unknown trigger" << _->text();
}

void MainWindowMenu::update()
{
	Widget::MainWindow* mainWindow = dynamic_cast<Widget::MainWindow*>(active());
	if(!mainWindow || !menuManager()->isConstructed()) return;
	
	m_closeNode->rawAction->setEnabled(mainWindow->canClose());
	m_nextNode->rawAction->setEnabled(mainWindow->canGoNext());
	m_prevNode->rawAction->setEnabled(mainWindow->canGoPrevious());
}

QString MainWindowMenu::menuName()
{
	return "General";
}
