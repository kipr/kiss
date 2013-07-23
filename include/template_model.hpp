#ifndef _TEMPLATE_MODEL_HPP_
#define _TEMPLATE_MODEL_HPP_

#include "template_file.hpp"
#include "template_pack.hpp"

#include <QStandardItemModel>

namespace kiss
{
	namespace templates
	{
		class Pack;
		class Manager;
		
		class Model : public QStandardItemModel
		{
		Q_OBJECT
		Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
		public:
			Model(const Manager *manager, QObject *parent = 0);
			
			Pack *indexToPack(const QModelIndex &index) const;
			templates::File indexToFile(const QModelIndex &index) const;
			bool isIndexPack(const QModelIndex &index) const;
			bool isIndexFile(const QModelIndex &index) const;
			
			void setReadOnly(const bool &readOnly);
			const bool &isReadOnly() const;
			
		private slots:
			void packAdded(const kiss::templates::PackPtr &pack);
			void packRemoved(kiss::templates::Pack *pack);
			void packNameChanged(const QString &name);
			void packFileAdded(const QString &path, const kiss::templates::File &file);
			void packFileRemoved(const QString &path);
			void itemRenamed(QStandardItem *item);
			
		private:
			void setReadOnly(QStandardItem *item);
			
			const Manager *m_manager;
			bool m_readOnly;
		};
	}
}

#endif
