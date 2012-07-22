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

#include "SourceFileMenu.h"
#include "MainWindowMenu.h"

#include "ResourceHelper.h"
#include "MainWindow.h"

#include <QIcon>
#include <QDebug>

SourceFileMenu::SourceFileMenu(MainWindow* mainWindow) : ConcreteMenuable(menuName())
{
	const QIcon disk(ResourceHelper::ref().lookup("disk"));
	
	m_file.append(node(activeAction(disk, "Save", QKeySequence::Save, this, "save")));
	m_file.append(node(activeAction(disk, "Save As...", QKeySequence::SaveAs, this, "saveAs")));
	m_file.append(convertToProject = node(activeAction("cog_go", "Convert to Project", QKeySequence::UnknownKey, this, "convertToProject")));
	m_file.append(node(activeAction("print", "Print", QKeySequence::Print, this, "print")));
	m_file.append(MenuNode::separator());
	
	MenuNode* copy = node(activeAction("copy", "Copy", QKeySequence::Copy, this, "copy"));
	MenuNode* cut = node(activeAction("cut", "Cut", QKeySequence::Cut, this, "cut"));
	MenuNode* paste = node(activeAction("paste", "Paste", QKeySequence::Paste, this, "paste"));
	
	m_edit.append(copy);
	m_edit.append(cut);
	m_edit.append(paste);
	
	m_edit.append(MenuNode::separator());
	m_edit.append(undo = node(activeAction("arrow_undo", "Undo", QKeySequence::Undo, this, "undo")));
	m_edit.append(redo = node(activeAction("arrow_redo", "Redo", QKeySequence::Redo, this, "redo")));
	m_edit.append(MenuNode::separator());
	m_edit.append(node(activeAction("magnifier_zoom_in", "Zoom In", QKeySequence::ZoomIn, this, "zoomIn")));
	m_edit.append(node(activeAction("magnifier_zoom_out", "Zoom Out", QKeySequence::ZoomOut, this, "zoomOut")));
	m_edit.append(node(activeAction("magnifier", "Reset Zoom", QKeySequence("Ctrl+0"), this, "zoomReset")));
	m_edit.append(MenuNode::separator());
	m_edit.append(node(activeAction("find", "Find", QKeySequence::Find, this, "find")));
	
	// undo->activeControl = redo->activeControl = true;
	
	MenuNode* source = new MenuNode("Source");
	
	source->children.append(node(activeAction("", "Indent All", QKeySequence("Ctrl+I"), this, "indentAll")));
	source->children.append(MenuNode::separator());
	source->children.append(breakpoint = node(activeCheckedAction(QIcon(), "Toggle Breakpoint", QKeySequence("Ctrl+Shift+B"), this, "toggleBreakpoint")));
	source->children.append(node(activeAction("", "Clear All Breakpoints", QKeySequence("Ctrl+Alt+Shift+B"), this, "clearBreakpoints")));
	
	m_actions.append(source);
	
	m_toolbar.append(copy);
	m_toolbar.append(cut);
	m_toolbar.append(paste);
	m_toolbar.append(MenuNode::separator());
}

void SourceFileMenu::activated()
{
	menuManager()->addActivation(this);
}

void SourceFileMenu::deactivated()
{
	menuManager()->removeActivation(this);
}

void SourceFileMenu::update()
{
	SourceFile* sourceFile = dynamic_cast<SourceFile*>(active());
	if(!sourceFile) return;
	qWarning() << "Called";
	breakpoint->rawAction->setEnabled(sourceFile->breakpointOnLine(sourceFile->currentLine()));
	convertToProject->rawAction->setEnabled(!sourceFile->isProjectAssociated());
}

QString SourceFileMenu::menuName()
{
	return "Source";
}
