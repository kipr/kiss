/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
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
#include "Kiss.h"
#include "Lexer.h"
#include "SourceFile.h"
#include "WebTab.h"
#include "TargetManager.h"
#include "WelcomeTab.h"
#include "KissArchive.h"
#include "ChoosePortDialog.h"
#include "FindDialog.h"
#include "Repository.h"

#include <QToolTip>
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
#include <QNetworkProxyFactory>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <shellapi.h>
#endif

#define TITLE "KIPR's Instructional Software System"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_currentTab(0), m_errorTab(0)
{
	QNetworkProxyFactory::setUseSystemConfiguration(true);
	
	setupUi(this);
	/* Turns off updates so all of these things are drawn at once */
	setUpdatesEnabled(false);

	/* Sets up the QTabWidget that handles the editor windows */
	QToolButton *cornerButton = new QToolButton(ui_tabWidget);
	cornerButton->setDefaultAction(actionClose);
	cornerButton->setAutoRaise(true);
	ui_tabWidget->setCornerWidget(cornerButton);

	/* Deletes the tab that QTabWidget starts with by default */
	deleteTab(0);

	hideErrors();
	
	connect(&m_errorList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(errorClicked(QListWidgetItem*)));
	connect(&m_warningList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(errorClicked(QListWidgetItem*)));
	connect(&m_linkErrorList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(errorClicked(QListWidgetItem*)));
	connect(&m_verboseList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(errorClicked(QListWidgetItem*)));
	
	initMenus(0);
	
	setTitle("");
	
	setUpdatesEnabled(true);
}

MainWindow::~MainWindow()
{
	delete ui_toolBar;
	ui_toolBar = 0;
	
	delete ui_menubar;
	ui_menubar = 0;
	
	while(ui_tabWidget->count() > 0) deleteTab(0);
}

void MainWindow::newFile() { addTab(new SourceFile(this)); }

bool MainWindow::openFile(const QString& file)
{
	QFileInfo fileInfo(file);

	if(!fileInfo.isFile() || !fileInfo.isReadable()) return false;

	for(int i = 0;i < ui_tabWidget->count();i++) {
		SourceFile* sourceFile = dynamic_cast<SourceFile*>(ui_tabWidget->widget(i));
		if(sourceFile && sourceFile->filePath() == file) {
			ui_tabWidget->setCurrentIndex(i);
			on_ui_tabWidget_currentChanged(i);
			return true;
		}
	}
	
	/* Attempt to open the selected file */
	SourceFile *sourceFile = new SourceFile(this);
	if(!sourceFile->fileOpen(file)) {
		QMessageBox::critical(this, tr("Error"), tr("Error: Could not open file ") + sourceFile->fileName() + tr(" for reading"));
		delete sourceFile;
		return false;
	}	

	QSettings settings;
	QStringList current = settings.value(RECENTS).toStringList().mid(0, 5);
	current.push_front(fileInfo.absoluteFilePath());
	current.removeDuplicates();
	settings.setValue(RECENTS, current);
	
	addTab(sourceFile);

	return true;
}

void MainWindow::initMenus(Tab* tab)
{
	menuBar()->clear();
	menuFile->clear();
	menuEdit->clear();
	menuSettings->clear();
	menuHelp->clear();
	ui_toolBar->clear();
	
	menuBar()->addMenu(menuFile);
	menuBar()->addMenu(menuEdit);
	
	menuFile->addAction(actionNew);
	menuFile->addAction(actionOpen);
	QMenu* recentMenu = menuFile->addMenu("Open Recent");
	foreach(const QString& recent, QSettings().value(RECENTS).toStringList()) {
		QAction* action = recentMenu->addAction(recent);
		action->setData(recent);
		connect(action, SIGNAL(triggered()), this, SLOT(openRecent()));
	}
	
	if(tab) tab->addActionsFile(menuFile);
	menuFile->addSeparator();
	menuFile->addAction(actionNext);
	menuFile->addAction(actionPrevious);
	menuFile->addAction(actionClose);
	menuFile->addSeparator();
	QMenu* menuPackages = menuFile->addMenu(tr("Packages"));
	menuPackages->addAction(actionInstallLocalPackage);
	menuPackages->addAction(actionManagePackages);
	menuFile->addAction(actionHideErrors);
	menuFile->addSeparator();
	menuFile->addAction(actionQuit);
	
	if(tab) {
		tab->addActionsEdit(menuEdit);
		menuEdit->addSeparator();
		tab->addActionsHelp(menuHelp);
		menuHelp->addSeparator();
	}
	
	menuEdit->addAction(actionEditor_Settings);
	menuHelp->addSeparator();
	menuHelp->addAction(actionAbout);
	
	if(tab) tab->addOtherActions(menuBar());
	
	menuBar()->addMenu(menuHelp);
	
	ui_toolBar->addAction(actionNew);
	ui_toolBar->addAction(actionOpen);
	if(tab) tab->addToolbarActions(ui_toolBar);
}

void MainWindow::setTitle(const QString& title) { setWindowTitle(tr(TITLE) + (title.isEmpty() ? "" : (" - " + title))); }
void MainWindow::setTabName(QWidget* widget, const QString& string) { ui_tabWidget->setTabText(ui_tabWidget->indexOf(widget), string); }
void MainWindow::setStatusMessage(const QString& message, int time) { ui_statusbar->showMessage(message, time); }

void MainWindow::setErrors(Tab* tab, 
	const QStringList& errors, const QStringList& warnings, 
	const QStringList& linker, const QStringList& verbose)
{
	m_errorTab = tab;
	
	ui_errorView->hide();
	ui_errorView->clear();

	m_errorList.clear();
	m_warningList.clear();
	m_linkErrorList.clear();
	m_verboseList.clear();
	
	m_errorList.addItems(errors);
	m_warningList.addItems(warnings);
	m_linkErrorList.addItems(linker);
	m_verboseList.addItems(verbose);
	
	errorViewShowSimple();
}

void MainWindow::hideErrors() { ui_errorView->hide(); }

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
		--widgetCount;
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
}

void MainWindow::deleteTab(int index)
{
	QWidget *w = ui_tabWidget->widget(index);
	if(m_errorTab == dynamic_cast<Tab*>(w)) {
		ui_errorView->hide();
		m_errorTab = 0;
	}
	ui_tabWidget->removeTab(index);
	delete w;
}

void MainWindow::addTab(Tab* tab)
{
	if(!tab->beginSetup()) return;
	setUpdatesEnabled(false);
	int tabNum = ui_tabWidget->addTab(dynamic_cast<QWidget*>(tab), QString::fromAscii(""));
	ui_tabWidget->setCurrentIndex(tabNum);
	setUpdatesEnabled(true);
	tab->completeSetup();
	
	QObject::connect(this, SIGNAL(settingsUpdated()), dynamic_cast<QWidget*>(tab), SLOT(refreshSettings()));
	
	actionClose->setEnabled(ui_tabWidget->count() > 0);
}

QTabWidget* MainWindow::tabWidget() { return ui_tabWidget; }
void MainWindow::closeAllOthers(Tab* tab)
{
	int i = 0;
	while(ui_tabWidget->count() > 1) {
		if(dynamic_cast<Tab*>(ui_tabWidget->widget(i)) == tab) ++i;
		deleteTab(i);
	}
}

void MainWindow::refreshMenus() { initMenus(dynamic_cast<Tab*>(ui_tabWidget->currentWidget())); }

void MainWindow::showErrorMessages(bool verbose)
{
	ui_errorView->clear();
	ui_errorView->hide();

	if(m_errorList.count() > 0) {
		ui_errorView->addTab(&m_errorList, tr("Errors"));
		ui_errorView->show();
	}
	if(m_warningList.count() > 0) {
		ui_errorView->addTab(&m_warningList, tr("Warnings"));
		ui_errorView->show();
	}
	if(m_linkErrorList.count() > 0) {
		ui_errorView->addTab(&m_linkErrorList, tr("Linker Output"));
		ui_errorView->show();
	}
	if( (verbose || ui_errorView->count() == 0) && m_verboseList.count() > 0) {
		ui_errorView->clear();
		ui_errorView->addTab(&m_verboseList, tr("Verbose Output"));
		ui_errorView->show();
	}
}

void MainWindow::on_actionNew_triggered() { newFile(); }

void MainWindow::on_actionOpen_triggered()
{
	QSettings settings;
	QString openPath = settings.value("openpath", QDir::homePath()).toString();
	QStringList filters = TargetManager::ref().allSupportedExtensions();
	filters.removeDuplicates();
	qWarning() << filters;
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), openPath, filters.join(";;") + ";;All Files (*)");
		
	if(filePath.isEmpty())
		return;

	QFileInfo fileInfo(filePath);
	settings.setValue("openpath", fileInfo.absolutePath());

	openFile(filePath);
}

void MainWindow::on_actionNext_triggered() { ui_tabWidget->setCurrentIndex(ui_tabWidget->currentIndex() + 1); }
void MainWindow::on_actionPrevious_triggered() { ui_tabWidget->setCurrentIndex(ui_tabWidget->currentIndex() - 1); }

void MainWindow::on_actionClose_triggered()
{	
	if(ui_tabWidget->count() == 0) return;
	
	if(!dynamic_cast<Tab*>(ui_tabWidget->currentWidget())->close()) return;
	
	deleteTab(ui_tabWidget->currentIndex());
	ui_errorView->hide();
	
	actionClose->setEnabled(ui_tabWidget->count() > 0);
}

void MainWindow::on_actionAbout_triggered()
{
	QString aboutString = tr("KISS Version ") + QString::number(KISS_C_VERSION_MAJOR) + "." + 
		QString::number(KISS_C_VERSION_MINOR) + "." +
		QString::number(KISS_C_VERSION_BUILD) + "\n\n";
	aboutString += tr("Copyright (C) 2007-2011 KISS Institute for Practical Robotics\n\n");
	aboutString += tr("http://www.kipr.org/\nhttp://www.botball.org/\n\n");
	aboutString += tr("KISS is a project of the KISS Institute for Practical Robotics. ");
	QMessageBox::about(this, tr("KIPR's Instructional Software System"), aboutString);
}


void MainWindow::errorViewShowVerbose()
{
	ui_errorView->setCornerWidget(new QPushButton(tr("Simple")));
	ui_errorView->cornerWidget()->show();
	connect(ui_errorView->cornerWidget(), SIGNAL(clicked()), this, SLOT(errorViewShowSimple()));

	showErrorMessages(true);
}

void MainWindow::errorViewShowSimple()
{
	ui_errorView->setCornerWidget(new QPushButton(tr("Verbose")));
	ui_errorView->cornerWidget()->show();
	connect(ui_errorView->cornerWidget(), SIGNAL(clicked()), this, SLOT(errorViewShowVerbose()));

	showErrorMessages(false);
}

void MainWindow::on_actionEditor_Settings_triggered() { if(m_editorSettingsDialog.exec()) emit settingsUpdated(); }

void MainWindow::on_ui_tabWidget_currentChanged(int i) 
{
	setUpdatesEnabled(false);
	m_currentTab = dynamic_cast<Tab*>(ui_tabWidget->widget(i));
	initMenus(m_currentTab);
	setTitle("");
	if(m_currentTab) m_currentTab->activate();
	actionNext->setEnabled(m_currentTab && i != ui_tabWidget->count() - 1);
	actionPrevious->setEnabled(m_currentTab && i != 0);
	setUpdatesEnabled(true);
}

void MainWindow::on_actionManagePackages_triggered() { addTab(new Repository(this)); }

void MainWindow::on_actionInstallLocalPackage_triggered()
{
	QSettings settings;
	QString openPath = settings.value("openpath", QDir::homePath()).toString();
	QStringList filters = TargetManager::ref().allSupportedExtensions();
	filters.removeDuplicates();
	qWarning() << filters;
	QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Open Packages"), openPath, "KISS Archives (*.kiss)");
	foreach(const QString& filePath, filePaths) {
		if(filePath.isEmpty())
			continue;

		QFileInfo fileInfo(filePath);
		settings.setValue("openpath", fileInfo.absolutePath());

		QFile f(filePath);
		if(!f.open(QIODevice::ReadOnly)) {
			QMessageBox::critical(this, tr("Install failed!") + fileInfo.fileName(), tr("Unable to open package"));
			return;
		}
	
		KissReturn ret(KissArchive::install(&f));
		if(ret.error) {
			QMessageBox::critical(this, tr("Install failed!") + fileInfo.fileName(), ret.message);
			return;
		} 
	}
	QMessageBox::information(this, tr("Install Complete!"), tr("Please restart KISS"));
}

void MainWindow::openRecent()
{
	QAction* action = qobject_cast<QAction*>(sender());
	QString file = action->data().toString();
	QSettings settings;
	QStringList current = settings.value(RECENTS).toStringList();
	current.push_front(file);
	current.removeDuplicates();
	settings.setValue(RECENTS, current);
	if(action) openFile(file);
}

void MainWindow::errorClicked(QListWidgetItem* item)
{
	if(!m_errorTab) return;
	int i = ui_tabWidget->indexOf(dynamic_cast<QWidget*>(m_errorTab));
	QWidget* widget = ui_tabWidget->widget(i);
	
	int line = item->text().section(":", 1, 1).toInt();
	
	if(line > 0) {
		m_errorTab->moveTo(line, 0);
	}
	
	ui_tabWidget->setCurrentIndex(i);
	
	widget->setFocus(Qt::OtherFocusReason); // For some reason this isn't working?
}

// TODO: Make Error Googlable
void MainWindow::showContextMenuForError(const QPoint &pos) { Q_UNUSED(pos); }
