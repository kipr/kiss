#ifndef _KOVAN_INTERFACE_HPP_
#define _KOVAN_INTERFACE_HPP_

#include <QtPlugin>
#include "interface.hpp"
#include <easydevice/DiscoveryServer.h>
#include <easydevice/DeviceInfo.h>

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
	void found(EasyDevice::DeviceInfo deviceInfo, const QHostAddress& address);
	
private:
	EasyDevice::DiscoveryServer *m_server;
	Kiss::Target::InterfaceResponder *m_responder;
};

#endif
