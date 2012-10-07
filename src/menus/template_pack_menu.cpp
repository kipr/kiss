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

#include "template_pack_menu.hpp"

#include "main_window_menu.hpp"
#include "resource_helper.hpp"
#include "main_window.hpp"

#include <QIcon>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Menu;

TemplatePackMenu::TemplatePackMenu(Widget::MainWindow *mainWindow)
	: ConcreteMenuable(menuName())
{
	const QIcon disk(ResourceHelper::ref().lookup("disk"));
	m_file.append(node(activeAction(disk, "Save", QKeySequence::Save, this, "save")));
	m_file.append(node(activeAction(disk, "Save As...", QKeySequence::SaveAs, this, "saveAs")));
	m_file.append(Node::separator());
}

void TemplatePackMenu::activated()
{
	menuManager()->addActivation(this);
}

void TemplatePackMenu::deactivated()
{
	menuManager()->removeActivation(this);
}

QString TemplatePackMenu::menuName()
{
	return "TemplatePack";
}
