#include "Cbc3Interface.h"

#include "TcpSocketDevice.h"
#include <kiss-compiler/QTinyArchiveStream.h>

#include <QProcess>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include <QTcpSocket>
#include <QHostAddress>
#include <QBuffer>

using namespace EasyDevice;

Cbc3Interface::Cbc3Interface()
	: Interface("CBC 3 (Networked)"),
	m_server(new DiscoveryServer()),
	m_responder(0)
{
	connect(m_server, SIGNAL(discoveryStarted()), SLOT(scanStarted()));
	connect(m_server,
		SIGNAL(discoveredDevice(EasyDevice::DeviceInfo, const QHostAddress&)),
		SLOT(found(EasyDevice::DeviceInfo, const QHostAddress&)));
}

Cbc3Interface::~Cbc3Interface()
{
	delete m_server;
}

const bool Cbc3Interface::scan(InterfaceResponder *responder)
{
	m_responder = responder;
	m_server->setup();
	m_server->discover("kovan");
	return true;
}

void Cbc3Interface::invalidateResponder()
{
	m_responder = 0;
}

void Cbc3Interface::scanStarted()
{
	if(!m_responder) return;
	m_responder->deviceScanStarted(this);
}

void Cbc3Interface::found(DeviceInfo deviceInfo, const QHostAddress& address)
{
	if(!m_responder) return;
	TcpSocketDevice *device = new TcpSocketDevice(this, address, 8075);
	device->setDeviceInfo(deviceInfo);
	m_responder->deviceFound(this, DevicePtr(device));
}

Q_EXPORT_PLUGIN2(cbc3_interface, Cbc3Interface);