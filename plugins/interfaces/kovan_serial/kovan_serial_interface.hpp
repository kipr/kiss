#ifndef _KOVAN_INTERFACE_HPP_
#define _KOVAN_INTERFACE_HPP_

#include <QtPlugin>
#include <QRunnable>
#include <QList>
#include <QHostAddress>
#include "interface.hpp"
#include <kovanserial/udp_advertiser.hpp>

class UdpAdvertiser;

namespace kiss
{
	namespace target
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

		class KovanSerialInterface : public QObject, public kiss::target::Interface
		{
		Q_OBJECT
		Q_INTERFACES(kiss::target::Interface)
		public:
			KovanSerialInterface();
			~KovanSerialInterface();
			
			virtual kiss::target::TargetPtr createTarget(const QString &address);
			virtual const bool scan(kiss::target::InterfaceResponder *responder);
			virtual void invalidateResponder();

		private slots:
			void scanStarted();
			void found(const QString &port);

		private:
			kiss::target::InterfaceResponder *m_responder;
		};
	}
}


#endif
