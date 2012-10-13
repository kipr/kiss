#include "tab.hpp"

using namespace Kiss::Widget;

Tab::Tab(QWidget *widget, MainWindow *mainWindow)
	: m_widget(widget),
	m_mainWindow(mainWindow),
	m_file(""),
	m_project(0)
{
	
}

Tab::~Tab()
{
	
}

void Tab::activate()
{
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
	fileChanged(file);
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

void Tab::setProject(const Kiss::Project::ProjectPtr& project)
{
	m_project = project;
	projectChanged(project);
}

const Kiss::Project::ProjectPtr& Tab::project() const
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

void Tab::projectChanged(const Kiss::Project::ProjectPtr&)
{
}