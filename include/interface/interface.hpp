#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

#include "interface_responder.hpp"
#include "target.hpp"

#include <QList>
#include <QString>

namespace Kiss
{
	namespace Target
	{
		class Interface
		{
		public:
			Interface(const QString& name);

			const QString& name() const;
			
			virtual Kiss::Target::TargetPtr createTarget(const QString &address) = 0;
			virtual const bool scan(InterfaceResponder *responder) = 0;
			virtual void invalidateResponder() = 0;
		private:
			QString m_name;
		};
	}
}

Q_DECLARE_INTERFACE(Kiss::Target::Interface, "com.kipr.kiss.Interface/1.1");

#endif
