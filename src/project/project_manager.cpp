#include "project_manager.hpp"
#include "project.hpp"

#include <QDebug>

using namespace kiss::project;

Manager::Manager()
	: m_activeProject(ProjectPtr())
{

}

Manager::~Manager()
{
	m_projects.clear();
}

bool Manager::openProject(const ProjectPtr &project)
{
	if(m_projects.contains(project)) return false;
	m_projects.append(project);

	return true;
}

bool Manager::closeProject(const ProjectPtr &project)
{
	if(!m_projects.contains(project)) return false;

	m_projects.removeAll(project);
	if(project == m_activeProject) {
		m_projects.isEmpty() ? setActiveProject(ProjectPtr()) : setActiveProject(m_projects.last());
	}

	return true;
}

void Manager::setActiveProject(const ProjectPtr &project)
{
	const ProjectPtr old = m_activeProject;
	m_activeProject = project;
	emit activeChanged(old, m_activeProject);
}

void Manager::unsetActiveProject(const ProjectPtr &project)
{
	if(project != m_activeProject) return;
	m_projects.isEmpty() ? setActiveProject(ProjectPtr()) : setActiveProject(m_projects.last());	
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
	return QStringList() << PROJECT_EXT << LINKS_EXT << DEPS_EXT;
}
