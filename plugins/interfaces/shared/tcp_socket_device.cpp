#include "tcp_socket_device.hpp"
#include "target_responder.hpp"

#include <QBuffer>

using namespace Kiss;
using namespace Kiss::Target;

TcpSocketDevice::TcpSocketDevice(Interface *interface, const QHostAddress& address, const quint32& port)
	: Target(interface, "TCP Socket Device"),
	m_address(address),
	m_port(port),
	m_payload(0)
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

const bool TcpSocketDevice::download(const QString& name)
{
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

const bool TcpSocketDevice::list()
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(LIST_KEY);
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_socket.write(block);
	return true;
}

const bool TcpSocketDevice::deleteProgram(const QString& name)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(DELETE_KEY);
	dataStream << name;
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_socket.write(block);
	return true;
}

const bool TcpSocketDevice::interaction(const QString& command)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(INTERACTION_KEY);
	dataStream << command;
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
	if(socketError == QAbstractSocket::ConnectionRefusedError) {
		foreach(Responder *responder, responders()) responder->connectionError(this);
	} else foreach(Responder *responder, responders()) responder->communicationError(this);
}

void TcpSocketDevice::processPayload(const QByteArray& payload)
{
	QDataStream stream(payload);
	QString command;
	stream >> command;
	qDebug() << "Processing payload" << command;
	
	if(command.isEmpty()) foreach(Responder *responder, responders()) responder->unknownResponse(this, payload);
	
	bool success = false;
	if(command == LOCKED_KEY) {
		foreach(Responder *responder, responders()) responder->notAuthenticatedError(this);
	} else if(command == AVAILABLE_KEY) {
		stream >> success;
		foreach(Responder *responder, responders()) responder->availableResponse(this, success);
	} else if(command == COMPILE_KEY) {
		Compiler::OutputList results;
		stream >> results;
		success = false; // TODO: Fix
		foreach(Responder *responder, responders()) responder->compileResponse(this, results);
	} else if(command == DOWNLOAD_KEY) {
		stream >> success;
		foreach(Responder *responder, responders()) responder->downloadResponse(this, success);
	} else if(command == RUN_KEY) {
		stream >> success;
		foreach(Responder *responder, responders()) responder->runResponse(this, success);
	} else if(command == LIST_KEY) {
		QStringList programs;
		stream >> programs;
		success = true;
		foreach(Responder *responder, responders()) responder->listResponse(this, programs);
	} else if(command == DELETE_KEY) {
		stream >> success;
		foreach(Responder *responder, responders()) responder->deleteResponse(this, success);
	} else if(command == INTERACTION_KEY) {
		QString ret;
		stream >> ret;
		foreach(Responder *responder, responders()) responder->interactionResponse(this, ret);
	} else if(command == AUTHENTICATE_KEY) {
		stream >> success;
		bool tryAgain = false;
		if(!success) stream >> tryAgain;
		Responder::AuthenticateReturn ret = success ? Responder::AuthSuccess : Responder::AuthWillNotAccept;
		if(!success && tryAgain)  ret = Responder::AuthTryAgain;
		qDebug() << "Got success" << success << "and try again" << tryAgain << "==" << ret;
		foreach(Responder *responder, responders()) responder->authenticationResponse(this, ret);
	} else {
		foreach(Responder *responder, responders()) responder->customResponse(this, command, payload);
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
