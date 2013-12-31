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

#include "source_file.hpp"
#include "ui_SourceFile.h"

#include "main_window.hpp"
#include "lexer_factory.hpp"
#include "password_dialog.hpp"
#include "save_as_dialog.hpp"
#include "source_file_menu.hpp"
#include "communication_manager.hpp"
#include "language_helper_manager.hpp"
 #include "file_utils.hpp"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QRegExp>
#include <QDir>
#include <QSettings>
#include <QFont>
#include <QPrintDialog>
#include <QDebug>
#include <math.h>
#include <Qsci/qsciprinter.h>

#include <memory>

#define DEFAULT_EXTENSION "default_extension"

using namespace kiss;
using namespace kiss::widget;

SourceFile::SourceFile(MainWindow *parent)
  : QWidget(parent)
  , Tab(this, parent)
  , ui(new Ui::SourceFile)
  , m_debuggerEnabled(false)
  , m_runTab(0)
  , m_currentLexer(0)
{
  ui->setupUi(this);
  
  ui->find->setSourceFile(this);
  
  m_errorIndicator = ui->editor->markerDefine(ResourceHelper::ref().pixmap("bullet_red"));
  m_warningIndicator = ui->editor->markerDefine(ResourceHelper::ref().pixmap("bullet_yellow"));
  m_breakIndicator = ui->editor->markerDefine(ResourceHelper::ref().pixmap("bullet_blue"));
  
  ui->editor->setModified(false);
  ui->editor->setEolMode(QsciScintilla::EolUnix);
  ui->editor->setWrapMode(QsciScintilla::WrapWord);
	
	mainWindow()->setStatusMessage("");
	
  connect(ui->editor, SIGNAL(textChanged()), this, SLOT(updateMargins()));
  connect(ui->editor, SIGNAL(modificationChanged(bool)), this, SLOT(sourceModified(bool)));
	
	connect(&target::CommunicationManager::ref(), SIGNAL(admitted(CommunicationEntryPtr)), SIGNAL(updateActivatable()));
	connect(&target::CommunicationManager::ref(), SIGNAL(queueFinished()), SIGNAL(updateActivatable()));
	
	ui->find->hide();
	
	refreshSettings();
		
	setName(tr("Untitled"));
}

SourceFile::~SourceFile()
{
  setLexer(0);
  delete ui;
}

void SourceFile::activate()
{
	mainWindow()->setStatusMessage("");
	
	mainWindow()->deactivateMenuablesExcept(mainWindow()->standardMenus()
		<< menu::SourceFileMenu::menuName());
	
	mainWindow()->activateMenuable(menu::SourceFileMenu::menuName(), this);
	
	emit updateActivatable();
}

bool SourceFile::beginSetup()
{
	if(!hasFile()) return false;
	updateLexer();
	return true;
}

void SourceFile::completeSetup()
{
	updateTitle();
	updateMargins();
}

bool SourceFile::close()
{
	if(!ui->editor->isModified()) return true;
	
	QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Unsaved Changes"),
		tr("Save changes to \"%1\" before closing?").arg(file().fileName()),
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

	if(ret == QMessageBox::Cancel) return false;
	
  if(ret == QMessageBox::Yes) {
    save();
    if(ui->editor->isModified()) return false;
	}
	
	mainWindow()->activateMenuable(menu::SourceFileMenu::menuName(), 0);

	emit updateActivatable();
	
	return true;
}

bool SourceFile::save()
{
	return hasFile() ? fileSaveAs(file().absoluteFilePath()) : saveAs();
}

bool SourceFile::fileSaveAs(const QString &filePath)
{
	if(filePath.isEmpty()) return false;
	
	ui->editor->convertEols(QsciScintilla::EolUnix);
	if(ui->editor->text(ui->editor->lines() - 1).length() > 0) {
		ui->editor->append("\n");
	}
	
	setFile(filePath);
	
	QFile fileHandle(file().filePath());
	if(!fileHandle.open(QIODevice::WriteOnly)) return false;
	QTextStream fileStream(&fileHandle);
	fileStream << ui->editor->text();
	fileHandle.close();
	
	ui->editor->setModified(false);
	
	updateLexer();
	
	return true;
}

bool SourceFile::fileOpen(const QString &filePath)
{
	setFile(filePath);
	
	QFile fileHandle(file().filePath());
	
	if(!fileHandle.open(QIODevice::ReadOnly)) return false;

  QTextStream fileStream(&fileHandle);
  ui->editor->setText(fileStream.readAll());
  fileHandle.close();
  
  ui->editor->setModified(false);

	updateLexer();
		
	return true;
}

bool SourceFile::memoryOpen(const QByteArray &ba, const QString &assocPath)
{
  setFile(assocPath);
  
  ui->editor->setText(ba);
  ui->editor->setModified(false);
  
  updateLexer();
  
  return true;
}

void SourceFile::indentAll()
{
	if(!m_currentLexer || !m_currentLexer->cStyleBlocks()) return;
	
	setUpdatesEnabled(false);

	int indentLevel = 0;
	int blockStartStyle = 0;
	ui->editor->lexer()->blockStart(&blockStartStyle);
	int blockEndStyle = 0;
	ui->editor->lexer()->blockEnd(&blockEndStyle);
	
	int currentLine = 0;
	int currentIndex = 0;

	ui->editor->getCursorPosition(&currentLine, &currentIndex);
	ui->editor->append(" ");

	QString outDocument;

	for(int i = 0, pos = 0; i < ui->editor->lines(); ++i) {
		// Get the current line of text and iterate through it looking for blockStart/End chars
		QString line = ui->editor->text(i);
		int blockStartCount = 0, blockEndCount = 0;

		for(int j = 0;j < line.length();j++,pos++) {
			int style = ui->editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos);
			//Increase the indentLevel on blockStart
			if(style == blockStartStyle && line.at(j) == *ui->editor->lexer()->blockStart())
				blockStartCount++;
			//Decrease the indentLevel on blockEnd
			if(style == blockEndStyle && line.at(j) == *ui->editor->lexer()->blockEnd())
				blockEndCount++;
		}
		//The logic here is confusing, but the idea is either a newline is
		// of the default style (I can't think of a case other than a comment in which it would not be)
		// or it is a comment, but the end of a one line comment so the next line would be a different style
		// also, if the indentLevel > 0, we should indent anyway
		if(blockEndCount > blockStartCount) {
			indentLevel -= blockEndCount-blockStartCount;
			blockEndCount = blockStartCount = 0;
		}
		if((ui->editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos - 1) == ui->editor->lexer()->defaultStyle() ||
			ui->editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos - 1) 
				!= ui->editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos)) || indentLevel)
			outDocument += QString(line).replace(QRegExp("^[ \\t]*"), QString(indentLevel,'\t'));
		else
			outDocument += line;
		indentLevel += blockStartCount-blockEndCount;
	}
	
	outDocument.chop(1);
	ui->editor->setText(outDocument + "\n");
	
	ui->editor->setCursorPosition(currentLine, currentIndex);
	
	setUpdatesEnabled(true);
}

void SourceFile::keyPressEvent(QKeyEvent *event)
{
#ifdef Q_OS_MAC
	int ctrlMod = Qt::MetaModifier;
#else
	int ctrlMod = Qt::ControlModifier;
#endif
	
	if(!(event->modifiers() &ctrlMod)) return;
	
	int line, index;
	ui->editor->getCursorPosition(&line, &index);
	
	switch(event->key()) {
	case Qt::Key_A:
		ui->editor->setCursorPosition(line, 0);
		break;
	case Qt::Key_E:
		if(ui->editor->lines() - 1 == line)
			ui->editor->setCursorPosition(line, ui->editor->lineLength(line));
		else ui->editor->setCursorPosition(line, ui->editor->lineLength(line) - 1);
		break;
	case Qt::Key_P:
		if(ui->editor->lineLength(line - 1) < index)
			ui->editor->setCursorPosition(line - 1, ui->editor->lineLength(line - 1) - 1);
		else ui->editor->setCursorPosition(line - 1, index);
		break;
	case Qt::Key_N:
		if(ui->editor->lineLength(line + 1) < index) {
			if(ui->editor->lines() - 1 == line + 1)
				ui->editor->setCursorPosition(line + 1, ui->editor->lineLength(line));
			else
				ui->editor->setCursorPosition(line + 1, ui->editor->lineLength(line + 1) - 1);
		} else ui->editor->setCursorPosition(line + 1, index);
		break;
	case Qt::Key_B:
		if(index - 1 < 0) ui->editor->setCursorPosition(line - 1, ui->editor->lineLength(line - 1) - 1);
		else ui->editor->setCursorPosition(line, index - 1);
		break;
	case Qt::Key_F:
		if(ui->editor->lineLength(line) < index + 1)
			ui->editor->setCursorPosition(line + 1, 0);
		else ui->editor->setCursorPosition(line, index + 1);
		break;
	case Qt::Key_D:
		ui->editor->setSelection(line, index, line, index + 1);
		ui->editor->removeSelectedText();
		break;
	case Qt::Key_K:
		ui->editor->setSelection(line, index, line, ui->editor->lineLength(line) - 1);
		ui->editor->cut();
		break;
	case Qt::Key_Y:
		ui->editor->paste();
		break;
	}
	event->accept();
}

void SourceFile::refreshSettings()
{
	QSettings settings;
	settings.beginGroup(EDITOR);
	if(ui->editor->lexer()) {
		QColor backgroundColor = settings.value(BACKGROUND_COLOR, QColor(255, 255, 255)).value<QColor>();
		ui->editor->lexer()->setPaper(backgroundColor);
		ui->editor->lexer()->setDefaultPaper(backgroundColor);
	}
	
	QFont defFont(settings.value(FONT).toString(), settings.value(FONT_SIZE).toInt());
	if(ui->editor->lexer()) {
		ui->editor->lexer()->setFont(defFont, -1);
	}
	
	lexer::Factory::ref().setFont(defFont);

  /* Set other options from settings */
	settings.beginGroup(AUTO_INDENT);
	ui->editor->setAutoIndent(settings.value(ENABLED).toBool());
	
	if(ui->editor->lexer()) {
		ui->editor->lexer()->setAutoIndentStyle(settings.value(STYLE).toString() == MAINTAIN ? QsciScintilla::AiMaintain : 0);
	}
	
	ui->editor->setTabWidth(settings.value(WIDTH).toInt());
	settings.endGroup();

	settings.beginGroup(AUTO_COMPLETION);
	ui->editor->setAutoCompletionSource(QsciScintilla::AcsNone);
	if(settings.value(ENABLED).toBool()) {
		if(settings.value(API_SOURCE).toBool()) ui->editor->setAutoCompletionSource(QsciScintilla::AcsAPIs);
		if(settings.value(DOC_SOURCE).toBool()) {
			if(ui->editor->autoCompletionSource() == QsciScintilla::AcsAPIs) {
				ui->editor->setAutoCompletionSource(QsciScintilla::AcsAll);
			} else {
				ui->editor->setAutoCompletionSource(QsciScintilla::AcsDocument);
			}
		}
	}
	ui->editor->setAutoCompletionThreshold(settings.value(THRESHOLD).toInt());
	settings.endGroup();

	ui->editor->setMarginLineNumbers(0, settings.value(LINE_NUMBERS).toBool());
	ui->editor->setMarginLineNumbers(1, false);
	
	
	ui->editor->setBraceMatching(settings.value(BRACE_MATCHING).toBool() ? QsciScintilla::StrictBraceMatch : 
		QsciScintilla::NoBraceMatch);
	
	ui->editor->setCallTipsStyle(settings.value(CALL_TIPS).toBool() ? QsciScintilla::CallTipsNoContext : 
		QsciScintilla::CallTipsNone);
		
	m_debuggerEnabled = settings.value(DEBUGGER_ENABLED).toBool();

	settings.endGroup();

	ui->editor->setLexer(ui->editor->lexer());
	
	updateMargins();
	ui->editor->setMarginsBackgroundColor(QColor(Qt::white));
	ui->editor->setMarginsForegroundColor(QColor(185, 185, 185));
}

void SourceFile::updateMargins()
{
  int size = 0;
  	if(ui->editor->marginLineNumbers(0)) {
  		int charWidth = 6;
  		if(ui->editor->lexer()) {
  			QFont font = ui->editor->lexer()->defaultFont();
  			font.setPointSize(font.pointSize() + getZoom());
  			charWidth = QFontMetrics(font).width("0");
  		}
  		size = charWidth + charWidth / 2
  			+ charWidth * (int)ceil(log10(std::max(ui->editor->lines(), 10) + 1));
  	}
  	ui->editor->setMarginWidth(0, size);
  	ui->editor->setMarginWidth(1, 16);
}

int SourceFile::getZoom()
{
	return ui->editor->SendScintilla(QsciScintilla::SCI_GETZOOM);
}

void SourceFile::moveTo(const int &line, const int &pos)
{
	if(line > 0 && pos >= 0) ui->editor->setCursorPosition(line - 1, pos);
}

QsciScintilla *SourceFile::editor()
{
  return ui->editor;
}

int SourceFile::currentLine() const
{
	return m_currentLine;
}

bool SourceFile::breakpointOnLine(int line) const
{
	bool markerOnLine = false;
	foreach(const int &i, m_breakpoints) markerOnLine |= (ui->editor->markerLine(i) == m_currentLine);
	return markerOnLine;
}

void SourceFile::zoomIn()
{
	ui->editor->zoomIn();
	updateMargins();
}

void SourceFile::zoomOut()
{
	ui->editor->zoomOut();
	updateMargins();
}

void SourceFile::zoomReset()
{
	ui->editor->zoomTo(0);
	updateMargins();
}

bool SourceFile::saveAs()
{
	return hasProject() ? saveAsProject() : saveAsFile();
}

bool SourceFile::saveAsFile()
{
	QSettings settings;
	QStringList exts = lexer::Factory::ref().formattedExtensions();
	
	QRegExp reg("*." + m_templateExt + "*");
	reg.setPatternSyntax(QRegExp::Wildcard);
	int i = exts.indexOf(reg);
	if(i != -1) exts.swap(0, i);
	
	QString filePath = FileUtils::getSaveFileName(mainWindow(), tr("Save File"),
		exts.join(";;") + (exts.size() < 1 ? "" : ";;") + "All Files (*)");
	if(filePath.isEmpty()) return false;

	QFileInfo fileInfo(filePath);
	QString ext = m_templateExt;
	if(!ext.isEmpty() && fileInfo.suffix().isEmpty()) {
		fileInfo.setFile(fileInfo.fileName() + "." + ext);
	}
	setFile(fileInfo.absoluteFilePath());
	
	const QString fileName = fileInfo.fileName();
	/* Saves the file with the new associatedFileName and updates the tabWidget label */
	if(fileSaveAs(fileInfo.absoluteFilePath())) {
		mainWindow()->setStatusMessage(tr("Saved file \"%1\"").arg(fileName));
	} else QMessageBox::critical(mainWindow(), "Error", "Error: Could not write file " + fileName);

	QStringList current = settings.value(RECENTS).toStringList().mid(0, 5);
	current.push_front(fileInfo.absoluteFilePath());
	current.removeDuplicates();
	settings.setValue(RECENTS, current);
	
	return true;
}

bool SourceFile::saveAsProject()
{
	if(!hasProject()) return false;
	
	QStringList exts = lexer::Factory::ref().extensions();
	QStringList nameFilters;
	foreach(const QString &ext, exts) {
		nameFilters << "*." + ext;
	}
	
	dialog::SaveAs dialog(this);
	dialog.setNameFilters(nameFilters);
	dialog.setRootPath(project()->location());
	dialog.setFileName(file().completeBaseName());
	if(dialog.exec() == QDialog::Rejected) return false;
	
	QFileInfo fileInfo(dialog.filePath());
	
	QString ext = m_templateExt;
	if(!ext.isEmpty() && fileInfo.suffix().isEmpty()) {
		fileInfo.setFile(fileInfo.fileName() + "." + ext);
	}
	
	setFile(fileInfo.absoluteFilePath());
	
	const QString fileName = fileInfo.fileName();
	if(fileSaveAs(fileInfo.absoluteFilePath())) {
		mainWindow()->setStatusMessage(tr("Saved file \"%1\"").arg(fileName));
	} else QMessageBox::critical(mainWindow(), "Error", "Error: Could not write file " + fileName);
	
	return true;
}

void SourceFile::sourceModified(bool)
{
	updateTitle();
}

void SourceFile::copy()
{
	ui->editor->copy();
}

void SourceFile::cut()
{
	ui->editor->cut();
}

void SourceFile::paste()
{
	ui->editor->paste();
}

void SourceFile::undo()
{
	ui->editor->undo();
}

void SourceFile::redo()
{
	ui->editor->redo();
}

void SourceFile::find()
{
	showFind();
}

void SourceFile::print()
{
  QsciPrinter printer;
  QPrintDialog printDialog(&printer, this);
  
  if(printDialog.exec()) printer.printRange(ui->editor);
}

void SourceFile::convertToProject()
{
	if(!save()) return;
	project::ProjectPtr project = mainWindow()->newProject();
	fileSaveAs(project->location() + "/" + file().fileName());
	//project->setTarget(target());
	setProject(project);
}

void SourceFile::screenGrab()
{
}

void SourceFile::requestFile()
{
}

void SourceFile::toggleBreakpoint(const bool &checked)
{
	if(checked) m_breakpoints.append(ui->editor->markerAdd(m_currentLine, m_breakIndicator));
 	else {
		m_breakpoints.removeAll(ui->editor->markerLine(m_currentLine));
		ui->editor->markerDelete(m_currentLine, m_breakIndicator);
	}
	
	emit updateActivatable();
}

void SourceFile::clearBreakpoints()
{	
	foreach(const int &i, m_breakpoints) ui->editor->markerDeleteHandle(i);
	m_breakpoints.clear();
	
	emit updateActivatable();
}

void SourceFile::on_editor_cursorPositionChanged(int line, int)
{
	m_currentLine = line;
	emit updateActivatable();
}

void SourceFile::showFind()
{
	ui->find->show();
}

void SourceFile::setLexer(lexer::Constructor *constructor)
{
	ui->editor->setLexer(0);
	if(m_currentLexer) {
		// TODO: Move away from QScintilla before I lose my mind.
		// delete m_currentLexer;
	}
	m_currentLexer = 0;
	if(!constructor) return;
	m_currentLexer = constructor->construct();
	ui->editor->setLexer(m_currentLexer->lexer());
	// lexer::Factory::setAPIsForLexer(m_currentLexer, m_lexAPI);
	refreshSettings();
	updateMargins();
}

void SourceFile::dropEvent(QDropEvent *)
{
}

void SourceFile::clearProblems()
{
	ui->editor->markerDeleteAll(m_errorIndicator);
	ui->editor->markerDeleteAll(m_warningIndicator);
}

void SourceFile::markProblems(const Lines &lines)
{
	foreach(const Lines::line_t &line, lines.warningLines()) {
		ui->editor->markerAdd(line, m_warningIndicator);
	}
	foreach(const Lines::line_t &line, lines.errorLines()) {
		ui->editor->markerAdd(line, m_errorIndicator);
	}
}

void SourceFile::updateErrors(const Compiler::OutputList &compileResult) 
{
	clearProblems();
	Lines lines;
	foreach(const Compiler::Output &output, compileResult) {
		lines = lines + OutputHelper::lines(output);
	}
	markProblems(lines);
}

void SourceFile::setTemplate(templates::File tFile)
{
  lexer::Constructor *constructor = 0;
  if(tFile.hasLexer()) {
  	constructor = lexer::Factory::ref().constructor(tFile.lexer());
  	m_templateExt = tFile.lexer();
  }
  ui->editor->setText(tFile.resolvedData());
	if(constructor) setLexer(constructor);
	
	refreshSettings();
}

void SourceFile::fileChanged(const QFileInfo &file)
{
	setName(file.fileName());
	updateTitle();
}

void SourceFile::projectChanged(const project::ProjectPtr &project)
{
	updateTitle();
}

void SourceFile::updateTitle()
{
  mainWindow()->setTabName(this, (ui->editor->isModified() ? "* " : "") + fullName());
}

void SourceFile::updateLexer()
{
	// Update the lexer to the new spec for that extension
	lexer::Constructor *constructor1 = lexer::Factory::ref().constructor(file().completeSuffix());
	lexer::Constructor *constructor2 = lexer::Factory::ref().constructor(file().suffix());
	lexer::Constructor *constructor = constructor1 ? constructor1 : constructor2;
	if(!lexer::Factory::isLexerFromConstructor(m_currentLexer, constructor)) setLexer(constructor);
}

void SourceFile::setName(const QString &name)
{
	m_name = name;
}

const QString &SourceFile::name() const
{
	return m_name;
}

QString SourceFile::fullName() const
{
	if(!hasProject()) return m_name;
	return m_name + " | " + project()->name();
}

const QString &SourceFile::templateExt() const
{
    return m_templateExt;
}

kiss::KarPtr SourceFile::archive() const
{
	kiss::KarPtr archive = kiss::Kar::create();
	archive->addFile(file().fileName(), ui->editor->text().toUtf8());
	Compiler::OutputList out = LanguageHelperManager::ref().preprocess(archive,
    QStringList() << file().path());
	// TODO: I really don't like that this is here.
	if(Compiler::Output::isSuccess(out)) return archive;
	mainWindow()->setOutputList(out);
	return kiss::KarPtr();
}
