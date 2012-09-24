/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
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

#ifndef _WELCOME_TAB_HPP_
#define _WELCOME_TAB_HPP_

// DEPRECATED

#include "build_options.hpp"

#include <QObject>

namespace Kiss
{
	namespace Widget
	{
		class WelcomeTab;
	}
}

#ifdef BUILD_WEB_TAB

#include "web_tab.hpp"

namespace Kiss
{
	namespace Widget
	{
		class WelcomeTab : public WebTab 
		{
		Q_OBJECT
		public:
			WelcomeTab(MainWindow *parent = 0);
			void activate();
			void completeSetup();
		};
	}
}

#endif

#endif