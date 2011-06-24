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
#include <Qsci/qsciprinter.h>
#include <QPrinter>
#include <QPrintDialog>
#include <QDebug>
#include <math.h>

#include "MainWindow.h"
#include "WebTab.h"
#include "LexerSpecManager.h"

#define BREAKPOINT_MASK 0xDEAD

SourceFile::SourceFile(MainWindow* parent) : Tab(parent), m_fileHandle("Untitled"), m_isNewFile(true), m_target(this), m_debugger(this)
{
	setupUi(this);
	
	ui_editor->setModified(false);
	m_fileInfo.setFile(m_fileHandle);
	m_mainWindow->setStatusMessage("");
	ui_editor->setEolMode(QsciScintilla::EolUnix);
	
	m_debugger.hide();
	
	connect(actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(ui_editor, SIGNAL(textChanged()), this, SLOT(updateMargins()));
	connect(ui_editor, SIGNAL(modificationChanged(bool)), this, SLOT(sourceModified(bool)));
}

SourceFile::~SourceFile()
{
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
	edit->addSeparator();
	edit->addAction(actionZoomIn);
	edit->addAction(actionZoomOut);
	edit->addAction(actionResetZoomLevel);
	edit->addSeparator();
	edit->addAction(actionFind);
}

void SourceFile::addActionsHelp(QMenu* help)
{
	Q_UNUSED(help)
}

void SourceFile::addOtherActions(QMenuBar* menuBar)
{
	QMenu* target = menuBar->addMenu("Target");
	if(m_target.hasCompile()) target->addAction(actionCompile);
	if(m_target.hasDownload()) target->addAction(actionDownload);
	if(m_target.hasSimulate()) target->addAction(actionSimulate);
	if(m_target.hasRun()) target->addAction(actionRun);
	if(m_target.hasStop()) target->addAction(actionStop);
	if(m_target.hasDebug()) target->addAction(actionDebug);
	target->addSeparator();
	target->addAction(actionChangeTarget);
	target->addAction(actionChoosePort);
	target->addSeparator();
	QList<QAction*> actionList = m_target.getActionList();
	for(int i = 0; i < actionList.size(); ++i) target->insertAction(0, actionList[i]);
	if(actionList.size() > 0) target->addSeparator();
	target->addAction(actionManual);
}

void SourceFile::addToolbarActions(QToolBar* toolbar) 
{
	toolbar->addAction(actionSave);
	toolbar->addAction(actionPrint);
	toolbar->addSeparator();
	toolbar->addAction(actionCopy);
	toolbar->addAction(actionCut);
	toolbar->addAction(actionPaste);
	toolbar->addSeparator();
	toolbar->addAction(actionUndo);
	toolbar->addAction(actionRedo);
	toolbar->addSeparator();
	toolbar->addAction(actionFind);
	toolbar->addSeparator();
	
	if(m_target.hasCompile()) toolbar->addAction(actionCompile);
	if(m_target.hasDownload()) toolbar->addAction(actionDownload);
	if(m_target.hasSimulate()) toolbar->addAction(actionSimulate);
	if(m_target.hasRun()) toolbar->addAction(actionRun);
	if(m_target.hasStop()) toolbar->addAction(actionStop);
	if(m_target.hasDebug()) toolbar->addAction(actionDebug);
}

bool SourceFile::beginSetup()
{
	ChooseTargetDialog* tDialog = m_mainWindow->chooseTargetDialog();
	if(!tDialog->exec()) return false;
	if(!m_target.setTargetFile(tDialog->getSelectedTargetFilePath())) {
		QMessageBox::critical(this, "Error", "Error loading target!");
		return false;
	}
	
	QSettings targetSettings(tDialog->getSelectedTargetFilePath(), QSettings::IniFormat);

	/* Tells the settings dialog which target file to use */
	m_target.setTargetFile(tDialog->getSelectedTargetFilePath());
	
	/* Adds custom buttons to the toolbar */
	
	
	/* Pops up a port select dialog if the target should have a port set */
	if(targetSettings.value("port_dialog").toBool()) {
		on_actionChoosePort_triggered();
		connect(&m_target, SIGNAL(requestPort()), SLOT(on_actionChangePort_triggered()));
	}

	/* Sets up the lexer for the target */
	m_lexSpec = LexerSpecManager::ref().lexerSpec(targetSettings.value("default_extension", "").toString());
	
	if(!m_lexSpec) qWarning() << "Target did not supply a LexerSpec.";

	/* Sets the api file for the new target */
	m_lexAPI = tDialog->getSelectedTargetFilePath().replace(".target",".api");
	
	refreshSettings();
	return true;
	
}

void SourceFile::completeSetup()
{
	m_mainWindow->setTabName(this, m_fileInfo.fileName());
	updateMargins();
}

bool SourceFile::close()
{
	if(ui_editor->isModified()) {
		QMessageBox::StandardButton ret = QMessageBox::question(this, "Unsaved Changes",
			"Save Changes to \"" + fileName() + "\" before closing?",
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		if(ret == QMessageBox::Cancel) return false;
		if(ret == QMessageBox::Yes) {
			fileSave();
			if(ui_editor->isModified()) return false;
		}
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

bool SourceFile::fileSaveAs(QString filePath)
{
	if(filePath.isEmpty())
		return false;
	
	m_fileHandle.setFileName(filePath);
	m_fileInfo.setFile(m_fileHandle);
	
	if(!m_fileHandle.open(QIODevice::WriteOnly))
		return false;

	if(ui_editor->text(ui_editor->lines()-1).length() > 0) ui_editor->append("\n");
		
	ui_editor->convertEols(QsciScintilla::EolUnix);

	QTextStream fileStream(&m_fileHandle);
	fileStream << ui_editor->text();
	m_fileHandle.close();
	ui_editor->setModified(false);

	m_isNewFile = false;
	
	m_mainWindow->setTabName(this, m_fileInfo.fileName());
	
	// Update the lexer to the new spec for that extension
	LexerSpec* lexerSpec = LexerSpecManager::ref().lexerSpec(m_fileInfo.completeSuffix());
	if(lexerSpec != m_lexSpec) {
		m_lexSpec = lexerSpec;
		refreshSettings();
		updateMargins();
	}
	
	return true;
}

bool SourceFile::fileOpen(QString filePath)
{
	m_fileHandle.setFileName(filePath);
	m_fileInfo.setFile(m_fileHandle);	
	
	if(!m_fileHandle.open(QIODevice::ReadOnly))
		return false;

	QTextStream fileStream(&m_fileHandle);
	ui_editor->setText(fileStream.readAll());
	m_fileHandle.close();

	ui_editor->setModified(false);

	m_isNewFile = false;

	m_mainWindow->setTabName(this, m_fileInfo.fileName());
	
	m_breakpointMarker = ui_editor->markerDefine('*');
	ui_editor->markerAdd(1, m_breakpointMarker);

	// Update the lexer to the new spec for that extension
	LexerSpec* lexerSpec = LexerSpecManager::ref().lexerSpec(m_fileInfo.completeSuffix());
	if(lexerSpec != m_lexSpec) {
		m_lexSpec = lexerSpec;
		refreshSettings();
		updateMargins();
	}

	return true;
}

void SourceFile::indentAll()
{
	if(!ui_editor->lexer())
		return;

	int indentLevel = 0;
	int blockStartStyle; ui_editor->lexer()->blockStart(&blockStartStyle);
	int blockEndStyle; ui_editor->lexer()->blockEnd(&blockEndStyle);
	
	int currentLine;
	int currentIndex;

	ui_editor->getCursorPosition(&currentLine,&currentIndex);
	ui_editor->append(" ");

	QString outDocument;

	for(int i = 0, pos =0;i < ui_editor->lines();i++) {
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
	ui_editor->setText(outDocument);
	
	ui_editor->setCursorPosition(currentLine, currentIndex);
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
	/* Get rid of the old lexer */
	QsciLexer *lexer = ui_editor->lexer();
	ui_editor->setLexer(0); delete lexer; lexer = 0;
	
	/* Get a new lexer if a spec is defined */
	if(m_lexSpec) lexer = new Lexer(m_lexSpec, m_lexAPI);
	
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
		ui_editor->setFont(defFont);

	/* Set other options from settings */
	settings.beginGroup("autoindent");
	ui_editor->setAutoIndent(settings.value("enabled").toBool());
	if(lexer && settings.value("style").toString() == "Maintain")
		lexer->setAutoIndentStyle(QsciScintilla::AiMaintain);
	else if(lexer && settings.value("style").toString() == "Intelligent")
		lexer->setAutoIndentStyle(0);  // for some reason 0 is the intelligent style
	ui_editor->setTabWidth(settings.value("width").toInt());
	settings.endGroup();

	settings.beginGroup("autocompletion");
	ui_editor->setAutoCompletionSource(QsciScintilla::AcsNone);
	if(settings.value("enabled").toBool()) {
		if(settings.value("apisource").toBool())
			ui_editor->setAutoCompletionSource(QsciScintilla::AcsAPIs);
		if(settings.value("docsource").toBool()) {
			if(ui_editor->autoCompletionSource() == QsciScintilla::AcsAPIs)
				ui_editor->setAutoCompletionSource(QsciScintilla::AcsAll);
			else
				ui_editor->setAutoCompletionSource(QsciScintilla::AcsDocument);
		}
	}
	ui_editor->setAutoCompletionThreshold(settings.value("threshold").toInt());
	settings.endGroup();

	if(settings.value("linenumbers").toBool()){
		ui_editor->setMarginLineNumbers(0, true);
		updateMargins();
	}
	else ui_editor->setMarginLineNumbers(0, false);
	
	ui_editor->setMarginLineNumbers(1, false);
	ui_editor->setMarginMarkerMask(1, m_breakpointMarker);
	
	if(settings.value("bracematching").toBool())
		 ui_editor->setBraceMatching(QsciScintilla::StrictBraceMatch);
	else ui_editor->setBraceMatching(QsciScintilla::NoBraceMatch);
	
	if(settings.value("calltips").toBool())
		 ui_editor->setCallTipsStyle(QsciScintilla::CallTipsNoContext);
	else ui_editor->setCallTipsStyle(QsciScintilla::CallTipsNone);

	settings.endGroup();
	// qWarning() << lexer->defaultColor(LexerCPP::Keyword).name();

	ui_editor->setLexer(lexer);
	qWarning() << "Updated Lexer! " << lexer;
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
	int charWidth = 6;
	if(ui_editor->lexer()) {
		QFont font = ui_editor->lexer()->defaultFont();
		font.setPointSize(font.pointSize() + getZoom());
		charWidth = QFontMetrics(font).width("0");
	}
	ui_editor->setMarginWidth(0, charWidth + charWidth/2 + charWidth * (int)ceil(log10(ui_editor->lines()+1)));
	ui_editor->setMarginWidth(1, charWidth + charWidth/2 + charWidth * (int)ceil(log10(ui_editor->lines()+1)));
}

int SourceFile::getZoom()
{
	return ui_editor->SendScintilla(QsciScintilla::SCI_GETZOOM);
}

QsciScintilla* SourceFile::getEditor()
{
	return ui_editor;
}

void SourceFile::moveTo(int line, int pos) 
{
	if(line > 0 && pos >= 0) ui_editor->setCursorPosition(line - 1, pos);
}

void SourceFile::zoomIn()
{
	updateMargins();
	ui_editor->zoomIn();
}

void SourceFile::zoomOut()
{
	updateMargins();
	ui_editor->zoomOut();
}

void SourceFile::on_actionSaveAs_triggered()
{
	QSettings settings;
	QString savePath = settings.value("savepath", QDir::homePath()).toString();
	QString filePath = QFileDialog::getSaveFileName(m_mainWindow, "Save File", savePath, m_target.getSourceExtensions().join(";;") + ";;All Files (*)");
	if(filePath.isEmpty())
		return;

	QFileInfo fileInfo(filePath);
	
	QString ext = m_target.getDefaultExtension();
	if(!ext.isEmpty() && fileInfo.suffix().isEmpty()) fileInfo.setFile(filePath + "." + ext);
	
	settings.setValue("savepath", fileInfo.absolutePath());

	/* Saves the file with the new fileName and updates the tabWidget label */
	if(fileSaveAs(fileInfo.absoluteFilePath())) {
		m_mainWindow->setTabName(this, fileName());
		m_mainWindow->setStatusMessage("Saved file\"" + fileName() + "\"");
	} else QMessageBox::critical(m_mainWindow, "Error", "Error: Could not write file " + fileName());
}

void SourceFile::sourceModified(bool m) {
	m_mainWindow->setTabName(this, "* " + m_fileInfo.fileName());
}

void SourceFile::on_actionDownload_triggered()
{	
	/* Save the file and hide the error view (in case this is not the first attempt */
	fileSave();
	
	m_mainWindow->setStatusMessage("Downloading...");
	QApplication::flush();
	
	if(!m_target.download(filePath()))
		m_mainWindow->setStatusMessage("Download Failed");
	else
		m_mainWindow->setStatusMessage("Download Succeeded");
	
	/* Show any error messages if any */
	m_mainWindow->setErrors(this,
				m_target.getErrorMessages(),
				m_target.getWarningMessages(),
				m_target.getLinkerMessages(),
				m_target.getVerboseMessages());
}

void SourceFile::on_actionCompile_triggered()
{
	fileSave();
	m_mainWindow->hideErrors();
	
	if(!m_target.compile(filePath()))
		m_mainWindow->setStatusMessage("Compile Failed");
	else
		m_mainWindow->setStatusMessage("Compile Succeeded");

	/* Show error messages if there are any */
	m_mainWindow->setErrors(this,
				m_target.getErrorMessages(),
				m_target.getWarningMessages(),
				m_target.getLinkerMessages(),
				m_target.getVerboseMessages());
}

void SourceFile::on_actionRun_triggered()
{	
	fileSave();
	m_mainWindow->hideErrors();
	
	if(!m_target.run(filePath()))
		m_mainWindow->setStatusMessage("Run Failed");
	else
		m_mainWindow->setStatusMessage("Run Succeeded");

	m_mainWindow->setErrors(this,
				m_target.getErrorMessages(),
				m_target.getWarningMessages(),
				m_target.getLinkerMessages(),
				m_target.getVerboseMessages());
}

void SourceFile::on_actionStop_triggered()
{
	m_target.stop();
}

void SourceFile::on_actionSimulate_triggered()
{	
	/* Save the file and hide the error view */
	fileSave();
	m_mainWindow->hideErrors();
	
	if(!m_target.simulate(filePath()))
		m_mainWindow->setStatusMessage("Simulation Failed");
	else
		m_mainWindow->setStatusMessage("Simulation Succeeded");

	m_mainWindow->setErrors(this,
				m_target.getErrorMessages(),
				m_target.getWarningMessages(),
				m_target.getLinkerMessages(),
				m_target.getVerboseMessages());
}

void SourceFile::on_actionDebug_triggered()
{
	fileSave();
	m_mainWindow->hideErrors();
	
	DebuggerInterface* interface = m_target.debug(filePath());
	
	if(!interface) {
		m_mainWindow->setStatusMessage("Debug Failed");
		
	} else {
		m_mainWindow->setStatusMessage("Debug Succeeded");
	}
	
	m_mainWindow->setErrors(this,
				m_target.getErrorMessages(),
				m_target.getWarningMessages(),
				m_target.getLinkerMessages(),
				m_target.getVerboseMessages());

	m_debugger.startDebug(interface, this);
}

void SourceFile::on_actionCopy_triggered()
{
	ui_editor->copy();
}

void SourceFile::on_actionCut_triggered()
{
	ui_editor->cut();
}

void SourceFile::on_actionPaste_triggered()
{
	ui_editor->paste();
}

void SourceFile::on_actionUndo_triggered()
{
	ui_editor->undo();
}

void SourceFile::on_actionRedo_triggered()
{
	ui_editor->redo();
}

void SourceFile::on_actionFind_triggered()
{
	m_mainWindow->showFindDialog(this);
}

void SourceFile::on_actionManual_triggered()
{
	WebTab* tab = new WebTab(m_mainWindow);
	m_mainWindow->addTab(tab);
	tab->load("file://" + m_target.getTargetManualPath(), true);
}

void SourceFile::on_actionPrint_triggered()
{
	QsciPrinter printer;
	QPrintDialog printDialog(&printer, this);
	
	if(printDialog.exec() == QDialog::Accepted) printer.printRange(ui_editor);
}

void SourceFile::on_actionZoomIn_triggered()
{
	ui_editor->zoomIn();
	updateMargins();
}

void SourceFile::on_actionZoomOut_triggered()
{
	ui_editor->zoomOut();
	updateMargins();
}

void SourceFile::on_actionResetZoomLevel_triggered()
{
	ui_editor->zoomTo(0);
	updateMargins();
}

void SourceFile::on_actionChangeTarget_triggered()
{
	ChooseTargetDialog* tDialog = m_mainWindow->chooseTargetDialog();
	if(!tDialog->exec()) return;
	if(!m_target.setTargetFile(tDialog->getSelectedTargetFilePath())) {
		QMessageBox::critical(this, "Error", "Error loading target!");
		return;
	}
	
	QSettings targetSettings(tDialog->getSelectedTargetFilePath(), QSettings::IniFormat);

	/* Tells the settings dialog which target file to use */
	m_target.setTargetFile(tDialog->getSelectedTargetFilePath());
	
	/* Pops up a port select dialog if the target should have a port set */
	if(targetSettings.value("port_dialog").toBool()) {
		on_actionChoosePort_triggered();
		connect(&m_target, SIGNAL(requestPort()), SLOT(on_actionChangePort_triggered()));
	}

	/* Sets the api file for the new target */
	m_lexAPI = tDialog->getSelectedTargetFilePath().replace(".target",".api");
	
	refreshSettings();
}

void SourceFile::on_actionChoosePort_triggered()
{
	ChoosePortDialog* pDialog = m_mainWindow->choosePortDialog();
	if(pDialog->exec()) {
		m_target.setPort(pDialog->getSelectedPortName());
	}
}

/*ADDED BY NB*///2/10/2010-dpm
void SourceFile::dropEvent(QDropEvent *event)
{
    /*Emit signal that can be connected to any slot(s) desired*/
   // emit handleDrop(event);

}
