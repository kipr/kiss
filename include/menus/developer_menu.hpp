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

#ifndef _DEVELOPER_MENU_HPP_
#define _DEVELOPER_MENU_HPP_

#include "build_options.hpp"

namespace kiss
{
	namespace menu
	{
		class DeveloperMenu;
	}
}

#ifdef BUILD_DEVELOPER_TOOLS

#include "activatable.hpp"
#include "menu_manager.hpp"
#include "source_file.hpp"
#include "concrete_menuable.hpp"

namespace kiss
{
	namespace widget
	{
		class MainWindow;
	}
	
	namespace menu
	{
		class DeveloperMenu : public ConcreteMenuable, public ActivatableObject
		{
		Q_OBJECT
		public:
			DeveloperMenu(widget::MainWindow *mainWindow);
	
			static QString menuName();
	
		protected slots:
			virtual void triggered();
	
		private:
			widget::MainWindow *m_mainWindow;
	
			QAction *declTab;
		};
	}
}

#endif

#endif
