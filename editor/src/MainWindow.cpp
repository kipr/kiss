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

#include "MainWindow.h"
#include "Version.h"
#include "Lexer.h"
#include "LexerSpec.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciprinter.h>
#include <QMessageBox>
#include <QCloseEvent>
#include <QInputDialog>
#include <QDir>
#include <QFileDialog>
#include <QMenu>
#include <QAction>
#include <QWidgetList>
#include <QList>
#include <QDebug>
#include <QPrintDialog>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <shellapi.h>
#endif

/* Constructor */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_editorSettingsDialog(this),
										  m_targetSettingsDialog(this), m_chooseTargetDialog(this), 
										  m_choosePortDialog(this), m_findDialog(this),
										  m_target(this), m_currentSourceFile(0)
{
	/* Sets all up all the widgets drawn in QT Designer */
	setupUi(this);

	/* Sets up the QSplitter proportions */
	ui_splitter->setSizes(QList<int>() << 200 << 50);

	/* Turns off updates so all of these things are drawn at once */
	setUpdatesEnabled(false);

	/* Hide the target tool bar since it's empty (no target selected yet) */
	ui_targetToolBar->setVisible(false);

	/* Sets up the QTabWidget that handles the editor windows */
	QToolButton *cornerButton = new QToolButton();
	cornerButton->setDefaultAction(actionShortcutClose);
	cornerButton->setAutoRaise(true);
	ui_tabWidget->setCornerWidget(cornerButton);

	/* Deletes the tab that QTabWidget starts with by default */
	deleteTab(0);

	/* Sets up the errorView tab widget which displays compiler errors */
	ui_errorView->hide();
	ui_errorView->setCornerWidget(new QPushButton("Verbose"));
	connect(ui_errorView->cornerWidget(), SIGNAL(clicked()), this, SLOT(errorViewShowVerbose()));
	
	/* Connect the error widgets to the error clicked slot */
	connect(&m_errorList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(errorClicked(QListWidgetItem*)));
	connect(&m_warningList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(errorClicked(QListWidgetItem*)));
	
	/* This is for OS X, binds the editor settings to preferences */
#ifdef Q_OS_MAC
	QAction *action = new QAction(this);
	action->setMenuRole(QAction::PreferencesRole);
	connect(action, SIGNAL(triggered(bool)), actionEditor_Settings, SIGNAL(triggered(bool)));
	ui_menuSettings->addAction(action);
#endif

	/* Show all of these changes */
	setUpdatesEnabled(true);

}

/* Destructor */
MainWindow::~MainWindow()
{
	while(ui_tabWidget->count() > 0)
		deleteTab(0);
}

/****************************
 *     File Menu slots      *
 ****************************/

void MainWindow::on_actionNew_triggered(bool)
{
	addTab(new SourceFile());
	
	m_currentSourceFile->setText(m_target.getSourceTemplate());
	m_currentSourceFile->setCursorPosition(m_currentSourceFile->lines(), 0);
}

void MainWindow::on_actionOpen_triggered(bool)
{
	/* Request a file to open from the user, looking in the last used save path first */
	QSettings settings;
	QString openPath = settings.value("openpath", QDir::homePath()).toString();
	QString filePath = QFileDialog::getOpenFileName(this, "Open File", openPath, 
													m_target.getSourceExtensions() + "All Files (*)");
	if(filePath.isEmpty())
		return;

	QFileInfo fileInfo(filePath);
	settings.setValue("openpath", fileInfo.absolutePath());

	openFile(filePath);
}

void MainWindow::on_actionSave_triggered(bool)
{
	/* Makes sure there's an editor window open */
	if(!m_currentSourceFile)
		return;	
	
	/* Checks if the current file is new, opens save as dialog if so, otherwise just saves */
	if(m_currentSourceFile->isNewFile())
		on_actionSave_As_triggered();
	else if(m_currentSourceFile->fileSave())
		statusBar()->showMessage("Saved file \"" + m_currentSourceFile->fileName() + "\"");
	else
		QMessageBox::critical(this, "Error", "Error: Could not write file " + m_currentSourceFile->fileName());
}

void MainWindow::on_actionSave_As_triggered(bool)
{
	/* Make sure there's a file currently open first */
	if(!m_currentSourceFile)
		return;

	/* Brings up a save dialog using the last used save path as the starting point */
	QSettings settings;
	QString savePath = settings.value("savepath", QDir::homePath()).toString();
	QString filePath = QFileDialog::getSaveFileName(this, "Save File", savePath,
													m_target.getSourceExtensions() + "All Files (*)");
	if(filePath.isEmpty())
		return;

	QFileInfo fileInfo(filePath);
	
	if(fileInfo.suffix() != "c")
		fileInfo.setFile(filePath + ".c");
	
	settings.setValue("savepath", fileInfo.absolutePath());

	/* Saves the file with the new fileName and updates the tabWidget label */
	if(m_currentSourceFile->fileSaveAs(filePath)) {
		ui_tabWidget->setTabText(ui_tabWidget->currentIndex(), m_currentSourceFile->fileName());
		statusBar()->showMessage("Saved file\"" + m_currentSourceFile->fileName() + "\"");
	}
	else
		QMessageBox::critical(this, "Error", "Error: Could not write file " + m_currentSourceFile->fileName());
}

void MainWindow::on_actionClose_triggered(bool)
{
	/* Make sure there's a file currently open first */
	if(!m_currentSourceFile)
		return;

	/* Ask the user to save if the file is modified */
	if(m_currentSourceFile->isModified()) {
		QMessageBox::StandardButton ret = QMessageBox::question(this, "Unsaved Changes",
										"Save Changes to \"" + m_currentSourceFile->fileName() + "\" before closing?",
										QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		if(ret == QMessageBox::Cancel)
			return;
		if(ret == QMessageBox::Yes) {
			on_actionSave_triggered();
			/* Break out if the file is still modified after trying to save */
			if(m_currentSourceFile->isModified())
				return;
		}
	}
	/* Close the current editor tab */
	m_currentSourceFile = 0;
  m_target.setCurrentFile(0);
	deleteTab(ui_tabWidget->currentIndex());
	ui_errorView->hide();
}

void MainWindow::on_actionPrint_triggered(bool)
{
	if(!m_currentSourceFile)
		return;

	QsciPrinter printer;
	QPrintDialog printDialog(&printer, this);
	
	if(printDialog.exec() == QDialog::Accepted)
		printer.printRange(m_currentSourceFile);
}


/***************************
 *     Edit Menu slots     *
 ***************************/

void MainWindow::on_actionUndo_triggered(bool)
{
	if(m_currentSourceFile)
		m_currentSourceFile->undo();
}

void MainWindow::on_actionRedo_triggered(bool)
{
	if(m_currentSourceFile)
		m_currentSourceFile->redo();
}

void MainWindow::on_actionCut_triggered(bool)
{
	if(m_currentSourceFile)
		m_currentSourceFile->cut();
}

void MainWindow::on_actionCopy_triggered(bool)
{
	if(m_currentSourceFile)
		m_currentSourceFile->copy();
}

void MainWindow::on_actionPaste_triggered(bool)
{
	if(m_currentSourceFile)
		m_currentSourceFile->paste();
}

void MainWindow::on_actionSelect_All_triggered(bool)
{
	if(m_currentSourceFile)
		m_currentSourceFile->selectAll();
}

void MainWindow::on_actionFind_triggered(bool)
{
	if(m_currentSourceFile)
		m_findDialog.show();
}

void MainWindow::on_actionFind_Next_triggered(bool)
{
	if(m_currentSourceFile)
		m_currentSourceFile->findNext();
}

void MainWindow::on_actionGoto_Line_triggered(bool)
{
	/* Make sure there's an editor window open */
	if(!m_currentSourceFile)
		return;

	/* Ask for the line number */
	bool hitOk;
	int line = QInputDialog::getInteger(this, "Goto Line", "Line Number", 1, 1, m_currentSourceFile->lines(), 1, &hitOk);

	/* Move to the line number selected */
	if(hitOk)
		m_currentSourceFile->setCursorPosition(line-1,0);
}

void MainWindow::on_actionIndent_All_triggered(bool)
{
	if(!m_currentSourceFile)
		return;
	m_currentSourceFile->indentAll();
}

void MainWindow::on_actionZoom_In_triggered(bool)
{
	if(m_currentSourceFile)
		m_currentSourceFile->zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered(bool)
{
	if(m_currentSourceFile)
		m_currentSourceFile->zoomOut();
}

/*******************************
 *     Settings Menu slots     *
 *******************************/

void MainWindow::on_actionChange_Target_triggered(bool)
{
	/* Open the target dialog, and continue only if a target is selected */
	if(!m_chooseTargetDialog.exec())
		return;

	ui_targetToolBar->clear();
	ui_targetToolBar->setVisible(false);
	
	/* Attempt to load the selected target */
	if(!m_target.setTargetFile(m_chooseTargetDialog.getSelectedTargetFilePath())) {
		QMessageBox::critical(this, "Error", "Error loading target!");
		return;
	}

	QSettings targetSettings(m_chooseTargetDialog.getSelectedTargetFilePath(), QSettings::IniFormat);

	/* Tells the settings dialog which target file to use */
	m_targetSettingsDialog.setTargetFile(m_chooseTargetDialog.getSelectedTargetFilePath());
	
	/* Determines which default buttons should go on the tool bar */
	if(m_target.hasCompile())
		ui_targetToolBar->insertAction(0,actionCompile);
	if(m_target.hasDownload())
		ui_targetToolBar->insertAction(0,actionDownload);
	if(m_target.hasSimulate())
	  ui_targetToolBar->insertAction(0,actionSimulate);
	if(m_target.hasRun())
		ui_targetToolBar->insertAction(0,actionRun);
	if(m_target.hasStop())
		ui_targetToolBar->insertAction(0,actionStop);
	ui_targetToolBar->setVisible(true);
	
	/* Adds custom buttons to the toolbar */
	QList<QAction*> actionList = m_target.getActionList();
	int i;
	
	for(i = 0;i < actionList.size();i++)
		ui_targetToolBar->insertAction(0, actionList[i]);
	
	/* Pops up a port select dialog if the target should have a port set */
	if(targetSettings.value("port_dialog").toBool())
		on_actionChange_Port_triggered();
	
	/* Makes a connection that allows the target to ask for a serial port */
	connect(&m_target, SIGNAL(requestPort()), SLOT(on_actionChange_Port_triggered()));

	/* Sets up the lexer for the target */
	Lexer::setLexerSpec(m_target.getLexerSpec());

	/* Sets the api file for the new target */
	Lexer::setAPIFile(m_chooseTargetDialog.getSelectedTargetFilePath().replace(".target",".api"));
	
	/* Finally, makes sure that all of the currently open editor window will recognize the new target */
	emit settingsUpdated();
}

void MainWindow::on_actionChange_Port_triggered(bool)
{
	/* Opens the port dialog and sets the target's port to whatever the user selected */
	if(m_choosePortDialog.exec()) {
		m_target.setPort(m_choosePortDialog.getSelectedPortName());
	}
}

void MainWindow::on_actionEditor_Settings_triggered(bool)
{
	/* opens the editor dialog and emits a settingsUpdated signal if needed */
	if(m_editorSettingsDialog.exec() == QDialog::Accepted)
		emit settingsUpdated();
}

void MainWindow::on_actionTarget_Settings_triggered(bool)
{
	/* FIXME UNFINISHED */
	m_targetSettingsDialog.exec();
}

/***************************
 *     Help Menu Slots     *
 ***************************/

void MainWindow::on_actionAbout_triggered(bool)
{
	QString aboutString;

	aboutString += "KISS Version " + QString::number(KISS_C_VERSION_MAJOR) + "." + 
									   QString::number(KISS_C_VERSION_MINOR) + "." +
									   QString::number(KISS_C_VERSION_BUILD) + "\n\n";
	aboutString += "Copyright (C) 2007-2009 KISS Institute for Practical Robotics\n\n";
	aboutString += "http://www.kipr.org/\nhttp://www.botball.org/\n\n";
	aboutString += "KISS Written by Jorge Villatoro";
	QMessageBox::about(this, "KIPR's Instructional Software System", aboutString);
}

void MainWindow::on_actionTarget_Manual_triggered(bool)
{
	QString targetManualUrl = "file://" + m_target.getTargetManualPath();

#ifdef Q_OS_WIN32

	targetManualUrl += '\0';
	wchar_t *buf;
	buf = new wchar_t[targetManualUrl.length()];
	targetManualUrl.toWCharArray(buf);
	
	ShellExecute(this->winId(), NULL, buf, NULL, NULL, SW_SHOW);

	delete [] buf;

#elif defined(Q_OS_MAC)
	QString cmd = "open \"" + targetManualUrl + "\"";
	system(cmd.toLocal8Bit());
#else
	QString cmd = "firefox \"" + targetManualUrl + "\"";
	system(cmd.toLocal8Bit());
#endif
}

/********************************
 *     Target ToolBar Slots     *
 ********************************/

void MainWindow::on_actionDownload_triggered(bool)
{
	/* Make sure there's a file to download */
	if(!m_currentSourceFile)
		return;
	
	/* Save the file and hide the error view (in case this is not the first attempt */
	on_actionSave_triggered();
	ui_errorView->hide();
	
	statusBar()->showMessage("Downloading...");
    QApplication::flush();
	
	if(!m_target.download(m_currentSourceFile->filePath()))
		statusBar()->showMessage("Download Failed");
	else
		statusBar()->showMessage("Download Succeeded");
	
	/* Show any error messages if any */
	readErrorMessages();
	errorViewShowSimple();
	ui_errorView->setProperty("currentlyAssociatedTab", QVariant(ui_tabWidget->currentIndex()));
}

void MainWindow::on_actionCompile_triggered(bool)
{
	/* Make sure there's a file to be compiled */
	if(!m_currentSourceFile)
		return;

	/* Save the file and hide the error window */
	on_actionSave_triggered();
	ui_errorView->hide();
	
	if(!m_target.compile(m_currentSourceFile->filePath()))
		statusBar()->showMessage("Compile Failed");
	else
		statusBar()->showMessage("Compile Succeeded");

	/* Show error messages if there are any */
	readErrorMessages();
	errorViewShowSimple();
	ui_errorView->setProperty("currentlyAssociatedTab", QVariant(ui_tabWidget->currentIndex()));
}

void MainWindow::on_actionRun_triggered(bool)
{
	/* Make sure there's a file to run */
	if(!m_currentSourceFile)
		return;
	
	/* Save the file and hide the error view */
	on_actionSave_triggered();
	ui_errorView->hide();
	
	if(!m_target.run(m_currentSourceFile->filePath()))
		statusBar()->showMessage("Run Failed");
	else
		statusBar()->showMessage("Run Succeeded");

	/* Show any error messages */
	readErrorMessages();
	showErrorMessages();
	ui_errorView->setProperty("currentlyAssociatedTab", QVariant(ui_tabWidget->currentIndex()));
}

void MainWindow::on_actionStop_triggered(bool)
{
	m_target.stop();
}

void MainWindow::on_actionSimulate_triggered(bool)
{
	/* Make sure there's a file to run */
	if(!m_currentSourceFile)
		return;
	
	/* Save the file and hide the error view */
	on_actionSave_triggered();
	ui_errorView->hide();
	
	if(!m_target.simulate(m_currentSourceFile->filePath()))
		statusBar()->showMessage("Simulate Failed");
	else
		statusBar()->showMessage("Simulate Succeeded");

	/* Show any error messages */
	readErrorMessages();
	showErrorMessages();
	ui_errorView->setProperty("currentlyAssociatedTab", QVariant(ui_tabWidget->currentIndex()));
}

/***********************
 *     Widget slots    *
 ***********************/

void MainWindow::on_ui_tabWidget_currentChanged(int index)
{
	/* The following disconnects the modification state signal
	 * and the status bar signal that keeps the source file
	 * up to date on the status for that file 				*/
	if(m_currentSourceFile) {
		disconnect(m_currentSourceFile, SIGNAL(modificationChanged(bool)), this, SLOT(sourceFileModified(bool)));
		disconnect(statusBar(), SIGNAL(messageChanged(QString)), m_currentSourceFile, SLOT(setStatusMessage(QString)));
	}

	if(ui_tabWidget->count() == 0)
		return;

	m_currentSourceFile = (SourceFile*)ui_tabWidget->widget(index);

	/* Connects the previously mentioned signals to the new editor widget */
	connect(m_currentSourceFile, SIGNAL(modificationChanged(bool)), this, SLOT(sourceFileModified(bool)));
	connect(statusBar(), SIGNAL(messageChanged(QString)), m_currentSourceFile, SLOT(setStatusMessage(QString)));

	/* Gives the new editor widget focus */
	m_currentSourceFile->setFocus(Qt::OtherFocusReason);

	/* Tells the find dialog which source file to use when searching */
	m_findDialog.setSourceFile(m_currentSourceFile);
	
	/* Update the status bar message for the new editor widget */
	statusBar()->clearMessage();
	statusBar()->showMessage(m_currentSourceFile->statusMessage());

	/* Checks to see if the error view is associated with this tab, and displays it if so */
	QVariant currentlyAssociated = ui_errorView->property("currentlyAssociatedTab");
	if(currentlyAssociated.isValid() && currentlyAssociated.toInt() == ui_tabWidget->currentIndex()) {
		if(ui_errorView->count() > 0)
			ui_errorView->show();
	}
	else
		ui_errorView->hide();
		
	m_target.setCurrentFile(m_currentSourceFile);
}

/********************
 *    Misc Slots    *
 ********************/

void MainWindow::sourceFileModified(bool m)
{
	int index = ui_tabWidget->currentIndex();
	
	if(m) {
		ui_tabWidget->setTabText(index, "*" + m_currentSourceFile->fileName());
		statusBar()->showMessage("Unsaved");
	}
	else
		ui_tabWidget->setTabText(index, m_currentSourceFile->fileName());
}

void MainWindow::errorViewShowVerbose()
{
	ui_errorView->setCornerWidget(new QPushButton("Simple"));
	ui_errorView->cornerWidget()->show();
	connect(ui_errorView->cornerWidget(), SIGNAL(clicked()), this, SLOT(errorViewShowSimple()));

	showErrorMessages(true);
}

void MainWindow::errorViewShowSimple()
{
	ui_errorView->setCornerWidget(new QPushButton("Verbose"));
	ui_errorView->cornerWidget()->show();
	connect(ui_errorView->cornerWidget(), SIGNAL(clicked()), this, SLOT(errorViewShowVerbose()));

	showErrorMessages(false);
}

void MainWindow::errorClicked(QListWidgetItem *errorItem)
{
	if(!m_currentSourceFile) {
		qWarning("MainWindow::errorClicked() you shouldn't be able to click on an error without a file open!!!");
		return;
	}
	
	QString error = errorItem->text();
	
	int line = error.section(":", 1, 1).toInt();
	
	if(line > 0) {
		m_currentSourceFile->setCursorPosition(line-1, 0);
		m_currentSourceFile->setFocus(Qt::OtherFocusReason);
	}
	
}

/************************
 *     Misc Methods     *
 ************************/

/* Displays any error messages encountered on the last compile */
void MainWindow::readErrorMessages()
{
	ui_errorView->clear();
	ui_errorView->hide();

	m_errorList.clear();
	m_warningList.clear();
	m_linkErrorList.clear();
	m_verboseErrorList.clear();

	m_errorList.addItems(m_target.getErrorMessages());
	m_warningList.addItems(m_target.getWarningMessages());
	m_linkErrorList.addItems(m_target.getLinkerMessages());
	m_verboseErrorList.addItems(m_target.getVerboseMessages());
}

void MainWindow::showErrorMessages(bool verbose)
{
	ui_errorView->clear();
	ui_errorView->hide();

	if(m_errorList.count() > 0) {
		ui_errorView->addTab(&m_errorList, "Errors");
		ui_errorView->show();
	}
	if(m_warningList.count() > 0) {
		ui_errorView->addTab(&m_warningList, "Warnings");
		ui_errorView->show();
	}
	if(m_linkErrorList.count() > 0) {
		ui_errorView->addTab(&m_linkErrorList, "Linker Output");
		ui_errorView->show();
	}
	if( (verbose || ui_errorView->count() == 0) && m_verboseErrorList.count() > 0) {
		ui_errorView->clear();
		ui_errorView->addTab(&m_verboseErrorList, "Verbose Output");
		ui_errorView->show();
	}
}

/* Handles closing all of the open editor windows when the window is closed */
void MainWindow::closeEvent(QCloseEvent *e)
{
	int widgetCount = ui_tabWidget->count();
	
	while(ui_tabWidget->count() > 0) {
		ui_tabWidget->setCurrentIndex(0);
		on_actionClose_triggered();
		
		if(ui_tabWidget->count() == widgetCount) {
			e->ignore();
			return;
		}
		widgetCount--;
	}

	//Close all other windows
	hide();
	QListIterator<QWidget *> i(qApp->topLevelWidgets());
	while(i.hasNext()) {
		QWidget *w = i.next();
		if(w->isVisible())
			w->close();
	}
	
	QMainWindow::closeEvent(e);
	
	return;
}

void MainWindow::deleteTab(int index)
{
	QWidget *w = ui_tabWidget->widget(index);
	ui_tabWidget->removeTab(index);
	delete w;
}

void MainWindow::addTab(SourceFile *sourceFile)
{
	/* Adds the tab to the tabWidget */
	setUpdatesEnabled(false);
	int tabNum = ui_tabWidget->addTab(sourceFile, sourceFile->fileName());
	ui_tabWidget->setCurrentIndex(tabNum);
	on_ui_tabWidget_currentChanged(tabNum);
	setUpdatesEnabled(true);

	QObject::connect(this, SIGNAL(settingsUpdated()), sourceFile, SLOT(refreshSettings()));
}

void MainWindow::setTitleBarTarget(QString txt)
{
	setWindowTitle("KIPR's Instructional Software System - " + txt);
}

bool MainWindow::openFile(QString filename)
{
	QFileInfo fileInfo(filename);

	if(!fileInfo.isFile() || !fileInfo.isReadable())
		return false;

	for(int i = 0;i < ui_tabWidget->count();i++) {
		SourceFile *sourceFile = (SourceFile*)ui_tabWidget->widget(i);
		if(sourceFile->filePath() == filename) {
			ui_tabWidget->setCurrentIndex(i);
			on_ui_tabWidget_currentChanged(i);
			return true;
		}
	}
	
	/* Attempt to open the selected file */
	SourceFile *sourceFile = new SourceFile();
	if(!sourceFile->fileOpen(filename)) {
		QMessageBox::critical(this, "Error", "Error: Could not open file " + sourceFile->fileName() + " for reading");
		delete sourceFile;
		return false;
	}	

	addTab(sourceFile);

	return true;
}
