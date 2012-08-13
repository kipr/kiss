#include "SerialDevice.h"
#include "TargetResponder.h"

#include <kiss-compiler/QTinyArchiveStream.h>
#include <QBuffer>
#include <QDebug>

SerialDevice::SerialDevice(Interface *interface, const QString& path)
	: Target(interface, "TCP Socket Device"), m_path(path), m_device(m_path)
{
	QObject::connect(&m_device, SIGNAL(readyRead()), SLOT(readyRead()));
}

SerialDevice::~SerialDevice()
{
	disconnect();
}

void SerialDevice::setDeviceInfo(EasyDevice::DeviceInfo deviceInfo)
{
	m_deviceInfo = deviceInfo;
}

const QMap<QString, QString> SerialDevice::information() const
{
	QMap<QString, QString> ret;
	ret[DISPLAY_NAME] = m_deviceInfo.displayName();
	ret[DEVICE_TYPE] = m_deviceInfo.deviceType();
	ret[SERIAL] = m_deviceInfo.serialNumber();
	ret[VERSION] = m_deviceInfo.version();
	ret[COMM_PORT] = m_path;
	return ret;
}

const bool SerialDevice::available()
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(AVAILABLE_KEY);
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_device.write(block);
	return true;
}

const bool SerialDevice::compile(const QString& name)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(COMPILE_KEY);
	dataStream << name;
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_device.write(block);
	return true;
}

const bool SerialDevice::download(const QString& name, TinyArchive* archive)
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
	m_device.write(block);
	qDebug() << "Sent download";
	return true;
}

const bool SerialDevice::run(const QString& name)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(RUN_KEY);
	dataStream << name;
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_device.write(block);
	return true;
}

const bool SerialDevice::list()
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(LIST_KEY);
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_device.write(block);
	return true;
}

const bool SerialDevice::deleteProgram(const QString& name)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(DELETE_KEY);
	dataStream << name;
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_device.write(block);
	return true;
}

const bool SerialDevice::interaction(const QString& command)
{
	if(!connect()) return false;
	QByteArray block;
	QDataStream dataStream(&block, QIODevice::WriteOnly);
	dataStream << (quint32)0;
	dataStream << QString(INTERACTION_KEY);
	dataStream << command;
	dataStream.device()->seek(0);
	dataStream << (quint32)(block.size() - sizeof(quint32));
	m_device.write(block);
	return true;
}

const bool SerialDevice::authenticate(const QByteArray& hash)
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
	m_device.write(block);
	return true;
}

const bool SerialDevice::sendCustom(const QString& custom, const QByteArray& payload)
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
	m_device.write(block);
	return true;
}

void SerialDevice::readyRead()
{
	for(;;) {
		qDebug() << "Serial ReadyRead w/" << m_device.bytesAvailable();
		if(!m_payload && m_device.bytesAvailable() < sizeof(quint32)) return;
		if(!m_payload && m_device.bytesAvailable() >= sizeof(quint32)) {
			QDataStream stream(&m_device);
			stream >> m_payload;
			qDebug() << "Awaiting payload of size" << m_payload;
		}
		if(!m_payload) return;
	
		if(m_device.bytesAvailable() >= m_payload) {
			QByteArray payload = m_device.read(m_payload);
			processPayload(payload);
			m_payload = 0;
		} else break;
	}
}

void SerialDevice::processPayload(const QByteArray& payload)
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
	} else if(command == LIST_KEY) {
		QStringList programs;
		stream >> programs;
		success = true;
		responder()->listResponse(this, programs);
	} else if(command == DELETE_KEY) {
		stream >> success;
		responder()->deleteResponse(this, success);
	} else if(command == INTERACTION_KEY) {
		QString ret;
		stream >> ret;
		responder()->interactionResponse(this, ret);
	} else if(command == AUTHENTICATE_KEY) {
		stream >> success;
		bool tryAgain = false;
		if(!success) stream >> tryAgain;
		TargetResponder::AuthenticateReturn ret = success ? TargetResponder::AuthSuccess : TargetResponder::AuthWillNotAccept;
		if(!success && tryAgain)  ret = TargetResponder::AuthTryAgain;
		qDebug() << "Got success" << success << "and try again" << tryAgain << "==" << ret;
		responder()->authenticationResponse(this, ret);
	} else {
		responder()->customResponse(this, command, payload);
		success = true; // TODO: Perhaps not the best assumption
	}
	
	notifyQueue(success);
}

bool SerialDevice::connect()
{
	if(m_device.isOpen()) return true;
	m_device.open(QIODevice::ReadWrite);
	qDebug() << "Connecting serial port";
	return m_device.isOpen();
}

const bool SerialDevice::disconnect()
{
	m_device.close();
	return true;
}
