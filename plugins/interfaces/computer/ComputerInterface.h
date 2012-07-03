#ifndef _COMPUTERINTERFACE_H_
#define _COMPUTERINTERFACE_H_

#include <QtPlugin>
#include "Interface.h"
#include <easydevice/DiscoveryServer.h>
#include <easydevice/DeviceInfo.h>

class Ohaiyo;
class OhaiyoListener;

class ComputerInterface : public QObject, public Interface
{
Q_OBJECT
Q_INTERFACES(Interface)
public:
	ComputerInterface();
	~ComputerInterface();
	
	virtual const bool scan(InterfaceResponder *responder);
private slots:
	void scanStarted();
	void found(EasyDevice::DeviceInfo deviceInfo, const QHostAddress& address);
	
private:
	EasyDevice::DiscoveryServer *m_server;
	InterfaceResponder *m_responder;
};

#endif
