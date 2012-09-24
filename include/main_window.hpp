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

#ifndef _MAIN_WINDOW_HPP_
#define _MAIN_WINDOW_HPP_

#include "ui_MainWindow.h"
#include "editor_settings_dialog.hpp"
#include "theme_settings_dialog.hpp"
#include "tab.hpp"

#include "menu_manager.hpp"
#include "project_manager.hpp"
#include "main_window_menu.hpp"

#include <Qsci/qscilexercpp.h>
#include <pcompiler/output.hpp>
#include <QEvent>
#include <QToolButton>
#include <QListWidget>

#define RECENTS "recents"

class QListWidgetItem;

namespace Kiss
{
	namespace Widget
	{
		class SourceFile;

		/*! \class MainWindow
		 * \brief Holds tabs to display
		 *
		 * Very little is actually implemented in MainWindow. Instead, most is in the implementation of every Tab.
		 */
		class MainWindow : public QMainWindow, private Ui::MainWindow
		{
		Q_OBJECT
		public:	
			MainWindow(QWidget *parent = 0);
			~MainWindow();

			void closeEvent(QCloseEvent *e);

			/*!
			 * Opens a file with SourceFile tab
			 * \param filePath Path to file
			 */
			bool openFile(const QString& filePath);
			bool memoryOpen(const QByteArray& ba, const QString& assocPath);
			bool openProject(const QString& filePath);
			bool newProject(const QString& filePath);

			void initMenus(Tab* tab);

			void initMenus();

			/*! Sets Window Title
			 * \param title Title to append to primary window name
			 */
			void setTitle(const QString& title);

			/*! Sets Tab title
			 * \param widget Tab to set
			 * \param string String to set tab's text to.
			 */
			void setTabName(QWidget *widget, const QString& string);

			/*! Sets Tab icon
			 * \param widget Tab to set
			 * \param icon icon to set
			 */
			void setTabIcon(QWidget *widget, const QIcon& icon);

			/*! Sets Window's status message
			 * \param message Message to display
			 * \param time Time to display in milliseconds
			 */
			void setStatusMessage(const QString& message, int time = 0);

			void hideErrors();

			/*! 
			 * Deletes tab at given index. Does NOT clean up tab via close method.
			 * \param index Tab to add
			 */
			void deleteTab(int index);

			/*! 
			 * Adds given tab window. Calls tab setup functions.
			 * \param tab Tab to add
			 */
			void addTab(Tab* tab);

			void moveToTab(Tab* tab);

			QTabWidget* tabWidget();

			QList<Tab *> tabs();

			template<typename T>
			QList<T*> tabs() {
				QList<T *> ret;
				QList<Tab *> all = tabs();
				foreach(Tab* tab, all) {
					T *t = dynamic_cast<T*>(tab);
					if(t) ret.append(t);
				}
				return ret;
			}

			/*! 
			 * Closes all but given tab 
			 * \param tab Tab to keep open
			 */
			void closeAllOthers(Tab* tab);

			/*! Reinits menus for current tab */
			void refreshMenus();

			bool eventFilter(QObject *target, QEvent *event);

			Project::Manager *projectManager();

			Menu::Manager *menuManager();
			Menu::Menuable* menuable(const QString& name);
			QList<Menu::Menuable*> menuablesExcept(const QStringList& name);
			QList<Menu::Menuable*> menuables();
			void activateMenuable(const QString& name, QObject *on);
			QStringList standardMenus() const;

			void restart();

			bool canClose();
			bool canGoPrevious();
			bool canGoNext();

			Project::Project *activeProject() const;

			friend class Menu::MainWindowMenu;

		public slots:
			Project::Project *newProject();
			void newFile();
			void open();
			void openProject();
			void next();
			void previous();
			void closeTab(bool force = false);
			void closeProjectTabs(Kiss::Project::Project *project);
			bool closeFile(const QString& file);
			void about();
			void settings();
			void theme();

			void projectAddNew();
			void projectAddExisting();
			void projectRemoveExisting();
			void projectExtractTo();

			void showProjectDock(bool show = true);
			void hideProjectDock();

			QList<QObject*> tabs(const QString& type);

		signals:
			void settingsUpdated();
			void updateActivatable();

		private slots:
			void on_ui_tabWidget_currentChanged(int i);
			void on_ui_addFile_clicked();
			void on_ui_removeFile_clicked();
			void openRecent();

			void errorClicked(QListWidgetItem* item);

			void showContextMenuForError(const QPoint &pos);

			void projectClicked(const QModelIndex& index);
			void projectFileClicked(const QModelIndex& index);

			void projectOpened(Project::Project *project);
			void projectClosed(Project::Project *project);

		private:
			Tab *m_currentTab;
			EditorSettingsDialog m_editorSettingsDialog;
			ThemeSettingsDialog m_themeSettingsDialog;
			QMap<QWidget *, Tab *> m_lookup;
			Menu::Manager m_menuManager;
			Project::Manager m_projectManager;
			QList<Menu::Menuable *> m_menuables;

			// ProjectsModel m_projectsModel;

			void addLookup(Tab *tab);
			void removeLookup(QWidget* widget);
			Tab *lookup(QWidget* widget) const;

			void showErrorMessages(bool verbose = false);
		};
	}
}

#endif
