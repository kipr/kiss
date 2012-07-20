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

#include "MainWindow.h"
#include "Kiss.h"
#include "SourceFile.h"
#include "WebTab.h"
#include "WelcomeTab.h"
#include "KissArchive.h"
#include "ChoosePortDialog.h"
#include "Repository.h"
#include "MessageDialog.h"
#include "Menus.h"
#include "Project.h"
#include "ProjectSettingsTab.h"
#include "ProjectManager.h"
#include <TinyArchive.h>
#include "Log.h"

#include "LexerFactory.h" // Used to query supported extensions

#include "NewProjectWizard.h"

#include "UiEventManager.h"

#include <QToolTip>
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
#include <QFileOpenEvent>
#include <QSettings>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <shellapi.h>
#endif

#define TITLE "KIPR's Instructional Software System"

#define OPEN_PATH "openpath"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_currentTab(0)
{
	QNetworkProxyFactory::setUseSystemConfiguration(true);
	
	setupUi(this);
	m_projectsModel.setProjectManager(&ProjectManager::ref());
	ui_projects->setModel(&m_projectsModel);
	
	// ui_projects->hide(); // Disabled for now
	
	/* Turns off updates so all of these things are drawn at once */
	setUpdatesEnabled(false);

	/* Deletes the tab that QTabWidget starts with by default */
	deleteTab(0);

	hideErrors();
	
	//connect(ui_projects, SIGNAL(clicked(const QModelIndex&)), SLOT(projectFileClicked(const QModelIndex&)));
	connect(ui_projects, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(projectClicked(const QModelIndex&)));
	
	connect(&ProjectManager::ref(), SIGNAL(projectOpened(Project*)), SLOT(projectOpened(Project*)));
	connect(&ProjectManager::ref(), SIGNAL(projectClosed(Project*)), SLOT(projectClosed(Project*)));
	
	ui_projectFrame->setVisible(false);
	
	initMenus();
	
	setTitle("");
	
	setUpdatesEnabled(true);
}

MainWindow::~MainWindow()
{
	delete ui_toolBar; ui_toolBar = 0;
	delete ui_menubar; ui_menubar = 0;
	
	while(ui_tabWidget->count() > 0) deleteTab(0);
}

Project* MainWindow::newProject()
{
	NewProjectWizard wizard(this);

	if(wizard.exec() == QDialog::Rejected) return 0;
	const QString& saveLocation = wizard.saveLocation();
	
	if(QFile::exists(saveLocation)) {
		QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Are You Sure?"),
			tr("Overwrite ") + saveLocation + "?",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			
		if(ret == QMessageBox::No) return 0;
	}
	
	Project* project = Project::create(saveLocation);
	if(!project) {
		MessageDialog::showError(this, "simple_error", QStringList() <<
			tr("Failed to create project.") <<
			tr("Attempted save location: ") + saveLocation);
		return 0;
	}

	ProjectManager::ref().openProject(project);
	return project;
}

void MainWindow::newFile() { UiEventManager::ref().sendEvent(UI_EVENT_NEW_FILE); addTab(new SourceFile(this)); }

bool MainWindow::openFile(const QString& file)
{
	QFileInfo fileInfo(file);

	if(!fileInfo.isFile() || !fileInfo.isReadable()) return false;

	for(int i = 0;i < ui_tabWidget->count();i++) {
		SourceFile* sourceFile = dynamic_cast<SourceFile*>(ui_tabWidget->widget(i));
		if(sourceFile && sourceFile->associatedFile() == file) {
			ui_tabWidget->setCurrentIndex(i);
			on_ui_tabWidget_currentChanged(i);
			return true;
		}
	}
	
	/* Attempt to open the selected file */
	SourceFile *sourceFile = new SourceFile(this);
	if(!sourceFile->fileOpen(file)) {
		MessageDialog::showError(this, "simple_error", QStringList() <<
			tr("Could not open ") + sourceFile->associatedFileName() <<
			tr("Unable to open file for reading."));
		delete sourceFile;
		return false;
	}

	QSettings settings;
	QStringList current = settings.value(RECENTS).toStringList().mid(0, 5);
	current.push_front(fileInfo.absoluteFilePath());
	current.removeDuplicates();
	settings.setValue(RECENTS, current);
	
	addTab(sourceFile);
	
	UiEventManager::ref().sendEvent(UI_EVENT_OPEN_FILE);

	return true;
}

bool MainWindow::memoryOpen(const QByteArray& ba, const QString& assocPath)
{
	QFileInfo fileInfo(assocPath);

	if(!fileInfo.isFile() || !fileInfo.isReadable()) return false;

	for(int i = 0; i < ui_tabWidget->count(); ++i) {
		SourceFile* sourceFile = dynamic_cast<SourceFile*>(ui_tabWidget->widget(i));
		if(sourceFile && sourceFile->associatedFile() == assocPath) {
			ui_tabWidget->setCurrentIndex(i);
			on_ui_tabWidget_currentChanged(i);
			return true;
		}
	}
	
	/* Attempt to open the selected file */
	SourceFile *sourceFile = new SourceFile(this);
	if(!sourceFile->memoryOpen(ba, assocPath)) {
		MessageDialog::showError(this, "simple_error", QStringList() <<
			tr("Could not open ") + sourceFile->associatedFileName() <<
			tr("Unable to open file from memory."));
		delete sourceFile;
		return false;
	}	

	addTab(sourceFile);
	
	UiEventManager::ref().sendEvent(UI_EVENT_OPEN_FILE);

	hideProjectDock();

	return true;
}

bool MainWindow::openProject(const QString& filePath)
{
	Project* project = Project::load(filePath);
	Log::ref().info(QString("Opening project at %1").arg(filePath));
	if(project) ProjectManager::ref().openProject(project);
	// ui_projects->expandAll();
	return project;
}

bool MainWindow::newProject(const QString& filePath)
{
	Project* project = Project::create(filePath);
	Log::ref().info(QString("Creating project at %1").arg(filePath));
	if(project) ProjectManager::ref().openProject(project);
	return project;
}


void MainWindow::initMenus()
{
	menuBar()->clear();
	
	FileOperationsMenu* fileOpMenu = new FileOperationsMenu();
	m_menuManager.registerMenus(fileOpMenu);
	fileOpMenu->setActive(this);
	m_menuManager.addActivation(fileOpMenu);
	m_menuables.append(fileOpMenu);
	
	SourceFileMenu* sourceFileMenu = new SourceFileMenu(this);
	m_menuManager.registerMenus(sourceFileMenu);
	m_menuables.append(sourceFileMenu);
	
	MainWindowMenu* mainWindowMenu = new MainWindowMenu(this);
	m_menuManager.registerMenus(mainWindowMenu);
	mainWindowMenu->setActive(this);
	m_menuManager.addActivation(mainWindowMenu);
	m_menuables.append(mainWindowMenu);
	
	DeviceMenu* deviceMenu = new DeviceMenu;
	m_menuManager.registerMenus(deviceMenu);
	m_menuables.append(deviceMenu);

	ProjectMenu* projectMenu = new ProjectMenu();
	m_menuManager.registerMenus(projectMenu);
	m_menuables.append(projectMenu);

#ifdef BUILD_WEB_TAB
	WebTabMenu* webTabMenu = new WebTabMenu;
	m_menuManager.registerMenus(webTabMenu);
	m_menuables.append(webTabMenu);
#endif

#ifdef BUILD_DEVELOPER_TOOLS
	DeveloperMenu* developerMenu = new DeveloperMenu(this);
	m_menuManager.registerMenus(developerMenu);
	m_menuManager.addActivation(developerMenu);
	m_menuables.append(developerMenu);
#endif

#ifdef BUILD_DOCUMENTATION_TAB
	DocumentationMenu* documentationMenu = new DocumentationMenu(this);
	m_menuManager.registerMenus(documentationMenu);
	m_menuManager.addActivation(documentationMenu);
	m_menuables.append(documentationMenu);
#endif

	m_menuManager.construct(ui_menubar, ui_toolBar);
	
	/* Sets up the QTabWidget that handles the editor windows */
	QToolButton *cornerButton = new QToolButton(ui_tabWidget);
	cornerButton->setDefaultAction(mainWindowMenu->closeNode()->rawAction);
	cornerButton->setAutoRaise(true);
	ui_tabWidget->setCornerWidget(cornerButton);
}

void MainWindow::setTitle(const QString& title) { setWindowTitle(tr(TITLE) + (title.isEmpty() ? "" : (" - " + title))); }
void MainWindow::setTabName(QWidget* widget, const QString& string) { ui_tabWidget->setTabText(ui_tabWidget->indexOf(widget), string); }
void MainWindow::setTabIcon(QWidget* widget, const QIcon& icon) { ui_tabWidget->setTabIcon(ui_tabWidget->indexOf(widget), icon); }
void MainWindow::setStatusMessage(const QString& message, int time) { ui_statusbar->showMessage(message, time); }

void MainWindow::setErrors(const WorkingUnit* unit, const CompileResult& results)
{
	ui_errors->setCompileResult(unit, results);
}

void MainWindow::showErrors(const WorkingUnit* unit)
{
	ui_errors->workingUnitChanged(unit);
}

void MainWindow::hideErrors() { ui_errors->hide(); }

/* Handles closing all of the open editor windows when the window is closed */
void MainWindow::closeEvent(QCloseEvent *e)
{
	int widgetCount = ui_tabWidget->count();
	
	while(ui_tabWidget->count() > 0) {
		ui_tabWidget->setCurrentIndex(0);
		closeTab();
		if(ui_tabWidget->count() == widgetCount) {
			qWarning() << "Ignoring close event.";
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
	QWidget* w = ui_tabWidget->widget(index);
	TabbedWidget* tab = 0;
	tab = lookup(w);
	qWarning() << tab;
	ui_tabWidget->removeTab(index);
	removeLookup(w);
	delete w;
	
	// TODO: We've got  memory leak here...
	// delete tab;
}

void MainWindow::addTab(TabbedWidget* tab)
{
	if(!tab->beginSetup()) return;
	addLookup(tab);
	setUpdatesEnabled(false);
	int tabNum = ui_tabWidget->addTab(tab->widget(), QString::fromAscii(""));
	ui_tabWidget->setCurrentIndex(tabNum);
	setUpdatesEnabled(true);
	tab->completeSetup();
	moveToTab(tab);
	
	QObject::connect(this, SIGNAL(settingsUpdated()), tab->widget(), SLOT(refreshSettings()));
	
	emit updateActivatable();
}

void MainWindow::moveToTab(TabbedWidget* tab)
{
	ui_tabWidget->setCurrentWidget(tab->widget());
}

QTabWidget* MainWindow::tabWidget() { return ui_tabWidget; }
QList<TabbedWidget*> MainWindow::tabs() { return m_lookup.values(); }

void MainWindow::closeAllOthers(TabbedWidget* tab)
{
	int i = 0;
	while(ui_tabWidget->count() > 1) {
		if(lookup(ui_tabWidget->widget(i)) == tab) ++i;
		deleteTab(i);
	}
}

void MainWindow::refreshMenus() { /* initMenus(dynamic_cast<Tab*>(ui_tabWidget->currentWidget())); */ }

void MainWindow::open()
{
	QSettings settings;
	QString openPath = settings.value(OPEN_PATH, QDir::homePath()).toString();
	QStringList filters = Lexer::Factory::ref().formattedExtensions();
	filters << "KISS Project (*.kissproj)";
	filters.removeDuplicates();
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), openPath, filters.join(";;") + ";;All Files (*)");
	
	if(filePath.isEmpty()) return;

	QFileInfo fileInfo(filePath);
	settings.setValue(OPEN_PATH, fileInfo.absolutePath());

	if(fileInfo.completeSuffix() == "kissproj") openProject(filePath); else openFile(filePath);
}

void MainWindow::openProject()
{
	QSettings settings;
	QString openPath = settings.value(OPEN_PATH, QDir::homePath()).toString();
	QStringList filters = Lexer::Factory::ref().formattedExtensions();
	filters.removeDuplicates();
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open Project"), openPath, tr(""));
		
	if(filePath.isEmpty()) return;

	QFileInfo fileInfo(filePath);
	settings.setValue(OPEN_PATH, fileInfo.absolutePath());

	openProject(filePath);
}

void MainWindow::next() { ui_tabWidget->setCurrentIndex(ui_tabWidget->currentIndex() + 1); }
void MainWindow::previous() { ui_tabWidget->setCurrentIndex(ui_tabWidget->currentIndex() - 1); }

void MainWindow::closeTab(bool force)
{	
	if(ui_tabWidget->count() == 0) return;
	
	if(!lookup(ui_tabWidget->currentWidget())->close() && !force) return;
	
	deleteTab(ui_tabWidget->currentIndex());
	ui_errors->hide();
	
	emit updateActivatable();
	
	UiEventManager::ref().sendEvent(UI_EVENT_CLOSE_TAB);
}

void MainWindow::closeProjectTabs(Project* project)
{
	QList<TabbedWidget*> all = tabs();
	int i = 0;
	while(i < ui_tabWidget->count()) {
		TabbedWidget* current = lookup(ui_tabWidget->widget(i));
		if(current->associatedProject() == project) deleteTab(i);
		else i++;
	}
	ui_errors->hide();
}

bool MainWindow::closeFile(const QString& file)
{
	if(file.isEmpty()) return false;
	
	QList<TabbedWidget*> fileTabs = tabs();
	
	bool removed = false;
	
	foreach(TabbedWidget* tab, fileTabs) {
		if(file == tab->associatedFile()) {
			deleteTab(ui_tabWidget->indexOf(tab->widget()));
			removed |= true;
		}
	}
	
	return removed;
}

bool MainWindow::closeNode(const TinyNode* node)
{
	return closeFile(QTinyNode::path(node));
}

void MainWindow::about()
{
	MessageDialog::showMessage(this, "About KISS IDE", "about_kiss", QStringList()
		<< QString::number(KISS_C_VERSION_MAJOR)
		<< QString::number(KISS_C_VERSION_MINOR)
		<< QString::number(KISS_C_VERSION_BUILD)
		<< KISS_C_VERSION_CODENAME);
}

void MainWindow::settings() { if(m_editorSettingsDialog.exec()) emit settingsUpdated(); }

void MainWindow::on_ui_tabWidget_currentChanged(int i) 
{
	if(i < 0) return;
	
	setUpdatesEnabled(false);
	m_currentTab = lookup(ui_tabWidget->widget(i));
	setTitle("");
	if(m_currentTab) {
		m_currentTab->activate();
	}
	
	emit updateActivatable();
	
	setUpdatesEnabled(true);
}

void MainWindow::on_ui_addFile_clicked()
{
	Project* project = activeProject();
	if(!project) {
		MessageDialog::showError(this, "simple_error", QStringList() << 
			tr("Unable to determine active project.") <<
			tr("Please make sure that you have a project open."));
		return;
	} 
	
	SourceFile* source = new SourceFile(this);
	source->setAssociatedProject(project);
	addTab(source);
	// ui_projects->expandAll();
}

void MainWindow::on_ui_removeFile_clicked()
{
	foreach(const QModelIndex& index, ui_projects->selectionModel()->selectedRows()) {
		const int type = m_projectsModel.indexType(index);
		Project* project = m_projectsModel.indexToProject(index);
		if(type == ProjectsModel::FileType) {
			const TinyNode* node = m_projectsModel.indexToNode(index);
			QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Are You Sure?"),
				tr("Permanently delete ") + QTinyNode::name(node) + "?",
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			
			if(ret == QMessageBox::No) continue;
			qWarning() << "Node path" << QString::fromStdString(node->path());
			closeNode(node);
			project->archive()->TinyArchive::remove(node->path());
			project->sync();
		} else if(type == ProjectsModel::ProjectType) {
			project->sync();
			ProjectManager::ref().closeProject(project);
		}
	}
	// ui_projects->expandAll();
}

void MainWindow::managePackages()
{
#ifdef BUILD_REPOSITORY_TAB
	addTab(new Repository(this));
#endif
}

void MainWindow::installLocalPackage()
{
	QSettings settings;
	QString openPath = settings.value(OPEN_PATH, QDir::homePath()).toString();
	QStringList filters = Lexer::Factory::ref().formattedExtensions();
	filters.removeDuplicates();
	QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Open Packages"), openPath, "KISS Archives (*.kiss)");
	foreach(const QString& filePath, filePaths) {
		if(filePath.isEmpty())
			continue;

		QFileInfo fileInfo(filePath);
		settings.setValue(OPEN_PATH, fileInfo.absolutePath());

		QFile f(filePath);
		if(!f.open(QIODevice::ReadOnly)) {
			MessageDialog::showError(this, "simple_error", QStringList() << 
				tr("Installation of KISS Archive ") + fileInfo.fileName() + tr(" failed.") <<
				tr("Unable to open package file for reading."));
			return;
		}
	
		KissReturn ret(KissArchive::install(&f));
		if(ret.error) {
			MessageDialog::showError(this, "simple_error", QStringList() << 
				tr("Installation of KISS Archive ") + fileInfo.fileName() + tr(" failed.") <<
				ret.message);
			return;
		} 
	}
	if(filePaths.size() > 0) QMessageBox::information(this, tr("Install Complete!"), tr("Please restart KISS"));
}

void MainWindow::showProjectDock(bool show)
{
	ui_projectFrame->setVisible(show);
}

void MainWindow::hideProjectDock()
{
	showProjectDock(false);
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
#if 0
	if(!m_errors) return;
	// int i = ui_tabWidget->indexOf(m_errors->widget());
	QWidget* widget = ui_tabWidget->widget(i);
	
	int line = item->text().section(":", 1, 1).toInt();
	
	if(line > 0) {
		// m_errorTab->moveTo(line, 0);
	}
	
	ui_tabWidget->setCurrentIndex(i);
	
	widget->setFocus(Qt::OtherFocusReason); // For some reason this isn't working?
#endif
}

void MainWindow::addLookup(TabbedWidget* tab)
{
	if(lookup(tab->widget())) return;
	m_lookup.insert(tab->widget(), tab);
}

void MainWindow::removeLookup(QWidget* widget)
{
	m_lookup.remove(widget);
}

TabbedWidget* MainWindow::lookup(QWidget* widget) const
{
	QMap<QWidget*, TabbedWidget*>::const_iterator it = m_lookup.find(widget);
	return it == m_lookup.end() ? 0 : *it;
}

// TODO: Make Error Googlable
void MainWindow::showContextMenuForError(const QPoint &pos) { Q_UNUSED(pos); }

void MainWindow::projectClicked(const QModelIndex& index)
{
	Project* project = m_projectsModel.indexToProject(index);
	qDebug() << "Type" << m_projectsModel.indexType(index);
	if(m_projectsModel.indexType(index) == ProjectsModel::ProjectType) {
		for(int i = 0; i < ui_tabWidget->count(); ++i) {
			ProjectSettingsTab* tab = dynamic_cast<ProjectSettingsTab*>(ui_tabWidget->widget(i));
			if(tab && tab->associatedProject() == project) {
				ui_tabWidget->setCurrentIndex(i);
				on_ui_tabWidget_currentChanged(i);
				return;
			}
		}
	
		ProjectSettingsTab* tab = new ProjectSettingsTab(project, this);
		addTab(tab);
	} else if(m_projectsModel.indexType(index) == ProjectsModel::FileType) {
		qDebug() << "File!!";
		const TinyNode* node = m_projectsModel.indexToNode(index);
		Project* proj = m_projectsModel.indexToProject(index);
		const QString& file = QString::fromStdString(node->path());
		if(!project) return;

		SourceFile* sourceFile = new SourceFile(this);
		for(int i = 0; i < ui_tabWidget->count(); ++i) {
			SourceFile* sourceFile = dynamic_cast<SourceFile*>(ui_tabWidget->widget(i));
			if(sourceFile && sourceFile->associatedFile() == file && sourceFile->associatedProject() == proj) {
				ui_tabWidget->setCurrentIndex(i);
				on_ui_tabWidget_currentChanged(i);
				return;
			}
		}

		if(!sourceFile->openProjectFile(project, node)) {
			delete sourceFile;
			return;
		}
		Log::ref().debug(QString("Opened %1 for editing").arg(node->name()));
		addTab(sourceFile);
	}
}

void MainWindow::projectFileClicked(const QModelIndex& index)
{
	// Project* project = m_projectsModel.indexToProject(index);
	
	#if 0
	
	
	
	#endif
}

void MainWindow::projectOpened(Project* project)
{
	ProjectManager* manager = qobject_cast<ProjectManager*>(sender());
	if(manager) ui_projectFrame->setVisible(manager->projects().size());
}

void MainWindow::projectClosed(Project* project)
{
	closeProjectTabs(project);
	ProjectManager* manager = qobject_cast<ProjectManager*>(sender());
	if(manager) ui_projectFrame->setVisible(manager->projects().size());
}

void MainWindow::availableFinished(bool avail)
{
	qDebug() << "Available finished";
}

void MainWindow::compileFinished(CompileResult result)
{
	qDebug() << "Compile finished";
	setStatusMessage(result.success() ? tr("Remote Compilation Succeeded") : tr("Remote Compilation Failed"));
	UiEventManager::ref().sendEvent(UI_EVENT_COMPILE);
}

void MainWindow::downloadFinished(bool success)
{
	qDebug() << "Download finished";
	setStatusMessage(success ? tr("Download Succeeded") : tr("Download Failed"));
	UiEventManager::ref().sendEvent(UI_EVENT_DOWNLOAD);
}

void MainWindow::runFinished(bool success)
{
	qDebug() << "Run finished";
	setStatusMessage(success ? tr("Run Succeeded") : tr("Run Failed"));
}

bool MainWindow::eventFilter(QObject * target, QEvent * event) {
        if(event->type() == QEvent::FileOpen) {
                QString fileName = dynamic_cast<QFileOpenEvent *>(event)->file();
                openFile(fileName);
                event->accept();
                return true;
        } else return QMainWindow::eventFilter(target, event);
}

MenuManager* MainWindow::menuManager()
{
	return &m_menuManager;
}

Menuable* MainWindow::menuable(const QString& name)
{
	foreach(Menuable* menuable, m_menuables) {
		if(menuable->name() == name) return menuable;
	}
	return 0;
}

QList<Menuable*> MainWindow::menuablesExcept(const QStringList& names)
{
	QList<Menuable*> ret;
	foreach(Menuable* menuable, m_menuables) {
		if(!names.contains(menuable->name())) ret.append(menuable);
	}
	return ret;
}

QList<Menuable*> MainWindow::menuables()
{
	return m_menuables;
}

void MainWindow::activateMenuable(const QString& name, QObject* on)
{
	ActivatableObject* activatable = dynamic_cast<ActivatableObject*>(menuable(name));
	activatable->setActive(on);
}

QStringList MainWindow::standardMenus() const
{
	return QStringList() << FileOperationsMenu::menuName() << MainWindowMenu::menuName() << ProjectMenu::menuName()
#ifdef BUILD_DOCUMENTATION_TAB
	<< DocumentationMenu::menuName()
#endif
#ifdef BUILD_DEVELOPER_TOOLS
	<< DeveloperMenu::menuName()
#endif
	;
}

// TODO: NYI
void MainWindow::restart()
{
}

QList<QObject*> MainWindow::tabs(const QString& name)
{
	QList<QObject*> ret;
	QList<TabbedWidget*> all = tabs();
	foreach(TabbedWidget* tab, all) {
		QObject* t = dynamic_cast<QObject*>(tab);
		if(!t) continue;
		const QMetaObject* meta = t->metaObject();
		while(meta != 0) {
			if(name == meta->className()) {
				ret.append(t);
				break;
			}
			meta = meta->superClass();
		}
	}
	return ret;
}

bool MainWindow::canClose()
{
	return ui_tabWidget->count() > 0;
}

bool MainWindow::canGoPrevious()
{
	return m_currentTab && ui_tabWidget->count() > 0 && ui_tabWidget->currentIndex() != 0;
}

bool MainWindow::canGoNext()
{
	return m_currentTab && ui_tabWidget->count() > 0 && ui_tabWidget->currentIndex() != ui_tabWidget->count() - 1;
}

Project* MainWindow::activeProject() const
{
	// Precedence is like so: Project Manager, Tab, Project Tree
	const QList<Project*> loadedProjects = ProjectManager::ref().projects();
	if(loadedProjects.size() == 0) return 0;
	if(loadedProjects.size() == 1) return loadedProjects[0];
	
	if(m_currentTab && m_currentTab->isProjectAssociated()) return m_currentTab->associatedProject();
	const QModelIndexList& list = ui_projects->selectionModel()->selectedRows();
	return list.size() > 0 ? m_projectsModel.indexToProject(list[0]) : 0;
}

ProjectsModel* MainWindow::projectsModel()
{
	return &m_projectsModel;
}
