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

#ifndef _MENU_MANAGER_HPP_
#define _MENU_MANAGER_HPP_

#include "singleton.hpp"
#include "resource_helper.hpp"

#include <QList>
#include <QSet>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QObject>

namespace Kiss
{
	namespace Menu
	{
		typedef QList<QAction *> QActionList;

		class Menuable;

		struct MenuableActionPair
		{
			MenuableActionPair(Menuable *menuable, QAction *action)
				: menuable(menuable), action(action)
			{
			}

			Menuable *menuable;
			QAction *action;
		};

		struct Node
		{
			Node();
			Node(const QString& name);
			Node(const QString& name, Menuable *menuable, QAction *action);
			Node(Menuable *menuable, QAction *action);
			~Node();

			static QList<Node *> unify(QList<Node *> nodes);
			static Node *separator();

			QActionList allActions();
			QString name;
			QAction *rawAction;
			QList<Node *> children;
			QList<MenuableActionPair> registers;

			bool activeControl;
			bool hideOnDisable;

		private:
			static QString uniqueName();
		};

		typedef QList<Node *> NodeList;
		class Manager;

		class Menuable
		{
		public:
			Menuable(const QString& name)
				: m_name(name), m_menuManager(0)
			{
			}

			virtual NodeList actionsFile() = 0;
			virtual NodeList actionsEdit() = 0;
			virtual NodeList actionsHelp() = 0;
			virtual NodeList actions() = 0;
			virtual NodeList toolbarActions() = 0;

			const QString& name() const
			{
				return m_name;
			}

			Manager *menuManager()
			{
				return m_menuManager;
			}
			
			void setManager(Manager* menuManager)
			{
				m_menuManager = menuManager;
			}

		private:
			QString m_name;

			Manager *m_menuManager;
		};

		class Manager : public QObject
		{
		Q_OBJECT
		public:
			Manager();
			~Manager();

			void registerMenus(Menuable *menuable);
			void unregisterMenus(Menuable *menuable);

			void activate(QList<Menuable *> menus);
			void addActivation(Menuable *menuable);
			void removeActivation(Menuable *menuable);

			void construct(QMenuBar *menuBar, QToolBar *toolBar);
			bool isConstructed() const;

			void refreshToolbar();

		private slots:
			void triggered();

		private:
			void addChildren(Node *parent, const NodeList& nodes);
			QList<MenuableActionPair> actionsToPair(Menuable *menuable, const QActionList& actions);
			void unregisterMenus(Menuable *menuable, Node* node);
			void construct(QMenu *menu, NodeList nodes);
			void construct(QToolBar *tool, NodeList nodes);
			void construct(QMenu *menu, Node* node);
			void construct(QToolBar *toolbar, Node* node);

			void refresh();
			void refresh(Node *node);

			bool isActivatable(Node *node) const;

			Node *lookup(QAction *action);

			Node* m_root;

			Node *m_file;
			Node *m_edit;
			Node *m_help;

			Node *m_tool;

			QList<Menuable *> m_active;

			QMenuBar *m_menuBar;
			QToolBar *m_toolBar;
		};
	}
}

#endif
