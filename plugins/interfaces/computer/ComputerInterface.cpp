#include "ComputerInterface.h"

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

ComputerInterface::ComputerInterface()
	: Interface("Computer"),
	m_server(new DiscoveryServer()),
	m_responder(0)
{
	connect(m_server, SIGNAL(discoveryStarted()), SLOT(scanStarted()));
	connect(m_server,
		SIGNAL(discoveredDevice(EasyDevice::DeviceInfo, const QHostAddress&)),
		SLOT(found(EasyDevice::DeviceInfo, const QHostAddress&)));
}

ComputerInterface::~ComputerInterface()
{
	delete m_server;
}

const bool ComputerInterface::scan(InterfaceResponder *responder)
{
	m_responder = responder;
	m_server->setup();
	m_server->discover("computer");
	return true;
}

void ComputerInterface::invalidateResponder()
{
	m_responder = 0;
}

void ComputerInterface::scanStarted()
{
	if(!m_responder) return;
	m_responder->deviceScanStarted(this);
}

void ComputerInterface::found(DeviceInfo deviceInfo, const QHostAddress& address)
{
	if(!m_responder) return;
	TcpSocketDevice *device = new TcpSocketDevice(this, address, 8075);
	device->setDeviceInfo(deviceInfo);
	m_responder->deviceFound(this, DevicePtr(device));
}

Q_EXPORT_PLUGIN2(computer_interface, ComputerInterface);