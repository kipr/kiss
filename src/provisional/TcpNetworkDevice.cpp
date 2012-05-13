#include "TcpNetworkDevice.h"
#include "MessageDefines.h"

#define CONNECTION_TIMEOUT 5000

TcpNetworkDevice::TcpNetworkDevice(const QHostAddress& address, const quint16& port)
	: m_address(address), m_port(port)
{
	
}

const bool TcpNetworkDevice::available()
{
	if(!connect()) return false;
	char msg[1] = { MESSAGE_AVAILABLE };
	m_socket->write(msg, 1);
	if(!waitWrite()) return false;
	if(!waitRead()) return false;
	bool ok;
	int ret = m_socket->readAll().toInt(&ok);
	disconnect();
	return ok && ret == 1;
}

const bool TcpNetworkDevice::download(const QString& name, TinyArchive* archive)
{
	if(!connect()) return false;
	char msg[1] = { MESSAGE_DOWNLOAD };
	m_socket->write(msg, 1);
	m_socket->write(name.toByteArray());
	QTinyArchiveStream stream(m_socket);
	if(!stream.write(archive)) return false;
	if(!waitWrite()) return false;
	if(!waitRead()) return false;
	bool ok;
	int ret = m_socket->readAll().toInt(&ok);
	disconnect();
	return ok && ret == 1;
}

const bool TcpNetworkDevice::run(const QString& name)
{
	char msg[1] = { MESSAGE_RUN };
	m_socket->write(msg, 1);
	m_socket->write(name.toByteArray());
	if(!waitWrite()) return false;
	if(!waitRead()) return false;
	bool ok;
	int ret = m_socket->readAll().toInt(&ok);
	disconnect();
	return ok && ret == 1;
}

const bool TcpNetworkDevice::connect()
{
	m_socket->connectToHost(m_address, m_port);
	return m_socket->waitForConnected(CONNECTION_TIMEOUT);
}

void TcpNetworkDevice::disconnect()
{
	m_socket->disconnectFromHost();
}

const bool TcpNetworkDevice::waitWrite()
{
	if(!m_socket->waitForBytesWritten(CONNECTION_TIMEOUT)) {
		disconnect();
		return false;
	}
	return true;
}

const bool TcpNetworkDevice::waitRead()
{
	if(!m_socket->waitForReadyRead(CONNECTION_TIMEOUT)) {
		disconnect();
		return false;
	}
	return true;
}