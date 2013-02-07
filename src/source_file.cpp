/**************************************************************************
 *  Copyright 2007 - 2012 KISS Institute for Practical Robotics           *
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

#include "main_window.hpp"
#include "singleton.hpp"
#include "message_dialog.hpp"
#include "macro_string.hpp"
#include "lexer_factory.hpp"
#include "template_manager.hpp"
#include "template_dialog.hpp"
#include "password_dialog.hpp"
#include "save_as_dialog.hpp"
#include "source_file_menu.hpp"
#include "target_menu.hpp"
#include "main_window_menu.hpp"
#include "project.hpp"
#include "project_manager.hpp"
#include "log.hpp"
#include "target_dialog.hpp"
#include "interface_manager.hpp"
#include "communication_manager.hpp"
#include "resource_helper.hpp"
#include "interface.hpp"
#include "output_helper.hpp"
#include "language_helper_manager.hpp"

#include <pcompiler/pcompiler.hpp>

#include <Qsci/qscilexercpp.h>
#include <QFile>
#include <QBuffer>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QDir>
#include <QSettings>
#include <QFont>
#include <Qsci/qsciprinter.h>
#include <QPrinter>
#include <QPrintDialog>
#include <QDebug>
#include <QShortcut>
#include <math.h>
#include <QDate>
#include <QDateTime>
#include <QPixmap>
#include <QDesktopServices>
#include <QUrl>

#include <memory>

#define SAVE_PATH "savepath"
#define DEFAULT_EXTENSION "default_extension"

using namespace Kiss;
using namespace Kiss::Widget;

SourceFile::SourceFile(MainWindow *parent)
	: QWidget(parent),
	Tab(this, parent),
	Unit(),
	m_debuggerEnabled(false),
	m_runTab(0),
	m_currentLexer(0)
{
	setupUi(this);
	
	ui_find->setSourceFile(this);
	
	m_errorIndicator = ui_editor->markerDefine(ResourceHelper::ref().pixmap("bullet_red"));
	m_warningIndicator = ui_editor->markerDefine(ResourceHelper::ref().pixmap("bullet_yellow"));
	m_breakIndicator = ui_editor->markerDefine(ResourceHelper::ref().pixmap("bullet_blue"));
	
	ui_editor->setModified(false);
	ui_editor->setEolMode(QsciScintilla::EolUnix);
	ui_editor->setWrapMode(QsciScintilla::WrapWord);
	
	mainWindow()->setStatusMessage("");
	
	connect(ui_editor, SIGNAL(textChanged()), this, SLOT(updateMargins()));
	connect(ui_editor, SIGNAL(modificationChanged(bool)), this, SLOT(sourceModified(bool)));
	
	connect(&Target::CommunicationManager::ref(), SIGNAL(admitted(CommunicationEntryPtr)), SIGNAL(updateActivatable()));
	connect(&Target::CommunicationManager::ref(), SIGNAL(queueFinished()), SIGNAL(updateActivatable()));
	
	ui_find->hide();
	
	refreshSettings();
		
	setName(tr("Untitled"));
}

SourceFile::~SourceFile()
{
	setLexer(0);
}

void SourceFile::activate()
{
	mainWindow()->setTitle(target()->displayName().isEmpty()
		? target()->commPort() : target()->displayName());
	
	mainWindow()->setStatusMessage("");
	
	mainWindow()->deactivateMenuablesExcept(mainWindow()->standardMenus()
		<< Menu::SourceFileMenu::menuName() << Menu::TargetMenu::menuName());
	
	mainWindow()->activateMenuable(Menu::SourceFileMenu::menuName(), this);
	mainWindow()->activateMenuable(Menu::TargetMenu::menuName(), this);
	
	emit updateActivatable();
}

bool SourceFile::beginSetup()
{
	if(!hasFile()) {
		if(!selectTemplate()) return false;
	} else updateLexer();
	if(!target().data()) changeTarget();
	return target().data();
}

void SourceFile::completeSetup()
{
	updateTitle();
	updateMargins();
}

bool SourceFile::close()
{
	if(!ui_editor->isModified()) return true;
	
	QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Unsaved Changes"),
		tr("Save Changes to \"%1\" before closing?").arg(file().fileName()),
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

	if(ret == QMessageBox::Cancel) return false;
	
	if(ret == QMessageBox::Yes) {
		save();
		if(ui_editor->isModified()) return false;
	}
	
	mainWindow()->activateMenuable(Menu::SourceFileMenu::menuName(), 0);
	mainWindow()->activateMenuable(Menu::TargetMenu::menuName(), 0);
	
	return true;
}

bool SourceFile::save()
{
	return hasFile() ? fileSaveAs(file().absoluteFilePath()) : saveAs();
}

bool SourceFile::fileSaveAs(const QString& filePath)
{
	if(filePath.isEmpty()) return false;
	
	ui_editor->convertEols(QsciScintilla::EolUnix);
	if(ui_editor->text(ui_editor->lines() - 1).length() > 0) {
		ui_editor->append("\n");
	}
	
	setFile(filePath);
	
	QFile fileHandle(file().filePath());
	if(!fileHandle.open(QIODevice::WriteOnly)) return false;
	QTextStream fileStream(&fileHandle);
	fileStream << ui_editor->text();
	fileHandle.close();
	
	ui_editor->setModified(false);
	
	updateLexer();
	
	return true;
}

bool SourceFile::fileOpen(const QString& filePath)
{
	setFile(filePath);
	
	QFile fileHandle(file().filePath());
	
	if(!fileHandle.open(QIODevice::ReadOnly)) return false;

	QTextStream fileStream(&fileHandle);
	ui_editor->setText(fileStream.readAll());
	fileHandle.close();

	ui_editor->setModified(false);

	updateLexer();
	
	setProject(Project::ProjectPtr());
	
	return true;
}

bool SourceFile::memoryOpen(const QByteArray& ba, const QString& assocPath)
{
	setFile(assocPath);
	
	ui_editor->setText(ba);
	ui_editor->setModified(false);

	updateLexer();
		
	return true;
}

bool SourceFile::openProjectFile(const Project::ProjectPtr& project)
{
	return false;
}

void SourceFile::indentAll()
{
	if(!m_currentLexer || !m_currentLexer->cStyleBlocks()) return;
	
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
	
	setUpdatesEnabled(true);
}

void SourceFile::keyPressEvent(QKeyEvent *event)
{
#ifdef Q_OS_MAC
	int ctrlMod = Qt::MetaModifier;
#else
	int ctrlMod = Qt::ControlModifier;
#endif
	
	if(!(event->modifiers() & ctrlMod)) return;
	
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
	event->accept();
}

void SourceFile::refreshSettings()
{
	/* Read font, indent, margin, etc. settings */
	QSettings settings;
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
	
	Lexer::Factory::ref().setFont(defFont);

	/* Set other options from settings */
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
	ui_editor->setMarginsForegroundColor(QColor(185, 185, 185));
	
	Menu::TargetMenu::ref().refresh();
}

void SourceFile::updateMargins()
{
	int size = 0;
	if(ui_editor->marginLineNumbers(0)) {
		int charWidth = 6;
		if(ui_editor->lexer()) {
			QFont font = ui_editor->lexer()->defaultFont();
			font.setPointSize(font.pointSize() + getZoom());
			charWidth = QFontMetrics(font).width("0");
		}
		size = charWidth + charWidth / 2
			+ charWidth * (int)ceil(log10(std::max(ui_editor->lines(), 10) + 1));
	}
	ui_editor->setMarginWidth(0, size);
	ui_editor->setMarginWidth(1, 16);
}

int SourceFile::getZoom()
{
	return ui_editor->SendScintilla(QsciScintilla::SCI_GETZOOM);
}

void SourceFile::moveTo(const int& line, const int& pos)
{
	if(line > 0 && pos >= 0) ui_editor->setCursorPosition(line - 1, pos);
}

QsciScintilla *SourceFile::editor()
{
	return ui_editor;
}

int SourceFile::currentLine() const
{
	return m_currentLine;
}

bool SourceFile::breakpointOnLine(int line) const
{
	bool markerOnLine = false;
	foreach(const int& i, m_breakpoints) markerOnLine |= (ui_editor->markerLine(i) == m_currentLine);
	return markerOnLine;
}

SourceFile *SourceFile::newProjectFile(MainWindow* mainWindow, const Project::ProjectPtr& project)
{
	SourceFile* sourceFile = new SourceFile(mainWindow);
	sourceFile->setProject(project);
	return sourceFile;
}

void SourceFile::zoomIn()
{
	ui_editor->zoomIn();
	updateMargins();
}

void SourceFile::zoomOut()
{
	ui_editor->zoomOut();
	updateMargins();
}

void SourceFile::zoomReset()
{
	ui_editor->zoomTo(0);
	updateMargins();
}

bool SourceFile::saveAs()
{
	return hasProject() ? saveAsProject() : saveAsFile();
}

bool SourceFile::saveAsFile()
{
	QSettings settings;
	QString savePath = settings.value(SAVE_PATH, QDir::homePath()).toString();
	QStringList exts = Lexer::Factory::ref().formattedExtensions();
	
	QRegExp reg("*." + m_templateExt + "*");
	reg.setPatternSyntax(QRegExp::Wildcard);
	int i = exts.indexOf(reg);
	if(i != -1) exts.swap(0, i);
	
	QString filePath = QFileDialog::getSaveFileName(mainWindow(), tr("Save File"), savePath, 
		exts.join(";;") + (exts.size() < 1 ? "" : ";;") + "All Files (*)");
	if(filePath.isEmpty()) return false;

	QFileInfo fileInfo(filePath);
	
	QString ext = m_templateExt;
	if(!ext.isEmpty() && fileInfo.suffix().isEmpty()) {
		fileInfo.setFile(fileInfo.fileName() + "." + ext);
	}
	
	settings.setValue(SAVE_PATH, fileInfo.absolutePath());
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
	
	QStringList exts = Lexer::Factory::ref().extensions();
	QStringList nameFilters;
	foreach(const QString& ext, exts) {
		nameFilters << "*." + ext;
	}
	
	Dialog::SaveAs dialog(this);
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

const bool SourceFile::download()
{
	if(!actionPreconditions()) return false;
	return execute(Unit::Download);
}

const bool SourceFile::compile()
{
	if(!actionPreconditions()) return false;
	bool success = true;
	success &= execute(Unit::Download);
	success &= execute(Unit::Compile);
	return success;
}

const bool SourceFile::run()
{
	if(!actionPreconditions()) return false;
	bool success = true;
	success &= execute(Unit::Download);
	success &= execute(Unit::Compile);
	success &= execute(Unit::Run);
	return success;
}

void SourceFile::stop()
{
}

void SourceFile::debug()
{
}

void SourceFile::copy()
{
	ui_editor->copy();
}

void SourceFile::cut()
{
	ui_editor->cut();
}

void SourceFile::paste()
{
	ui_editor->paste();
}

void SourceFile::undo()
{
	ui_editor->undo();
}

void SourceFile::redo()
{
	ui_editor->redo();
}

void SourceFile::find()
{
	showFind();
}

void SourceFile::print()
{
	QsciPrinter printer;
	QPrintDialog printDialog(&printer, this);
	
	if(printDialog.exec()) printer.printRange(ui_editor);
}

void SourceFile::convertToProject()
{
	if(!save()) return;
	Project::ProjectPtr project = mainWindow()->newProject();
	fileSaveAs(project->location() + "/" + file().fileName());
	project->setTarget(target());
	setProject(project);
}

const bool SourceFile::changeTarget()
{
	Dialog::Target targetDialog(&Target::InterfaceManager::ref(), this);
	if(targetDialog.exec() == QDialog::Rejected) return false;
	if(targetDialog.target().isNull()) return false;
	setTarget(targetDialog.target());
	
	// This hooks up all important callbacks
	target()->setResponder(mainWindow()->mainResponder());
	
	updateTitle();
	
	return true;
}

void SourceFile::screenGrab()
{
}

void SourceFile::requestFile()
{
}

void SourceFile::toggleBreakpoint(const bool& checked)
{
	if(checked) m_breakpoints.append(ui_editor->markerAdd(m_currentLine, m_breakIndicator));
 	else {
		m_breakpoints.removeAll(ui_editor->markerLine(m_currentLine));
		ui_editor->markerDelete(m_currentLine, m_breakIndicator);
	}
	
	emit updateActivatable();
}

void SourceFile::clearBreakpoints()
{	
	foreach(const int& i, m_breakpoints) ui_editor->markerDeleteHandle(i);
	m_breakpoints.clear();
	
	emit updateActivatable();
}

void SourceFile::on_ui_editor_cursorPositionChanged(int line, int)
{
	m_currentLine = line;
	emit updateActivatable();
}

void SourceFile::showFind()
{
	ui_find->show();
}

void SourceFile::setLexer(Lexer::Constructor *constructor)
{
	ui_editor->setLexer(0);
	if(m_currentLexer) {
		// TODO: Move away from QScintilla before I lose my mind.
		// delete m_currentLexer;
	}
	m_currentLexer = 0;
	if(!constructor) return;
	m_currentLexer = constructor->construct();
	ui_editor->setLexer(m_currentLexer->lexer());
	// Lexer::Factory::setAPIsForLexer(m_currentLexer, m_lexAPI);
	refreshSettings();
	updateMargins();
}

void SourceFile::dropEvent(QDropEvent *)
{
}

void SourceFile::clearProblems()
{
	ui_editor->markerDeleteAll(m_errorIndicator);
	ui_editor->markerDeleteAll(m_warningIndicator);
}

void SourceFile::markProblems(const Lines& lines)
{
	foreach(const Lines::line_t& line, lines.warningLines()) {
		ui_editor->markerAdd(line, m_warningIndicator);
	}
	foreach(const Lines::line_t& line, lines.errorLines()) {
		ui_editor->markerAdd(line, m_errorIndicator);
	}
}

void SourceFile::updateErrors(const Compiler::OutputList& compileResult) 
{
	clearProblems();
	Lines lines;
	foreach(const Compiler::Output& output, compileResult) {
		lines = lines + OutputHelper::lines(output);
	}
	markProblems(lines);
}

const bool SourceFile::selectTemplate()
{
	Dialog::Template tDialog(mainWindow()->templateManager(), this);
	if(tDialog.exec() == QDialog::Rejected) return false;

	Template::File tFile = tDialog.file();
	
	Lexer::Constructor *constructor = 0;
	if(tFile.hasLexer()) {
		constructor = Lexer::Factory::ref().constructor(tFile.lexer());
		m_templateExt = tFile.lexer();
	}
	
	ui_editor->setText(tFile.resolvedData());
	
	// m_lexAPI = QString(targetPath).replace(QString(".") + TARGET_EXT, ".api");
	
	if(constructor) setLexer(constructor);
	
	refreshSettings();
	
	return true;
}

void SourceFile::fileChanged(const QFileInfo& file)
{
	setName(file.fileName());
	updateTitle();
}

void SourceFile::projectChanged(const Project::ProjectPtr& project)
{
	Unit::setParent(project.data());
	updateTitle();
}

void SourceFile::updateTitle()
{
	mainWindow()->setTabName(this, (ui_editor->isModified() ? "* " : "") + fullName());
}

void SourceFile::updateLexer()
{
	// Update the lexer to the new spec for that extension
	Lexer::Constructor *constructor1 = Lexer::Factory::ref().constructor(file().completeSuffix());
	Lexer::Constructor *constructor2 = Lexer::Factory::ref().constructor(file().suffix());
	Lexer::Constructor *constructor = constructor1 ? constructor1 : constructor2;
	if(!Lexer::Factory::isLexerFromConstructor(m_currentLexer, constructor)) setLexer(constructor);
}

bool SourceFile::actionPreconditions()
{
	if(!save()) return false;
	if(!target()->available()) {
		changeTarget();
		if(!target()->available()) {
			mainWindow()->setStatusMessage(tr("Target \"%1\" is not available for communication.").arg(target()->displayName()));
			return false;
		}
	}
	return true;
}

Kiss::KarPtr SourceFile::archive() const
{
	// TODO: Projects
	Kiss::KarPtr archive = Kiss::Kar::create();
	archive->addFile(file().fileName(), ui_editor->text().toAscii());
	Compiler::OutputList out = LanguageHelperManager::ref().preprocess(archive, QStringList() << file().path());
	// TODO: I really don't like that this is here.
	if(Compiler::Output::isSuccess(out)) return archive;
	mainWindow()->setOutputList(out);
	return Kiss::KarPtr();
}
