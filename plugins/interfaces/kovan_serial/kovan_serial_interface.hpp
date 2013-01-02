#ifndef _KOVAN_INTERFACE_HPP_
#define _KOVAN_INTERFACE_HPP_

#include <QtPlugin>
#include <QRunnable>
#include <QList>
#include <QHostAddress>
#include "interface.hpp"
#include <kovanserial/udp_advertiser.hpp>

class UdpAdvertiser;

namespace Kiss
{
	namespace Target
	{
		class PortSampler : public QObject, public QRunnable
		{
		Q_OBJECT
		public:
			PortSampler();
			~PortSampler();
			void run();

		signals:
			void found(const QString &port);
		};

		class KovanSerialInterface : public QObject, public Kiss::Target::Interface
		{
		Q_OBJECT
		Q_INTERFACES(Kiss::Target::Interface)
		public:
			KovanSerialInterface();
			~KovanSerialInterface();
			
			virtual Kiss::Target::TargetPtr createTarget(const QString &address);
			virtual const bool scan(Kiss::Target::InterfaceResponder *responder);
			virtual void invalidateResponder();

		private slots:
			void scanStarted();
			void found(const QString &port);

		private:
			Kiss::Target::InterfaceResponder *m_responder;
		};
	}
}


#endif
