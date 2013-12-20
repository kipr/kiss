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
#include "template_manager.hpp"
#include "template_dialog.hpp"
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
#include <scintex/line_numbers_view.hpp>
#include <scintex/basic_input_controller.hpp>

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
{
  ui->setupUi(this);
  ui->find->setSourceFile(this);
  ui->editor->addMarginView(scintex::TextView::Left, new scintex::LineNumbersView);
  ui->editor->setInputController(new scintex::BasicInputController);
  ui->editor->setFont(QFont("Menlo", 12));
  ui->editor->setTextMargins(QMargins(5, 5, 5, 5));
  ui->editor->setModel(&_model);
  
  // ui_editor->setEolMode(QsciScintilla::EolUnix);
  // ui_editor->setWrapMode(QsciScintilla::WrapWord);
	
	mainWindow()->setStatusMessage("");
	
  // connect(ui_editor, SIGNAL(textChanged()), this, SLOT(updateMargins()));
  // connect(ui_editor, SIGNAL(modificationChanged(bool)), this, SLOT(sourceModified(bool)));
	
	connect(&target::CommunicationManager::ref(), SIGNAL(admitted(CommunicationEntryPtr)), SIGNAL(updateActivatable()));
	connect(&target::CommunicationManager::ref(), SIGNAL(queueFinished()), SIGNAL(updateActivatable()));
	
	ui->find->hide();
	
	refreshSettings();
		
	setName(tr("Untitled"));
}

SourceFile::~SourceFile()
{
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
	if(!hasFile()) {
		if(!selectTemplate()) return false;
	} else updateLexer();

	return true;
}

void SourceFile::completeSetup()
{
	updateTitle();
	updateMargins();
}

bool SourceFile::close()
{
	// if(!ui->editor->isModified()) return true;
	
	QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Unsaved Changes"),
		tr("Save changes to \"%1\" before closing?").arg(file().fileName()),
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

	if(ret == QMessageBox::Cancel) return false;
	
  if(ret == QMessageBox::Yes) {
    save();
    // TODO: Save didn't work
    // if(ui_editor->isModified()) return false;
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
	
	// ui->editor->convertEols(QsciScintilla::EolUnix);
  // TODO: Append newline
	/*if(ui_editor->text(ui_editor->lines() - 1).length() > 0) {
		ui_editor->append("\n");
	}*/
	
	setFile(filePath);
	
	QFile fileHandle(file().filePath());
	if(!fileHandle.open(QIODevice::WriteOnly)) return false;
	QTextStream fileStream(&fileHandle);
	fileStream << _model.backing();
	fileHandle.close();
	
	// ui_editor->setModified(false);
	
	updateLexer();
	
	return true;
}

bool SourceFile::fileOpen(const QString &filePath)
{
	setFile(filePath);
	
	QFile fileHandle(file().filePath());
	
	if(!fileHandle.open(QIODevice::ReadOnly)) return false;

  QTextStream fileStream(&fileHandle);
  _model.setBacking(fileStream.readAll());
  qDebug() << "Model setBacking";
  fileHandle.close();

	updateLexer();
		
	return true;
}

bool SourceFile::memoryOpen(const QByteArray &ba, const QString &assocPath)
{
  setFile(assocPath);
  _model.setBacking(ba);
  updateLexer();
  return true;
}

void SourceFile::indentAll()
{
	/*if(!m_currentLexer || !m_currentLexer->cStyleBlocks()) return;
	
	setUpdatesEnabled(false);

	int indentLevel = 0;
	int blockStartStyle = 0;
	ui_editor->lexer()->blockStart(&blockStartStyle);
	int blockEndStyle = 0;
	ui_editor->lexer()->blockEnd(&blockEndStyle);
	
	int currentLine = 0;
	int currentIndex = 0;

	ui_editor->getCursorPosition(&currentLine, &currentIndex);
	ui_editor->append(" ");

	QString outDocument;

	for(int i = 0, pos = 0; i < ui_editor->lines(); ++i) {
		// Get the current line of text and iterate through it looking for blockStart/End chars
		QString line = ui_editor->text(i);
		int blockStartCount = 0, blockEndCount = 0;

		for(int j = 0;j < line.length();j++,pos++) {
			int style = ui_editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos);
			//Increase the indentLevel on blockStart
			if(style == blockStartStyle && line.at(j) == *ui_editor->lexer()->blockStart())
				blockStartCount++;
			//Decrease the indentLevel on blockEnd
			if(style == blockEndStyle && line.at(j) == *ui_editor->lexer()->blockEnd())
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
		if((ui_editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos - 1) == ui_editor->lexer()->defaultStyle() ||
			ui_editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos - 1) 
				!= ui_editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos)) || indentLevel)
			outDocument += QString(line).replace(QRegExp("^[ \\t]*"), QString(indentLevel,'\t'));
		else
			outDocument += line;
		indentLevel += blockStartCount-blockEndCount;
	}
	
	outDocument.chop(1);
	ui_editor->setText(outDocument + "\n");
	
	ui_editor->setCursorPosition(currentLine, currentIndex);
	
	setUpdatesEnabled(true);*/
}

void SourceFile::keyPressEvent(QKeyEvent *event)
{
  /*#ifdef Q_OS_MAC
	int ctrlMod = Qt::MetaModifier;
#else
	int ctrlMod = Qt::ControlModifier;
#endif
	
	if(!(event->modifiers() &ctrlMod)) return;
	
	int line, index;
	ui_editor->getCursorPosition(&line, &index);
	
	switch(event->key()) {
	case Qt::Key_A:
		ui_editor->setCursorPosition(line, 0);
		break;
	case Qt::Key_E:
		if(ui_editor->lines() - 1 == line)
			ui_editor->setCursorPosition(line, ui_editor->lineLength(line));
		else ui_editor->setCursorPosition(line, ui_editor->lineLength(line) - 1);
		break;
	case Qt::Key_P:
		if(ui_editor->lineLength(line - 1) < index)
			ui_editor->setCursorPosition(line - 1, ui_editor->lineLength(line - 1) - 1);
		else ui_editor->setCursorPosition(line - 1, index);
		break;
	case Qt::Key_N:
		if(ui_editor->lineLength(line + 1) < index) {
			if(ui_editor->lines() - 1 == line + 1)
				ui_editor->setCursorPosition(line + 1, ui_editor->lineLength(line));
			else
				ui_editor->setCursorPosition(line + 1, ui_editor->lineLength(line + 1) - 1);
		} else ui_editor->setCursorPosition(line + 1, index);
		break;
	case Qt::Key_B:
		if(index - 1 < 0) ui_editor->setCursorPosition(line - 1, ui_editor->lineLength(line - 1) - 1);
		else ui_editor->setCursorPosition(line, index - 1);
		break;
	case Qt::Key_F:
		if(ui_editor->lineLength(line) < index + 1)
			ui_editor->setCursorPosition(line + 1, 0);
		else ui_editor->setCursorPosition(line, index + 1);
		break;
	case Qt::Key_D:
		ui_editor->setSelection(line, index, line, index + 1);
		ui_editor->removeSelectedText();
		break;
	case Qt::Key_K:
		ui_editor->setSelection(line, index, line, ui_editor->lineLength(line) - 1);
		ui_editor->cut();
		break;
	case Qt::Key_Y:
		ui_editor->paste();
		break;
	}
	event->accept();*/
}

void SourceFile::refreshSettings()
{
	/*QSettings settings;
	settings.beginGroup(EDITOR);
	if(ui_editor->lexer()) {
		QColor backgroundColor = settings.value(BACKGROUND_COLOR, QColor(255, 255, 255)).value<QColor>();
		ui_editor->lexer()->setPaper(backgroundColor);
		ui_editor->lexer()->setDefaultPaper(backgroundColor);
	}
	
	QFont defFont(settings.value(FONT).toString(), settings.value(FONT_SIZE).toInt());
	if(ui_editor->lexer()) {
		ui_editor->lexer()->setFont(defFont, -1);
	}
	
	lexer::Factory::ref().setFont(defFont);


	settings.beginGroup(AUTO_INDENT);
	ui_editor->setAutoIndent(settings.value(ENABLED).toBool());
	
	if(ui_editor->lexer()) {
		ui_editor->lexer()->setAutoIndentStyle(settings.value(STYLE).toString() == MAINTAIN ? QsciScintilla::AiMaintain : 0);
	}
	
	ui_editor->setTabWidth(settings.value(WIDTH).toInt());
	settings.endGroup();

	settings.beginGroup(AUTO_COMPLETION);
	ui_editor->setAutoCompletionSource(QsciScintilla::AcsNone);
	if(settings.value(ENABLED).toBool()) {
		if(settings.value(API_SOURCE).toBool()) ui_editor->setAutoCompletionSource(QsciScintilla::AcsAPIs);
		if(settings.value(DOC_SOURCE).toBool()) {
			if(ui_editor->autoCompletionSource() == QsciScintilla::AcsAPIs) {
				ui_editor->setAutoCompletionSource(QsciScintilla::AcsAll);
			} else {
				ui_editor->setAutoCompletionSource(QsciScintilla::AcsDocument);
			}
		}
	}
	ui_editor->setAutoCompletionThreshold(settings.value(THRESHOLD).toInt());
	settings.endGroup();

	ui_editor->setMarginLineNumbers(0, settings.value(LINE_NUMBERS).toBool());
	ui_editor->setMarginLineNumbers(1, false);
	
	
	ui_editor->setBraceMatching(settings.value(BRACE_MATCHING).toBool() ? QsciScintilla::StrictBraceMatch : 
		QsciScintilla::NoBraceMatch);
	
	ui_editor->setCallTipsStyle(settings.value(CALL_TIPS).toBool() ? QsciScintilla::CallTipsNoContext : 
		QsciScintilla::CallTipsNone);
		
	m_debuggerEnabled = settings.value(DEBUGGER_ENABLED).toBool();

	settings.endGroup();

	ui_editor->setLexer(ui_editor->lexer());
	
	updateMargins();
	ui_editor->setMarginsBackgroundColor(QColor(Qt::white));
	ui_editor->setMarginsForegroundColor(QColor(185, 185, 185));*/
}

void SourceFile::updateMargins()
{
}

int SourceFile::getZoom()
{
	return 0;
}

void SourceFile::moveTo(const int &line, const int &pos)
{
	// if(line > 0 && pos >= 0) ui_editor->setCursorPosition(line - 1, pos);
}

int SourceFile::currentLine() const
{
	return m_currentLine;
}

bool SourceFile::breakpointOnLine(int line) const
{
	bool markerOnLine = false;
	// foreach(const int &i, m_breakpoints)
  //  markerOnLine |= (ui_editor->markerLine(i) == m_currentLine);
	return markerOnLine;
}

void SourceFile::zoomIn()
{
	// ui_editor->zoomIn();
	updateMargins();
}

void SourceFile::zoomOut()
{
	// ui_editor->zoomOut();
	updateMargins();
}

void SourceFile::zoomReset()
{
	// ui_editor->zoomTo(0);
	updateMargins();
}

bool SourceFile::saveAs()
{
	return hasProject() ? saveAsProject() : saveAsFile();
}

bool SourceFile::saveAsFile()
{
	QSettings settings;
	QStringList exts;
	
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
	
	QStringList exts;
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
	// ui_editor->copy();
}

void SourceFile::cut()
{
	// ui_editor->cut();
}

void SourceFile::paste()
{
	// ui_editor->paste();
}

void SourceFile::undo()
{
	// ui_editor->undo();
}

void SourceFile::redo()
{
	// ui_editor->redo();
}

void SourceFile::find()
{
	showFind();
}

void SourceFile::print()
{
  // TODO: Printing
}

void SourceFile::convertToProject()
{
	if(!save()) return;
	project::ProjectPtr project = mainWindow()->newProject();
	fileSaveAs(project->location() + "/" + file().fileName());
	// project->setTarget(target());
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
	/*if(checked) m_breakpoints.append(ui_editor->markerAdd(m_currentLine, m_breakIndicator));
 	else {
		// m_breakpoints.removeAll(ui_editor->markerLine(m_currentLine));
		// ui_editor->markerDelete(m_currentLine, m_breakIndicator);
	}*/
	
	emit updateActivatable();
}

void SourceFile::clearBreakpoints()
{	
	// foreach(const int &i, m_breakpoints) ui_editor->markerDeleteHandle(i);
	// m_breakpoints.clear();
	
	emit updateActivatable();
}

void SourceFile::on_ui_editor_cursorPositionChanged(int line, int)
{
	m_currentLine = line;
	emit updateActivatable();
}

void SourceFile::showFind()
{
	ui->find->show();
}

void SourceFile::dropEvent(QDropEvent *)
{
}

void SourceFile::clearProblems()
{
	// ui_editor->markerDeleteAll(m_errorIndicator);
	// ui_editor->markerDeleteAll(m_warningIndicator);
}

void SourceFile::markProblems(const Lines &lines)
{
	foreach(const Lines::line_t &line, lines.warningLines()) {
		// ui_editor->markerAdd(line, m_warningIndicator);
	}
	foreach(const Lines::line_t &line, lines.errorLines()) {
		// ui_editor->markerAdd(line, m_errorIndicator);
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

const bool SourceFile::selectTemplate()
{
	dialog::Template tDialog(mainWindow()->templateManager(), this);
	if(tDialog.exec() == QDialog::Rejected) return false;

	templates::File tFile = tDialog.file();
	
	_model.setBacking(tFile.resolvedData());
	
	// m_lexAPI = QString(targetPath).replace(QString(".") + TARGET_EXT, ".api");
	
	// if(constructor) setLexer(constructor);
	
	refreshSettings();
	
	return true;
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
  // TODO: Modified *
  mainWindow()->setTabName(this, fullName());
}

void SourceFile::updateLexer()
{
	// Update the lexer to the new spec for that extension
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
	archive->addFile(file().fileName(), _model.backing().toUtf8());
	Compiler::OutputList out = LanguageHelperManager::ref().preprocess(archive,
    QStringList() << file().path());
	// TODO: I really don't like that this is here.
	if(Compiler::Output::isSuccess(out)) return archive;
	mainWindow()->setOutputList(out);
	return kiss::KarPtr();
}
