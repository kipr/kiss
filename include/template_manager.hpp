#ifndef _TEMPLATE_MANAGER_HPP_
#define _TEMPLATE_MANAGER_HPP_

#include <QObject>
#include <QList>
#include <QString>

namespace Kiss
{
	namespace Template
	{
		class Pack;
		
		class Manager : public QObject
		{
		Q_OBJECT
		public:
			~Manager();
			
			void addPacks(const QString& path);
			bool addPack(const QString& path);
			void addPack(Pack *pack);
			void removePack(Pack *pack);
			
			void addDefaultPack(Pack *pack);
			void loadDefaultPacks();
			
			QList<Pack *> packs() const;
			
		signals:
			void packAdded(Kiss::Template::Pack *pack);
			void packRemoved(Kiss::Template::Pack *pack);
			
		private:
			QList<Pack *> m_packs;
		};
	}
}

#endif
