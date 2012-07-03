#ifndef _CBC3INTERFACE_H_
#define _CBC3INTERFACE_H_

#include <QtPlugin>
#include "Interface.h"
#include <easydevice/DiscoveryServer.h>
#include <easydevice/DeviceInfo.h>

class Ohaiyo;
class OhaiyoListener;

class Cbc3Interface : public QObject, public Interface
{
Q_OBJECT
Q_INTERFACES(Interface)
public:
	Cbc3Interface();
	~Cbc3Interface();
	
	virtual const bool scan(InterfaceResponder *responder);
private slots:
	void scanStarted();
	void found(EasyDevice::DeviceInfo deviceInfo, const QHostAddress& address);
	
private:
	EasyDevice::DiscoveryServer *m_server;
	InterfaceResponder *m_responder;
};

#endif
