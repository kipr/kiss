#ifndef _PROJECT_MANAGER_HPP_
#define _PROJECT_MANAGER_HPP_

#include <QObject>
#include <QList>

namespace Kiss
{
	namespace Project
	{
		class Project;
		
		typedef QList<Project *> ProjectPtrList; 
		
		class Manager : public QObject
		{
		Q_OBJECT
		public:
			Manager();
			~Manager();
			
			void openProject(Project *project);
			void closeProject(Project *project);
			
			ProjectPtrList projects() const;
		signals:
			void projectOpened(Kiss::Project::Project *project);
			void projectClosed(Kiss::Project::Project *project);
			
		private:
			ProjectPtrList m_projects;
		};
	}
}

#endif
