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

#include "MenuManager.h"
#include "Singleton.h"
#include "Log.h"

#include <QSet>
#include <QDebug>

struct MenuNodeSingleton : Singleton<MenuNodeSingleton>
{
	~MenuNodeSingleton() {
		foreach(MenuNode* child, nodes) delete child;
	}
	
	QSet<MenuNode*> nodes;
};

MenuNode::MenuNode() : name(uniqueName()), rawAction(0), activeControl(false), hideOnDisable(true) { MenuNodeSingleton::ref().nodes += this; }
MenuNode::MenuNode(const QString& name) : name(name), rawAction(0), activeControl(false), hideOnDisable(true) { MenuNodeSingleton::ref().nodes += this; }

MenuNode::MenuNode(const QString& name, Menuable* menuable, QAction* action) : name(name), rawAction(0), activeControl(false), hideOnDisable(true)
{
	registers.append(MenuableActionPair(menuable, action));
	MenuNodeSingleton::ref().nodes += this;
}

MenuNode::MenuNode(Menuable* menuable, QAction* action) : name(action->text()), rawAction(0), activeControl(false), hideOnDisable(true)
{
	registers.append(MenuableActionPair(menuable, action));
	MenuNodeSingleton::ref().nodes += this;
}

MenuNode::~MenuNode() {}

QList<MenuNode*> MenuNode::unify(QList<MenuNode*> nodes)
{
	QMap<QString, MenuNode*> unity;
	foreach(MenuNode* node, nodes) {
		QMap<QString, MenuNode*>::iterator it = unity.find(node->name);
		if(it == unity.end()) unity[node->name] = node;
		else {
			Log::ref().info(QString("%1 collides").arg(node->name));
			(*it)->registers.append(node->registers);
		}
	}
	
	return unity.values();
}

MenuNode* MenuNode::separator()
{
	static MenuNode* separator = new MenuNode;
	return separator;
}

QActionList MenuNode::allActions()
{
	QActionList ret;
	foreach(MenuableActionPair pair, registers) ret.append(pair.action);
	foreach(MenuNode* child, children) ret.append(child->allActions());
	return ret;
}

QString MenuNode::uniqueName()
{
	static int i = 0;
	return QString("__") + QString::number(++i);
}

MenuManager::MenuManager() : m_root(new MenuNode("")), 
	m_file(new MenuNode("File")), m_edit(new MenuNode("Edit")),
	m_help(new MenuNode("Help")), m_tool(new MenuNode), m_menuBar(0), m_toolBar(0)
{
}

MenuManager::~MenuManager()
{
}

void MenuManager::registerMenus(Menuable* menuable)
{
	menuable->setMenuManager(this);
	addChildren(m_file, menuable->actionsFile());
	addChildren(m_edit, menuable->actionsEdit());
	addChildren(m_help, menuable->actionsHelp());
	addChildren(m_root, menuable->actions());
	addChildren(m_tool, menuable->toolbarActions());
}

void MenuManager::unregisterMenus(Menuable* menuable)
{
	menuable->setMenuManager(0);
	unregisterMenus(menuable, m_root);
	
	// FIXME: Redundant if construct already called
	unregisterMenus(menuable, m_file);
	unregisterMenus(menuable, m_edit);
	unregisterMenus(menuable, m_help);
	unregisterMenus(menuable, m_tool);
}

void MenuManager::activate(QList<Menuable*> menus)
{
	m_active = menus;
	refresh();
}

void MenuManager::addActivation(Menuable* menuable)
{
	if(m_active.contains(menuable)) return;
	m_active.append(menuable);
	refresh();
}

void MenuManager::removeActivation(Menuable* menuable)
{
	m_active.removeAll(menuable);
	refresh();
}

void MenuManager::construct(QMenuBar* menuBar, QToolBar* toolBar)
{
	construct(menuBar->addMenu("File"), m_file->children);

	construct(menuBar->addMenu("Edit"), m_edit->children);

	foreach(MenuNode* rootChild, m_root->children)
		construct(menuBar->addMenu(rootChild->name), rootChild->children);
	
	construct(menuBar->addMenu("Help"), m_help->children);
	
	construct(toolBar, m_tool->children);
	
	m_menuBar = menuBar;
	m_toolBar = toolBar;
	refresh();
}

bool MenuManager::isConstructed() const { return m_menuBar; }

void MenuManager::refreshToolbar()
{
	m_toolBar->hide();
	m_toolBar->clear();
	MenuNodeList::iterator it = m_tool->children.begin();
	QAction* lastSep = 0;
	for(; it != m_tool->children.end(); ++it) {
		MenuNode* child = *it;
		if(child == MenuNode::separator() && !lastSep) {
			lastSep = m_toolBar->addSeparator();
		} else if(isActivatable(child)
			&& (child->rawAction->isEnabled()
			|| !child->hideOnDisable)) {
				m_toolBar->addAction(child->rawAction);
				lastSep = 0;
			}
	}
	if(lastSep) m_toolBar->removeAction(lastSep);
	m_toolBar->show();
}

void MenuManager::triggered()
{
	QAction* action = (QAction*)sender();
	MenuNode* node = (MenuNode*)action->data().value<void*>();
	// qWarning() << node->name << "triggered (" << node->registers.size() << "registers )";
	
	bool called = false;
	foreach(MenuableActionPair pair, node->registers) {
		if(m_active.contains(pair.menuable)) {
			called = true;
			// qWarning() << "Calling" << pair.action << "on" << pair.menuable;
			pair.action->trigger();
		}
	}
	if(!called) Log::ref().warning(QString("Did not trigger %1 on any menuables.").arg(node->name));
}

void MenuManager::addChildren(MenuNode* parent, const MenuNodeList& nodes)
{
	// Hopefully makes this a O(2n) instead of O(n^2)
	QMap<QString, MenuNode*> lookup;
	foreach(MenuNode* child, parent->children) lookup[child->name] = child;
	
	foreach(MenuNode* node, nodes) {
		QMap<QString, MenuNode*>::iterator it = lookup.find(node->name);
		if(it == lookup.end() || *it == MenuNode::separator()) {
			parent->children.append(node);
			lookup[node->name] = node;
		} else {
			(*it)->registers.append(node->registers);
			(*it)->children.append(node->children);
			addChildren(node, node->children);
		}
	}
}

QList<MenuableActionPair> MenuManager::actionsToPair(Menuable* menuable, const QActionList& actions)
{
	QList<MenuableActionPair> ret;
	foreach(QAction* action, actions)
		ret.append(MenuableActionPair(menuable, action));
	
	return ret;
}

void MenuManager::unregisterMenus(Menuable* menuable, MenuNode* node)
{
	QList<MenuableActionPair>::iterator it = node->registers.begin();
	for(; it != node->registers.end(); it++) {
		if((*it).menuable == menuable) it = node->registers.erase(it);
	}
	QList<MenuNode*>::iterator _it = node->children.begin();
	for(; _it != node->children.end(); _it++) {
		unregisterMenus(menuable, *_it);
		if((*_it)->registers.size() == 0 && (*_it)->children.size() == 0)
			_it = node->children.erase(_it);
		
	}
}

void MenuManager::construct(QMenu* menu, QList<MenuNode*> nodes)
{
	foreach(MenuNode* node, nodes) construct(menu, node);
}

void MenuManager::construct(QToolBar* tool, QList<MenuNode*> nodes)
{
	foreach(MenuNode* node, nodes) construct(tool, node);
}

void MenuManager::construct(QMenu* menu, MenuNode* node)
{
	if(node == MenuNode::separator()) {
		menu->addSeparator();
		return;
	}
	if(node->children.size() == 0) { // Terminal node
		if(node->registers.size() == 0) {
			Log::ref().warning(QString("Terminal node %1 has no registers. Ignoring.").arg(node->name));
			return;
		}
		if(node->rawAction) return;
		QAction* first = node->registers[0].action;
		QAction* action = menu->addAction(first->icon(), node->name);
		action->setShortcut(first->shortcut());
		action->setCheckable(first->isCheckable());
		action->setMenuRole(first->menuRole());
		action->setData(QVariant::fromValue((void*)node));
		node->rawAction = action;
		connect(action, SIGNAL(triggered()), this, SLOT(triggered()));
	} else { // Non-terminal node
		QMenu* subMenu = menu->addMenu(node->name);
		construct(subMenu, node->children);
	}
}

void MenuManager::construct(QToolBar* toolbar, MenuNode* node)
{
	if(node->children.size() == 0) { // Terminal node
		if(node->registers.size() == 0) {
			Log::ref().warning(QString("Terminal node %1 has no registers. Ignoring.").arg(node->name));
			return;
		}
		if(node->rawAction) return;
		QAction* action = new QAction(node->registers[0].action->icon(), node->name, toolbar);
		action->setShortcut(node->registers[0].action->shortcut());
		// qWarning() << "Checkable?" << node->registers[0].action->isCheckable();
		action->setCheckable(node->registers[0].action->isCheckable());
		action->setData(QVariant::fromValue((void*)node));
		node->rawAction = action;
	} else  // Non-terminal node
		Log::ref().warning(QString("Non-terminal nodes not supported on toolbar. Ignoring %1").arg(node->name));
	
}

void MenuManager::refresh()
{
	if(!isConstructed()) return;
	
	refresh(m_file);
	refresh(m_edit);
	refresh(m_root);
	refresh(m_help);

	refreshToolbar();
}

void MenuManager::refresh(MenuNode* node)
{
	if(node->rawAction && !node->activeControl) node->rawAction->setEnabled(isActivatable(node));
	foreach(MenuNode* child, node->children) refresh(child);
}

bool MenuManager::isActivatable(MenuNode* node) const
{
	foreach(MenuableActionPair pair, node->registers)
		if(m_active.contains(pair.menuable)) return true;
	
	return false;
}
