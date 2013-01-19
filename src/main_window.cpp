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
#include "menus.hpp"
#include "project.hpp"
#include "project_settings.hpp"
#include "project_manager.hpp"
#include "log.hpp"
#include "lexer_factory.hpp" // Used to query supported extensions
#include "new_project_wizard.hpp"
#include "template_manager.hpp"
#include "template_tab.hpp"
#include "template_pack.hpp"
#include "main_responder.hpp"
#include "communication_progress_bar.hpp"
#include "communication_manager.hpp"
#include "file_utils.hpp"

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

using namespace Kiss;
using namespace Kiss::Widget;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	m_currentTab(0),
	m_templateManager(new Template::Manager),
	m_commProgress(new CommunicationProgressBar(&Target::CommunicationManager::ref(), this)),
	m_mainResponder(new Target::MainResponder(this))
{
	QNetworkProxyFactory::setUseSystemConfiguration(true);
	
	m_templateManager->loadDefaultPacks();
	
	setupUi(this);
	
	ui_projects->setModel(&m_projectsModel);
	
	
	// ui_projects->viewport()->setAcceptDrops(true);
	// ui_projects->setDropIndicatorShown(true);
	// ui_projects->setDragDropMode(QAbstractItemView::DropOnly);
	
	// ui_projects->hide(); // Disabled for now
	
	setUpdatesEnabled(false);

	/* Deletes the tab that QTabWidget starts with by default */
	deleteTab(0);

	hideErrors();
	
	connect(ui_projects, SIGNAL(clicked(QModelIndex)),
		SLOT(projectFileClicked(QModelIndex)));
	connect(ui_projects, SIGNAL(doubleClicked(QModelIndex)),
		SLOT(projectClicked(QModelIndex)));
	
	connect(&m_projectManager, SIGNAL(projectOpened(Kiss::Project::ProjectPtr)),
		SLOT(projectOpened(Kiss::Project::ProjectPtr)));
	connect(&m_projectManager, SIGNAL(projectClosed(Kiss::Project::ProjectPtr)),
		SLOT(projectClosed(Kiss::Project::ProjectPtr)));
	
	ui_projectFrame->setVisible(false);
	
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
}

void MainWindow::importTemplatePack()
{
	QSettings settings;
	QString openPath = settings.value(OPEN_PATH, QDir::homePath()).toString();
	QStringList filters;
	filters << "Template Pack (*.pack)";
	filters.removeDuplicates();
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open"), openPath, filters.join(";;") + ";;All Files (*)");
	
	if(filePath.isEmpty()) return;
	
	Template::PackPtr pack = Template::Pack::load(filePath);
	if(!pack) {
		Dialog::Message::showError(this, "simple_error_with_action", QStringList()
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
	addTab(new TemplateTab(Template::Pack::create(), this));
}

Project::ProjectPtr MainWindow::newProject()
{
	Wizard::NewProject wizard(this);

	if(wizard.exec() == QDialog::Rejected) return Project::ProjectPtr();
	const QString &saveLocation = wizard.saveLocation();
	
	if(QDir(saveLocation).exists()) {
		QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Are You Sure?"),
			tr("Overwrite %1?").arg(saveLocation),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if(ret == QMessageBox::No) return Project::ProjectPtr();
	}
	
	return newProject(saveLocation);
}

void MainWindow::newFile()
{
	addTab(new SourceFile(this));
}

bool MainWindow::openFile(const QString& file, const Project::ProjectPtr& project)
{
	QFileInfo fileInfo(file);

	if(!fileInfo.isFile() || !fileInfo.isReadable()) return false;

	for(int i = 0; i < ui_tabWidget->count(); ++i) {
		SourceFile* sourceFile = dynamic_cast<SourceFile *>(ui_tabWidget->widget(i));
		if(sourceFile && sourceFile->file() == file) {
			ui_tabWidget->setCurrentIndex(i);
			on_ui_tabWidget_currentChanged(i);
			return true;
		}
	}
	
	/* Attempt to open the selected file */
	SourceFile *sourceFile = new SourceFile(this);
	
	if(!sourceFile->fileOpen(file)) {
		Dialog::Message::showError(this, "simple_error", QStringList() <<
			tr("Could not open ") + QFileInfo(sourceFile->file()).fileName() <<
			tr("Unable to open file for reading."));
		delete sourceFile;
		return false;
	}
	
	sourceFile->setProject(project);

	QSettings settings;
	QStringList current = settings.value(RECENTS).toStringList().mid(0, 5);
	current.push_front(fileInfo.absoluteFilePath());
	current.removeDuplicates();
	settings.setValue(RECENTS, current);
	
	addTab(sourceFile);

	return true;
}

bool MainWindow::memoryOpen(const QByteArray& ba, const QString& assocPath)
{
	QFileInfo fileInfo(assocPath);

	if(!fileInfo.isFile() || !fileInfo.isReadable()) return false;

	for(int i = 0; i < ui_tabWidget->count(); ++i) {
		SourceFile* sourceFile = dynamic_cast<SourceFile*>(ui_tabWidget->widget(i));
		if(sourceFile && sourceFile->file() == assocPath) {
			ui_tabWidget->setCurrentIndex(i);
			on_ui_tabWidget_currentChanged(i);
			return true;
		}
	}
	
	/* Attempt to open the selected file */
	SourceFile *sourceFile = new SourceFile(this);
	if(!sourceFile->memoryOpen(ba, assocPath)) {
		Dialog::Message::showError(this, "simple_error", QStringList() <<
			tr("Could not open ") + QFileInfo(sourceFile->file()).fileName() <<
			tr("Unable to open file from memory."));
		delete sourceFile;
		return false;
	}	

	addTab(sourceFile);

	hideProjectDock();

	return true;
}

bool MainWindow::openProject(const QString& filePath)
{
	Project::ProjectPtr project = Project::Project::load(filePath);
	Log::ref().info(QString("Opening project at %1").arg(filePath));
	if(project) m_projectManager.openProject(project);
	// ui_projects->expandAll();
	return project;
}

Project::ProjectPtr MainWindow::newProject(const QString& folderPath)
{
	if(!FileUtils::remove(folderPath)) return Project::ProjectPtr();
	if(!QDir().mkpath(folderPath)) return Project::ProjectPtr();
	Project::ProjectPtr project = Project::Project::create(folderPath);
	if(!project) return Project::ProjectPtr();
	
	m_projectManager.openProject(project);
	qDebug() << "Creating project at" << folderPath;
	return project;
}

void MainWindow::initMenus()
{
	using namespace Kiss::Menu;
	
	menuBar()->clear();
	
	FileOperationsMenu* fileOpMenu = new FileOperationsMenu();
	m_menuManager.registerMenus(fileOpMenu);
	fileOpMenu->setActive(this);
	m_menuManager.addActivation(fileOpMenu);
	m_menuables.append(fileOpMenu);
	
	SourceFileMenu* sourceFileMenu = new SourceFileMenu(this);
	m_menuManager.registerMenus(sourceFileMenu);
	m_menuables.append(sourceFileMenu);
	
	TemplatePackMenu* templatePackMenu = new TemplatePackMenu(this);
	m_menuManager.registerMenus(templatePackMenu);
	m_menuables.append(templatePackMenu);
	
	MainWindowMenu* mainWindowMenu = new MainWindowMenu(this);
	m_menuManager.registerMenus(mainWindowMenu);
	mainWindowMenu->setActive(this);
	m_menuManager.addActivation(mainWindowMenu);
	m_menuables.append(mainWindowMenu);
	
	TargetMenu* targetMenu = new TargetMenu;
	m_menuManager.registerMenus(targetMenu);
	m_menuables.append(targetMenu);

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

void MainWindow::setTitle(const QString& title)
{
	setWindowTitle(tr(TITLE) + (title.isEmpty() ? "" : (" - " + title)));
}

void MainWindow::setTabName(QWidget *widget, const QString& string)
{
	ui_tabWidget->setTabText(ui_tabWidget->indexOf(widget), string);
}

void MainWindow::setTabIcon(QWidget *widget, const QIcon& icon)
{
	ui_tabWidget->setTabIcon(ui_tabWidget->indexOf(widget), icon);
}

void MainWindow::setStatusMessage(const QString& message, int time)
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
		if(w->isVisible()) w->close();
	}
	
	QMainWindow::closeEvent(e);
}

void MainWindow::deleteTab(int index)
{
	QWidget* w = ui_tabWidget->widget(index);
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
	int tabNum = ui_tabWidget->addTab(tab->widget(), QString::fromAscii(""));
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

QTabWidget *MainWindow::tabWidget()
{
	return ui_tabWidget;
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
		deleteTab(i);
	}
}

void MainWindow::refreshMenus()
{
	
}

void MainWindow::open()
{
	QSettings settings;
	QString openPath = settings.value(OPEN_PATH, QDir::homePath()).toString();
	QStringList filters = Lexer::Factory::ref().formattedExtensions();
	filters << "Template Pack (*.pack)";
	filters << "KISS Project (*.kissproj)";
	filters.removeDuplicates();
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open"), openPath, filters.join(";;") + ";;All Files (*)");
	
	if(filePath.isEmpty()) return;

	QFileInfo fileInfo(filePath);
	settings.setValue(OPEN_PATH, fileInfo.absolutePath());

	if(fileInfo.completeSuffix() == "pack") {
		addTab(new TemplateTab(filePath, this));
		return;
	}

	if(fileInfo.completeSuffix() == "kissproj") {
		openProject(filePath);
		return;
	}
	
	openFile(filePath);
}

void MainWindow::openProject()
{
	/* QSettings settings;
	QString openPath = settings.value(OPEN_PATH, QDir::homePath()).toString();
	QStringList filters = Lexer::Factory::ref().formattedExtensions();
	filters.removeDuplicates();
	QString filePath = QFileDialog::getOpenFolderName(this, tr("Open Project"), openPath, tr(""));
		
	if(filePath.isEmpty()) return;

	QFileInfo fileInfo(filePath);
	settings.setValue(OPEN_PATH, fileInfo.absolutePath());

	openProject(filePath); */
}

void MainWindow::next()
{
	ui_tabWidget->setCurrentIndex(ui_tabWidget->currentIndex() + 1);
}

void MainWindow::previous()
{
	ui_tabWidget->setCurrentIndex(ui_tabWidget->currentIndex() - 1);
}

void MainWindow::closeTab(bool force)
{	
	if(ui_tabWidget->count() == 0) return;
	
	if(!lookup(ui_tabWidget->currentWidget())->close() && !force) return;
	
	deleteTab(ui_tabWidget->currentIndex());
	ui_errors->hide();
	
	emit updateActivatable();
}

void MainWindow::closeProjectTabs(const Project::ProjectPtr& project)
{
	QList<Tab *> all = tabs();
	int i = 0;
	while(i < ui_tabWidget->count()) {
		Tab *current = lookup(ui_tabWidget->widget(i));
		if(current->project() == project) deleteTab(i);
		else i++;
	}
	ui_errors->hide();
}

bool MainWindow::closeFile(const QString& file)
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
	Dialog::Message::showMessage(this, "About KISS IDE", "about_kiss", QStringList()
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

void MainWindow::showCompilerOutput(const Compiler::OutputList& results)
{
	ui_errors->setOutputList(results);
}

void MainWindow::on_ui_tabWidget_currentChanged(int i) 
{
	if(i < 0) return;
	
	setUpdatesEnabled(false);
	m_currentTab = lookup(ui_tabWidget->widget(i));
	setTitle("");
	if(m_currentTab) m_currentTab->activate();
	
	emit updateActivatable();
	
	setUpdatesEnabled(true);
}

void MainWindow::on_ui_addFile_clicked()
{
	/*Project::ProjectPtr project = activeProject();
	if(!project) {
		Dialog::Message::showError(this, "simple_error", QStringList() << 
			tr("Unable to determine active project.") <<
			tr("Please make sure that you have a project open."));
		return;
	} 
	
	SourceFile* source = new SourceFile(this);
	source->setProject(project);
	addTab(source);*/
}

void MainWindow::on_ui_removeFile_clicked()
{
	/*foreach(const QModelIndex& index, ui_projects->selectionModel()->selectedRows()) {
		Project::ProjectPtr project = m_projectsModel.indexToProject(index);
		if(m_projectsModel.isIndexProject(index)) {
			m_projectManager.closeProject(project);
		}
	}*/
}

void MainWindow::projectAddNew()
{
	on_ui_addFile_clicked();
}

void MainWindow::projectAddExisting()
{
	/* Project::ProjectPtr project = activeProject();
	if(!project) return;
	QString openPath = QSettings().value(OPEN_PATH, QDir::homePath()).toString();
	QStringList filters = Lexer::Factory::ref().formattedExtensions();
	filters << "KISS Project (*.kissproj)";
	filters.removeDuplicates();
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select One or More Files to Add"),
		openPath, filters.join(";;") + ";;All Files (*)");
	foreach(const QString& file, files) project->copyFile(file); */
}

void MainWindow::projectRemoveExisting()
{
	on_ui_removeFile_clicked();
}

void MainWindow::projectExtractTo()
{
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

void MainWindow::errorClicked(QListWidgetItem* item)
{
}

void MainWindow::addLookup(Tab *tab)
{
	if(lookup(tab->widget())) return;
	m_lookup.insert(tab->widget(), tab);
}

void MainWindow::removeLookup(QWidget* widget)
{
	m_lookup.remove(widget);
}

Tab *MainWindow::lookup(QWidget* widget) const
{
	QMap<QWidget*, Tab *>::const_iterator it = m_lookup.find(widget);
	return it == m_lookup.end() ? 0 : *it;
}

// TODO: Make Error Googlable
void MainWindow::showContextMenuForError(const QPoint&)
{
	
}

void MainWindow::projectClicked(const QModelIndex& index)
{
	/* Project* project = m_projectsModel.indexToProject(index);
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
	} */
}

void MainWindow::projectFileClicked(const QModelIndex& index)
{
	/* if(m_projectsModel.isIndexProject(index)) return;
	QString path = m_projectsModel.indexToPath(index);
	openFile(path, m_projectsModel.indexToProject(index));*/
}

void MainWindow::projectOpened(const Project::ProjectPtr& project)
{
	/*Project::Manager *manager = qobject_cast<Project::Manager *>(sender());
	m_projectsModel.addProject(project);
	if(manager) ui_projectFrame->setVisible(manager->projects().size());*/
}

void MainWindow::projectClosed(const Project::ProjectPtr& project)
{
	/*closeProjectTabs(project);
	m_projectsModel.removeProject(project);
	Project::Manager *manager = qobject_cast<Project::Manager *>(sender());
	if(manager) ui_projectFrame->setVisible(manager->projects().size());*/
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
        if(event->type() == QEvent::FileOpen) {
                QString fileName = dynamic_cast<QFileOpenEvent *>(event)->file();
                openFile(fileName);
                event->accept();
                return true;
        } else return QMainWindow::eventFilter(target, event);
}

Target::Responder *MainWindow::mainResponder() const
{
	return m_mainResponder;
}

Project::Manager *MainWindow::projectManager()
{
	return &m_projectManager;
}

Menu::Manager* MainWindow::menuManager()
{
	return &m_menuManager;
}

Menu::Menuable* MainWindow::menuable(const QString& name)
{
	foreach(Menu::Menuable *menuable, m_menuables) {
		if(menuable->name() == name) return menuable;
	}
	return 0;
}

QList<Menu::Menuable *> MainWindow::menuablesExcept(const QStringList& names)
{
	QList<Menu::Menuable *> ret;
	foreach(Menu::Menuable *menuable, m_menuables) {
		if(!names.contains(menuable->name())) ret.append(menuable);
	}
	return ret;
}

void MainWindow::deactivateMenuablesExcept(const QStringList& names)
{
	foreach(Menu::Menuable *menu, menuablesExcept(names)) {
		ActivatableObject *activatable = dynamic_cast<ActivatableObject *>(menu);
		if(activatable) activatable->setActive(0);
	}
}

QList<Menu::Menuable *> MainWindow::menuables()
{
	return m_menuables;
}

void MainWindow::activateMenuable(const QString& name, QObject *on)
{
	ActivatableObject *activatable = dynamic_cast<ActivatableObject *>(menuable(name));
	activatable->setActive(on);
}

QStringList MainWindow::standardMenus() const
{
	return QStringList() << Menu::FileOperationsMenu::menuName() << Menu::MainWindowMenu::menuName()
#ifdef BUILD_DOCUMENTATION_TAB
	<< Menu::DocumentationMenu::menuName()
#endif
#ifdef BUILD_DEVELOPER_TOOLS
	<< Menu::DeveloperMenu::menuName()
#endif
	;
}

Template::Manager *MainWindow::templateManager() const
{
	return m_templateManager;
}

QList<QObject*> MainWindow::tabs(const QString& name)
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

Project::ProjectPtr MainWindow::activeProject() const
{
	// Precedence is like so: Project Manager, Tab, Project Tree
	const Project::ProjectPtrList& loadedProjects = m_projectManager.projects();
	if(loadedProjects.size() == 0) return Project::ProjectPtr();
	if(loadedProjects.size() == 1) return loadedProjects[0];
	
	if(m_currentTab && m_currentTab->hasProject()) return m_currentTab->project();
	const QModelIndexList& list = ui_projects->selectionModel()->selectedRows();
//	return list.size() > 0 ? m_projectsModel.indexToProject(list[0]) : Project::ProjectPtr();
	return Project::ProjectPtr();
}
