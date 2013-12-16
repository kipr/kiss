#ifndef _KAR_MODEL_HPP_
#define _KAR_MODEL_HPP_

#include <QStandardItemModel>
#include <QModelIndex>
#include <QFileSystemWatcher>
#include <QMimeData>

#include "project.hpp"

namespace kiss
{
	namespace project
	{	
		class Model : public QStandardItemModel
		{
		Q_OBJECT
		public:
			Model(QObject *parent = 0);
			~Model();

			void addProject(ProjectPtr project);
			void removeProject(ProjectPtr project);
			const QStringList &projects() const;

			// void setDependencies(ProjectPtr project, const QStringList &deps);

			// bool isDependency(const QModelIndex &index) const;
			bool isProject(const QModelIndex &index) const;
			bool isLink(const QModelIndex &index) const;
			bool isFile(const QModelIndex &index) const;
			bool isFileEditable(const QModelIndex &index) const;
			ProjectPtr project(const QModelIndex &index) const;
			QString filePath(const QModelIndex &index) const;

			Qt::ItemFlags flags(const QModelIndex &index) const;
			QStringList mimeTypes() const;
			QMimeData *mimeData(const QModelIndexList &indexes) const;
			bool dropMimeData(const QMimeData *data, Qt::DropAction action,
				int row, int column, const QModelIndex &parent);

		public slots:
			void activeChanged(const kiss::project::ProjectPtr &oldActive,
				const kiss::project::ProjectPtr &newActive);

		private slots:
			void directoryChanged(const QString &path);
			void fileChanged(const QString &path);
			void itemChanged(QStandardItem *item);
      
    private:
      void refreshAll(QStandardItem *item);
			
		signals:
			void filesDropped(QStringList files);

		private:
			QStringList m_paths;
			QFileSystemWatcher m_watcher;
		};
	}
}

#endif
