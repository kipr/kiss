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

#include "documentation_menu.hpp"

#ifdef BUILD_DOCUMENTATION_TAB

#include "target_menu.hpp"
#include "main_window.hpp"
#include "documentation.hpp"

using namespace Kiss;
using namespace Kiss::Menu;

DocumentationMenu::DocumentationMenu(Widget::MainWindow* mainWindow)
	: ConcreteMenuable(menuName()),
	m_mainWindow(mainWindow)
{
	Node *targetMenu = new Node("Target");
	Node *doc = node(action("report", "Documentation"));
	m_help.append(doc);
	m_help.append(Node::separator());
}

void DocumentationMenu::triggered()
{
	QList<Widget::Documentation *> docs = m_mainWindow->tabs<Widget::Documentation>();
	if(docs.size() >= 1) m_mainWindow->moveToTab(docs[0]);
	else m_mainWindow->addTab(new Widget::Documentation(m_mainWindow));
}

QString DocumentationMenu::menuName()
{
	return "Documentation";
}

#endif
