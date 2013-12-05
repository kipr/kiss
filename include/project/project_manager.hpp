#ifndef _PROJECT_MANAGER_HPP_
#define _PROJECT_MANAGER_HPP_

#include "project.hpp"

#include <QObject>
#include <QList>

namespace kiss
{
	namespace project
	{
		class Project;
		
		typedef QList<ProjectPtr> ProjectPtrList; 
		
		class Manager : public QObject
		{
		Q_OBJECT
		public:
			Manager();
			~Manager();
			
			bool openProject(const ProjectPtr &project);
			bool closeProject(const ProjectPtr &project);

			void setActiveProject(const ProjectPtr &project);
			void unsetActiveProject(const ProjectPtr &project);
			const ProjectPtr &activeProject() const;
			
			const ProjectPtrList &projects() const;

			static const QStringList hiddenExtensions();
		signals:
			void activeChanged(const kiss::project::ProjectPtr &oldActive,
				const kiss::project::ProjectPtr &newActive);
			
		private:
			ProjectPtrList m_projects;
			ProjectPtr m_activeProject;
		};
	}
}

#endif
