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

#ifndef __SourceFile_H__
#define __SourceFile_H__

#include "Lexer.h"
#include "TemplateDialog.h"
#include "ChoosePortDialog.h"
#include "EditorSettingsDialog.h"
#include "Target.h"
#include "ui_SourceFile.h"
#include "Tab.h"
#include "Debugger.h"

#include <QtGlobal>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QKeyEvent>
#include <QObject>
#include <QMenuBar>
#include <QToolBar>

class FindDialog;
class MainWindow;

class SourceFile : public QWidget, public Tab, private Ui::SourceFile
{
Q_OBJECT
public:
	SourceFile(QWidget* parent = 0);
	
	void activate();
	void addActionsFile(QMenu* file);
	void addActionsEdit(QMenu* edit);
	void addActionsHelp(QMenu* help);
	void addOtherActions(QMenuBar* menuBar);
	void addToolbarActions(QToolBar* toolbar);
	
	bool beginSetup();
	void completeSetup();
	
	bool fileSaveAs(const QString& filePath);
	bool fileOpen(const QString& filePath);
	
	bool close();

	//! \return File's Name
	QString fileName();
	//! \return Absolute path to current file
	QString filePath();
	//! \return true if file is new
	bool isNewFile();
	
	//! \return Current zoom level
	int getZoom();
	
	QsciScintilla* getEditor();
	
	void moveTo(int line, int pos);

public slots:
	void indentAll();
	void refreshSettings();
	void updateMargins();

	//! Increase editor's zoom level
	void zoomIn();
	//! Decreate editor's zoom level
	void zoomOut();

	//! Promps user to save file to a browsed location
	void on_actionSaveAs_triggered();
	
	//! Save current file
	bool fileSave();

	void sourceModified(bool m);
	
	void on_actionDownload_triggered();
	void on_actionCompile_triggered();
	void on_actionRun_triggered();
	void on_actionStop_triggered();
	void on_actionSimulate_triggered();
	void on_actionDebug_triggered();
	
private slots:
	void on_actionCopy_triggered();
	void on_actionCut_triggered();
	void on_actionPaste_triggered();
	void on_actionUndo_triggered();
	void on_actionRedo_triggered();
	void on_actionFind_triggered();
	
	void openManual();
	void on_actionPrint_triggered();
	
	void on_actionZoomIn_triggered();
	void on_actionZoomOut_triggered();
	void on_actionResetZoomLevel_triggered();
	
	void on_actionChangeTarget_triggered();
	void on_actionChoosePort_triggered();
	
	void on_actionScreenGrab_triggered();
	void on_actionRequestFile_triggered();
	
	void on_actionToggleBreakpoint_triggered(bool checked);
	void on_actionClearBreakpoints_triggered();
	
	void on_ui_editor_cursorPositionChanged(int line, int index);
	
	void on_ui_next_clicked();
	void on_ui_find_textChanged(const QString& text);
	void on_ui_matchCase_stateChanged(int state);
	void on_ui_replaceNext_clicked();
	void on_ui_replaceAll_clicked();
private:
	void showFind();
	bool checkPort();
	
	QFile m_fileHandle;
	QFileInfo m_fileInfo;
	bool m_isNewFile;
	int m_zoomLevel;
        void dropEvent(QDropEvent *event);
	Target m_target;
	LexerSpec* m_lexSpec;
	QString m_lexAPI;
	QString m_targetName;
	QString m_templateExt;
	
	bool m_debugger;
	
	// These are handles for the markers shown in the margin
	int m_errorIndicator;
	int m_warningIndicator;
	int m_breakIndicator;
	
	QList<int> m_breakpoints;
	
	int m_currentLine;
	QWidget* m_runTab;
	bool m_findModified;
	
	void clearProblems();
	void markProblems(const QStringList& errors, const QStringList& warnings);
	void updateErrors();
	void updateBreakpointToggle();
	bool changeTarget(bool _template = true);

protected:
	void keyPressEvent(QKeyEvent *event);
};

#endif
