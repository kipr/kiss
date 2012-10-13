#ifndef _TEMPLATE_MODEL_HPP_
#define _TEMPLATE_MODEL_HPP_

#include "template_file.hpp"
#include "template_pack.hpp"

#include <QStandardItemModel>

namespace Kiss
{
	namespace Template
	{
		class Pack;
		class Manager;
		
		class Model : public QStandardItemModel
		{
		Q_OBJECT
		Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
		public:
			Model(const Manager *manager, QObject *parent = 0);
			
			Pack *indexToPack(const QModelIndex& index) const;
			Template::File indexToFile(const QModelIndex& index) const;
			bool isIndexPack(const QModelIndex& index) const;
			bool isIndexFile(const QModelIndex& index) const;
			
			void setReadOnly(const bool& readOnly);
			const bool& isReadOnly() const;
			
		private slots:
			void packAdded(const Kiss::Template::PackPtr& pack);
			void packRemoved(Kiss::Template::Pack *pack);
			void packNameChanged(const QString& name);
			void packFileAdded(const QString& path, const Kiss::Template::File& file);
			void packFileRemoved(const QString& path);
			void itemRenamed(QStandardItem *item);
			
		private:
			void setReadOnly(QStandardItem *item);
			
			const Manager *m_manager;
			bool m_readOnly;
		};
	}
}

#endif
