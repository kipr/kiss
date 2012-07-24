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

#include "SourceFile.h"

#include "MainWindow.h"
#include "WebTab.h"
#include "Singleton.h"
#include "RequestFileDialog.h"
#include "MessageDialog.h"
#include "MacroString.h"
#include "LexerFactory.h"
#include "TemplateManager.h"
#include "TemplateFormat.h"
#include "MakeTemplateDialog.h"
#include "PasswordDialog.h"
#include "SourceFileMenu.h"
#include "DeviceMenu.h"
#include "MainWindowMenu.h"
#include "Project.h"
#include "ProjectSaveAs.h"
#include "ProjectManager.h"
#include <kiss-compiler/QTinyArchive.h>
#include "Log.h"
#include <kiss-compiler/Compiler.h>
#include <kiss-compiler/CompilerManager.h>
#include "DeviceDialog.h"
#include "InterfaceManager.h"

#include "UiEventManager.h"
#include "ResourceHelper.h"
#include "Interface.h"

#include <Qsci/qscilexercpp.h>
#include <QFile>
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
#include <Qsci/qscilexercpp.h>

#include <memory>

#define SAVE_PATH "savepath"
#define DEFAULT_EXTENSION "default_extension"

#define MAX(a, b) (a > b ? a : b)

SourceFile::SourceFile(MainWindow* parent) : QWidget(parent), TabbedWidget(this, parent),
	WorkingUnit("File"), m_isNewFile(true),
	m_debuggerEnabled(false), m_runTab(0),
	m_temporaryArchive(0)
{
	setupUi(this);
	
	ui_localCompileFailed->setSourceFile(this);
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
	
	ui_find->hide();
	ui_localCompileFailed->hide();
	
	setAssociatedFile("Untitled");
	
	refreshSettings();
	
	connect(&m_responder, SIGNAL(compileFinished(CompileResult)), SLOT(compileFinished(CompileResult)));
	connect(&m_responder, SIGNAL(downloadFinished(bool)), SLOT(downloadFinished(bool)));
	connect(&m_responder, SIGNAL(runFinished(bool)), SLOT(runFinished(bool)));
	connect(&m_responder, SIGNAL(availableFinished(bool)), SLOT(availableFinished(bool)));
	connect(&m_responder, SIGNAL(connectionError()), SLOT(connectionError()));
	connect(&m_responder, SIGNAL(communicationError()), SLOT(communicationError()));
	connect(&m_responder, SIGNAL(notAuthenticatedError()), SLOT(notAuthenticatedError()));
	connect(&m_responder, SIGNAL(authenticationResponse(DeviceResponder::AuthenticateReturn)), SLOT(authenticationResponse(DeviceResponder::AuthenticateReturn)));
}

SourceFile::~SourceFile()
{
	setTemporaryArchive(0);
}

void SourceFile::activate()
{
	mainWindow()->setTitle(device()->displayName());
	mainWindow()->showErrors(topLevelUnit());
	mainWindow()->setStatusMessage("");
	
	QList<Menuable*> menus = mainWindow()->menuablesExcept(mainWindow()->standardMenus() << SourceFileMenu::menuName() << DeviceMenu::menuName());
	foreach(Menuable* menu, menus) {
		Log::ref().debug(QString("Deactivating %1").arg(menu->name()));
		ActivatableObject* activatable = dynamic_cast<ActivatableObject*>(menu);
		if(activatable) activatable->setActive(0);
	}
	
	mainWindow()->activateMenuable(SourceFileMenu::menuName(), this);
	mainWindow()->activateMenuable(DeviceMenu::menuName(), this); 
}

bool SourceFile::beginSetup() {
	setParentUnit(isProjectAssociated() ? associatedProject() : 0);
	if(isNewFile()) { if(!selectTemplate()) return false; }
	else setLexer(Lexer::Factory::ref().constructor(associatedFileSuffix()));
	if(!device().get()) changeDevice();
	return device().get();
}

void SourceFile::completeSetup()
{
	mainWindow()->setTabName(this, associatedFileName() + (isProjectAssociated() ? (QString(" (") + associatedProject()->name() + ")") : QString()));
	updateMargins();
}

bool SourceFile::close()
{
	if(!ui_editor->isModified()) return true;
	
	QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Unsaved Changes"),
		tr("Save Changes to \"") + associatedFileName() + tr("\" before closing?"),
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

	if(ret == QMessageBox::Cancel) return false;
	if(ret == QMessageBox::Yes) {
		save();
		if(ui_editor->isModified()) return false;
	}
	
	mainWindow()->activateMenuable(SourceFileMenu::menuName(), 0);
	mainWindow()->activateMenuable(DeviceMenu::menuName(), 0);
	
	return true;
}

bool SourceFile::save()
{
	if(m_isNewFile) return saveAs();
	UiEventManager::ref().sendEvent(UI_EVENT_FILE_SAVE);
	return fileSaveAs(associatedFile());
}

bool SourceFile::fileSaveAs(const QString& filePath)
{
	if(filePath.isEmpty()) return false;
	
	ui_editor->convertEols(QsciScintilla::EolUnix);
	if(ui_editor->text(ui_editor->lines() - 1).length() > 0) ui_editor->append("\n");
	
	setAssociatedFile(filePath);
	
	if(isProjectAssociated()) {
		Log::ref().info(QString("Saving %1 with project association").arg(filePath));
		QTinyArchive* archive = associatedProject()->archive();
		if(!archive->put(filePath, ui_editor->text().toLatin1())) {
			Log::ref().error(QString("Failed to put %1").arg(filePath));
			return false;
		}
		associatedProject()->sync();
	} else {
		QFile fileHandle(associatedFile());
		if(!fileHandle.open(QIODevice::WriteOnly)) return false;

		QTextStream fileStream(&fileHandle);
		fileStream << ui_editor->text();
		fileHandle.close();
	}
	
	ui_editor->setModified(false);
	m_isNewFile = false;
	mainWindow()->setTabName(this, associatedFileName() + (isProjectAssociated() ? (QString(" (") + associatedProject()->name() + ")") : QString()));
	
	// Update the lexer to the new spec for that extension
	Lexer::Constructor* constructor = Lexer::Factory::ref().constructor(associatedFileSuffix());
	if(!Lexer::Factory::isLexerFromConstructor((Lexer::LexerBase*)ui_editor->lexer(), constructor))
		setLexer(constructor);
	
	return true;
}

bool SourceFile::fileOpen(const QString& filePath)
{
	setAssociatedFile(filePath);
	
	QFile fileHandle(associatedFile());
	
	if(!fileHandle.open(QIODevice::ReadOnly)) return false;

	QTextStream fileStream(&fileHandle);
	ui_editor->setText(fileStream.readAll());
	fileHandle.close();

	ui_editor->setModified(false);

	m_isNewFile = false;

	mainWindow()->setTabName(this, associatedFileName());

	Lexer::Constructor* constructor = Lexer::Factory::ref().constructor(associatedFileSuffix());
	if(Lexer::Factory::isLexerFromConstructor((Lexer::LexerBase*)ui_editor->lexer(), constructor))
		setLexer(constructor);
	
	setAssociatedProject(0);
	
	return true;
}

bool SourceFile::memoryOpen(const QByteArray& ba, const QString& assocPath)
{
	setAssociatedFile(assocPath);
	
	ui_editor->setText(ba);

	ui_editor->setModified(false);
	m_isNewFile = false;

	mainWindow()->setTabName(this, associatedFileName());

	Lexer::Constructor* constructor = Lexer::Factory::ref().constructor(associatedFileSuffix());
	if(Lexer::Factory::isLexerFromConstructor((Lexer::LexerBase*)ui_editor->lexer(), constructor))
		setLexer(constructor);
		
	return true;
}

bool SourceFile::openProjectFile(Project* project, const TinyNode* node)
{
	if(!node) return false;
	bool ret = memoryOpen(QTinyNode::data(node), QTinyNode::name(node));
	setAssociatedProject(ret ? project : 0);
	return ret;
}

void SourceFile::indentAll()
{
	if(!ui_editor->lexer()) return;
	if(!dynamic_cast<Lexer::LexerBase*>(ui_editor->lexer())->cStyleBlocks()) return;
	
	setUpdatesEnabled(false);

	int indentLevel = 0;
	int blockStartStyle; ui_editor->lexer()->blockStart(&blockStartStyle);
	int blockEndStyle; ui_editor->lexer()->blockEnd(&blockEndStyle);
	
	int currentLine;
	int currentIndex;

	ui_editor->getCursorPosition(&currentLine,&currentIndex);
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
		//	of the default style (I can't think of a case other than a comment in which it would not be)
		//	or it is a comment, but the end of a one line comment so the next line would be a different style
		//  also, if the indentLevel > 0, we should indent anyway
		if(blockEndCount > blockStartCount) {
			indentLevel -= blockEndCount-blockStartCount;
			blockEndCount = blockStartCount = 0;
		}
		if((ui_editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos-1) == ui_editor->lexer()->defaultStyle() ||
			ui_editor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos-1) 
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
	
	if(event->modifiers() & ctrlMod) {
		int line, index;
		ui_editor->getCursorPosition(&line, &index);
		
		switch(event->key()) {
			case Qt::Key_A:
				ui_editor->setCursorPosition(line, 0);
				break;
			case Qt::Key_E:
				if(ui_editor->lines()-1 == line)
					ui_editor->setCursorPosition(line, ui_editor->lineLength(line));
				else ui_editor->setCursorPosition(line, ui_editor->lineLength(line)-1);
				break;
			case Qt::Key_P:
				if(ui_editor->lineLength(line-1) < index)
					ui_editor->setCursorPosition(line-1, ui_editor->lineLength(line-1)-1);
				else ui_editor->setCursorPosition(line-1, index);
				break;
			case Qt::Key_N:
				if(ui_editor->lineLength(line+1) < index) {
					if(ui_editor->lines()-1 == line+1)
						ui_editor->setCursorPosition(line+1, ui_editor->lineLength(line));
					else
						ui_editor->setCursorPosition(line+1, ui_editor->lineLength(line+1)-1);
				} else ui_editor->setCursorPosition(line+1, index);
				break;
			case Qt::Key_B:
				if(index-1 < 0)
					ui_editor->setCursorPosition(line-1, ui_editor->lineLength(line-1)-1);
				else
					ui_editor->setCursorPosition(line, index-1);
				break;
			case Qt::Key_F:
				if(ui_editor->lineLength(line) < index+1)
					ui_editor->setCursorPosition(line+1, 0);
				else ui_editor->setCursorPosition(line, index+1);
				break;
			case Qt::Key_D:
				ui_editor->setSelection(line, index, line, index+1);
				ui_editor->removeSelectedText();
				break;
			case Qt::Key_K:
				ui_editor->setSelection(line, index, line, ui_editor->lineLength(line)-1);
				ui_editor->cut();
				break;
			case Qt::Key_Y:
				ui_editor->paste();
				break;
		}
		event->accept();
	}
}

void SourceFile::refreshSettings()
{
	/* Read font, indent, margin, etc. settings */
	QSettings settings;
	settings.beginGroup(EDITOR);

	/* Set the default font from settings */
	QFont defFont(settings.value(FONT).toString(), settings.value(FONT_SIZE).toInt());
	
	if(ui_editor->lexer()) ui_editor->lexer()->setFont(defFont, -1);
	Lexer::Factory::ref().setFont(defFont);

	/* Set other options from settings */
	settings.beginGroup(AUTO_INDENT);
	ui_editor->setAutoIndent(settings.value(ENABLED).toBool());
	if(ui_editor->lexer()) ui_editor->lexer()->setAutoIndentStyle(settings.value(STYLE).toString() == MAINTAIN ? 
		QsciScintilla::AiMaintain : 0);
	ui_editor->setTabWidth(settings.value(WIDTH).toInt());
	settings.endGroup();

	settings.beginGroup(AUTO_COMPLETION);
	ui_editor->setAutoCompletionSource(QsciScintilla::AcsNone);
	if(settings.value(ENABLED).toBool()) {
		if(settings.value(API_SOURCE).toBool()) ui_editor->setAutoCompletionSource(QsciScintilla::AcsAPIs);
		if(settings.value(DOC_SOURCE).toBool()) {
			if(ui_editor->autoCompletionSource() == QsciScintilla::AcsAPIs)
				ui_editor->setAutoCompletionSource(QsciScintilla::AcsAll);
			else ui_editor->setAutoCompletionSource(QsciScintilla::AcsDocument);
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
	
	DeviceMenu::ref().refresh();
}

bool SourceFile::isNewFile() 	{ return m_isNewFile; }

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
		size = charWidth + charWidth/2 + charWidth * (int)ceil(log10(MAX(ui_editor->lines(), 10) + 1));
	}
	ui_editor->setMarginWidth(0, size);
	ui_editor->setMarginWidth(1, 16);
}

int SourceFile::getZoom() { return ui_editor->SendScintilla(QsciScintilla::SCI_GETZOOM); }
void SourceFile::moveTo(int line, int pos)  { if(line > 0 && pos >= 0) ui_editor->setCursorPosition(line - 1, pos); }
QsciScintilla* SourceFile::editor() { return ui_editor; }
int SourceFile::currentLine() const { return m_currentLine; }

bool SourceFile::breakpointOnLine(int line) const
{
	bool markerOnLine = false;
	foreach(const int& i, m_breakpoints) markerOnLine |= (ui_editor->markerLine(i) == m_currentLine);
	return markerOnLine;
}

SourceFile* SourceFile::newProjectFile(MainWindow* mainWindow, Project* project)
{
	SourceFile* sourceFile = new SourceFile(mainWindow);
	sourceFile->setAssociatedProject(project);
	return sourceFile;
}

void SourceFile::zoomIn() { ui_editor->zoomIn(); updateMargins(); UiEventManager::ref().sendEvent(UI_EVENT_ZOOM_IN); }
void SourceFile::zoomOut() { ui_editor->zoomOut(); updateMargins(); UiEventManager::ref().sendEvent(UI_EVENT_ZOOM_OUT); }
void SourceFile::zoomReset() { ui_editor->zoomTo(0); updateMargins(); UiEventManager::ref().sendEvent(UI_EVENT_ZOOM_RESET); }

bool SourceFile::saveAs()
{
	return isProjectAssociated() ? saveAsProject() : saveAsFile();
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
	
	QString filePath = QFileDialog::getSaveFileName(mainWindow(), "Save File", savePath, 
		exts.join(";;") + (exts.size() < 1 ? "" : ";;") + "All Files (*)");
	if(filePath.isEmpty()) return false;

	QFileInfo fileInfo(filePath);
	
	QString ext = m_templateExt;
	if(!ext.isEmpty() && fileInfo.suffix().isEmpty()) fileInfo.setFile(filePath.section(".", 0, 0) + "." + ext);
	
	settings.setValue(SAVE_PATH, fileInfo.absolutePath());

	/* Saves the file with the new associatedFileName and updates the tabWidget label */
	if(fileSaveAs(fileInfo.absoluteFilePath())) {
		mainWindow()->setTabName(this, associatedFileName());
		mainWindow()->setStatusMessage("Saved file \"" + associatedFileName() + "\"");
	} else QMessageBox::critical(mainWindow(), "Error", "Error: Could not write file " + associatedFileName());

	QStringList current = settings.value(RECENTS).toStringList().mid(0, 5);
	current.push_front(fileInfo.absoluteFilePath());
	current.removeDuplicates();
	settings.setValue(RECENTS, current);
	
	UiEventManager::ref().sendEvent(UI_EVENT_FILE_SAVE);
	
	return true;
}

bool SourceFile::saveAsProject()
{
	ProjectSaveAs saveAsProject(mainWindow());
	saveAsProject.setDefaultProject(associatedProject());
	if(saveAsProject.exec() == QDialog::Rejected) return false;
	qWarning() << "Saving to" << QTinyNode::path(saveAsProject.parent()) << saveAsProject.fileName();
	setAssociatedProject(saveAsProject.activeProject());
	const QString& path = QPathUtils::appendComponent(QTinyNode::path(saveAsProject.parent()), saveAsProject.fileName());
	if(associatedProject()->archive()->exists(path)) {
		QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Are You Sure?"), tr("Overwrite ") + path + "?",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			
		if(ret == QMessageBox::No) return false;
	}
	qWarning() << "Which is" << path;
	return fileSaveAs(path);
}

void SourceFile::sourceModified(bool) { mainWindow()->setTabName(this, "* " + associatedFileName()
	+ (isProjectAssociated() ? (QString(" (") + associatedProject()->name() + ")") : QString())); }

const bool SourceFile::download()
{
	if(device()->isQueueExecuting()) return false;
	if(!save()) return false;
	if(!device().get()) if(!changeDevice()) return false;
	
	mainWindow()->setStatusMessage(tr("Downloading..."));
	QApplication::flush();
	
	const bool assoc = isProjectAssociated();
	QTinyArchive* archive = 0;
	if(assoc) archive = associatedProject()->archive();
	else {
		archive = new QTinyArchive();
		archive->add(associatedFileName(), ui_editor->text().toUtf8());
	}
	const QString remoteName = assoc ? associatedProject()->name() : associatedFileName();
	CommunicationQueue queue;
	queue.enqueue(CommunicationEntryPtr(new CommunicationEntry(CommunicationEntry::Download, remoteName, archive)));
	queue.enqueue(CommunicationEntryPtr(new CommunicationEntry(CommunicationEntry::Disconnect)));
	bool success = device()->executeQueue(queue);
	if(!success) {
		mainWindow()->setStatusMessage(tr("Error communicating with ") + device()->displayName());
	}
	if(!assoc && archive) {
		setTemporaryArchive(archive);
	}
	return success;
}

const bool SourceFile::compile()
{
	if(device()->isQueueExecuting()) return false;
	if(!save()) return false;
	if(!device().get()) if(!changeDevice()) return false;
	
	ui_localCompileFailed->hide();
	
	mainWindow()->setStatusMessage(tr("Downloading and Compiling..."));
	QApplication::flush();
	
	const bool assoc = isProjectAssociated();
	QTinyArchive* archive = 0;
	if(assoc) archive = associatedProject()->archive();
	else {
		archive = new QTinyArchive();
		archive->add(associatedFileName(), ui_editor->text().toUtf8());
	}
	
	const QString remoteName = assoc ? associatedProject()->name() : associatedFileName();
	CommunicationQueue queue;
	queue.enqueue(CommunicationEntryPtr(new CommunicationEntry(CommunicationEntry::Download, remoteName, archive)));
	queue.enqueue(CommunicationEntryPtr(new CommunicationEntry(CommunicationEntry::Compile, remoteName)));
	queue.enqueue(CommunicationEntryPtr(new CommunicationEntry(CommunicationEntry::Disconnect)));
	bool success = device()->executeQueue(queue);
	if(!assoc && archive) {
		setTemporaryArchive(archive);
	}
	if(!success) {
		mainWindow()->setStatusMessage(tr("Error starting download and compile procedure"));
	}
	return success;
}

const bool SourceFile::run()
{
	if(device()->isQueueExecuting()) return false;
	if(!save()) return false;
	if(!device().get()) if(!changeDevice()) return false;
	
	ui_localCompileFailed->hide();
	
	mainWindow()->setStatusMessage(tr("Downloading, Compiling, and Running..."));
	QApplication::flush();
	
	const bool assoc = isProjectAssociated();
	QTinyArchive* archive = 0;
	if(assoc) archive = associatedProject()->archive();
	else {
		archive = new QTinyArchive();
		archive->add(associatedFileName(), ui_editor->text().toUtf8());
	}
	
	const QString remoteName = assoc ? associatedProject()->name() : associatedFileName();
	
	CommunicationQueue queue;
	queue.enqueue(CommunicationEntryPtr(new CommunicationEntry(CommunicationEntry::Download, remoteName, archive)));
	queue.enqueue(CommunicationEntryPtr(new CommunicationEntry(CommunicationEntry::Compile, remoteName)));
	queue.enqueue(CommunicationEntryPtr(new CommunicationEntry(CommunicationEntry::Run, remoteName)));
	queue.enqueue(CommunicationEntryPtr(new CommunicationEntry(CommunicationEntry::Disconnect)));
	
	bool success = device()->executeQueue(queue);
	if(!assoc && archive) {
		setTemporaryArchive(archive);
	}
	if(!success) {
		mainWindow()->setStatusMessage(tr("Error starting download, compile, and run procedure"));
	}
	return success;
}

void SourceFile::stop() { /* target()->stop(); UiEventManager::ref().sendEvent(UI_EVENT_STOP); */ }

void SourceFile::debug()
{
	/* ui_localCompileFailed->hide();
	
	if(!save()) return;
	mainWindow()->hideErrors();
	DebuggerInterface* interface = 0;
	QList<Location> bkpts;
	if(!m_debuggerEnabled) {
		foreach(const int& i, m_breakpoints) {
			bkpts.append(Location(associatedFileName(), ui_editor->markerLine(i) + 1));
		}
	}
	bool success = m_debuggerEnabled ? !(interface = target()->debug(associatedFile())) : target()->debugConsole(associatedFile(), bkpts);
	mainWindow()->setStatusMessage(success ? tr("Debug Succeeded") : tr("Debug Failed"));
	updateErrors();
	if(!interface) return;
	foreach(const int& i, m_breakpoints) {
		// QScintilla starts at 0, while gdb starts at 1, + 1
		interface->addBreakpoint(associatedFileName(), ui_editor->markerLine(i) + 1); 
	}
	
	m_debugger.startDebug(interface);
	
	UiEventManager::ref().sendEvent(UI_EVENT_DEBUG); */
}

void SourceFile::copy() { ui_editor->copy(); }
void SourceFile::cut() { ui_editor->cut(); }
void SourceFile::paste() { ui_editor->paste(); }
void SourceFile::undo() { ui_editor->undo(); }
void SourceFile::redo() { ui_editor->redo(); }
void SourceFile::find() { showFind(); }

void SourceFile::print()
{
	QsciPrinter printer;
	QPrintDialog printDialog(&printer, this);
	
	if(printDialog.exec()) printer.printRange(ui_editor);
}

void SourceFile::convertToProject()
{
	Project* project = mainWindow()->newProject();
	if(!project) return;
	const TinyNode* node = project->addFile(associatedFile());
	setAssociatedProject(project);
	sourceModified(true);
	setAssociatedFile(QTinyNode::path(node));
}

const bool SourceFile::changeDevice()
{
	DeviceDialog deviceDialog(&InterfaceManager::ref(), this);
	if(deviceDialog.exec() == QDialog::Rejected) return false;
	setDevice(deviceDialog.device());
	device()->setResponder(&m_responder);
	return true;
}

void SourceFile::screenGrab()
{
	/* if(!checkPort()) return;
	QByteArray file = target()->screenGrab();
	QSettings settings;
	QString savePath = settings.value(SAVE_PATH, QDir::homePath()).toString();
	QString filePath = QFileDialog::getExistingDirectory(mainWindow(), "Save Screen Grab", savePath, QFileDialog::ShowDirsOnly);
	if(filePath.isEmpty()) return;
	QString saveFilePath = filePath + "/Screen Grab on " + QDateTime::currentDateTime().toString() + ".jpg";
	QFile f(saveFilePath);
	if(!f.open(QIODevice::WriteOnly)) return;
	f.write(file);
	f.close();
	QDesktopServices::openUrl(QUrl::fromUserInput(saveFilePath)); */
}

void SourceFile::requestFile()
{
	/* if(!checkPort()) return;
	RequestFileDialog dialog(target());
	if(!dialog.exec()) return;
	QByteArray file = target()->requestFile(target()->requestFilePath() + "/" + dialog.selectedFile());
	QSettings settings;
	QString savePath = settings.value(SAVE_PATH, QDir::homePath()).toString();
	QString filePath = QFileDialog::getExistingDirectory(mainWindow(), "Save Remote File", savePath, QFileDialog::ShowDirsOnly);
	if(filePath.isEmpty()) return;
	QString saveFilePath = filePath + "/" + dialog.selectedFile();
	QFile f(saveFilePath);
	if(!f.open(QIODevice::WriteOnly)) return;
	f.write(file);
	f.close();
	mainWindow()->openFile(saveFilePath); */
}

void SourceFile::makeTemplate()
{
	save();
	
	UiEventManager::ref().sendEvent(UI_EVENT_MAKE_TEMPLATE);
	
	MakeTemplateDialog makeTemplateDialog(this);
	makeTemplateDialog.setName(associatedFileBaseName());
	makeTemplateDialog.setTypes(TemplateManager::ref().types());
	makeTemplateDialog.setExtension(associatedFileSuffix());
	
	if(makeTemplateDialog.exec() == QDialog::Rejected) return;
	
	QString output;
	QTextStream outputStream(&output);
	
	// Write the template with metadata to output
	TemplateFormatWriter writer(&outputStream);
	writer.setLexerName(makeTemplateDialog.extension());
	writer.setContent(ui_editor->text());
	writer.update();
	
	qDebug() << output;
	
	TemplateManager::ref().addUserTemplate(makeTemplateDialog.type(), makeTemplateDialog.name(), output);
	
	UiEventManager::ref().sendEvent(UI_EVENT_MAKE_TEMPLATE2);
}

void SourceFile::toggleBreakpoint(bool checked)
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

void SourceFile::availableFinished(bool avail)
{
	qDebug() << "Available finished";
}

void SourceFile::compileFinished(CompileResult result)
{
	qDebug() << "Compile finished";
	updateErrors(result);
	mainWindow()->setStatusMessage(tr("Compile ") + (result.success() ? tr("Succeeded") : tr("Failed")));
	mainWindow()->setErrors(topLevelUnit(), result);
	UiEventManager::ref().sendEvent(UI_EVENT_COMPILE);
}

void SourceFile::downloadFinished(bool success)
{
	qDebug() << "Download finished";
	mainWindow()->setStatusMessage(tr("Download ") + (success ? tr("Succeeded") : tr("Failed")));
}

void SourceFile::runFinished(bool success)
{
	qDebug() << "Run finished";
	mainWindow()->setStatusMessage(tr("Run ") + (success ? tr("Succeeded") : tr("Failed")));
}

void SourceFile::connectionError()
{
	mainWindow()->setStatusMessage(tr("Unable to establish a connection with ") + device()->displayName());
}

void SourceFile::communicationError()
{
	mainWindow()->setStatusMessage(tr("Error communicating with ") + device()->displayName());
}

void SourceFile::notAuthenticatedError()
{
	PasswordDialog dialog(this);
	if(dialog.exec() == QDialog::Rejected) return;
	device()->authenticate(dialog.hash());
}

void SourceFile::authenticationResponse(const DeviceResponder::AuthenticateReturn& response)
{
	qDebug() << "Authed?" << (response == DeviceResponder::AuthSuccess);
	if(response == DeviceResponder::AuthSuccess) {
		device()->retryLastQueue();
		return;
	}
	if(response == DeviceResponder::AuthWillNotAccept) {
		mainWindow()->setStatusMessage(device()->displayName() + tr(" told KISS not to try authenticating again."));
		return;
	}
	
	PasswordDialog dialog(this);
	if(dialog.exec() == QDialog::Rejected) return;
	device()->authenticate(dialog.hash());
}

void SourceFile::showFind() { ui_find->show(); }

void SourceFile::setLexer(Lexer::Constructor* constructor)
{
	delete ui_editor->lexer();
	Lexer::LexerBase* lex = constructor->construct();
	ui_editor->setLexer(lex->lexer());
	Lexer::Factory::setAPIsForLexer(lex, m_lexAPI);
	refreshSettings();
	updateMargins();
}

void SourceFile::dropEvent(QDropEvent *event) { Q_UNUSED(event); }

void SourceFile::clearProblems()
{
	ui_editor->markerDeleteAll(m_errorIndicator);
	ui_editor->markerDeleteAll(m_warningIndicator);
}

void SourceFile::markProblems(const QStringList& errors, const QStringList& warnings)
{
	foreach(const QString& error, errors) {
		int line = error.section(":", 1, 1).toInt();
		if(--line < 0) continue;
		ui_editor->markerAdd(line, m_errorIndicator);
	}
	
	foreach(const QString& warning, warnings) {
		int line = warning.section(":", 1, 1).toInt();
		if(--line < 0) continue;
		ui_editor->markerAdd(line, m_warningIndicator);
	}
}

void SourceFile::updateErrors(const CompileResult& compileResult) 
{
	clearProblems();
	
	const QStringList& errors = compileResult.output(DEFAULT_ERROR_KEY);
	const QStringList& warnings = compileResult.output(DEFAULT_WARNING_KEY);
	
	markProblems(errors, warnings);
}

const bool SourceFile::selectTemplate()
{
	QString targetPath;
	QString templateFile;
	
	TemplateDialog tDialog(this);
	if(tDialog.exec() == QDialog::Rejected) return false;
	templateFile = tDialog.templateFile();
	
	Lexer::Constructor* constructor = 0;

	QFile tFile(templateFile);
	if(!tFile.open(QIODevice::ReadOnly)) {
		MessageDialog::showError(this, "simple_error_with_action", QStringList() <<
			tr("Error loading template file ") + templateFile <<
			tr("Unable to open file for reading.") <<
			tr("Continuing without selected template."));
		return true;
	}
	QTextStream stream(&tFile);
	TemplateFormatReader templateReader(&stream);

	if(templateReader.hasLexerName()) {
		QString lex = templateReader.lexerName();
		Log::ref().debug(QString("Template Lexer specified: %1").arg(lex));
		constructor = Lexer::Factory::ref().constructor(lex);
		m_templateExt = lex;
	}
	
	ui_editor->setText(templateReader.content());
	tFile.close();
	
	Log::ref().info("Template selected");
	
	// m_lexAPI = QString(targetPath).replace(QString(".") + TARGET_EXT, ".api");
	
	if(constructor) setLexer(constructor);
	refreshSettings();
	UiEventManager::ref().sendEvent(UI_EVENT_TEMPLATE_SELECTED);
	return true;
}

void SourceFile::setTemporaryArchive(TinyArchive *archive)
{
	if(m_temporaryArchive) delete m_temporaryArchive;
	m_temporaryArchive = archive;
}
