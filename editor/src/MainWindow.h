/**************************************************************************
 *  Copyright 2007,2008,2009 KISS Institute for Practical Robotics        *
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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "ui_MainWindow.h"
#include "SourceFile.h"
#include "ChooseTargetDialog.h"
#include "ChoosePortDialog.h"
#include "EditorSettingsDialog.h"
#include "TargetSettingsDialog.h"
#include "Target.h"
#include "FindDialog.h"

#include <Qsci/qscilexercpp.h>
#include <QEvent>
#include <QToolButton>
#include <QListWidget>

/* MainWindow Class inherits QMainWindow and Ui::MainWindow
 *
 * List of Ui::MainWindow inherited widgets:
 *	QTabWidget	ui_errorView
 *	QTabWidget	ui_tabWidget
 *	QMenuBar	ui_menubar
 *	QMenu		ui_menuFile
 *	QMenu		ui_menuEdit
 *  QMenu		ui_menuSettings
 *	QMenu		ui_menuHelp
 *	QToolBar	ui_mainToolBar
 *	QToolBar	ui_targetToolBar
 *
 * List of Ui::MainWindow inherited actions:
 *		actionNew		actionShortcutNew					
 *		actionSave		actionShortcutSave
 *		actionOpen		actionShortcutOpen
 *		actionClose		actionShortcutClose
 *		actionUndo		actionShortcutUndo
 *		actionRedo		actionShortcutRedo
 *		actionCut		actionShortcutCut
 *		actionCopy		actionShortcutCopy
 *		actionPaste		actionShortcutPaste
 *		actionSave_As	actionPrint
 *		actionQuit		actionSelect_All
 *		actionFind		actionNext
 *		actionGoto_Line	actionIndent_All
 *		actionCompile	actionRun
 *		actionStop		actionDownload
 *		actionZoom_In	actionZoom_Out
 *		actionChange_Font_Size	
 *		actionChange_Port		
 *		actionChange_Target
 *		actionChange_Port
 *		actionEditor_Settings
 *		actionTarget_Settings
 *		actionTarget_Manual
 */

class MainWindow: public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void closeEvent(QCloseEvent *e);
	
	bool openFile(QString filename);
	
private:
	/* Dialogs used by this class */
	EditorSettingsDialog m_editorSettingsDialog;
	TargetSettingsDialog m_targetSettingsDialog;
	ChooseTargetDialog   m_chooseTargetDialog;
	ChoosePortDialog     m_choosePortDialog;
	FindDialog			 m_findDialog;

	/* Other necessary objects:
	 *	m_target: references the currently selected target
	 *	m_currentSourceFileEditor: references the currently selected editor widget
	 *	m_*List: these widgets display errors returned by the target compiler
	 */
	Target m_target;
	SourceFile *m_currentSourceFile;
	QListWidget m_errorList, m_warningList, m_linkErrorList, m_verboseErrorList;

	/* Deletes the editor object at tab <index>, and removes the tab */
	void deleteTab(int index);
	void addTab(SourceFile *sourceFile);

	/* Displays compiler messages stored in the m_*List widgets in the errorView tab */
	void showErrorMessages(bool verbose = false);
	void readErrorMessages();

public slots:

	/*  Auto-connected slots
     *  Format is as follows:
	 *  void on_<widget name>_<signal name>(<parameters>); */

	/* File Menu connections */
	void on_actionNew_triggered(bool checked = false);
	void on_actionOpen_triggered(bool checked = false);
	void on_actionSave_triggered(bool checked = false);
	void on_actionSave_As_triggered(bool checked = false);
	void on_actionClose_triggered(bool checked = false);
	void on_actionPrint_triggered(bool checked = false);

	/* Edit Menu connections */
	void on_actionUndo_triggered(bool checked = false);
	void on_actionRedo_triggered(bool checked = false);

	void on_actionCut_triggered(bool checked = false);
	void on_actionCopy_triggered(bool checked = false);
	void on_actionPaste_triggered(bool checked = false);
	void on_actionSelect_All_triggered(bool checked = false);

	void on_actionFind_triggered(bool checked = false);
	void on_actionFind_Next_triggered(bool checked = false);
	void on_actionGoto_Line_triggered(bool checked = false);
	void on_actionIndent_All_triggered(bool checked = false);

	void on_actionZoom_In_triggered(bool checked = false);
	void on_actionZoom_Out_triggered(bool checked = false);

	/* Settings Menu connections */
	void on_actionChange_Target_triggered(bool checked = false);
	void on_actionChange_Port_triggered(bool checked = false);
	void on_actionEditor_Settings_triggered(bool checked = false);
	void on_actionTarget_Settings_triggered(bool checked = false);

	/* Help Menu connections */
	void on_actionAbout_triggered(bool checked = false);
	void on_actionTarget_Manual_triggered(bool checked = false);

	/* Target Tool Bar connections */
	void on_actionDownload_triggered(bool checked = false);
	void on_actionCompile_triggered(bool checked = false);
	void on_actionRun_triggered(bool checked = false);
	void on_actionStop_triggered(bool checked = false);
	void on_actionSimulate_triggered(bool checked = false);

	/* Widget connections */
	void on_ui_tabWidget_currentChanged(int index);
	
	/* Miscellaneous connections */
	void sourceFileModified(bool m);
	void errorViewShowVerbose();
	void errorViewShowSimple();
	
	void errorClicked(QListWidgetItem *errorItem);
	
	void setTitleBarTarget(QString txt);

signals:
	void settingsUpdated();
};

#endif
