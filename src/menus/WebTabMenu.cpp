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

#include "WebTabMenu.h"

WebTabMenu::WebTabMenu() : ConcreteMenuable(menuName())
{
	m_edit.append(node(activeAction("copy", "Copy", QKeySequence::Copy, this, "copy")));
	m_edit.append(node(activeAction("cut", "Cut", QKeySequence::Cut, this, "cut")));
	m_edit.append(node(activeAction("paste", "Paste", QKeySequence::Paste, this, "paste")));
	m_edit.append(node(activeAction("find", "Find", QKeySequence::Find, this, "find")));
	
	
	MenuNode* back = node(this->back = action("arrow_left", "Back"));
	MenuNode* forward = node(this->forward = action("arrow_right", "Forward"));
	MenuNode* reload = node(this->reload = activeAction("arrow_refresh", "Refresh", QKeySequence::Refresh, this, "refresh"));
	MenuNode* open = node(this->open = activeAction("resultset_next", "Open in Browser", QKeySequence::UnknownKey, this, "openInBrowser"));
	
	MenuNode* browser = new MenuNode("Browser");
	browser->children.append(back);
	browser->children.append(forward);
	browser->children.append(reload);
	browser->children.append(open);
	
	m_actions.append(browser);
	
	m_toolbar.append(back);
	m_toolbar.append(forward);
	m_toolbar.append(reload);
	m_toolbar.append(open);
	m_toolbar.append(MenuNode::separator());
	
	finish();
}

void WebTabMenu::activated()
{
	menuManager()->addActivation(this);
}

void WebTabMenu::deactivated()
{
	menuManager()->removeActivation(this);
}

void WebTabMenu::triggered()
{
	if(!isActive()) return;
	WebTab* source = qobject_cast<WebTab*>(active());
	
	QAction* _ = (QAction*)sender();
}


#if 0
void WebTab::addActionsFile(QMenu*) {}

void WebTab::addActionsEdit(QMenu* edit)
{	
	edit->addAction(actionCopy);
	edit->addAction(actionCut);
	edit->addAction(actionPaste);
	edit->addSeparator();
	edit->addAction(actionFind);
}

void WebTab::addActionsHelp(QMenu*) {}

void WebTab::addOtherActions(QMenuBar* menuBar)
{
	QMenu* browser = menuBar->addMenu(tr("Browser"));
	browser->addAction(actionBack);
	browser->addAction(actionForward);
	browser->addAction(actionReload);
	browser->addAction(actionGo);
	browser->addAction(actionOpenInBrowser);
}

void WebTab::addToolbarActions(QToolBar* toolbar)
{
	toolbar->addSeparator();
	toolbar->addAction(actionCopy);
	toolbar->addAction(actionCut);
	toolbar->addAction(actionPaste);
	toolbar->addAction(actionFind);
	toolbar->addSeparator();
	toolbar->addAction(actionBack);
	toolbar->addAction(actionForward);
	toolbar->addAction(actionReload);
	toolbar->addAction(actionGo);
	toolbar->addAction(actionOpenInBrowser);
	
}
#endif

QString WebTabMenu::menuName()
{
	return "Web";
}