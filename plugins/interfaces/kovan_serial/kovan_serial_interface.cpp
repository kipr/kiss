#include "kovan_serial_interface.hpp"
#include "kovan_proto_target.hpp"

#include <kovanserial/usb_serial.hpp>

#include <QProcess>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include <QTcpSocket>
#include <QHostAddress>
#include <QRunnable>
#include <QThreadPool>
#include <QBuffer>

using namespace Kiss::Target;

PortSampler::PortSampler()
{
}

PortSampler::~PortSampler()
{
}

void PortSampler::run()
{
#ifdef Q_OS_MAC
	QDir dir("/dev/");
	const QStringList list = dir.entryList(QStringList() << "tty.usbmodem*", QDir::System);
	
#elif defined(Q_OS_WIN)
	qDebug() << "Device discovery not yet implemented for windows!";
#endif
}

KovanSerialInterface::KovanSerialInterface()
	: Interface("Kovan (Serial)"),
	m_responder(0)
{
}

KovanSerialInterface::~KovanSerialInterface()
{
}

Kiss::Target::TargetPtr KovanSerialInterface::createTarget(const QString &address)
{
	// TODO: Add input verification
	UsbSerial *serial = new UsbSerial(address.toAscii());
	KovanProtoTarget *device = new Kiss::Target::KovanProtoTarget(serial, this);
	device->fillDisplayName(address);
	return TargetPtr(device);
}

const bool KovanSerialInterface::scan(InterfaceResponder *responder)
{
	m_responder = responder;
	return true;
}

void KovanSerialInterface::invalidateResponder()
{
	m_responder = 0;
}

void KovanSerialInterface::scanStarted()
{
	if(!m_responder) return;
	m_responder->targetScanStarted(this);
}

void KovanSerialInterface::found(const QString &port)
{
	if(!m_responder) return;
	
	UsbSerial *serial = new UsbSerial(port.toAscii());
	KovanProtoTarget *device = new Kiss::Target::KovanProtoTarget(serial, this);
	m_responder->targetFound(this, TargetPtr(device));
}

Q_EXPORT_PLUGIN2(kovan_serial_interface, KovanSerialInterface);
