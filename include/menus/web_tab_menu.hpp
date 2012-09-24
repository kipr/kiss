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

#ifndef _WEB_TAB_MENU_HPP_
#define _WEB_TAB_MENU_HPP_

#include "build_options.hpp"

#include <QObject>

namespace Kiss
{
	namespace Menu
	{
		class WebTabMenu;
	}
}

#ifdef BUILD_WEB_TAB

#include "activatable.hpp"
#include "singleton.hpp"
#include "menu_manager.hpp"
#include "web_tab.hpp"
#include "concrete_menuable.hpp"

namespace Kiss
{
	namespace Menu
	{
		class WebTabMenu : public ConcreteMenuable, public ActivatableObject
		{
		Q_OBJECT
		public:
			WebTabMenu();

			static QString menuName();

		protected:
			void activated();
			void deactivated();

			void triggered();

		public slots:
			void update();

		private:	
			QAction *zoomOut;
			QAction *zoomIn;
			QAction *resetZoom;

			Node *back;
			Node *forward;
			Node *reload;
			Node *open;
		};
	}
}

#endif

#endif
