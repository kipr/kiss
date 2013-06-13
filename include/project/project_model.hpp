#ifndef _KAR_MODEL_HPP_
#define _KAR_MODEL_HPP_

#include <QStandardItemModel>
#include <QModelIndex>
#include <QFileSystemWatcher>

#include "project.hpp"

namespace Kiss
{
	namespace Project
	{	
		class Model : public QStandardItemModel
		{
			Q_OBJECT
		public:
			Model(QObject *parent = 0);
			~Model();

			void addProject(ProjectPtr project);
			void removeProject(ProjectPtr project);

			void addRootPath(ProjectPtr project);
			void removeRootPath(const QString& path);
			const QStringList& rootPaths() const;

			bool isIndexProject(const QModelIndex& index) const;
			bool isLink(const QModelIndex &index) const;
			bool isFile(const QModelIndex &index) const;
			bool isProjectRoot(const QModelIndex &index) const;
			QString filePath(const QModelIndex &index) const;
			ProjectPtr project(const QModelIndex &index) const;

		private slots:
			void pathChanged(const QString& path);
			void itemChanged(QStandardItem *item);

		private:
			QStringList m_paths;
			QFileSystemWatcher m_watcher;
		};
	}
}

#endif
