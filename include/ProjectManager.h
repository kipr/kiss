#ifndef _PROJECTMANAGER_H_
#define _PROJECTMANAGER_H_

#include "Singleton.h"

#include <QList>

class Project;

class ProjectManager : public Singleton<ProjectManager>
{
public:
	~ProjectManager();
	
	void openProject(Project* project);
	void closeProject(Project* project);
	
	const QList<Project*>& projects();
private:
	QList<Project*> m_projects;
};

#endif
