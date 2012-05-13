#ifndef _TCPNETWORKDEVICE_H_
#define _TCPNETWORKDEVICE_H_

#include "Device.h"

#include <QTcpSocket>

class TcpNetworkDevice : public Device
{
public:
	TcpNetworkDevice(const QHostAddress& address, const quint16& port);
	
	const QMap<QString, QString> information();
	virtual const bool available();
	virtual const bool download(const QString& name, TinyArchive* archive);
	virtual const bool run(const QString& name);
	
private:
	const bool connect();
	void disconnect();
	
	const bool waitWrite();
	const bool waitRead();
	
	QHostAddress m_address;
	quint16 m_port;
	QTcpSocket* m_socket;
};


#endif
