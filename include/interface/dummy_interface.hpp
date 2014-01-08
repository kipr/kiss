#ifndef _DUMMY_INTERFACE_HPP_
#define _DUMMY_INTERFACE_HPP_

#include "interface.hpp"

#include <QObject>

namespace kiss
{
	namespace target
	{
		class DummyInterface : public Interface
		{
		Q_OBJECT
		Q_INTERFACES(kiss::target::Interface)
		public:
			DummyInterface();
			
			virtual kiss::target::TargetPtr createTarget(const QString &address);
			virtual const bool scan(kiss::target::InterfaceResponder *responder);
			virtual void invalidateResponder();
		};
	}
}

#endif
