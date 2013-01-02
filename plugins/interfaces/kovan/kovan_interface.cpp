#include "kovan_interface.hpp"
#include "kovan_proto_target.hpp"

#include <kovanserial/tcp_serial.hpp>

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

AdvertSampler::AdvertSampler(UdpAdvertiser *advertiser, const quint64 &sampleTime, const quint16 &samples)
	: m_advertiser(advertiser),
	m_sampleTime(sampleTime),
	m_samples(samples)
{
}

AdvertSampler::~AdvertSampler()
{
}

void AdvertSampler::run()
{
	for(quint16 i = 0; i < m_samples; ++i) {
		std::list<IncomingAdvert> adverts = m_advertiser->sample(m_sampleTime);
		std::list<IncomingAdvert>::const_iterator it = adverts.begin();
		for(; it != adverts.end(); ++it) {
			QHostAddress addr((sockaddr *)&(*it).sender);
			if(m_found.contains(addr)) continue;
			m_found.push_back(addr);
			emit found((*it).ad, (*it).sender);
		}
	}
}

KovanInterface::KovanInterface()
	: Interface("Kovan (Networked)"),
	m_advertiser(new UdpAdvertiser(false)),
	m_responder(0)
{
}

KovanInterface::~KovanInterface()
{
	delete m_advertiser;
}

Kiss::Target::TargetPtr KovanInterface::createTarget(const QString &address)
{
	// TODO: Add input verification
	TcpSerial *serial = new TcpSerial(address.toAscii(), KOVAN_SERIAL_PORT);
	KovanProtoTarget *device = new Kiss::Target::KovanProtoTarget(serial, this);
	return TargetPtr(device);
}

const bool KovanInterface::scan(InterfaceResponder *responder)
{
	m_responder = responder;
	m_advertiser->reset();
	AdvertSampler *sampler = new AdvertSampler(m_advertiser, 100, 100);
	qRegisterMetaType<Advert>("Advert");
	qRegisterMetaType<sockaddr_in>("sockaddr_in");
	connect(sampler, SIGNAL(found(Advert, sockaddr_in)), SLOT(found(Advert, sockaddr_in)));
	QThreadPool::globalInstance()->start(sampler);
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

void KovanInterface::found(const Advert &ad, const sockaddr_in& addr)
{
	if(!m_responder) return;
	QHostAddress ha((sockaddr *)&addr);
	
	TcpSerial *serial = new TcpSerial(ha.toString().toAscii(), KOVAN_SERIAL_PORT);
	KovanProtoTarget *device = new Kiss::Target::KovanProtoTarget(serial, this);
	device->fillInformation(ad);
	m_responder->targetFound(this, TargetPtr(device));
}

Q_EXPORT_PLUGIN2(kovan_interface, KovanInterface);
