#ifndef _TEMPLATE_MANAGER_HPP_
#define _TEMPLATE_MANAGER_HPP_

#include "template_pack.hpp"

#include <QObject>
#include <QList>
#include <QString>
#include <QDir>

namespace kiss
{
	namespace templates
	{
		class Manager : public QObject
		{
		Q_OBJECT
		public:
			~Manager();
			
			void addPacks(const QString &path);
			bool addPack(const QString &path);
			void addPack(const PackPtr &pack);
			bool removePack(const PackPtr &pack, bool removeAsDefault = false);
			bool removePack(Pack *pack, bool removeAsDefault = false);
			
			void addDefaultPack(const PackPtr &pack);
			void loadDefaultPacks();
			
			QList<PackPtr> packs() const;
			
		signals:
			void packAdded(const kiss::templates::PackPtr &pack);
			void packRemoved(kiss::templates::Pack *pack);
			
		private:
			QDir templatePacksDir() const;
			
			QList<PackPtr> m_packs;
		};
	}
}

#endif
