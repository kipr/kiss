#ifndef _DUMMY_INTERFACE_HPP_
#define _DUMMY_INTERFACE_HPP_

#include "interface.hpp"

#include <QObject>

namespace Kiss
{
	namespace Target
	{
		class DummyInterface : public QObject, public Interface
		{
		Q_OBJECT
		Q_INTERFACES(Kiss::Target::Interface)
		public:
			DummyInterface();
			
			virtual Kiss::Target::TargetPtr createTarget(const QString &address);
			virtual const bool scan(Kiss::Target::InterfaceResponder *responder);
			virtual void invalidateResponder();
		};
	}
}

#endif
