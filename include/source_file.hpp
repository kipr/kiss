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

#ifndef _SOURCE_FILE_HPP_
#define _SOURCE_FILE_HPP_

#include "tab.hpp"
#include "output_helper.hpp"

#include "ui_SourceFile.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qsciapis.h>
#include <QString>
#include <QFileInfo>
#include <QKeyEvent>

namespace kiss
{
	namespace project
	{
		class Project;
	}
	
	namespace lexer
	{
		class Constructor;
		class Base;
	}
	
	namespace widget
	{
		class MainWindow;

		class SourceFile : public QWidget, public Tab, private Ui::SourceFile
		{
		Q_OBJECT
		public:
			SourceFile(MainWindow *parent = 0);
			~SourceFile();

			void activate();

			bool beginSetup();
			void completeSetup();

			bool fileSaveAs(const QString &filePath);
			bool fileOpen(const QString &filePath);
			bool memoryOpen(const QByteArray &ba, const QString &assocPath);
			bool openProjectFile(const project::ProjectPtr &project);

			bool close();

			//! \return Current zoom level
			int getZoom();

			void moveTo(const int &line, const int &pos);

			QsciScintilla *editor();

			int currentLine() const;
			bool breakpointOnLine(int line) const;

		public slots:
			bool saveAs();

			void copy();
			void cut();
			void paste();
			void undo();
			void redo();
			void find();
			void print();

			void convertToProject();

			void screenGrab();
			void requestFile();

			void toggleBreakpoint(const bool &checked);
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

			void setName(const QString &name);
			const QString &name() const;
			QString fullName() const;

			virtual kiss::KarPtr archive() const;

		signals:
			void updateActivatable();
			
		protected:
			void keyPressEvent(QKeyEvent *event);
			virtual void fileChanged(const QFileInfo &file);
			virtual void projectChanged(const project::ProjectPtr &project);

		private slots:
			void on_ui_editor_cursorPositionChanged(int line, int index);
			
		private:
			bool saveAsFile();
			bool saveAsProject();
			
			const bool selectTemplate();

			void showFind();

			void setLexer(lexer::Constructor *constructor);

			int m_zoomLevel;
			void dropEvent(QDropEvent *event);

			QString m_lexAPI;
			QString m_templateExt;

			bool m_debuggerEnabled;

			// These are handles for the markers shown in the margin
			int m_errorIndicator;
			int m_warningIndicator;
			int m_breakIndicator;

			QList<int> m_breakpoints;

			int m_currentLine;
			QWidget *m_runTab;

			void clearProblems();
			void markProblems(const Lines &lines);
			void updateErrors(const Compiler::OutputList &compileResult);
			
			void updateTitle();
			void updateLexer();
			
			kiss::lexer::Base *m_currentLexer;

			QString m_name;
		};
	}
}


#endif
