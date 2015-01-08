#include "kovan_serial_interface.hpp"
#include "kovan_proto_target.hpp"

#include <kovanserial/usb_serial.hpp>
#include <kovanserial/kovan_serial.hpp>
#include <kovanserial/transport_layer.hpp>

#include <QProcess>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include <QRunnable>
#include <QThreadPool>
#include <QBuffer>
#include <QSemaphore>

using namespace kiss::target;

PortSampler::PortSampler(const QString &path, QSemaphore *const sema)
  : _path(path)
  , _sema(sema)
{
}

PortSampler::~PortSampler()
{
}

void PortSampler::run()
{
  qDebug() << "Checking port" << _path;
	UsbSerial usb(_path.toUtf8());
	if(!usb.makeAvailable()) {
		qWarning() << "Failed to make port" << _path << "available";
		return;
	}
	TransportLayer transport(&usb);
	KovanSerial proto(&transport);
	if(proto.knockKnock(150))
  {
    emit found(_path);
    qDebug() << "EMITTING FOUND";
  }
	proto.hangup();
	usb.endSession();
  
  _sema->release();
}

SamplerFinished::SamplerFinished(const int n, QSemaphore *const sema)
  : _n(n)
  , _sema(sema)
{
}

void SamplerFinished::run()
{
  _sema->acquire(_n);
  delete _sema;
  emit runFinished();
  qDebug() << "Semaphore acquired; sampler finished";
}

KovanSerialInterface::KovanSerialInterface()
	: Interface("USB Serial"),
	m_responder(0)
{
}

KovanSerialInterface::~KovanSerialInterface()
{
}

kiss::target::TargetPtr KovanSerialInterface::createTarget(const QString &address)
{
	// TODO: Add input verification
	UsbSerial *serial = new UsbSerial(address.toUtf8());
	KovanProtoTarget *device = new kiss::target::KovanProtoTarget(serial, this);
	device->fillDisplayName(address);
	return TargetPtr(device);
}

const bool KovanSerialInterface::scan(InterfaceResponder *responder)
{
	m_responder = responder;
	QStringList paths;
#ifdef Q_OS_MAC
	QDir dir("/dev/");
	QFileInfoList list = dir.entryInfoList(QStringList() << "tty.usbmodem*", QDir::System);
	foreach(const QFileInfo &info, list) paths << info.filePath();
#elif defined(Q_OS_WIN)
	for(int i = 0; i < 30; ++i) paths << QString("COM%1").arg(i);
#else
	QDir dir("/dev/");
	QFileInfoList list = dir.entryInfoList(QStringList() << "ttyACM*", QDir::System);
	foreach(const QFileInfo &info, list) paths << info.filePath();
#endif
  
  QThreadPool::globalInstance()->setMaxThreadCount(qMax(20, QThreadPool::globalInstance()->maxThreadCount()));
  QSemaphore *sema = new QSemaphore;
  foreach(const QString &path, paths) {
    PortSampler *sampler = new PortSampler(path, sema);
    sampler->setAutoDelete(true);
    connect(sampler, SIGNAL(found(QString)), SLOT(found(QString)));
	  QThreadPool::globalInstance()->start(sampler);
  }
  
  SamplerFinished *finished = new SamplerFinished(paths.size(), sema);
  connect(finished, SIGNAL(runFinished()), this, SLOT(emitScanFinished()));
  QThreadPool::globalInstance()->start(finished);
	return true;
}

void KovanSerialInterface::invalidateResponder()
{
  qDebug() << "Invalidate";
	m_responder = 0;
}

void KovanSerialInterface::scanStarted()
{
	if(!m_responder) return;
	m_responder->targetScanStarted(this);
}

void KovanSerialInterface::found(const QString &port)
{
  qDebug() << "Found called";
  
	if(!m_responder) return;
	
	UsbSerial *serial = new UsbSerial(port.toUtf8());
	KovanProtoTarget *device = new kiss::target::KovanProtoTarget(serial, this);
	device->fillCommPort(port);
	m_responder->targetFound(this, TargetPtr(device));
}


