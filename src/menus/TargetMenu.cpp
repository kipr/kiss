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

#include "TargetMenu.h"

#include "Target.h"
#include "TargetManager.h"

#include "ResourceHelper.h"
#include "Documentation.h"
#include "MainWindow.h"

TargetMenu::TargetMenu() : ConcreteMenuable(menuName()), m_additional(MenuNode::insertionPoint())
{
	// Put manuals in "Target"
	MenuNode* targetMenu = new MenuNode("Target");
	
	targetMenu->children.append(compileNode = node(activeAction("bricks", "Compile", QKeySequence("Alt+C"), this, "compile")));
	targetMenu->children.append(downloadNode = node(activeAction("ruby_blue", "Download", QKeySequence("Alt+D"), this, "download")));
	targetMenu->children.append(simulateNode = node(activeAction("application_go", "Simulate", QKeySequence("Alt+S"), this, "simulate")));
	targetMenu->children.append(runNode = node(activeAction("arrow_right", "Run", QKeySequence("Alt+R"), this, "run")));
	targetMenu->children.append(stopNode = node(activeAction("stop.png", "Stop", QKeySequence("Alt+X"), this, "stop")));
	targetMenu->children.append(debugNode = node(activeAction("bug", "Debug", QKeySequence("Alt+G"), this, "debug")));
	
	compileNode->activeControl = downloadNode->activeControl = true;
	simulateNode->activeControl = runNode->activeControl = true;
	stopNode->activeControl = debugNode->activeControl = true;
	
	m_toolbar.append(targetMenu->children);
	targetMenu->children.append(MenuNode::separator());
	targetMenu->children.append(node(activeAction("", "Change Target", QKeySequence("Alt+T"), this, "changeTarget")));
	targetMenu->children.append(node(activeAction("link", "Choose Port", QKeySequence("Alt+P"), this, "choosePort")));
	targetMenu->children.append(MenuNode::separator());
	targetMenu->children.append(m_additional);
	
	m_actions.append(targetMenu);
	
	finish();
}

MenuNode* TargetMenu::additional()
{
	return m_additional;
}

void TargetMenu::refresh()
{
	if(!isActive()) return;
	Target* target = qobject_cast<SourceFile*>(active())->target();
	
	compileNode->rawAction->setEnabled(target->hasCompile());
	downloadNode->rawAction->setEnabled(target->hasDownload());
	simulateNode->rawAction->setEnabled(target->hasSimulate());
	runNode->rawAction->setEnabled(target->hasRun());
	stopNode->rawAction->setEnabled(target->hasStop());
	debugNode->rawAction->setEnabled(target->hasDebug());
	
	menuManager()->refreshToolbar();
}

void TargetMenu::activated()
{
	menuManager()->addActivation(this);
	
	refresh();
}

void TargetMenu::deactivated()
{
	compileNode->rawAction->setEnabled(false);
	downloadNode->rawAction->setEnabled(false);
	simulateNode->rawAction->setEnabled(false);
	runNode->rawAction->setEnabled(false);
	stopNode->rawAction->setEnabled(false);
	debugNode->rawAction->setEnabled(false);
	
	menuManager()->removeActivation(this);
}

MenuNodeList TargetMenu::manuals()
{
	return m_manuals.values();
}

MenuNodeList TargetMenu::manuals(const QString& target)
{
	return m_manuals.values(target);
}

void TargetMenu::triggered()
{
	QAction* _ = (QAction*)sender();
}

QString TargetMenu::menuName()
{
	return "Target";
}
