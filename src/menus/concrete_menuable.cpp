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

#include "concrete_menuable.hpp"

#include <QDebug>

using namespace Kiss::Menu;

ConcreteMenuable::ConcreteMenuable(const QString& name)
	: Menuable(name)
{
}

ConcreteMenuable::~ConcreteMenuable()
{
	foreach(QAction *action, m_rawActions) delete action;
}

NodeList ConcreteMenuable::actionsFile()
{
	return m_file;
}

NodeList ConcreteMenuable::actionsEdit()
{
	return m_edit;
}

NodeList ConcreteMenuable::actionsHelp()
{
	return m_help;
}

NodeList ConcreteMenuable::actions()
{
	return m_actions;
}

NodeList ConcreteMenuable::toolbarActions()
{
	return m_toolbar;
}

void ConcreteMenuable::triggered()
{
	
}

void ConcreteMenuable::activeTriggered()
{
	QAction *_ = (QAction*)sender();
	QMap<QAction*, Invokable>::iterator it = m_activatables.find(_);
	if(it == m_activatables.end()) return;
	if(!QMetaObject::invokeMethod(it->obj->active(), it->slot, Qt::DirectConnection))
		qWarning() << "Failed to call" << it->slot << "on" << it->obj->active();

}

void ConcreteMenuable::activeToggled(bool state)
{
	QAction *_ = (QAction*)sender();
	QMap<QAction*, Invokable>::iterator it = m_activatables.find(_);
	if(it == m_activatables.end()) return;

	qWarning() << "toggle";
	if(!QMetaObject::invokeMethod(it->obj->active(), it->slot, Qt::DirectConnection, Q_ARG(bool, state)))
		qWarning() << "Failed to call" << it->slot << "on" << it->obj->active();

}

Node* ConcreteMenuable::node(QAction *action)
{
	return new Node(this, action);
}

QAction* ConcreteMenuable::activeAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot)
{
	QAction *ret = new QAction(icon, text, 0);
	ret->setShortcut(shortcut);
	QObject::connect(ret, SIGNAL(triggered()), this, SLOT(activeTriggered()));
	m_activatables.insert(ret, Invokable(activatable, slot));
	return ret;
}

QAction *ConcreteMenuable::activeAction(const QString& iconRes, const QString& text, const QKeySequence& shortcut, ActivatableObject *activatable, const char *slot)
{
	return activeAction(ResourceHelper::ref().icon(iconRes), text, shortcut, activatable, slot);
}

QAction *ConcreteMenuable::activeAction(const QString& text, const QKeySequence& shortcut, ActivatableObject *activatable, const char *slot)
{
	return activeAction("", text, shortcut, activatable, slot);
}

QAction *ConcreteMenuable::activeCheckedAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char *slot)
{
	QAction* ret = new QAction(icon, text, 0);
	ret->setCheckable(true);
	ret->setShortcut(shortcut);
	QObject::connect(ret, SIGNAL(triggered(bool)), this, SLOT(activeToggled(bool)));
	m_activatables.insert(ret, Invokable(activatable, slot));
	m_rawActions += ret;
	return ret;
}

QAction *ConcreteMenuable::activeCheckedAction(const QString& iconRes, const QString& text, const QKeySequence& shortcut, ActivatableObject *activatable, const char *slot)
{
	return activeCheckedAction(ResourceHelper::ref().icon(iconRes), text, shortcut, activatable, slot);
}

QAction *ConcreteMenuable::activeCheckedAction(const QString& text, const QKeySequence& shortcut, ActivatableObject *activatable, const char *slot)
{
	return activeCheckedAction("", text, shortcut, activatable, slot);
}

QAction *ConcreteMenuable::action(const QIcon& icon, const QString& text, const QKeySequence& shortcut)
{
	QAction* ret = new QAction(icon, text, 0);
	ret->setShortcut(shortcut);
	QObject::connect(ret, SIGNAL(triggered()), this, SLOT(triggered()));
	m_rawActions += ret;
	return ret;
}

QAction *ConcreteMenuable::action(const QString& iconRes, const QString& text, const QKeySequence& shortcut)
{
	return action(ResourceHelper::ref().icon(iconRes), text, shortcut);
}

QAction *ConcreteMenuable::action(const QString& text, const QKeySequence& shortcut)
{
	return action("", text, shortcut);
}
