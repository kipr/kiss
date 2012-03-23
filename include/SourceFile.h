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

#ifndef __SourceFile_H__
#define __SourceFile_H__

#include "TemplateDialog.h"
#include "ChoosePortDialog.h"
#include "EditorSettingsDialog.h"
#include "Target.h"
#include "ui_SourceFile.h"
#include "Tab.h"
#include "Debugger.h"

#include "WorkingUnit.h"

#include "SourceFindWidget.h"
#include "SourceLocalFailed.h"

#include <QtGlobal>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qsciapis.h>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QKeyEvent>
#include <QObject>
#include <QMenuBar>
#include <QToolBar>

#define UI_EVENT_FILE_SAVE "save"
#define UI_EVENT_FILE_SAVE_AS "saveAs"
#define UI_EVENT_ZOOM_IN "zoomIn"
#define UI_EVENT_ZOOM_OUT "zoomOut"
#define UI_EVENT_ZOOM_RESET "zoomReset"

#define UI_EVENT_COMPILE "compile"
#define UI_EVENT_DOWNLOAD "download"
#define UI_EVENT_SIMULATE "simulate"
#define UI_EVENT_RUN "run"
#define UI_EVENT_STOP "stop"
#define UI_EVENT_DEBUG "debug"

#define UI_EVENT_MAKE_TEMPLATE "makeTemplate"
#define UI_EVENT_MAKE_TEMPLATE2 "makeTemplate2"

#define UI_EVENT_CHANGE_PORT "changePort"
#define UI_EVENT_CHANGE_TARGET "changeTarget"

#define UI_EVENT_TEMPLATE_SELECTED "templateSelected"

class FindDialog;
class MainWindow;
class Project;

namespace Lexer { struct Constructor; }

class SourceFile : public QWidget, public TabbedWidget, public WorkingUnit, private Ui::SourceFile
{
Q_OBJECT
public:
	SourceFile(MainWindow* parent = 0);
	
	void activate();
	
	bool beginSetup();
	void completeSetup();
	
	bool fileSaveAs(const QString& filePath);
	bool fileOpen(const QString& filePath);
	bool memoryOpen(const QByteArray& ba, const QString& assocPath);
	bool openProjectFile(Project* project, const QString& path);
	
	bool close();

	//! \return File's Name
	QString fileName();
	//! \return File's Base Name. Ex: test.c -> test
	QString fileBaseName();
	//! \return File's Suffix. Ex: test.c -> c
	QString fileSuffix();
	//! \return Absolute path to current file
	QString filePath();
	//! \return true if file is new
	bool isNewFile();
	
	//! \return Current zoom level
	int getZoom();
	
	void moveTo(int line, int pos);
	
	QsciScintilla* editor();
	
	int currentLine() const;
	bool breakpointOnLine(int line) const;
	
	void setAssociatedProject(Project* project);
	Project* associatedProject() const;
	bool isAssociatedWithProject() const;
	
public slots:
	bool saveAs();

	void copy();
	void cut();
	void paste();
	void undo();
	void redo();
	void find();
	void print();
	
	bool changeTarget(bool skipIfValid, bool _template = false);
	void choosePort();

	void download();
	void compile();
	void run();
	void stop();
	void simulate();
	void debug();
	void screenGrab();
	void requestFile();

	void makeTemplate();

	void toggleBreakpoint(bool checked);
	void clearBreakpoints();

	void indentAll();
	void refreshSettings();
	void updateMargins();

	//! Increase editor's zoom level
	void zoomIn();
	//! Decreate editor's zoom level
	void zoomOut();
	
	void zoomReset();
	
	//! Save current file
	bool save();

	void sourceModified(bool m);

signals:
	void updateActivatable();

private slots:
	void on_ui_editor_cursorPositionChanged(int line, int index);
	
private:
	void showFind();
	bool checkPort();
	
	void setLexer(Lexer::Constructor* constructor);
	
	QFile m_fileHandle;
	QFileInfo m_fileInfo;
	bool m_isNewFile;
	int m_zoomLevel;
        void dropEvent(QDropEvent *event);

	QString m_lexAPI;
	QString m_targetName;
	QString m_templateExt;
	
	bool m_debuggerEnabled;
	
	// These are handles for the markers shown in the margin
	int m_errorIndicator;
	int m_warningIndicator;
	int m_breakIndicator;
	
	QList<int> m_breakpoints;
	
	int m_currentLine;
	QWidget* m_runTab;
	
	//QsciAPIs m_apis;
	
	void clearProblems();
	void markProblems(const QStringList& errors, const QStringList& warnings);
	void updateErrors();
	
	Debugger m_debugger;

	Project* m_associatedProject;
protected:
	void keyPressEvent(QKeyEvent *event);
};

#endif
