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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "ui_MainWindow.h"
#include "Tab.h"

#include <Qsci/qscilexercpp.h>
#include <QEvent>
#include <QToolButton>
#include <QListWidget>
#include "ChooseTargetDialog.h"
#include "EditorSettingsDialog.h"
#include "ChoosePortDialog.h"

class QListWidgetItem;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void closeEvent(QCloseEvent *e);
	
	void newFile();
	bool openFile(QString filePath);
	
	void initMenus(Tab* tab);
	
	void setTabName(QWidget* widget, QString string);
	void setStatusMessage(QString message, int time = 0);
	
	ChooseTargetDialog* chooseTargetDialog();
	ChoosePortDialog* choosePortDialog();
	
	void setErrors(Tab* tab, QStringList errors, QStringList warnings, QStringList linker, QStringList verbose);
	void hideErrors();
	
	void addTab(Tab* tab);
	
	QTabWidget* tabWidget();
public slots:
	void on_actionNew_triggered();
	void on_actionOpen_triggered();
	void on_actionNext_triggered();
	void on_actionPrevious_triggered();
	void on_actionClose_triggered();
	void on_actionAbout_triggered();
	void errorViewShowVerbose();
	void errorViewShowSimple();
	void on_actionEditor_Settings_triggered();
	
private slots:
	void on_ui_tabWidget_currentChanged(int i);
	
	void errorClicked(QListWidgetItem* item);
	
	void showContextMenuForError(const QPoint &pos);
	
private:
	Tab* m_currentTab;
	Tab* m_errorTab;
	ChooseTargetDialog m_chooseTargetDialog;
	ChoosePortDialog m_choosePortDialog;
	EditorSettingsDialog m_editorSettingsDialog;
	QListWidget m_errorList, m_warningList, m_linkErrorList, m_verboseList;

	void deleteTab(int index);
	void showErrorMessages(bool verbose = false);

signals:
	void settingsUpdated();
};

#endif
