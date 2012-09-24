#include "project_manager.hpp"
#include "project.hpp"

using namespace Kiss::Project;

Manager::Manager()
{
	
}

Manager::~Manager()
{
	while(!m_projects.isEmpty()) {
		closeProject(m_projects.first());
	}
}

void Manager::openProject(Project *project)
{
	if(m_projects.contains(project)) return;
	
	m_projects.append(project);
	emit projectOpened(project);
}

void Manager::closeProject(Project *project)
{
	if(!m_projects.contains(project)) return;
	
	m_projects.removeAll(project);
	emit projectClosed(project);
	delete project;
}

ProjectPtrList Manager::projects() const
{
	return m_projects;
}