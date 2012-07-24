#include "TcpSocketDevice.h"
#include "DeviceResponder.h"

#include <kiss-compiler/QTinyArchiveStream.h>
#include <QBuffer>

TcpSocketDevice::TcpSocketDevice(Interface *interface, const QHostAddress& address, const quint32& port)
	: Device(interface, "TCP Socket Device"), m_address(address), m_port(port), m_payload(0)
{
	QObject::connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(error(QAbstractSocket::SocketError)));
	QObject::connect(&m_socket, SIGNAL(readyRead()), SLOT(readyRead()));
}

TcpSocketDevice::~TcpSocketDevice()
{
	disconnect();
}

void TcpSocketDevice::setDeviceInfo(EasyDevice::DeviceInfo deviceInfo)
{
	m_deviceInfo = deviceInfo;
}

const QMap<QString, QString> TcpSocketDevice::information() const
{
	QMap<QString, QString> ret;
	ret[DISPLAY_NAME] = m_deviceInfo.displayName();
	ret[DEVICE_TYPE] = m_deviceInfo.deviceType();
	ret[SERIAL] = m_deviceInfo.serialNumber();
	ret[VERSION] = m_deviceInfo.version();
	ret[COMM_PORT] = m_address.toString();
	return ret;
}

const bool TcpSocketDevice::available()
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(AVAILABLE_KEY);
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_socket.write(block);
	return true;
}

const bool TcpSocketDevice::compile(const QString& name)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(COMPILE_KEY);
	dataStream << name;
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_socket.write(block);
	return true;
}

const bool TcpSocketDevice::download(const QString& name, TinyArchive* archive)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(DOWNLOAD_KEY);
	dataStream << name;
	QTinyArchiveStream stream(dataStream.device());
	archive->write(&stream);
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	qDebug() << "Download payload size" << block.size();
	m_socket.write(block);
	qDebug() << "Sent download";
	return true;
}

const bool TcpSocketDevice::run(const QString& name)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(RUN_KEY);
	dataStream << name;
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_socket.write(block);
	return true;
}

const bool TcpSocketDevice::authenticate(const QByteArray& hash)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(AUTHENTICATE_KEY);
	qDebug() << "Sending hash of length" << hash.size();
	dataStream << hash;
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_socket.write(block);
	return true;
}

const bool TcpSocketDevice::sendCustom(const QString& custom, const QByteArray& payload)
{
	if(custom.isEmpty()) {
		qWarning() << "Empty custom commands are not allowed.";
		return false;
	}
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << custom;
	dataStream.device()->write(payload);
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_socket.write(block);
	return true;
}

void TcpSocketDevice::readyRead()
{
	for(;;) {
		qDebug() << "ReadyRead w/" << m_socket.bytesAvailable();
		if(!m_payload && m_socket.bytesAvailable() < sizeof(quint32)) return;
		if(!m_payload && m_socket.bytesAvailable() >= sizeof(quint32)) {
			QDataStream stream(&m_socket);
			stream >> m_payload;
			qDebug() << "Awaiting payload of size" << m_payload;
		}
		if(!m_payload) return;
	
		if(m_socket.bytesAvailable() >= m_payload) {
			QByteArray payload = m_socket.read(m_payload);
			processPayload(payload);
			m_payload = 0;
		} else break;
	}
}

void TcpSocketDevice::error(QAbstractSocket::SocketError socketError)
{
	qCritical() << "Socket Error!" << socketError;
	if(socketError == QAbstractSocket::ConnectionRefusedError) responder()->connectionError(this);
	else responder()->communicationError(this);
}

void TcpSocketDevice::processPayload(const QByteArray& payload)
{
	QDataStream stream(payload);
	QString command;
	stream >> command;
	qDebug() << "Processing payload" << command;
	
	if(command.isEmpty()) responder()->unknownResponse(this, payload);
	
	bool success = false;
	if(command == LOCKED_KEY) {
		responder()->notAuthenticatedError(this);
	} else if(command == AVAILABLE_KEY) {
		stream >> success;
		responder()->availableResponse(this, success);
	} else if(command == COMPILE_KEY) {
		CompileResult results;
		stream >> results;
		success = results.success();
		responder()->compileResponse(this, results);
	} else if(command == DOWNLOAD_KEY) {
		stream >> success;
		responder()->downloadResponse(this, success);
	} else if(command == RUN_KEY) {
		stream >> success;
		responder()->runResponse(this, success);
	} else if(command == AUTHENTICATE_KEY) {
		stream >> success;
		responder()->authenticationResponse(this, success);
	} else {
		responder()->customResponse(this, command, payload);
		success = true; // TODO: Perhaps not the best assumption
	}
	
	notifyQueue(success);
}

bool TcpSocketDevice::connect()
{
	if(m_socket.state() == QAbstractSocket::ConnectedState) return true;
	m_socket.connectToHost(m_address, m_port);
	m_socket.waitForConnected(5000);
	return m_socket.state() == QAbstractSocket::ConnectedState;
}

const bool TcpSocketDevice::disconnect()
{
	m_socket.disconnectFromHost();
	return true;
}
