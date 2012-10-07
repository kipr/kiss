#ifndef _PROJECT_MODEL_HPP_
#define _PROJECT_MODEL_HPP_

#include <QStandardItemModel>
#include <QFileSystemWatcher>
#include <QString>
#include <QStringList>
#include <QObject>

namespace Kiss
{
	namespace Project
	{
		class Project;
		
		class MultiRootFilesystemModel : public QStandardItemModel
		{
		Q_OBJECT
		public:
			MultiRootFilesystemModel(QObject *parent = 0);
			
			void addRootPath(const QString& path);
			void removeRootPath(const QString& path);
			const QStringList& rootPaths() const;
		
		private slots:
			void pathChanged(const QString& path);
			void itemChanged(QStandardItem *item);
		
		private:
			QStringList m_paths;
			QFileSystemWatcher m_watcher;
		};
		
		class Model : public MultiRootFilesystemModel
		{
		Q_OBJECT
		public:
			Model();
			~Model();
			
			Kiss::Project::Project *indexToProject(const QModelIndex& index) const;
			
			void addProject(Kiss::Project::Project *project);
			void removeProject(Kiss::Project::Project *project);
		private:
			QMap<QString, Kiss::Project::Project *> m_projects;
		};
	}
}

#endif
