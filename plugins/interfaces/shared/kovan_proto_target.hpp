#ifndef _KOVAN_PROTO_TARGET_HPP_
#define _KOVAN_PROTO_TARGET_HPP_

#include "target.hpp"

#include <kar.hpp>

#include <kovanserial/transmitter.hpp>
#include <kovanserial/transport_layer.hpp>
#include <kovanserial/kovan_serial.hpp>
#include <kovanserial/udp_advertiser.hpp>

#undef interface

namespace Kiss
{
	namespace Target
	{
		class KovanProtoTarget : public Kiss::Target::Target
		{
		Q_OBJECT
		public:
			KovanProtoTarget(Transmitter *transmitter, Interface *interface);
			~KovanProtoTarget();
			
			void fillDisplayName(const QString &displayName);
			void fillCommPort(const QString &commPort);
			void fillInformation(const Advert &advert);
			
			virtual const QMap<QString, QString> information() const; // Needs to be cached

			virtual const bool disconnect();

			virtual const bool available();
			
			virtual const bool compile(quint64 id, const QString& name);
			virtual const bool download(quint64 id, const QString& name, const Kiss::KarPtr& archive);
			virtual const bool run(quint64 id, const QString& name);

			virtual const bool list(quint64 id);
			virtual const bool deleteProgram(quint64 id, const QString& name);
			virtual const bool interaction(quint64 id, const QString& command);

			virtual const bool authenticate(quint64 id, const QByteArray& hash);
			virtual const bool sendCustom(quint64 id, const QString& custom, const QByteArray& payload = QByteArray());

		private:
			Transmitter *m_transmitter;
			TransportLayer m_transport;
			KovanSerial m_proto;
			QMap<QString, QString> m_information;
		};
	}
}

#endif
