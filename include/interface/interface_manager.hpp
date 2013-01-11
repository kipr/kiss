#ifndef _INTERFACE_MANAGER_HPP_
#define _INTERFACE_MANAGER_HPP_

#include "singleton.hpp"

#include <QObject>
#include <QList>

namespace Kiss
{
	namespace Target
	{
		class Interface;

		class InterfaceManager : public QObject, public Singleton<InterfaceManager>
		{
		Q_OBJECT
		public:
			InterfaceManager();
			~InterfaceManager();

			void addInterface(Interface *interface);
			void removeInterface(Interface *interface);

			const QList<Interface *>& interfaces() const;
			Interface *interface(const QString& name);

		signals:
			void interfaceAdded(Interface *interface);
			void interfaceRemoved(Interface *interface);

		private:
			QList<Interface *> m_interfaces;
		};
	}
}

#define REGISTER_INTERFACE(x) __attribute__((constructor)) \
static void __##x##_register() \
{ \
	InterfaceManager::ref().addInterface(new x()); \
}

#endif
