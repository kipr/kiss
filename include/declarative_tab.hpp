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

#ifndef _DECLARATIVE_TAB_HPP_
#define _DECLARATIVE_TAB_HPP_

#include "build_options.hpp"

#include <QObject>

#ifdef BUILD_DECLARATIVE_TAB

#include "tab.hpp"

#include <QUrl>

#ifdef BUILD_DEVELOPER_TOOLS
#include <QFileSystemWatcher>
#endif

namespace Kiss
{
	namespace Widget
	{
		class MainWindow;
		
		class DeclarativeTools : public QObject
		{
		Q_OBJECT
		public:
			DeclarativeTools(MainWindow *mainWindow);

		public slots:
			void newFile();
			void open();
			void openWeb(const QString& url);
			void settings();

			const QStringList templates(const QString& target);

			void tellActivated();
			void tellCompletedSetup();

		signals:
			void activated();
			void completedSetup();

		private:
			MainWindow *m_mainWindow;
		};

		class DeclarativeTab : public QObject, public Tab
		{
		Q_OBJECT
		public:
			DeclarativeTab(const QUrl& file, MainWindow *parent);
			~DeclarativeTab();

			void activate();
			bool beginSetup();
			void completeSetup();
			bool close();

		public slots:
			void reload();
			void setTitle(const QString& title);
			void refreshSettings();

		private:
		#ifdef BUILD_DEVELOPER_TOOLS
			QFileSystemWatcher m_watcher;
		#endif
			DeclarativeTools m_kiss;

		};
	}
}

#endif

#endif
