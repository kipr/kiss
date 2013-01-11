#ifndef _KAR_MODEL_HPP_
#define _KAR_MODEL_HPP_

#include <QStandardItemModel>
#include <QModelIndex>

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

			bool isLink(const QModelIndex &index) const;
			bool isFile(const QModelIndex &index) const;
			bool isProjectRoot(const QModelIndex &index) const;
			QString filePath(const QModelIndex &index) const;
			ProjectPtr project(const QModelIndex &index) const;
		};
	}
}

#endif
