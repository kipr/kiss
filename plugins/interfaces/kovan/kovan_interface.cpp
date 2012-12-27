#include "kovan_interface.hpp"
#include "tcp_socket_device.hpp"

#include <QProcess>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include <QTcpSocket>
#include <QHostAddress>
#include <QBuffer>

using namespace EasyDevice;
using namespace Kiss::Target;

KovanInterface::KovanInterface()
	: Interface("Kovan (Networked)"),
	m_server(new DiscoveryServer()),
	m_responder(0)
{
	connect(m_server, SIGNAL(discoveryStarted()), SLOT(scanStarted()));
	connect(m_server,
		SIGNAL(discoveredDevice(EasyDevice::DeviceInfo, const QHostAddress&)),
		SLOT(found(EasyDevice::DeviceInfo, const QHostAddress&)));
}

KovanInterface::~KovanInterface()
{
	delete m_server;
}

const bool KovanInterface::scan(InterfaceResponder *responder)
{
	m_responder = responder;
	m_server->setup();
	m_server->discover("kovan");
	return true;
}

void KovanInterface::invalidateResponder()
{
	m_responder = 0;
}

void KovanInterface::scanStarted()
{
	if(!m_responder) return;
	m_responder->targetScanStarted(this);
}

void KovanInterface::found(DeviceInfo deviceInfo, const QHostAddress& address)
{
	if(!m_responder) return;
	TcpSocketDevice *device = new Kiss::Target::TcpSocketDevice(this, address, 8075);
	device->setDeviceInfo(deviceInfo);
	m_responder->targetFound(this, TargetPtr(device));
}

Q_EXPORT_PLUGIN2(kovan_interface, KovanInterface);
