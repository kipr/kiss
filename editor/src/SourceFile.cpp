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

#include "SourceFile.h"
#include <Qsci/qscilexercpp.h>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QDir>
#include <QSettings>
#include <QFont>
#include <math.h>

SourceFile::SourceFile(QWidget *parent) : QsciScintilla(parent), m_fileHandle("Untitled"), m_isNewFile(true)
{
	setModified(false);
	m_fileInfo.setFile(m_fileHandle);
	m_statusMessage="";
	
	setEolMode(QsciScintilla::EolUnix);
	
	connect(this, SIGNAL(textChanged()), this, SLOT(updateMargins()));
	
	refreshSettings();
}

SourceFile::~SourceFile()
{
}

bool SourceFile::fileSave()
{
	return fileSaveAs(m_fileInfo.filePath());
}

bool SourceFile::fileSaveAs(QString filePath)
{
	if(filePath.isEmpty())
		return false;
	
	m_fileHandle.setFileName(filePath);
	m_fileInfo.setFile(m_fileHandle);
	
	if(!m_fileHandle.open(QIODevice::WriteOnly))
		return false;

	if(text(lines()-1).length() > 0)
		append("\n");
		
	convertEols(QsciScintilla::EolUnix);

	QTextStream fileStream(&m_fileHandle);
	fileStream << text();
	m_fileHandle.close();
	setModified(false);

	m_isNewFile = false;
	
	return true;
}

bool SourceFile::fileOpen(QString filePath)
{
	m_fileHandle.setFileName(filePath);
	m_fileInfo.setFile(m_fileHandle);	
	
	if(!m_fileHandle.open(QIODevice::ReadOnly))
		return false;

	QTextStream fileStream(&m_fileHandle);
	setText(fileStream.readAll());
	m_fileHandle.close();

	setModified(false);

	m_isNewFile = false;

	return true;
}

void SourceFile::indentAll()
{
	if(!lexer())
		return;

	int indentLevel = 0;
	int blockStartStyle; lexer()->blockStart(&blockStartStyle);
	int blockEndStyle; lexer()->blockEnd(&blockEndStyle);
	
	int currentLine;
	int currentIndex;

	getCursorPosition(&currentLine,&currentIndex);
	append(" ");

	QString outDocument;

	for(int i = 0, pos =0;i < lines();i++) {
		// Get the current line of text and iterate through it looking for blockStart/End chars
		QString line = text(i);
		int blockStartCount = 0, blockEndCount = 0;

		for(int j = 0;j < line.length();j++,pos++) {
			int style = SendScintilla(SCI_GETSTYLEAT, pos);
			//Increase the indentLevel on blockStart
			if(style == blockStartStyle && line.at(j) == *lexer()->blockStart())
				blockStartCount++;
			//Decrease the indentLevel on blockEnd
			if(style == blockEndStyle && line.at(j) == *lexer()->blockEnd())
				blockEndCount++;
		}
		//The logic here is confusing, but the idea is either a newline is
		//	of the default style (I can't think of a case other than a comment in which it would not be)
		//	or it is a comment, but the end of a one line comment so the next line would be a different style
		//  also, if the indentLevel > 0, we should indent anyway
		if(blockEndCount > blockStartCount) {
			indentLevel -= blockEndCount-blockStartCount;
			blockEndCount = blockStartCount = 0;
		}
		if((SendScintilla(SCI_GETSTYLEAT, pos-1) == lexer()->defaultStyle() ||
			SendScintilla(SCI_GETSTYLEAT, pos-1) != SendScintilla(SCI_GETSTYLEAT, pos)) || indentLevel)
			outDocument += QString(line).replace(QRegExp("^[ \\t]*"), QString(indentLevel,'\t'));
		else
			outDocument += line;
		indentLevel += blockStartCount-blockEndCount;
	}
	
	outDocument.chop(1);
	setText(outDocument);
	
	setCursorPosition(currentLine, currentIndex);
}

QString SourceFile::statusMessage()
{
	return m_statusMessage;
}

void SourceFile::setStatusMessage(QString message)
{
	m_statusMessage = message;
}

void SourceFile::keyPressEvent(QKeyEvent *event)
{
	int ctrlMod;
	
#ifdef Q_OS_MAC
	ctrlMod = Qt::MetaModifier;
#else
	ctrlMod = Qt::ControlModifier;
#endif
	
	if(event->modifiers() & ctrlMod) {
		int line, index;
		getCursorPosition(&line, &index);
		
		switch(event->key()) {
			case Qt::Key_A:
				setCursorPosition(line, 0);
				break;
			case Qt::Key_E:
				if(lines()-1 == line)
					setCursorPosition(line, lineLength(line));
				else
					setCursorPosition(line, lineLength(line)-1);
				break;
			case Qt::Key_P:
				if(lineLength(line-1) < index)
					setCursorPosition(line-1, lineLength(line-1)-1);
				else
					setCursorPosition(line-1, index);
				break;
			case Qt::Key_N:
				if(lineLength(line+1) < index) {
					if(lines()-1 == line+1)
						setCursorPosition(line+1, lineLength(line));
					else
						setCursorPosition(line+1, lineLength(line+1)-1);
				}
				else
					setCursorPosition(line+1, index);
				break;
			case Qt::Key_B:
				if(index-1 < 0)
					setCursorPosition(line-1, lineLength(line-1)-1);
				else
					setCursorPosition(line, index-1);
				break;
			case Qt::Key_F:
				if(lineLength(line) < index+1)
					setCursorPosition(line+1, 0);
				else
					setCursorPosition(line, index+1);
				break;
			case Qt::Key_D:
				setSelection(line, index, line, index+1);
				removeSelectedText();
				break;
			case Qt::Key_K:
				setSelection(line, index, line, lineLength(line)-1);
				cut();
				break;
			case Qt::Key_Y:
				paste();
				break;
			default:
				QsciScintilla::keyPressEvent(event);
				return;
		}
		event->accept();
	}
	else
		QsciScintilla::keyPressEvent(event);
}

void SourceFile::refreshSettings()
{
	/* Get rid of the old lexer */
	QsciLexer *lexer = QsciScintilla::lexer();
	setLexer(0); delete lexer; lexer = 0;
	
	/* Get a new lexer if a spec is defined */
	if(Lexer::hasLexerSpec()) lexer = new Lexer();
	
	/* Read font, indent, margin, etc. settings */
	QSettings settings;
	settings.beginGroup("Editor");

	/* Set the default font from settings */
	QFont defFont(settings.value("font").toString(), settings.value("fontsize").toInt());
	
	if(lexer) {
		lexer->defaultColor(0);
		lexer->setDefaultFont(defFont);
	}
	else
		setFont(defFont);

	/* Set other options from settings */
	settings.beginGroup("autoindent");
	setAutoIndent(settings.value("enabled").toBool());
	if(lexer && settings.value("style").toString() == "Maintain")
		lexer->setAutoIndentStyle(AiMaintain);
	else if(lexer && settings.value("style").toString() == "Intelligent")
		lexer->setAutoIndentStyle(0);  // for some reason 0 is the intelligent style
	setTabWidth(settings.value("width").toInt());
	settings.endGroup();

	settings.beginGroup("autocompletion");
	setAutoCompletionSource(AcsNone);
	if(settings.value("enabled").toBool()) {
		if(settings.value("apisource").toBool())
			setAutoCompletionSource(AcsAPIs);
		if(settings.value("docsource").toBool()) {
			if(autoCompletionSource() == AcsAPIs)
				setAutoCompletionSource(AcsAll);
			else
				setAutoCompletionSource(AcsDocument);
		}
	}
	setAutoCompletionThreshold(settings.value("threshold").toInt());
	settings.endGroup();

	if(settings.value("linenumbers").toBool()){
		setMarginLineNumbers(1, true);
		updateMargins();
	}
	else setMarginLineNumbers(1, false);
	
	if(settings.value("bracematching").toBool())
		 setBraceMatching(QsciScintilla::StrictBraceMatch);
	else setBraceMatching(QsciScintilla::NoBraceMatch);
	
	if(settings.value("calltips").toBool())
		 setCallTipsStyle(CallTipsNoContext);
	else setCallTipsStyle(CallTipsNone);

	settings.endGroup();

	setLexer(lexer);
}

QString SourceFile::fileName()
{
	return m_fileInfo.fileName();
	
}

QString SourceFile::filePath()
{
	return m_fileInfo.filePath();
}

bool SourceFile::isNewFile()
{
	return m_isNewFile;
}

void SourceFile::updateMargins()
{
	int charWidth = 5;
	if(lexer()) {
		QFont font = lexer()->defaultFont();
		font.setPointSize(font.pointSize() + getZoom());
		charWidth = QFontMetrics(font).width("0");
	}
	setMarginWidth(1, charWidth/2 + charWidth * (int)ceil(log10(lines()+1)));
}

int SourceFile::getZoom()
{
	return SendScintilla(SCI_GETZOOM);
}

void SourceFile::zoomIn()
{
	updateMargins();
	QsciScintilla::zoomIn();
}

void SourceFile::zoomOut()
{
	updateMargins();
	QsciScintilla::zoomOut();
}
/*ADDED BY NB*///2/10/2010-dpm
void SourceFile::dropEvent(QDropEvent *event)
{
    /*Emit signal that can be connected to any slot(s) desired*/
   emit handleDrop(event);

}

