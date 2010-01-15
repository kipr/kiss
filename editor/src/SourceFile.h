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

#ifndef __SourceFile_H__
#define __SourceFile_H__

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

#include "Lexer.h"
#include "LexerSpec.h"

class SourceFile : public QsciScintilla
{
Q_OBJECT
public:
	SourceFile(QWidget *parent = 0);
	~SourceFile();

	bool fileSave();
	bool fileSaveAs(QString filePath);
	bool fileOpen(QString filePath);

	QString fileName();
	QString filePath();
	bool isNewFile();
	
	QString statusMessage();
	
	int getZoom();

private:
	QFile m_fileHandle;
	QFileInfo m_fileInfo;
	QString m_statusMessage;
	bool m_isNewFile;
	int m_zoomLevel;

protected:
	void keyPressEvent(QKeyEvent *event);

public slots:
	void indentAll();
	void setStatusMessage(QString message);
	void refreshSettings();
	void updateMargins();
	
	void zoomIn();
	void zoomOut();
};

#endif
