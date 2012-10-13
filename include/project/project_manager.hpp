#ifndef _PROJECT_MANAGER_HPP_
#define _PROJECT_MANAGER_HPP_

#include "project.hpp"

#include <QObject>
#include <QList>

namespace Kiss
{
	namespace Project
	{
		class Project;
		
		typedef QList<ProjectPtr> ProjectPtrList; 
		
		class Manager : public QObject
		{
		Q_OBJECT
		public:
			Manager();
			~Manager();
			
			void openProject(const ProjectPtr& project);
			void closeProject(const ProjectPtr& project);
			
			const ProjectPtrList& projects() const;
		signals:
			void projectOpened(const Kiss::Project::ProjectPtr& project);
			void projectClosed(const Kiss::Project::ProjectPtr& project);
			
		private:
			ProjectPtrList m_projects;
		};
	}
}

#endif
