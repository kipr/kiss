#ifndef _KOVAN_INTERFACE_HPP_
#define _KOVAN_INTERFACE_HPP_

#include <QtPlugin>
#include "interface.hpp"
#include <kovanserial/udp_advertiser.hpp>

class KovanInterface : public QObject, public Kiss::Target::Interface
{
Q_OBJECT
Q_INTERFACES(Kiss::Target::Interface)
public:
	KovanInterface();
	~KovanInterface();
	
	virtual const bool scan(Kiss::Target::InterfaceResponder *responder);
	virtual void invalidateResponder();
	
private slots:
	void scanStarted();
	void found(const Advert &ad, const QHostAddress& address);
	
private:
	UdpAdvertiser *m_advertiser;
	Kiss::Target::InterfaceResponder *m_responder;
};

#endif
