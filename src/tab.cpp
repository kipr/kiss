#include "tab.hpp"

using namespace Kiss::Widget;

Tab::Tab(QWidget *widget, MainWindow *mainWindow)
	: m_widget(widget),
	m_mainWindow(mainWindow),
 	m_project(0),
	m_file("")
{
	
}

Tab::~Tab()
{
	
}

void Tab::activate()
{
	qDebug() << "Activated";
}

QWidget* Tab::widget() const
{
	return m_widget;
}

MainWindow *Tab::mainWindow() const
{
	return m_mainWindow;
}

void Tab::setMainWindow(MainWindow *mainWindow)
{
	m_mainWindow = mainWindow;
}

void Tab::setFile(const QString& file)
{
	m_file.setFile(file);
}

void Tab::setFile(const QFileInfo& file)
{
	m_file = file;
	fileChanged(file);
}

const QFileInfo& Tab::file() const
{
	return m_file;
}

bool Tab::hasFile() const
{
	return !m_file.fileName().isEmpty();
}

void Tab::setProject(Kiss::Project::Project *project)
{
	m_project = project;
	projectChanged(project);
}

Kiss::Project::Project *Tab::project() const
{
	return m_project;
}

bool Tab::hasProject() const
{
	return m_project;
}

void Tab::fileChanged(const QFileInfo&)
{
}

void Tab::projectChanged(Kiss::Project::Project *)
{
}