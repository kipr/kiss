#ifndef _CBC3_H_
#define _CBC3_H_

#include "Device.h"
#include <easydevice/DeviceInfo.h>
#include <QtGlobal>
#include <QTcpSocket>
#include <QHostAddress>

class Interface;

#define AVAILABLE_KEY "available?"
#define COMPILE_KEY "compile"
#define DOWNLOAD_KEY "download"
#define RUN_KEY "run"
#define LOCKED_KEY "locked"
#define AUTHENTICATE_KEY "authenticate"

class TcpSocketDevice : public QObject, public Device
{
Q_OBJECT
public:
	TcpSocketDevice(Interface *interface, const QHostAddress& address, const quint32& port);
	~TcpSocketDevice();
	
	void setDeviceInfo(EasyDevice::DeviceInfo deviceInfo);
	
	virtual const QMap<QString, QString> information() const;
	virtual const bool available();
	virtual const bool compile(const QString& name);
	virtual const bool download(const QString& name, TinyArchive* archive);
	virtual const bool run(const QString& name);
	virtual const bool authenticate(const QByteArray& hash);
	virtual const bool sendCustom(const QString& custom, const QByteArray& payload);
	
private slots:
	void readyRead();
	void error(QAbstractSocket::SocketError socketError);
	
private:
	void processPayload(const QByteArray& payload);
	bool connect();
	bool disconnect();
	
	EasyDevice::DeviceInfo m_deviceInfo;
	QHostAddress m_address;
	quint16 m_port;
	QTcpSocket m_socket;
	
	quint32 m_payload;
};

#endif
