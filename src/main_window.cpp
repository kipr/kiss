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

#include "main_window.hpp"
#include "kiss.hpp"
#include "source_file.hpp"
#include "message_dialog.hpp"
#include "target_dialog.hpp"
#include "menus.hpp"
#include "project.hpp"
#include "log.hpp"
#include "lexer_factory.hpp"

#include "new_project_dialog.hpp"
#include "template_manager.hpp"
#include "template_tab.hpp"
#include "template_pack.hpp"
#include "main_responder.hpp"
#include "communication_progress_bar.hpp"
#include "communication_manager.hpp"
#include "file_utils.hpp"
#include "password_dialog.hpp"
#include "add_to_project_dialog.hpp"
#include "project_settings_dialog.hpp"

#include "system_utils.hpp"

#include <QMessageBox>
#include <QInputDialog>
#include <QAction>
#include <QDebug>
#include <QNetworkProxyFactory>
#include <QSettings>
#include <QDesktopServices>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <shellapi.h>
#endif

#define TITLE "KIPR's Instructional Software System"

using namespace kiss;
using namespace kiss::widget;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	m_currentTab(0),
	m_templateManager(new templates::Manager),
	m_commProgress(new CommunicationProgressBar(&target::CommunicationManager::ref(), this)),
	m_mainResponder(new target::MainResponder(this))
{
	QNetworkProxyFactory::setUseSystemConfiguration(true);
	
	m_templateManager->loadDefaultPacks();
	
	setupUi(this);
	
	ui_projects->setModel(&m_projectsModel);
	ui_projects->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
	ui_projects->setContextMenuPolicy(Qt::CustomContextMenu);
  ui_projectSplitter->setStretchFactor(0, 6);
  ui_projectSplitter->setStretchFactor(1, 1);
  ui_infoBox->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui_projectFrame->setVisible(false);
	connect(&m_projectsModel, SIGNAL(filesDropped(QStringList)), this,
    SLOT(droppedProjectAddExisting(QStringList)));
		
	setUpdatesEnabled(false);

	/* Deletes the tab that QTabWidget starts with by default */
	deleteTab(0);

	hideErrors();
	
	connect(ui_projects, SIGNAL(clicked(QModelIndex)),
		SLOT(projectClicked(QModelIndex)));
	connect(ui_projects, SIGNAL(doubleClicked(QModelIndex)),
		SLOT(projectDoubleClicked(QModelIndex)));
	connect(ui_projects, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(projectContextMenu(const QPoint&)));
	
	connect(&m_projectManager, SIGNAL(activeChanged(kiss::project::ProjectPtr, kiss::project::ProjectPtr)),
		&m_projectsModel, SLOT(activeChanged(kiss::project::ProjectPtr, kiss::project::ProjectPtr)));
    
  connect(ui_projects->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
    this, SLOT(updateInfoBox(const QModelIndex &)));
		
	connect(&target::CommunicationManager::ref(),
		SIGNAL(targetNeedsAuthentication(kiss::target::TargetPtr, kiss::target::CommunicationManager *)),
		SLOT(authenticateTarget(kiss::target::TargetPtr, kiss::target::CommunicationManager *)));
		
	connect(&target::CommunicationManager::ref(),
		SIGNAL(oldDeviceSoftware(kiss::target::TargetPtr)),
		SLOT(oldDeviceSoftware(kiss::target::TargetPtr)));
		
	connect(&target::CommunicationManager::ref(),
		SIGNAL(oldHostSoftware(kiss::target::TargetPtr)),
		SLOT(oldHostSoftware(kiss::target::TargetPtr)));
		
	ui_statusbar->addPermanentWidget(m_commProgress);
	
	ui_comm->hide();
	
	connect(m_commProgress, SIGNAL(clicked()), SLOT(toggleCommunicationWidget()));
	
	initMenus();
	
	setTitle("");
	
	setUpdatesEnabled(true);
}

MainWindow::~MainWindow()
{
	delete ui_toolBar; ui_toolBar = 0;
	delete ui_menubar; ui_menubar = 0;
	
	while(ui_tabWidget->count() > 0) deleteTab(0);
	
	delete m_templateManager;
	delete m_mainResponder;
	delete m_projectContextMenu;
	delete m_fileContextMenu;
}

void MainWindow::importTemplatePack()
{
	QStringList filters;
	filters << "Template Pack (*.pack)";
	filters.removeDuplicates();
	QString filePath = FileUtils::getOpenFileName(this, tr("Open"), filters.join(";;") + ";;All Files (*)");
	if(filePath.isEmpty()) return;
	
	templates::PackPtr pack = templates::Pack::load(filePath);
	if(!pack) {
		dialog::Message::showError(this, "simple_error_with_action", QStringList()
			<< tr("Failed to open %1 for reading.").arg(filePath)
			<< tr("This probably means that the file is malformed.").arg(filePath)
			<< tr("Aborting import."));
		return;
	}
	m_templateManager->addDefaultPack(pack);
	m_templateManager->addPack(pack);
}

void MainWindow::newTemplatePack()
{
	addTab(new TemplateTab(templates::Pack::create(), this));
}

void MainWindow::newFile()
{
	addTab(new SourceFile(this));
}

bool MainWindow::openFile(const QString &file, const project::ProjectPtr &project)
{
	QFileInfo fileInfo(file);
	if(!fileInfo.isFile() || !fileInfo.isReadable()) return false;

  // If the file is already open, just switch to it
	for(int i = 0; i < ui_tabWidget->count(); ++i) {
		SourceFile *sourceFile = dynamic_cast<SourceFile *>(ui_tabWidget->widget(i));
		if(sourceFile && sourceFile->file() == file) {
			ui_tabWidget->setCurrentIndex(i);
			return true;
		}
	}
  
  SourceFile *sourceFile = new SourceFile(this);
  sourceFile->setProject(project);
  QString fileToOpen = file;
  
  // Files opened outside of projects
  if(!project) {
    const QString projectDir = project::Project::associatedProject(file);
    if(projectDir.isNull()) {
      if(QMessageBox::question(this, tr("Create project?"),
        tr("You are opening a file that is not part of a KISS project. "
          "Creating a project will allow you to compile and run your program. Do you want to create a project with the file?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
          project::ProjectPtr createdProject = newProject();
          createdProject->addFileAsCopy(file, createdProject->location());
          sourceFile->setProject(createdProject);
          fileToOpen = QDir(createdProject->location()).filePath(fileInfo.fileName());
        }
    }
    else {
      if(project::ProjectPtr project = m_projectManager.openedProject(projectDir))
        sourceFile->setProject(project);
      else if(QMessageBox::question(this, tr("Open associated project?"),
        tr("You are opening a file that is part of a KISS project. "
          "Opening the whole project will allow you to compile and run your program. Do you want to open the project?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
          sourceFile->setProject(openProject(projectDir));
    }
  }
	
	// Attempt to open the selected file
	if(!sourceFile->fileOpen(fileToOpen)) {
		dialog::Message::showError(this, "simple_error", QStringList() <<
			tr("Could not open ") + QFileInfo(sourceFile->file()).fileName() <<
			tr("Unable to open file for reading."));
		delete sourceFile;
		return false;
	}
	
	QSettings settings;
	QStringList current = settings.value(RECENTS).toStringList().mid(0, 5);
	current.push_front(QFileInfo(fileToOpen).absoluteFilePath());
	current.removeDuplicates();
	settings.setValue(RECENTS, current);
	
	addTab(sourceFile);

	return true;
}

bool MainWindow::memoryOpen(const QByteArray &ba, const QString &assocPath)
{
	QFileInfo fileInfo(assocPath);

	if(!fileInfo.isFile() || !fileInfo.isReadable()) return false;

	for(int i = 0; i < ui_tabWidget->count(); ++i) {
		SourceFile *sourceFile = dynamic_cast<SourceFile*>(ui_tabWidget->widget(i));
		if(sourceFile && sourceFile->file() == assocPath) {
			ui_tabWidget->setCurrentIndex(i);
			return true;
		}
	}
	
	/* Attempt to open the selected file */
	SourceFile *sourceFile = new SourceFile(this);
	if(!sourceFile->memoryOpen(ba, assocPath)) {
		dialog::Message::showError(this, "simple_error", QStringList() <<
			tr("Could not open ") + QFileInfo(sourceFile->file()).fileName() <<
			tr("Unable to open file from memory."));
		delete sourceFile;
		return false;
	}	

	addTab(sourceFile);

	hideProjectDock();

	return true;
}

project::ProjectPtr MainWindow::openProject(const QString &projectPath)
{
	project::ProjectPtr project = project::Project::load(projectPath);
	Log::ref().info(QString("Opening project at %1").arg(projectPath));
	if(!project) return project::ProjectPtr();
  m_projectsModel.addProject(project);
  if(!m_projectManager.openProject(project)) return project::ProjectPtr();
	if(!m_projectManager.projects().isEmpty()) {
		ui_projectFrame->setVisible(true);
    activateMenuable(menu::ProjectMenu::menuName(), this);
	}

	return project;
}

project::ProjectPtr MainWindow::newEmptyProject()
{
  project::ProjectPtr project = newProject();
  if(project.isNull()) return project;
  
  // Prompt the user to add a new file to their empty project
  if(QMessageBox::question(this, tr("Add a New File?"),
  tr("You just created an empty project. Do you want to add a new file?"),
  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    projectAddNew(project, project->location());
  
  return project;
}

project::ProjectPtr MainWindow::newProject()
{
	dialog::NewProjectDialog dialog(this);
	if(dialog.exec() == QDialog::Rejected) return project::ProjectPtr();

	const QString &saveLocation = dialog.saveLocation();
	if(QDir(saveLocation).exists()) {
		QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Are You Sure?"),
			tr("Overwrite %1?").arg(saveLocation),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if(ret == QMessageBox::No) return project::ProjectPtr();
	}
	
  project::ProjectPtr ret = newProject(saveLocation);
  if(ret.isNull()) return ret;
  
  ret->setAutoCompileDeps(true);
  ret->setCompileLib(false);
  
  return ret;
}

project::ProjectPtr MainWindow::newProject(const QString &projectPath)
{
	if(!FileUtils::remove(projectPath)) return project::ProjectPtr();
	if(!QDir().mkpath(projectPath)) return project::ProjectPtr();

	return openProject(projectPath);
}

void MainWindow::initMenus()
{
	using namespace kiss::menu;
	
	menuBar()->clear();
	
	FileOperationsMenu *fileOpMenu = new FileOperationsMenu();
	m_menuManager.registerMenus(fileOpMenu);
	fileOpMenu->setActive(this);
	m_menuManager.addActivation(fileOpMenu);
	m_menuables.append(fileOpMenu);
	
	SourceFileMenu *sourceFileMenu = new SourceFileMenu(this);
	m_menuManager.registerMenus(sourceFileMenu);
	m_menuables.append(sourceFileMenu);
	
	TemplatePackMenu *templatePackMenu = new TemplatePackMenu(this);
	m_menuManager.registerMenus(templatePackMenu);
	m_menuables.append(templatePackMenu);
	
	MainWindowMenu *mainWindowMenu = new MainWindowMenu(this);
	m_menuManager.registerMenus(mainWindowMenu);
	mainWindowMenu->setActive(this);
	m_menuManager.addActivation(mainWindowMenu);
	m_menuables.append(mainWindowMenu);
  
  ProjectMenu *projectMenu = new ProjectMenu();
  m_menuManager.registerMenus(projectMenu);
  m_menuables.append(projectMenu);

#ifdef BUILD_DEVELOPER_TOOLS
	DeveloperMenu *developerMenu = new DeveloperMenu(this);
	m_menuManager.registerMenus(developerMenu);
	m_menuManager.addActivation(developerMenu);
	m_menuables.append(developerMenu);
#endif

#ifdef BUILD_DOCUMENTATION_TAB
	DocumentationMenu *documentationMenu = new DocumentationMenu(this);
	m_menuManager.registerMenus(documentationMenu);
	m_menuManager.addActivation(documentationMenu);
	m_menuables.append(documentationMenu);
#endif

	m_menuManager.construct(ui_menubar, ui_toolBar);
	
	ui_tabWidget->setTabsClosable(true);
	connect(ui_tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));

  const QString &trashLabel = SystemUtils::supportsMoveToTrash() ? tr("Move to Trash") : tr("Delete");
	m_projectContextMenu = new QMenu(this);
	QAction *newFileAction =
    m_projectContextMenu->addAction(ResourceHelper::ref().icon("page_white_add.png"), tr("Add New File..."), this, SLOT(selectedProjectAddNew()));
	QAction *existFileAction =
    m_projectContextMenu->addAction(ResourceHelper::ref().icon("page_white_add.png"), tr("Add Existing Files..."), this, SLOT(selectedProjectAddExisting()));
  QAction *addFolderAction = 
    m_projectContextMenu->addAction(ResourceHelper::ref().icon("folder_add"), tr("Add Folder"), this, SLOT(selectedProjectAddFolder()));
	m_projectContextMenu->addSeparator();
  m_projectContextMenu->addAction(ResourceHelper::ref().icon("ruby_blue"), tr("Download"), this, SLOT(selectedProjectDownload()));
  m_projectContextMenu->addAction(ResourceHelper::ref().icon("bricks"), tr("Compile"), this, SLOT(selectedProjectCompile()));
  m_projectContextMenu->addAction(ResourceHelper::ref().icon("arrow_right"), tr("Run"), this, SLOT(selectedProjectRun()));
  m_projectContextMenu->addAction(ResourceHelper::ref().icon("computer"), tr("Change Target"), this, SLOT(selectedProjectChangeTarget()));
  m_projectContextMenu->addSeparator();
  QAction *fileBrowserAction = 
    m_projectContextMenu->addAction(ResourceHelper::ref().icon("mouse"), tr("View in File Browser"), this, SLOT(selectedProjectFileBrowser()));
	m_projectContextMenu->addAction(ResourceHelper::ref().icon("folder_wrench.png"), tr("Project Settings"), this, SLOT(selectedProjectOpenSettings()));
	m_projectContextMenu->addSeparator();
	m_projectContextMenu->addAction(ResourceHelper::ref().icon("folder.png"), tr("Close Project"), this, SLOT(selectedProjectClose()));
	m_projectContextMenu->addAction(ResourceHelper::ref().icon("bin_closed.png"), trashLabel, this, SLOT(selectedProjectDelete()));
  
  m_folderContextMenu = new QMenu(this);
	m_folderContextMenu->addAction(newFileAction);
	m_folderContextMenu->addAction(existFileAction);
  m_folderContextMenu->addAction(addFolderAction);
  m_folderContextMenu->addSeparator();
  m_folderContextMenu->addAction(fileBrowserAction);
  m_folderContextMenu->addSeparator();
  m_folderContextMenu->addAction(ResourceHelper::ref().icon("bin_closed.png"), trashLabel, this, SLOT(projectRemoveFolder()));
  
  m_linkContextMenu = new QMenu(this);
  m_linkContextMenu->addAction(ResourceHelper::ref().icon("bin_closed.png"), tr("Delete"), this, SLOT(projectRemoveFile()));

	m_fileContextMenu = new QMenu(this);
	m_fileContextMenu->addAction(ResourceHelper::ref().icon("textfield_rename.png"), tr("Rename"), this, SLOT(projectRenameFile()));
  m_fileContextMenu->addAction(fileBrowserAction);
	m_fileContextMenu->addSeparator();
	m_fileContextMenu->addAction(ResourceHelper::ref().icon("bin_closed.png"), trashLabel, this, SLOT(projectRemoveFile()));
}

void MainWindow::setTitle(const QString &title)
{
	setWindowTitle(tr(TITLE) + (title.isEmpty() ? "" : (" - " + title)));
}

void MainWindow::setTabName(QWidget *widget, const QString &string)
{
	ui_tabWidget->setTabText(ui_tabWidget->indexOf(widget), string);
}

void MainWindow::setTabIcon(QWidget *widget, const QIcon &icon)
{
	ui_tabWidget->setTabIcon(ui_tabWidget->indexOf(widget), icon);
}

void MainWindow::setStatusMessage(const QString &message, int time)
{
	ui_statusbar->showMessage(message, time);
}

void MainWindow::setOutputList(const Compiler::OutputList &output)
{
	ui_errors->setOutputList(output);
}

void MainWindow::hideErrors()
{
	ui_errors->hide();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	int widgetCount = ui_tabWidget->count();
	
	while(ui_tabWidget->count() > 0) {
		closeTab(0);
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
		if(w->isVisible()) w->close();
	}
	
	QMainWindow::closeEvent(e);
}

void MainWindow::deleteTab(int index)
{
	QWidget *w = ui_tabWidget->widget(index);
	// Tab *tab = lookup(w);
	ui_tabWidget->removeTab(index);
	removeLookup(w);
	delete w;
	
	// TODO: We've got a possible memory leak here...
	// delete tab;
}

void MainWindow::addTab(Tab *tab)
{
	if(!tab->beginSetup()) return;
	addLookup(tab);
	setUpdatesEnabled(false);
	int tabNum = ui_tabWidget->addTab(tab->widget(), QString::fromLocal8Bit(""));
	ui_tabWidget->setCurrentIndex(tabNum);
	setUpdatesEnabled(true);
	tab->completeSetup();
	moveToTab(tab);
	
	QObject::connect(this, SIGNAL(settingsUpdated()), tab->widget(), SLOT(refreshSettings()));
	
	emit updateActivatable();
}

void MainWindow::moveToTab(Tab *tab)
{
	ui_tabWidget->setCurrentWidget(tab->widget());
}

bool MainWindow::closeTab(const QString &filePath)
{
	for(int i = 0; i < ui_tabWidget->count(); ++i) {
		Tab *current = lookup(ui_tabWidget->widget(i));
		if(filePath == current->file().absoluteFilePath()) {
			closeTab(i);
			return true;
		}
	}

	return false;
}

QList<Tab *> MainWindow::tabs()
{
	return m_lookup.values();
}

void MainWindow::closeAllOthers(Tab *tab)
{
	int i = 0;
	while(ui_tabWidget->count() > 1) {
		if(lookup(ui_tabWidget->widget(i)) == tab) ++i;
		closeTab(i);
	}
}

void MainWindow::open()
{
	QStringList filters;
	filters << "KISS Project (*.kissproj)";
  filters << lexer::Factory::ref().formattedExtensions();
	filters << "Template Pack (*.pack)";
	filters.removeDuplicates();
	QString filePath = FileUtils::getOpenFileName(this, tr("Open"), filters.join(";;") + ";;All Files (*)");
	if(filePath.isEmpty()) return;

	const QString &suffix = QFileInfo(filePath).suffix();
  if(suffix == "kissproj") openProject(QFileInfo(filePath).absolutePath());
	else if(suffix == "pack") addTab(new TemplateTab(filePath, this));
	else openFile(filePath);
}

void MainWindow::openProject()
{
	QStringList filters = lexer::Factory::ref().formattedExtensions();
	filters.removeDuplicates();
	QString filePath = FileUtils::getOpenFileName(this, tr("Open Project"));
	if(filePath.isEmpty()) return;

	openProject(filePath);
}

void MainWindow::next()
{
	ui_tabWidget->setCurrentIndex(ui_tabWidget->currentIndex() + 1);
}

void MainWindow::previous()
{
	ui_tabWidget->setCurrentIndex(ui_tabWidget->currentIndex() - 1);
}

void MainWindow::closeTab(int index, bool force)
{	
	if(ui_tabWidget->count() == 0) return;
	
	if(!lookup(ui_tabWidget->widget(index))->close() && !force) return;
	
	deleteTab(index);
	ui_errors->hide();
	
	emit updateActivatable();
}

void MainWindow::closeCurrentTab(bool force)
{
	closeTab(ui_tabWidget->currentIndex(), force);
}

void MainWindow::closeProjectTabs(const project::ProjectPtr &project)
{
	int i = 0;
	while(i < ui_tabWidget->count()) {
		Tab *current = lookup(ui_tabWidget->widget(i));
		if(current->project() == project) closeTab(i);
		else i++;
	}
	ui_errors->hide();
}

bool MainWindow::closeFile(const QString &file)
{
	if(file.isEmpty()) return false;
	
	QList<Tab *> fileTabs = tabs();
	
	bool removed = false;
	
	foreach(Tab *tab, fileTabs) {
		if(file == tab->file().filePath()) {
			deleteTab(ui_tabWidget->indexOf(tab->widget()));
			removed |= true;
		}
	}
	
	return removed;
}

void MainWindow::about()
{
	dialog::Message::showMessage(this, "About KISS IDE", "about_kiss", QStringList()
		<< QString::number(KISS_IDE_VERSION_MAJOR)
		<< QString::number(KISS_IDE_VERSION_MINOR)
		<< QString::number(KISS_IDE_VERSION_BUILD)
		<< KISS_IDE_VERSION_CODENAME);
}

void MainWindow::settings()
{
	if(m_editorSettingsDialog.exec() == QDialog::Rejected) return;
	emit settingsUpdated();
}

void MainWindow::theme()
{
	if(m_themeSettingsDialog.exec() == QDialog::Rejected) return;
	emit settingsUpdated();
}

bool MainWindow::commPreconditions(const kiss::project::ProjectPtr &project)
{
	for(int i = 0; i < ui_tabWidget->count(); ++i) {
		Tab *current = lookup(ui_tabWidget->widget(i));
		if(current->project() != project) continue;
		SourceFile *sourceFile = dynamic_cast<SourceFile *>(ui_tabWidget->widget(i));
		if(!sourceFile) continue;
		if(!sourceFile->save()) return false;
	}

	if(project->target() && project->target()->available()) return true;
	
	if(!projectChangeTarget(project)) return false;
	if(!project->target()->available()) {
		setStatusMessage(tr("Target \"%1\" is not available for communication.").arg(project->target()->displayName()));
		return false;
	}

	return true;
}

void MainWindow::showCompilerOutput(const Compiler::OutputList &results)
{
	ui_errors->setOutputList(results);
}

void MainWindow::on_ui_tabWidget_currentChanged(int i) 
{
	if(i < 0) return;
	
	setUpdatesEnabled(false);
  setTitle("");
	m_currentTab = lookup(ui_tabWidget->widget(i));
	if(m_currentTab) {
    m_currentTab->activate();
    const project::ProjectPtr project = m_currentTab->project();
    if(project) projectSetActive(project);
  }
	
	emit updateActivatable();
	
	setUpdatesEnabled(true);
}

void MainWindow::updateInfoBox(const QModelIndex &current)
{
  if(m_projectsModel.isProject(current)) {
    const project::ProjectPtr &project = m_projectsModel.project(current);
    if(!project) return;
    ui_infoBox->showProjectInfo(project->name(), project->location(), project->deps());
  }
  else {
    QFileInfo fileInfo(m_projectsModel.filePath(current));
    ui_infoBox->showFileInfo(fileInfo.fileName(), fileInfo.absoluteFilePath());
  }
}

void MainWindow::activeProjectAddNew()
{
  const project::ProjectPtr &project = m_projectManager.activeProject();
  projectAddNew(project, project->location());
}

void MainWindow::selectedProjectAddNew()
{
  const project::ProjectPtr &project = m_projectsModel.project(ui_projects->currentIndex());
  projectAddNew(project, m_projectsModel.filePath(ui_projects->currentIndex()));
}

void MainWindow::projectAddNew(const project::ProjectPtr &project, const QString &dest)
{
  if(!project) return;
  
	SourceFile *const sourceFile = new SourceFile(this);
	sourceFile->setProject(project);
	if(!sourceFile->selectTemplate()) {
		delete sourceFile;
		return;
	}

	bool ok = false;
	QString fileName = QInputDialog::getText(this, tr("New File"), tr("New File Name:"),
		QLineEdit::Normal, QString(), &ok);
	if(!ok) {
		delete sourceFile;
		return;
	}
    
    if(QFileInfo(fileName).baseName() == fileName) {
        fileName += ".";
        fileName += sourceFile->templateExt();
    }

  sourceFile->setFile(QDir(dest).filePath(fileName));
  addTab(sourceFile);
  sourceFile->save();
}

void MainWindow::activeProjectAddExisting()
{
	QStringList filters = lexer::Factory::ref().formattedExtensions();
	filters.removeDuplicates();
	QStringList files = FileUtils::getOpenFileNames(this,
		tr("Select Files to Add"), filters.join(";;") + ";;All Files (*)");
    
  const project::ProjectPtr &project = m_projectManager.activeProject();
  projectAddExisting(project, files, project->location());
}

void MainWindow::selectedProjectAddExisting()
{
	QStringList filters = lexer::Factory::ref().formattedExtensions();
	filters.removeDuplicates();
	QStringList files = FileUtils::getOpenFileNames(this,
		tr("Select Files to Add"), filters.join(";;") + ";;All Files (*)");
  
  const project::ProjectPtr &project = m_projectsModel.project(ui_projects->currentIndex());
	projectAddExisting(project, files, m_projectsModel.filePath(ui_projects->currentIndex()));
}

void MainWindow::droppedProjectAddExisting(QStringList files)
{
  const QModelIndex current = ui_projects->currentIndex();
  projectAddExisting(m_projectsModel.project(current), files,
    m_projectsModel.isFolder(current) ? m_projectsModel.filePath(current) : m_projectsModel.filePath(current.parent()));
}

void MainWindow::projectAddExisting(const project::ProjectPtr &project, QStringList files, const QString &dest)
{
  if(!project || files.isEmpty()) return;

	for(int i = 0; i < ui_tabWidget->count(); ++i) {
		SourceFile *sourceFile = dynamic_cast<SourceFile *>(ui_tabWidget->widget(i));
		if(!sourceFile) continue;
		const QFileInfo &file = sourceFile->file();
		if(files.contains(file.absoluteFilePath())) {
			QMessageBox::warning(this, tr("File Already Open"),
				tr("The following file is open inside KISS: %1\n").arg(file.fileName()) +
				tr("Close the file before adding it to a project."));
			return;
		}
	}

	AddToProjectDialog dialog(this);
	if(dialog.exec() != QDialog::Accepted) return;
	switch(dialog.type()) {
		case AddToProjectDialog::Move:
			foreach(QString file, files) project->addFileAsMovedCopy(file, dest);
			break;
		case AddToProjectDialog::Copy:
			foreach(QString file, files) project->addFileAsCopy(file, dest);
			break;
		case AddToProjectDialog::AbsoluteReference:
			foreach(QString file, files) project->addFileAsLink(file);
			break;
		case AddToProjectDialog::RelativeReference:
			foreach(QString file, files) project->addFileAsRelativeLink(file);
			break;
		default:
			return;
	}
}

void MainWindow::activeProjectAddFolder()
{
  const project::ProjectPtr &project = m_projectManager.activeProject();
  projectAddFolder(project, project->location());
}

void MainWindow::selectedProjectAddFolder()
{
  projectAddFolder(m_projectsModel.project(ui_projects->currentIndex()),
    m_projectsModel.filePath(ui_projects->currentIndex()));
}

void MainWindow::projectAddFolder(const project::ProjectPtr &project, const QString &dest)
{
	bool ok = false;
	const QString folderName = QInputDialog::getText(this, tr("New Folder"), tr("New Folder Name:"),
		QLineEdit::Normal, QString(), &ok);
	if(!ok) return;
  
  if(!project->addFolder(dest, folderName))
		QMessageBox::warning(this, tr("Failed to Create Folder"),
      tr("KISS IDE could not create that folder. Make sure the name is valid."));
}

void MainWindow::activeProjectClose()
{
  projectClose(m_projectManager.activeProject());
}

void MainWindow::selectedProjectClose()
{
	projectClose(m_projectsModel.project(ui_projects->currentIndex()));
}

void MainWindow::projectClose(const project::ProjectPtr &project)
{
	if(!project || !m_projectManager.closeProject(project)) return;

	closeProjectTabs(project);
	m_projectsModel.removeProject(project);
	if(m_projectManager.projects().isEmpty()) {
		ui_projectFrame->setVisible(false);
    activateMenuable(menu::ProjectMenu::menuName(), 0);
	}
}

void MainWindow::activeProjectDelete()
{
  projectDelete(m_projectManager.activeProject());
}

void MainWindow::selectedProjectDelete()
{
  projectDelete(m_projectsModel.project(ui_projects->currentIndex()));
}

void MainWindow::projectDelete(const project::ProjectPtr &project)
{
  if(!project) return;
  
	if(QMessageBox::question(this, QT_TR_NOOP("Are You Sure?"),
			QT_TR_NOOP("Deleting this project will delete all contents of the project folder. Are you sure you want to delete it?"), 
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return;

  if(!project->remove(project->location()))
    QMessageBox::warning(this, tr("Failed to Delete Project"), tr("KISS IDE could not delete the project folder."));
	else projectClose(project);
}

void MainWindow::activeProjectFileBrowser()
{
  projectFileBrowser(m_projectManager.activeProject()->location());
}

void MainWindow::selectedProjectFileBrowser()
{
  projectFileBrowser(m_projectsModel.filePath(ui_projects->currentIndex()));
}

void MainWindow::projectFileBrowser(const QString &path)
{
  if(!QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(path).isDir() ? path : QFileInfo(path).absolutePath())))
    QMessageBox::warning(this, tr("Failed to Open in File Browser"), tr("KISS IDE could not open the file browser."));
}

void MainWindow::activeProjectOpenSettings()
{
  projectOpenSettings(m_projectManager.activeProject());
}

void MainWindow::selectedProjectOpenSettings()
{
  projectOpenSettings(m_projectsModel.project(ui_projects->currentIndex()));
}

void MainWindow::projectOpenSettings(const project::ProjectPtr &project)
{
  if(!project) return;
  
	dialog::ProjectSettingsDialog dialog(project, this);
	dialog.setWindowTitle(tr(QString("Project Settings for " + project->name()).toStdString().c_str()));
	dialog.exec();

	project->setDeps(dialog.depPaths());
	project->setCompilerFlags(dialog.compilerFlags());
	project->setCompileLib(dialog.compileLib());
	project->setAutoCompileDeps(dialog.autoCompileDeps());
  
  updateInfoBox(ui_projects->currentIndex());
}

void MainWindow::projectSetActive(const project::ProjectPtr &project)
{
  if(!m_projectManager.setActiveProject(project)) return;
  emit updateActivatable();
}

const bool MainWindow::activeProjectDownload()
{
	return projectDownload(m_projectManager.activeProject());
}

const bool MainWindow::selectedProjectDownload()
{
  return projectDownload(m_projectsModel.project(ui_projects->currentIndex()));
}

const bool MainWindow::projectDownload(const project::ProjectPtr &project)
{
	if(!commPreconditions(project)) return false;

	return project->download();
}

const bool MainWindow::activeProjectCompile()
{
	return projectCompile(m_projectManager.activeProject());
}

const bool MainWindow::selectedProjectCompile()
{
  return projectCompile(m_projectsModel.project(ui_projects->currentIndex()));
}

const bool MainWindow::projectCompile(const project::ProjectPtr &project)
{
	if(!commPreconditions(project)) return false;

	bool success = true;
	success &= project->download();
	success &= project->compile();

	return success;
}

const bool MainWindow::activeProjectRun()
{
	return projectRun(m_projectManager.activeProject());
}

const bool MainWindow::selectedProjectRun()
{
  return projectRun(m_projectsModel.project(ui_projects->currentIndex()));
}

const bool MainWindow::projectRun(const project::ProjectPtr &project)
{
	if(!commPreconditions(project)) return false;

	bool success = true;
	success &= project->download();
	success &= project->compile();
	success &= project->run();

	return success;
}

const bool MainWindow::activeProjectChangeTarget()
{
	return projectChangeTarget(m_projectManager.activeProject());
}

const bool MainWindow::selectedProjectChangeTarget()
{
  return projectChangeTarget(m_projectsModel.project(ui_projects->currentIndex()));
}

const bool MainWindow::projectChangeTarget(kiss::project::ProjectPtr project)
{
	dialog::Target targetDialog(&target::InterfaceManager::ref(), this);
	if(targetDialog.exec() == QDialog::Rejected) return false;
	if(targetDialog.target().isNull()) return false;
	project->setTarget(targetDialog.target());
	
	// This hooks up all important callbacks
	project->target()->setResponder(m_mainResponder);
		
	return true;
}

void MainWindow::projectRemoveFolder()
{
	if(QMessageBox::question(this, tr("Are You Sure?"),
		tr("Deleting this folder will delete it and its contents. Are you sure you want to delete it?"), 
		QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return;

  project::ProjectPtr project = m_projectsModel.project(ui_projects->currentIndex());
  if(!project->remove(m_projectsModel.filePath(ui_projects->currentIndex())))
    QMessageBox::warning(this, tr("Failed to Delete Folder"), tr("KISS IDE could not delete that folder."));
}

void MainWindow::projectRenameFile()
{
	ui_projects->edit(ui_projects->currentIndex());
}

void MainWindow::projectRemoveFile()
{
	const QModelIndex &index = ui_projects->currentIndex();
	const QString &path = m_projectsModel.filePath(index);
	project::ProjectPtr project = m_projectsModel.project(index);

	if(m_projectsModel.isLink(index)) {
		if(QMessageBox::question(this, QT_TR_NOOP("Are You Sure?"),
			QT_TR_NOOP("Deleting this file will unlink the file from the project. Are you sure you want to delete it?"), 
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return;

		closeTab(path);
		project->removeLink(path);
	}
	else if(m_projectsModel.isFile(index)) {
		if(QMessageBox::question(this, QT_TR_NOOP("Are You Sure?"),
			QT_TR_NOOP("Are you sure you want to delete the file?"), 
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return;

		closeTab(path);
		project->remove(path);
	}
}

void MainWindow::showProjectDock(bool show)
{
	ui_projectFrame->setVisible(show);
}

void MainWindow::hideProjectDock()
{
	showProjectDock(false);
}

void MainWindow::toggleCommunicationWidget()
{
	ui_comm->setVisible(!ui_comm->isVisible());
}

void MainWindow::projectContextMenu(const QPoint &pos)
{
	const QModelIndex &index = ui_projects->indexAt(pos);
	if(!index.isValid()) return;

	if(m_projectsModel.isProject(index))
		m_projectContextMenu->exec(QCursor::pos());
  else if(m_projectsModel.isFolder(index))
    m_folderContextMenu->exec(QCursor::pos());
	else if(m_projectsModel.isLink(index))
    m_linkContextMenu->exec(QCursor::pos());
	else if(m_projectsModel.isFile(index))
		m_fileContextMenu->exec(QCursor::pos());
}

void MainWindow::openRecent()
{
	QAction *action = qobject_cast<QAction*>(sender());
	QString file = action->data().toString();
	QSettings settings;
	QStringList current = settings.value(RECENTS).toStringList();
	current.push_front(file);
	current.removeDuplicates();
	settings.setValue(RECENTS, current);
	if(!action) return;
	openFile(file);
}

void MainWindow::errorClicked(QListWidgetItem *item)
{
}

void MainWindow::addLookup(Tab *tab)
{
	if(lookup(tab->widget())) return;
	m_lookup.insert(tab->widget(), tab);
}

void MainWindow::removeLookup(QWidget *widget)
{
	m_lookup.remove(widget);
}

Tab *MainWindow::lookup(QWidget *widget) const
{
	QMap<QWidget*, Tab *>::const_iterator it = m_lookup.find(widget);
	return it == m_lookup.end() ? 0 : *it;
}

// TODO: Make Error Googlable
void MainWindow::showContextMenuForError(const QPoint&)
{	
}

void MainWindow::projectClicked(const QModelIndex &index)
{
}

void MainWindow::projectDoubleClicked(const QModelIndex &index)
{
	project::ProjectPtr project = m_projectsModel.project(index);
	if(m_projectsModel.isFileEditable(index))
		openFile(m_projectsModel.filePath(index), project);
  else if(m_projectsModel.isFolder(index))
    ui_projects->setExpanded(index, !ui_projects->isExpanded(index));
}

void MainWindow::authenticateTarget(const kiss::target::TargetPtr &target,
	kiss::target::CommunicationManager *manager)
{
	dialog::Password dialog(this);
	if(dialog.exec() != QDialog::Accepted) {
		manager->clearQueue();
	} else {
		target->setPassword(dialog.password());
	}
	
	manager->setPaused(false);
}

void MainWindow::oldDeviceSoftware(const kiss::target::TargetPtr &target)
{
	dialog::Message::showError(this, "simple_error_with_action", QStringList() <<
		tr("Communication failed with %1.").arg(target->displayName().isEmpty()
			? tr("the active target") : target->displayName()) <<
		tr("The target has outdated software installed. It isn't compatible with this version "
			"of KISS IDE.") <<
		tr("Please update the target's software or use an older version of KISS IDE."));
}

void MainWindow::oldHostSoftware(const kiss::target::TargetPtr &target)
{
	dialog::Message::showError(this, "simple_error_with_action", QStringList() <<
		tr("Communication failed with %1.").arg(target->displayName().isEmpty()
			? tr("the active target") : target->displayName()) <<
		tr("The target's installed software is too new. It isn't compatible with this version "
			"of KISS IDE.") <<
		tr("Please update KISS IDE or downgrade the target's installed software."));
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
	if(event->type() == QEvent::FileOpen) {
		QString fileName = dynamic_cast<QFileOpenEvent *>(event)->file();
		openFile(fileName);
		event->accept();
		return true;
	} else return QMainWindow::eventFilter(target, event);
}

menu::Menuable *MainWindow::menuable(const QString &name)
{
	foreach(menu::Menuable *menuable, m_menuables) {
		if(menuable->name() == name) return menuable;
	}
	return 0;
}

QList<menu::Menuable *> MainWindow::menuablesExcept(const QStringList &names)
{
	QList<menu::Menuable *> ret;
	foreach(menu::Menuable *menuable, m_menuables) {
		if(!names.contains(menuable->name())) ret.append(menuable);
	}
	return ret;
}

void MainWindow::deactivateMenuablesExcept(const QStringList &names)
{
	foreach(menu::Menuable *menu, menuablesExcept(names)) {
		ActivatableObject *activatable = dynamic_cast<ActivatableObject *>(menu);
		if(activatable) activatable->setActive(0);
	}
}

QList<menu::Menuable *> MainWindow::menuables()
{
	return m_menuables;
}

void MainWindow::activateMenuable(const QString &name, QObject *on)
{
	ActivatableObject *activatable = dynamic_cast<ActivatableObject *>(menuable(name));
	activatable->setActive(on);
}

QStringList MainWindow::standardMenus() const
{
	return QStringList() << menu::FileOperationsMenu::menuName() << menu::MainWindowMenu::menuName()
    << menu::ProjectMenu::menuName()
#ifdef BUILD_DOCUMENTATION_TAB
	<< menu::DocumentationMenu::menuName()
#endif
#ifdef BUILD_DEVELOPER_TOOLS
	<< menu::DeveloperMenu::menuName()
#endif
	;
}

templates::Manager *MainWindow::templateManager() const
{
	return m_templateManager;
}

QList<QObject*> MainWindow::tabs(const QString &name)
{
	QList<QObject *> ret;
	QList<Tab *> all = tabs();
	foreach(Tab *tab, all) {
		QObject *t = dynamic_cast<QObject *>(tab);
		if(!t) continue;
		const QMetaObject *meta = t->metaObject();
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

project::Manager *MainWindow::projectManager()
{
  return &m_projectManager;
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