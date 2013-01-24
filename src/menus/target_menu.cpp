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

#include "target_menu.hpp"

#include "resource_helper.hpp"
#include "documentation.hpp"
#include "main_window.hpp"
#include "interface.hpp"
#include "communication_manager.hpp"

using namespace Kiss::Menu;

TargetMenu::TargetMenu()
	: ConcreteMenuable(menuName())
{
	// Put manuals in "Target"
	m_targetMenu = new Node("Target");
	
	m_targetMenu->children.append(compileNode = node(activeAction("bricks", "Compile", QKeySequence("Alt+C"), this, "compile")));
	m_targetMenu->children.append(downloadNode = node(activeAction("ruby_blue", "Download", QKeySequence("Alt+D"), this, "download")));
	m_targetMenu->children.append(runNode = node(activeAction("arrow_right", "Run", QKeySequence("Alt+R"), this, "run")));
	m_toolbar.append(m_targetMenu->children);
	m_targetMenu->children.append(Node::separator());
	m_targetMenu->children.append(Node::separator());
	m_targetMenu->children.append(node(activeAction("computer", "Change Target", QKeySequence("Alt+T"), this, "changeTarget")));
	m_targetMenu->children.append(Node::separator());
	
	compileNode->hideOnDisable = false;
	downloadNode->hideOnDisable = false;
	runNode->hideOnDisable = false;
	
	m_actions.append(m_targetMenu);
}

void TargetMenu::refresh()
{
	if(!isActive()) return;
	menuManager()->refreshToolbar();
}

void TargetMenu::activated()
{
	ActivatableObject::activated();
	menuManager()->addActivation(this);
	refresh();
}

void TargetMenu::deactivated()
{
	menuManager()->removeActivation(this);
	ActivatableObject::deactivated();
}

void TargetMenu::update()
{
	const bool enabled = Target::CommunicationManager::ref().isIdle();
	compileNode->rawAction->setEnabled(enabled);
	downloadNode->rawAction->setEnabled(enabled);
	runNode->rawAction->setEnabled(enabled);
	refresh();
}

QString TargetMenu::menuName()
{
	return "Target";
}
