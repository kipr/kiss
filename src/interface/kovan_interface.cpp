#include "kovan_interface.hpp"
#include "kovan_proto_target.hpp"

#include <kovanserial/tcp_serial.hpp>

#include <QDebug>

#include <QHostAddress>
#include <QThreadPool>

using namespace kiss::target;

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
      sockaddr_in mod = it->sender;
      mod.sin_port = it->ad.port;
      QHostAddress addr((sockaddr *)&mod);
      if(m_found.contains(addr)) continue;
      m_found.push_back(addr);
      emit found((*it).ad, mod);
    }
  }
}

KovanInterface::KovanInterface()
	: Interface("Network"),
	m_advertiser(new UdpAdvertiser(false)),
	m_responder(0)
{
}

KovanInterface::~KovanInterface()
{
	delete m_advertiser;
}

kiss::target::TargetPtr KovanInterface::createTarget(const QString &address)
{
  if(address.isEmpty()) return TargetPtr();
	// TODO: Add input verification
  const QStringList chunks = address.split(":");
  unsigned short port = KOVAN_SERIAL_PORT;
  if(chunks.size() == 2) {
    bool ok = false;
    port = chunks[1].toUShort(&ok);
    if(!ok) return TargetPtr();
  } else if(chunks.size() > 2) return TargetPtr();
  
	TcpSerial *serial = new TcpSerial(address.toUtf8(), port);
	KovanProtoTarget *device = new kiss::target::KovanProtoTarget(serial, this);
	return TargetPtr(device);
}

const bool KovanInterface::scan(InterfaceResponder *responder)
{
	m_responder = responder;
	m_advertiser->reset();
	AdvertSampler *sampler = new AdvertSampler(m_advertiser, 100, 75);
	sampler->setAutoDelete(true);
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

void KovanInterface::found(const Advert &ad, const sockaddr_in addr)
{
	if(!m_responder) return;
	QHostAddress ha((sockaddr *)&addr);
	TcpSerial *serial = new TcpSerial(ha.toString().toUtf8(), addr.sin_port);
	KovanProtoTarget *device = new kiss::target::KovanProtoTarget(serial, this);
	device->fillInformation(ad);
	device->fillCommPort(ha.toString());
	m_responder->targetFound(this, TargetPtr(device));
}


