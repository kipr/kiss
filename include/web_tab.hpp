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

#ifndef _WEB_TAB_HPP_
#define _WEB_TAB_HPP_

#include "build_options.hpp"

#include <QObject>

namespace Kiss
{
	namespace Widget
	{
		class WebTab;
	}
}

#ifdef BUILD_WEB_TAB

#include "tab.hpp"
#include "macro_string.hpp"

#include "ui_WebTab.h"

#define KISS_BACKGROUND "KISS_BACKGROUND"

class QWebHistory;
class QWebView;

namespace Kiss
{
	namespace Widget
	{
		class MainWindow;
		
		class WebTab : public QWidget, public Tab, protected Ui::WebTab
		{
			Q_OBJECT
		public:
			WebTab(MainWindow* parent = 0);
			~WebTab();

			virtual void activate();

			bool beginSetup();
			virtual void completeSetup();

			bool close();

			//! Loads an unformatted URL
			void load(QString url, bool hideUrl = false);
			QString current();
			QWebHistory* history();

			QWebView* webView();

		public slots:
			void copy();
			void cut();
			void paste();

			void back();
			void forward();
			void go();
			void refresh();

			//! Opens current web page in system's default browser
			void openInBrowser();

			void find();

		signals:
			void updateActivatable();

		private slots:
			void updateTitle(const QString& title);
			void updateUrl(const QUrl& url);

			void on_actionGo_triggered();

			void on_ui_prevFind_clicked();
			void on_ui_nextFind_clicked();

			//! Check if there was an error loading the page
			void on_ui_webView_loadFinished(bool ok);

			void refreshSettings();

		private slots:
			void linkClicked(const QUrl& url);

		private:
			QUrl m_prevUrl;
			MacroString m_fragmentMacro;
		};
	}
}

#endif

#endif
