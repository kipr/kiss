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

#ifndef _CONCRETE_MENUABLE_HPP_
#define _CONCRETE_MENUABLE_HPP_

#include "activatable.hpp"
#include "menu_manager.hpp"

namespace Kiss
{
	namespace Menu
	{
		struct Invokable
		{
			Invokable(ActivatableObject *obj, const char *slot)
				: obj(obj), slot(slot)
			{
			}

			ActivatableObject *obj;
			const char *slot;
		};

		struct ConcreteMenuable : QObject, Menuable
		{
		Q_OBJECT
		public:
			ConcreteMenuable(const QString& name);
			~ConcreteMenuable();

			NodeList actionsFile();
			NodeList actionsEdit();
			NodeList actionsHelp();
			NodeList actions();
			NodeList toolbarActions();

		protected slots:
			virtual void triggered();
			virtual void activeTriggered();
			virtual void activeToggled(bool state);

		protected:
			NodeList m_file;
			NodeList m_edit;
			NodeList m_help;
			NodeList m_actions;
			NodeList m_toolbar;

			Node *node(QAction* action);

			QAction* activeAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);
			QAction* activeAction(const QString& iconRes, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);
			QAction* activeAction(const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);

			QAction* activeCheckedAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);
			QAction* activeCheckedAction(const QString& iconRes, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);
			QAction* activeCheckedAction(const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);

			QAction* action(const QIcon& icon, const QString& text, const QKeySequence& shortcut = QKeySequence::UnknownKey);
			QAction* action(const QString& iconRes, const QString& text, const QKeySequence& shortcut = QKeySequence::UnknownKey);
			QAction* action(const QString& text, const QKeySequence& shortcut = QKeySequence::UnknownKey);

		private:

			// Sets don't allow duplicates.
			QSet<QAction *> m_rawActions;

			QMap<QAction *, Invokable> m_activatables;
		};
	}
}

#endif
