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

#include "MainWindow.h"
#include "WebTab.h"
#include "TargetManager.h"
#include "Singleton.h"
#include "RequestFileDialog.h"
#include "ErrorDialog.h"
#include "MacroString.h"
#include "LexerFactory.h"

#include "SourceFileShared.h"

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

#define SAVE_PATH "savepath"
#define DEFAULT_EXTENSION "default_extension"

#define KISS_LEXER "KISS_LEXER"
#define KISS_DATE "KISS_DATE"
#define END_KISS "END_KISS_META"

SourceFile::SourceFile(QWidget* parent) : QWidget(parent), m_fileHandle(tr("Untitled")), m_isNewFile(true), m_target(this), 
 	m_targetName("?"), m_debugger(false), m_runTab(0)
{
	setupUi(this);
	
	ui_localCompileFailed->setSourceFile(this);
	ui_find->setSourceFile(this);
	
	m_errorIndicator = ui_editor->markerDefine(SourceFileShared::ref().redBullet());
	m_warningIndicator = ui_editor->markerDefine(SourceFileShared::ref().yellowBullet());
	m_breakIndicator = ui_editor->markerDefine(SourceFileShared::ref().blueBullet());
	
	ui_editor->setModified(false);
	ui_editor->setEolMode(QsciScintilla::EolUnix);
	ui_editor->setWrapMode(QsciScintilla::WrapWord);
	
	m_fileInfo.setFile(m_fileHandle);
	MainWindow::ref().setStatusMessage("");
	
	connect(actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(ui_editor, SIGNAL(textChanged()), this, SLOT(updateMargins()));
	connect(ui_editor, SIGNAL(modificationChanged(bool)), this, SLOT(sourceModified(bool)));
	connect(actionIndentAll, SIGNAL(triggered()), this, SLOT(indentAll()));
	
	ui_find->hide();
	ui_localCompileFailed->hide();
	
	refreshSettings();
}

void SourceFile::activate()
{
	MainWindow::ref().setTitle(m_targetName + (!m_target.port().isEmpty() ? (" - " + m_target.port()) : ""));
	MainWindow::ref().showErrors(this);
}

void SourceFile::addActionsFile(QMenu* file) 
{
	file->addAction(actionSave);
	file->addAction(actionSaveAs);
	file->addSeparator();
	file->addAction(actionPrint);
}

void SourceFile::addActionsEdit(QMenu* edit)
{
	edit->addAction(actionCopy);
	edit->addAction(actionCut);
	edit->addAction(actionPaste);
	edit->addSeparator();
	edit->addAction(actionUndo);
	edit->addAction(actionRedo);
}

void SourceFile::addActionsHelp(QMenu* help) 
{
	const QMap<QString, QString>& manuals = m_target.targetManualPaths();
	foreach(const QString& manual, manuals.keys()) {
		if(!QFileInfo(manuals[manual]).exists()) continue;
		QAction* action = help->addAction(QIcon(":/shortcuts/target/icon_set/icons/report.png"), manual);
		action->setData(manuals[manual]);
		connect(action, SIGNAL(triggered()), this, SLOT(openManual()));
	}
}

void SourceFile::addOtherActions(QMenuBar* menuBar)
{
	QMenu* source = menuBar->addMenu(tr("Source"));
	QMenu* target = menuBar->addMenu(tr("Target"));
	source->addAction(actionZoomIn);
	source->addAction(actionZoomOut);
	source->addAction(actionResetZoomLevel);
	if(m_target.cStyleBlocks()) {
		source->addSeparator();
		source->addAction(actionIndentAll);
	}
	source->addSeparator();
	source->addAction(actionFind);
	if(m_target.hasCompile()) target->addAction(actionCompile);
	if(m_target.hasDownload()) target->addAction(actionDownload);
	if(m_target.hasSimulate()) target->addAction(actionSimulate);
	if(m_target.hasRun()) target->addAction(actionRun);
	if(m_target.hasStop()) target->addAction(actionStop);
	if(m_target.hasDebug()) {
		target->addAction(actionDebug);
		source->addSeparator();
		source->addAction(actionToggleBreakpoint);
		source->addAction(actionClearBreakpoints);
	}
	target->addSeparator();
	target->addAction(actionChangeTarget);
	target->addAction(actionChoosePort);
	target->addSeparator();
	const QList<QAction*>& actionList = m_target.actionList();
	for(int i = 0; i < actionList.size(); ++i) target->insertAction(0, actionList[i]);
	if(actionList.size() > 0) target->addSeparator();
	bool hasExtra = false;
	if(m_target.hasRequestFile()) {
		target->addAction(actionScreenGrab);
		hasExtra = true;
	}
	if(m_target.hasRequestFile()) {
		target->addAction(actionRequestFile);
		hasExtra = true;
	}
	if(hasExtra) target->addSeparator();
	addActionsHelp(target);
}

void SourceFile::addToolbarActions(QToolBar* toolbar) 
{
	toolbar->addAction(actionSave);
	toolbar->addSeparator();
	toolbar->addAction(actionCopy);
	toolbar->addAction(actionCut);
	toolbar->addAction(actionPaste);
	toolbar->addAction(actionFind);
	toolbar->addSeparator();
	
	if(m_target.hasCompile()) toolbar->addAction(actionCompile);
	if(m_target.hasDownload()) toolbar->addAction(actionDownload);
	if(m_target.hasSimulate()) toolbar->addAction(actionSimulate);
	if(m_target.hasRun()) toolbar->addAction(actionRun);
	if(m_target.hasStop()) toolbar->addAction(actionStop);
	if(m_target.hasDebug()) toolbar->addAction(actionDebug);
}

bool SourceFile::beginSetup() { return changeTarget(isNewFile()) && !m_target.error(); }
void SourceFile::completeSetup() { MainWindow::ref().setTabName(this, m_fileInfo.fileName()); updateMargins(); }

bool SourceFile::close()
{
	if(!ui_editor->isModified()) return true;
	
	QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Unsaved Changes"),
		tr("Save Changes to \"") + fileName() + tr("\" before closing?"),
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

	if(ret == QMessageBox::Cancel) return false;
	if(ret == QMessageBox::Yes) {
		fileSave();
		if(ui_editor->isModified()) return false;
	}
	
	return true;
}

bool SourceFile::fileSave()
{
	if(m_isNewFile) {
		on_actionSaveAs_triggered();
		return true;
	}
	return fileSaveAs(m_fileInfo.filePath());
}

bool SourceFile::fileSaveAs(const QString& filePath)
{
	if(filePath.isEmpty()) return false;
	
	m_fileHandle.setFileName(filePath);
	m_fileInfo.setFile(m_fileHandle);
	
	if(!m_fileHandle.open(QIODevice::WriteOnly)) return false;

	if(ui_editor->text(ui_editor->lines()-1).length() > 0) ui_editor->append("\n");
		
	ui_editor->convertEols(QsciScintilla::EolUnix);

	QTextStream fileStream(&m_fileHandle);
	fileStream << ui_editor->text();
	m_fileHandle.close();
	ui_editor->setModified(false);

	m_isNewFile = false;
	
	MainWindow::ref().setTabName(this, m_fileInfo.fileName());
	
	// Update the lexer to the new spec for that extension
	Lexer::Constructor* constructor = Lexer::Factory::ref().constructor(m_fileInfo.suffix());
	if(Lexer::Factory::isLexerFromConstructor((Lexer::LexerBase*)ui_editor->lexer(), constructor))
		setLexer(constructor);
	
	return true;
}

bool SourceFile::fileOpen(const QString& filePath)
{
	m_fileHandle.setFileName(filePath);
	m_fileInfo.setFile(m_fileHandle);	
	
	if(!m_fileHandle.open(QIODevice::ReadOnly)) return false;

	QTextStream fileStream(&m_fileHandle);
	ui_editor->setText(fileStream.readAll());
	m_fileHandle.close();

	ui_editor->setModified(false);

	m_isNewFile = false;

	MainWindow::ref().setTabName(this, m_fileInfo.fileName());

	Lexer::Constructor* constructor = Lexer::Factory::ref().constructor(m_fileInfo.suffix());
	if(Lexer::Factory::isLexerFromConstructor((Lexer::LexerBase*)ui_editor->lexer(), constructor))
		setLexer(constructor);
		
	return true;
}

void SourceFile::indentAll()
{
	if(!ui_editor->lexer()) return;
	if(!m_target.cStyleBlocks()) return;
	
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
				else
					ui_editor->setCursorPosition(line, ui_editor->lineLength(line)-1);
				break;
			case Qt::Key_P:
				if(ui_editor->lineLength(line-1) < index)
					ui_editor->setCursorPosition(line-1, ui_editor->lineLength(line-1)-1);
				else
					ui_editor->setCursorPosition(line-1, index);
				break;
			case Qt::Key_N:
				if(ui_editor->lineLength(line+1) < index) {
					if(ui_editor->lines()-1 == line+1)
						ui_editor->setCursorPosition(line+1, ui_editor->lineLength(line));
					else
						ui_editor->setCursorPosition(line+1, ui_editor->lineLength(line+1)-1);
				}
				else
					ui_editor->setCursorPosition(line+1, index);
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
				else
					ui_editor->setCursorPosition(line, index+1);
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
		if(settings.value(API_SOURCE).toBool())
			ui_editor->setAutoCompletionSource(QsciScintilla::AcsAPIs);
		if(settings.value(DOC_SOURCE).toBool()) {
			if(ui_editor->autoCompletionSource() == QsciScintilla::AcsAPIs)
				ui_editor->setAutoCompletionSource(QsciScintilla::AcsAll);
			else
				ui_editor->setAutoCompletionSource(QsciScintilla::AcsDocument);
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
		
	m_debugger = settings.value(DEBUGGER_ENABLED).toBool();

	settings.endGroup();
	
	ui_editor->setLexer(ui_editor->lexer());
	
	updateMargins();
	ui_editor->setMarginsBackgroundColor(QColor(Qt::white));
	
	MainWindow::ref().refreshMenus();
}

QString SourceFile::fileName() 	{ return m_fileInfo.fileName(); }
QString SourceFile::filePath() 	{ return m_fileInfo.filePath(); }
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
		size = charWidth + charWidth/2 + charWidth * (int)ceil(log10(ui_editor->lines()+1));
	}
	ui_editor->setMarginWidth(0, size);
	ui_editor->setMarginWidth(1, 16);
}

int SourceFile::getZoom() { return ui_editor->SendScintilla(QsciScintilla::SCI_GETZOOM); }
QsciScintilla* SourceFile::getEditor() { return ui_editor; }
void SourceFile::moveTo(int line, int pos)  { if(line > 0 && pos >= 0) ui_editor->setCursorPosition(line - 1, pos); }

Target* SourceFile::target() { return &m_target; }
QsciScintilla* SourceFile::editor() { return ui_editor; }

void SourceFile::zoomIn() { ui_editor->zoomIn(); updateMargins(); }
void SourceFile::zoomOut() { ui_editor->zoomOut(); updateMargins(); }

void SourceFile::on_actionSaveAs_triggered()
{
	QSettings settings;
	QString savePath = settings.value(SAVE_PATH, QDir::homePath()).toString();
	QStringList exts = m_target.sourceExtensions();
	
	QRegExp reg("*." + m_target.defaultExtension() + "*");
	reg.setPatternSyntax(QRegExp::Wildcard);
	int i = exts.indexOf(reg);
	if(i != -1) exts.swap(0, i);
	
	if(!m_templateExt.isEmpty()) {
		QRegExp reg("*." + m_templateExt + "*");
		reg.setPatternSyntax(QRegExp::Wildcard);
		int i = exts.indexOf(reg);
		if(i != -1) exts.swap(0, i);
	}
	
	QString filePath = QFileDialog::getSaveFileName(&MainWindow::ref(), "Save File", savePath, 
		exts.join(";;") + (exts.size() < 1 ? "" : ";;") + "All Files (*)");
	if(filePath.isEmpty()) return;

	QFileInfo fileInfo(filePath);
	
	QString ext = m_target.defaultExtension();
	if(!ext.isEmpty() && fileInfo.suffix().isEmpty()) fileInfo.setFile(filePath.section(".", 0, 0) + "." + ext);
	
	settings.setValue(SAVE_PATH, fileInfo.absolutePath());

	/* Saves the file with the new fileName and updates the tabWidget label */
	if(fileSaveAs(fileInfo.absoluteFilePath())) {
		MainWindow::ref().setTabName(this, fileName());
		MainWindow::ref().setStatusMessage("Saved file \"" + fileName() + "\"");
	} else QMessageBox::critical(&MainWindow::ref(), "Error", "Error: Could not write file " + fileName());

	QStringList current = settings.value(RECENTS).toStringList().mid(0, 5);
	current.push_front(fileInfo.absoluteFilePath());
	current.removeDuplicates();
	settings.setValue(RECENTS, current);
}

void SourceFile::sourceModified(bool) { MainWindow::ref().setTabName(this, "* " + m_fileInfo.fileName()); }

void SourceFile::on_actionDownload_triggered()
{	
	fileSave();
	
	if(!checkPort()) return;
	
	ui_localCompileFailed->hide();
	
	MainWindow::ref().setStatusMessage("Downloading...");
	QApplication::flush();
	int message = m_target.download(filePath());
	MainWindow::ref().setStatusMessage(!message ? tr("Download Succeeded") : tr("Download Failed"));
	if(message == TargetInterface::CompileFailed) {
		ui_localCompileFailed->performAction(filePath());
	}
	
	updateErrors();
}

void SourceFile::on_actionCompile_triggered()
{
	ui_localCompileFailed->hide();
	
	fileSave();
	MainWindow::ref().hideErrors();
	MainWindow::ref().setStatusMessage(m_target.compile(filePath()) ? tr("Compile Succeeded") : tr("Compile Failed"));

	updateErrors();
}

void SourceFile::on_actionRun_triggered()
{	
	fileSave();
	
	if(!checkPort()) return;
	
	ui_localCompileFailed->hide();
	
	MainWindow::ref().hideErrors();
	bool success = false;
	MainWindow::ref().setStatusMessage((success = m_target.run(filePath())) ? tr("Run Succeeded") : tr("Run Failed"));
	
	if(m_runTab) {
		int i = MainWindow::ref().tabWidget()->indexOf(m_runTab);
		if(i >= 0) MainWindow::ref().deleteTab(i);
	}
	
	Tab* ui = success ? m_target.ui() : 0;
	m_runTab = !ui ? 0 : dynamic_cast<QWidget*>(ui);
	if(ui) {
		MainWindow::ref().addTab(ui);
		const QString& port = m_target.port();
		MainWindow::ref().setTabName(dynamic_cast<QWidget*>(ui), QString("Running") + (port.isEmpty() ? "" : (" on " + port)));
	}

	updateErrors();
}

void SourceFile::on_actionStop_triggered() { m_target.stop(); }

void SourceFile::on_actionSimulate_triggered()
{
	ui_localCompileFailed->hide();
	
	fileSave();
	MainWindow::ref().hideErrors();
	MainWindow::ref().setStatusMessage(m_target.simulate(filePath()) ? tr("Simulation Succeeded") : tr("Simulation Failed"));

	updateErrors();
}

void SourceFile::on_actionDebug_triggered()
{
	ui_localCompileFailed->hide();
	
	fileSave();
	MainWindow::ref().hideErrors();
	DebuggerInterface* interface = 0;
	QList<Location> bkpts;
	if(!m_debugger) {
		foreach(const int& i, m_breakpoints) {
			bkpts.append(Location(m_fileInfo.fileName(), ui_editor->markerLine(i) + 1));
		}
	}
	bool success = m_debugger ? !(interface = m_target.debug(filePath())) : m_target.debugConsole(filePath(), bkpts);
	MainWindow::ref().setStatusMessage(success ? tr("Debug Succeeded") : tr("Debug Failed"));
	updateErrors();
	if(!interface) return;
	foreach(const int& i, m_breakpoints) {
		// QScintilla starts at 0, while gdb starts at 1, + 1
		interface->addBreakpoint(m_fileInfo.fileName(), ui_editor->markerLine(i) + 1); 
	}
	
	SourceFileShared::ref().debugger()->startDebug(interface);
}

void SourceFile::on_actionCopy_triggered() 	{ ui_editor->copy(); }
void SourceFile::on_actionCut_triggered() 	{ ui_editor->cut(); }
void SourceFile::on_actionPaste_triggered() 	{ ui_editor->paste(); }
void SourceFile::on_actionUndo_triggered() 	{ ui_editor->undo(); }
void SourceFile::on_actionRedo_triggered() 	{ ui_editor->redo(); }
void SourceFile::on_actionFind_triggered() 	{ showFind(); }

void SourceFile::openManual()
{
	const QString& location = qobject_cast<QAction*>(sender())->data().toString();
	if(location.endsWith("pdf")) {
		QDesktopServices::openUrl(QUrl::fromUserInput(location));
		return;
	}
	WebTab* tab = new WebTab(&MainWindow::ref());
	MainWindow::ref().addTab(tab);
	#ifdef Q_OS_WIN
	tab->load("file:///" + location, true);
	#else
	tab->load("file://" + location, true);
	#endif
}

void SourceFile::on_actionPrint_triggered()
{
	QsciPrinter printer;
	QPrintDialog printDialog(&printer, this);
	
	if(printDialog.exec()) printer.printRange(ui_editor);
}

void SourceFile::on_actionZoomIn_triggered() { zoomIn(); }
void SourceFile::on_actionZoomOut_triggered() { zoomOut(); }
void SourceFile::on_actionResetZoomLevel_triggered() { ui_editor->zoomTo(0); updateMargins(); }
void SourceFile::on_actionChangeTarget_triggered() { changeTarget(false); }

void SourceFile::on_actionChoosePort_triggered()
{
	ChoosePortDialog pDialog(this);
	if(pDialog.exec()) m_target.setPort(pDialog.getSelectedPortName());
}

void SourceFile::on_actionScreenGrab_triggered()
{
	if(!checkPort()) return;
	QByteArray file = m_target.screenGrab();
	QSettings settings;
	QString savePath = settings.value(SAVE_PATH, QDir::homePath()).toString();
	QString filePath = QFileDialog::getExistingDirectory(&MainWindow::ref(), "Save Screen Grab", savePath, QFileDialog::ShowDirsOnly);
	if(filePath.isEmpty()) return;
	QString saveFilePath = filePath + "/Screen Grab on " + QDateTime::currentDateTime().toString() + ".jpg";
	QFile f(saveFilePath);
	if(!f.open(QIODevice::WriteOnly)) return;
	f.write(file);
	f.close();
	QDesktopServices::openUrl(QUrl::fromUserInput(saveFilePath));
}

void SourceFile::on_actionRequestFile_triggered()
{
	if(!checkPort()) return;
	RequestFileDialog dialog(&m_target);
	if(!dialog.exec()) return;
	QByteArray file = m_target.requestFile(m_target.requestFilePath() + "/" + dialog.selectedFile());
	QSettings settings;
	QString savePath = settings.value(SAVE_PATH, QDir::homePath()).toString();
	QString filePath = QFileDialog::getExistingDirectory(&MainWindow::ref(), "Save Remote File", savePath, QFileDialog::ShowDirsOnly);
	if(filePath.isEmpty()) return;
	QString saveFilePath = filePath + "/" + dialog.selectedFile();
	QFile f(saveFilePath);
	if(!f.open(QIODevice::WriteOnly)) return;
	f.write(file);
	f.close();
	MainWindow::ref().openFile(saveFilePath);
}

void SourceFile::on_actionToggleBreakpoint_triggered(bool checked)
{
	if(checked) m_breakpoints.append(ui_editor->markerAdd(m_currentLine, m_breakIndicator));
 	else {
		m_breakpoints.removeAll(ui_editor->markerLine(m_currentLine));
		ui_editor->markerDelete(m_currentLine, m_breakIndicator);
	}
	
	updateBreakpointToggle();
}

void SourceFile::on_actionClearBreakpoints_triggered()
{	
	foreach(const int& i, m_breakpoints) ui_editor->markerDeleteHandle(i);
	m_breakpoints.clear();
	
	actionToggleBreakpoint->setChecked(false);
}

void SourceFile::on_ui_editor_cursorPositionChanged(int line, int)
{
	m_currentLine = line;
	updateBreakpointToggle();
}

void SourceFile::showFind() { ui_find->show(); }

bool SourceFile::checkPort()
{
	if(m_target.hasPort() && m_target.port().isEmpty()) {
		on_actionChoosePort_triggered();
		if(m_target.port().isEmpty()) return false;
	}
	MainWindow::ref().refreshMenus();
	return true;
}

void SourceFile::setLexer(Lexer::Constructor* constructor)
{
	delete ui_editor->lexer();
	Lexer::LexerBase* lex = constructor->construct();
	ui_editor->setLexer(lex);
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

void SourceFile::updateErrors() 
{
	clearProblems();
	
	const QStringList& errors = m_target.errorMessages();
	const QStringList& warnings = m_target.warningMessages();
	
	MainWindow::ref().setErrors(this, errors, warnings,
				m_target.linkerMessages(),
				m_target.verboseMessages());
		
	MainWindow::ref().showErrors(this);

	markProblems(errors, warnings);
}

void SourceFile::updateBreakpointToggle()
{
	bool markerOnLine = false;
	foreach(const int& i, m_breakpoints) markerOnLine |= (ui_editor->markerLine(i) == m_currentLine);
	actionToggleBreakpoint->setChecked(markerOnLine);
}

bool SourceFile::changeTarget(bool _template)
{
	TemplateDialog tDialog(this);
	if((_template ? tDialog.exec() : tDialog.execTarget()) == QDialog::Rejected) return false;
	const QString& targetPath = tDialog.selectedTargetFilePath();
	if(!m_target.setTargetFile(targetPath)) {
		ErrorDialog::showError(this, "simple_error", QStringList() << 
			tr("Error loading target at for ") + targetPath <<
			tr("Target plugin was probably installed incorrectly"));
		return false;
	}
	
	
	QSettings targetSettings(targetPath, QSettings::IniFormat);
	
	/* Tells the settings dialog which target file to use */
	m_target.setTargetFile(targetPath);
	
	if(m_target.error()) return false;
	
	/* Pops up a port select dialog if the target should have a port set */
	if(m_target.hasPort()) {
		on_actionChoosePort_triggered();
		connect(&m_target, SIGNAL(requestPort()), SLOT(on_actionChangePort_triggered()));
	}
	
	Lexer::Constructor* constructor = 0;

	if(!_template) {
		qWarning() << m_fileInfo.completeSuffix();
		
		if(!isNewFile()) constructor = Lexer::Factory::ref().constructor(m_fileInfo.suffix());
		else constructor = Lexer::Factory::ref().constructor(targetSettings.value(DEFAULT_EXTENSION, "").toString());
	} else {
		QFile tFile(tDialog.templateFile());
		if(!tFile.open(QIODevice::ReadOnly)) {
			ErrorDialog::showError(this, "simple_error_with_action", QStringList() <<
				tr("Error loading template file ") + tDialog.templateFile() <<
				tr("Unable to open file for reading.") <<
				tr("Continuing without selected template."));
			return true;
		}
	
		QString str = QTextStream(&tFile).readAll();

		const MacroString* templateMacro = SourceFileShared::ref().templateMacro();
		str = templateMacro->apply(str);
		bool lexerSet = false;
		
		if(templateMacro->macroExists(str, KISS_LEXER, END_KISS)) {
			QString lex = templateMacro->macroArguments(str, KISS_LEXER, END_KISS)[0];
			qWarning() << "Template Lexer specified" << lex;
			constructor = Lexer::Factory::ref().constructor(lex);
			m_templateExt = lex;
			lexerSet = true;
		}
		ui_editor->setText(templateMacro->nonMeta(str, END_KISS).trimmed() + "\n");
		
		if(!lexerSet) constructor = Lexer::Factory::ref().constructor(targetSettings.value(DEFAULT_EXTENSION, "").toString());
	}
	
	m_lexAPI = QString(targetPath).replace(QString(".") + TARGET_EXT, ".api");
	m_targetName = QFileInfo(targetPath).baseName();
	
	setLexer(constructor);
	
	refreshSettings();
	
	return true;
}