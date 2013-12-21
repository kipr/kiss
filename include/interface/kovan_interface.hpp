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
		class AdvertSampler : public QObject, public QRunnable
		{
		Q_OBJECT
		public:
			AdvertSampler(UdpAdvertiser *advertiser, const quint64 &sampleTime, const quint16 &samples);
			~AdvertSampler();
			void run();

		signals:
			void found(const Advert &ad, const sockaddr_in &addr);

		private:
			UdpAdvertiser *m_advertiser;
			quint64 m_sampleTime;
			quint16 m_samples;
			
			QList<IncomingAdvert> m_found;
		};

		class KovanInterface : public QObject, public kiss::target::Interface
		{
		Q_OBJECT
		Q_INTERFACES(kiss::target::Interface)
		public:
			KovanInterface();
			~KovanInterface();
			
			virtual kiss::target::TargetPtr createTarget(const QString &address);
			virtual const bool scan(kiss::target::InterfaceResponder *responder);
			virtual void invalidateResponder();

		private slots:
			void scanStarted();
			void found(const Advert &ad, const sockaddr_in addr);

		private:
			UdpAdvertiser *m_advertiser;
			kiss::target::InterfaceResponder *m_responder;
		};
	}
}


#endif
