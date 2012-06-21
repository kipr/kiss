#include "Cbc3Interface.h"

#include "Ohaiyo.h"
#include "QTinyArchiveStream.h"

#include <QProcess>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "cni.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QBuffer>

class Cbc3 : public Device
{
public:
	Cbc3(Interface *interface, struct sockaddr_in addr) : Device(interface, "CBC 3"), m_addr(addr) {}
	~Cbc3() { disconnect(); }
	
	virtual const QMap<QString, QString> information() const {
		QMap<QString, QString> ret;
		ret[DISPLAY_NAME] = QString("CBC 3");
		ret[DEVICE_TYPE] = "CBC 3";
		ret[DOWNLOAD_TYPE] = DOWNLOAD_SOURCE;
		ret[SERIAL] = inet_ntoa(m_addr.sin_addr);
		return ret;
	}
	
	virtual const bool available()
	{
		if(!connect()) return false;
		writeCommand(CNI_AVAILABLE_MESSAGE, "\0", 1);
		QByteArray ret = m_socket.readAll();
		disconnect();
		if(ret.size() != 1) return false;
		return ret[0];
	}
	
	virtual const bool download(const QString& name, TinyArchive* archive)
	{
		if(!connect()) return false;
		QBuffer buffer;
		buffer.write(name.toLatin1());
		QTinyArchiveStream stream(&buffer);
		archive->write(&stream);
		writeCommand(CNI_DOWNLOAD_MESSAGE, buffer.data(), buffer.size());
		disconnect();
		
		return true;
	}
	
	virtual const bool run(const QString& name)
	{
		if(!connect()) return false;
		QBuffer buffer;
		buffer.write(name.toLatin1());
		writeCommand(CNI_RUN_MESSAGE, buffer.data(), buffer.size());
		disconnect();
		return true;
	}
	
private:
	bool connect()
	{
		m_socket.connectToHost(QHostAddress((struct sockaddr *)&m_addr), CNI_PORT);
		m_socket.waitForConnected(5000);
		return m_socket.state() == QAbstractSocket::ConnectedState;
	}
	
	bool disconnect()
	{
		m_socket.disconnectFromHost();
		return true;
	}
	
	void writeCommand(size_t type, const char *data, size_t length)
	{
		m_socket.write((char *)&type, sizeof(size_t));
		m_socket.write((char *)&length, sizeof(size_t));
		m_socket.write(data, length * sizeof(char));
	}
	
	struct sockaddr_in m_addr;
	QTcpSocket m_socket;
};

class Listener : public OhaiyoListener
{
public:
	Listener(Interface *interface) : m_interface(interface) {}
	~Listener() {}
	
	virtual void discoveryStarted(Ohaiyo *ohaiyo)
	{
		m_discovered.clear();
	}
	
	virtual void deviceDiscovered(Ohaiyo *ohaiyo, const Address& address)
	{
		m_discovered << DevicePtr(new Cbc3(m_interface, address.address()));
	}
	
	virtual void discoveryEnded(Ohaiyo *ohaiyo) {}
	
	QList<DevicePtr> discovered()
	{
		return m_discovered;
	}
private:
	Interface *m_interface;
	QList<DevicePtr> m_discovered;
};

Cbc3Interface::Cbc3Interface()
	: Interface("CBC 3 (Networked)"),
	m_ohaiyo(new Ohaiyo("cbc3")),
	m_listener(new Listener(this))
{
	m_ohaiyo->addListener(m_listener);
}

Cbc3Interface::~Cbc3Interface()
{
	delete m_ohaiyo;
	delete m_listener;
}

QList<DevicePtr> Cbc3Interface::devices()
{
	m_ohaiyo->discover(1);
	return ((Listener *)m_listener)->discovered();
}

const bool Cbc3Interface::local() { return false; }

Q_EXPORT_PLUGIN2(cbc3_interface, Cbc3Interface);