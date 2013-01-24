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

#ifndef _TARGET_MENU_HPP_
#define _TARGET_MENU_HPP_

#include "menu_manager.hpp"
#include "singleton.hpp"
#include "activatable.hpp"
#include "source_file.hpp"
#include "concrete_menuable.hpp"

#include <QMultiMap>

namespace Kiss
{
	namespace Menu
	{
		class TargetMenu : public ConcreteMenuable, public Singleton<TargetMenu>, public ActivatableObject
		{
		Q_OBJECT
		public:
			TargetMenu();
			void refresh();

			static QString menuName();

		public slots:
			void update();

		protected:
			void activated();
			void deactivated();

		private:
			Node *m_targetMenu;

			Node *compileNode;
			Node *downloadNode;
			Node *runNode;
		};
	}
}

#endif
