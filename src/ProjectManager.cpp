#include "ProjectManager.h"

#include "Project.h"

ProjectManager::~ProjectManager()
{
	foreach(Project* p, m_projects) delete p;
}

void ProjectManager::openProject(Project* project)
{
	m_projects.append(project);
}

void ProjectManager::closeProject(Project* project)
{
	m_projects.removeAll(project);
	delete project;
}

const QList<Project*>& ProjectManager::projects() { return m_projects; }