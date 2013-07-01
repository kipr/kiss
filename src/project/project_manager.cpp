#include "project_manager.hpp"
#include "project.hpp"

#include <QDebug>

using namespace Kiss::Project;

Manager::Manager()
	: m_activeProject(ProjectPtr())
{

}

Manager::~Manager()
{
	m_projects.clear();
}

void Manager::openProject(const ProjectPtr &project)
{
	if(m_projects.contains(project)) return;
	
	m_projects.append(project);
	m_activeProject = project;
	emit projectOpened(project);
}

void Manager::closeProject(const ProjectPtr &project)
{
	if(!m_projects.contains(project)) return;

	ProjectPtr ref = project;
	m_projects.removeAll(project);
	if(project == m_activeProject) {
		m_projects.isEmpty() ? m_activeProject = ProjectPtr() : m_activeProject = m_projects.last();
	}
	emit projectClosed(ref);
}

void Manager::setActiveProject(const ProjectPtr &project)
{
	m_activeProject = project;
}

const ProjectPtr &Manager::activeProject() const
{
	return m_activeProject;
}

const ProjectPtrList &Manager::projects() const
{
	return m_projects;
}

const QStringList Manager::hiddenExtensions()
{
	return QStringList() << PROJECT_EXT << LINKS_EXT;
}