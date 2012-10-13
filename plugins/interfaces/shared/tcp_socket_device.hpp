#ifndef _TCP_SOCKET_DEVICE_HPP_
#define _TCP_SOCKET_DEVICE_HPP_

#include "target.hpp"
#include "keys.hpp"
#include <easydevice/DeviceInfo.h>
#include <QtGlobal>
#include <QTcpSocket>
#include <QHostAddress>

namespace Kiss
{
	namespace Target
	{
		class TcpSocketDevice : public QObject, public Target
		{
		Q_OBJECT
		public:
			TcpSocketDevice(Interface *interface, const QHostAddress& address, const quint32& port);
			~TcpSocketDevice();

			void setDeviceInfo(EasyDevice::DeviceInfo deviceInfo);

			virtual const bool disconnect();

			virtual const QMap<QString, QString> information() const;
			virtual const bool available();
			virtual const bool compile(const QString& name);
			virtual const bool download(const QString& name, const KarPtr& archive);
			virtual const bool run(const QString& name);
			virtual const bool list();
			virtual const bool deleteProgram(const QString& name);
			virtual const bool interaction(const QString& command);
			virtual const bool authenticate(const QByteArray& hash);
			virtual const bool sendCustom(const QString& custom, const QByteArray& payload);

		private slots:
			void readyRead();
			void error(QAbstractSocket::SocketError socketError);

		private:
			void processPayload(const QByteArray& payload);
			bool connect();


			EasyDevice::DeviceInfo m_deviceInfo;
			QHostAddress m_address;
			quint16 m_port;
			QTcpSocket m_socket;

			quint32 m_payload;
		};
	}
}


#endif
