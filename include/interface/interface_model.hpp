#ifndef _INTERFACEMODEL_H_
#define _INTERFACEMODEL_H_

#include <QStandardItemModel>

namespace Kiss
{
	namespace Target
	{
		class Interface;
		class InterfaceManager;

		class InterfaceModel : public QStandardItemModel
		{
		Q_OBJECT
		public:
			InterfaceModel(InterfaceManager *manager, QObject *parent = 0);

			Interface *rowToInterface(const int& row) const;
			Interface *indexToInterface(const QModelIndex& index) const;

			void setAllInterface(const bool& on);

		private slots:
			void interfaceAdded(Interface *interface);
			void interfaceRemoved(Interface *interface);

		private:
			void reload();

			InterfaceManager *m_manager;
		};
	}
}

#endif
