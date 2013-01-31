#include "kovan_serial_interface.hpp"
#include "kovan_proto_target.hpp"

#include <kovanserial/usb_serial.hpp>
#include <kovanserial/kovan_serial.hpp>
#include <kovanserial/transport_layer.hpp>

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
	QStringList paths;
#ifdef Q_OS_MAC
	QDir dir("/dev/");
	QFileInfoList list = dir.entryInfoList(QStringList() << "tty.usbmodem*", QDir::System);
	foreach(const QFileInfo &info, list) paths << info.filePath();
#elif defined(Q_OS_WIN)
	for(int i = 0; i < 15; ++i) paths << QString("COM%1").arg(i);
#else
	QDir dir("/dev/");
	QFileInfoList list = dir.entryInfoList(QStringList() << "ttyACM*", QDir::System);
	foreach(const QFileInfo &info, list) paths << info.filePath();
#endif
	foreach(const QString &path, paths) {
		UsbSerial usb(path.toAscii());
		if(!usb.makeAvailable()) {
			qWarning() << "Failed to make port" << path << "available";
			continue;
		}
		TransportLayer transport(&usb);
		KovanSerial proto(&transport);
		if(proto.knockKnock(150)) emit found(path);
		proto.hangup();
		usb.endSession();
	}
}

KovanSerialInterface::KovanSerialInterface()
	: Interface("USB Serial"),
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
	PortSampler *sampler = new PortSampler();
	sampler->setAutoDelete(true);
	connect(sampler, SIGNAL(found(QString)), SLOT(found(QString)));
	QThreadPool::globalInstance()->start(sampler);
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
	device->fillCommPort(port);
	m_responder->targetFound(this, TargetPtr(device));
}

Q_EXPORT_PLUGIN2(kovan_serial_interface, KovanSerialInterface);
