#ifndef _KOVAN_PROTO_TARGET_HPP_
#define _KOVAN_PROTO_TARGET_HPP_

#include "target.hpp"

#include <kar.hpp>

#include <kovanserial/transmitter.hpp>
#include <kovanserial/transport_layer.hpp>
#include <kovanserial/kovan_serial.hpp>
#include <kovanserial/udp_advertiser.hpp>


#undef interface

namespace kiss
{
	namespace target 
	{
		class KovanProtoTarget : public kiss::target::Target
		{
		Q_OBJECT
		public:
			KovanProtoTarget(Transmitter *transmitter, Interface *interface);
			~KovanProtoTarget();
			
			void fillDisplayName(const QString &displayName);
			void fillCommPort(const QString &commPort);
			void fillInformation(const Advert &advert);
			
			virtual QMap<QString, QString> information() const; // Needs to be cached

			virtual bool disconnect();

			virtual bool available();
			
			virtual Target::Target::ReturnCode compile(quint64 id, const QString& name);
			virtual Target::Target::ReturnCode download(quint64 id, const QString& name, const kiss::KarPtr& archive);
			virtual Target::Target::ReturnCode run(quint64 id, const QString& name);

			virtual Target::Target::ReturnCode list(quint64 id);
			virtual Target::Target::ReturnCode deleteProgram(quint64 id, const QString& name);
			virtual Target::Target::ReturnCode interaction(quint64 id, const QString& command);

			virtual Target::Target::ReturnCode sendCustom(quint64 id, const QString& custom, const QByteArray& payload = QByteArray());

			virtual bool setPassword(const QString &password);
			virtual void clearPassword();

		private:
			Target::Target::ReturnCode verifyProtocolVersion();
			Target::Target::ReturnCode authenticate(const quint64 id);
			
			Transmitter *m_transmitter;
			TransportLayer m_transport;
			KovanSerial m_proto;
			QMap<QString, QString> m_information;
		};
	}
}

#endif
