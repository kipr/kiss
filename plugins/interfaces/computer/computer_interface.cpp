#include "computer_interface.hpp"

#include "tcp_socket_device.hpp"

#include <QProcess>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include <QTcpSocket>
#include <QHostAddress>
#include <QBuffer>

using namespace EasyDevice;
using namespace Kiss;

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

const bool ComputerInterface::scan(Target::InterfaceResponder *responder)
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
	m_responder->targetScanStarted(this);
}

void ComputerInterface::found(DeviceInfo deviceInfo, const QHostAddress& address)
{
	if(!m_responder) return;
}

Q_EXPORT_PLUGIN2(computer_interface, ComputerInterface);
