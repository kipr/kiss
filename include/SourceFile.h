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
#include "LexerSpec.h"
#include "ChooseTargetDialog.h"
#include "ChoosePortDialog.h"
#include "EditorSettingsDialog.h"
#include "TargetSettingsDialog.h"
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
	~SourceFile();
	
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

	QString fileName();
	QString filePath();
	bool isNewFile();
	
	int getZoom();
	
	QsciScintilla* getEditor();
	
	void moveTo(int line, int pos);

public slots:
	void indentAll();
	void refreshSettings();
	void updateMargins();

	void zoomIn();
	void zoomOut();

	void on_actionSaveAs_triggered();
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
	
	void on_actionManual_triggered();
	void on_actionPrint_triggered();
	
	void on_actionZoomIn_triggered();
	void on_actionZoomOut_triggered();
	void on_actionResetZoomLevel_triggered();
	
	void on_actionChangeTarget_triggered();
	void on_actionChoosePort_triggered();
	
	void on_actionToggleBreakpoint_triggered(bool checked);
	
	void on_ui_editor_cursorPositionChanged(int line, int index);
private:
	QFile m_fileHandle;
	QFileInfo m_fileInfo;
	bool m_isNewFile;
	int m_zoomLevel;
        void dropEvent(QDropEvent *event);
	Target m_target;
	LexerSpec* m_lexSpec;
	QString m_lexAPI;
	int m_breakpointMarker;
	
	// These are handles for the markers shown in the margin
	int m_errorIndicator;
	int m_warningIndicator;
	int m_breakIndicator;
	
	QList<int> m_breakpoints;
	
	int m_currentLine;
	
	void clearProblems();
	void markProblems(const QStringList& errors, const QStringList& warnings);
	void updateErrors();
	void updateBreakpointToggle();

protected:
	void keyPressEvent(QKeyEvent *event);
};

#endif
