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

#ifndef _TABBED_WIDGET_HPP_
#define _TABBED_WIDGET_HPP_

#include <QString>
#include <QWidget>
#include <QDebug>
#include <QFileInfo>

#include "project.hpp"

#define TABBED_WIDGET_PROPERTY "_kiss_tabbed_"

namespace Kiss
{
	namespace Widget
	{
		class MainWindow;
		
		class Tab
		{
		public:
			Tab(QWidget *widget, MainWindow *mainWindow = 0);
			
			~Tab();

			/*!
			 * Called every time this Tab is switched to. Useful for setting window title.
			 */
			virtual void activate();

			/*!
			 * Begin setup is called before a Tab is added to the Tab Widget. 
			 * \return false cancels adding this Tab, true continues setup process
			 */
			virtual bool beginSetup() = 0;

			/*!
			 * The Tab has been added to the Tab Widget. Set the Tab Name here.
			 */
			virtual void completeSetup() = 0;

			/*!
			 * Requests a Tab to close
			 * \return false cancels the close operation, true closes the Tab
			 */
			virtual bool close() = 0;

			virtual void refreshSettings() = 0;

			QWidget *widget() const;

			MainWindow *mainWindow() const;
			void setMainWindow(MainWindow *mainWindow);

			void setFile(const QString& file);
			void setFile(const QFileInfo& file);
			const QFileInfo& file() const;
			bool hasFile() const;

			void setProject(const Kiss::Project::ProjectPtr& project);
			const Kiss::Project::ProjectPtr& project() const;
			bool hasProject() const;
			
		protected:
			virtual void fileChanged(const QFileInfo& file);
			virtual void projectChanged(const Kiss::Project::ProjectPtr& project);
			
		private:
			QWidget *m_widget;
			MainWindow *m_mainWindow;
			QFileInfo m_file;

			Kiss::Project::ProjectPtr m_project;
		};
	}
}

#endif
