#ifndef _COMPUTERINTERFACE_H_
#define _COMPUTERINTERFACE_H_

#include <QtPlugin>
#include "interface.hpp"
#include <easydevice/DiscoveryServer.h>
#include <easydevice/DeviceInfo.h>

class ComputerInterface : public QObject, public Kiss::Target::Interface
{
Q_OBJECT
Q_INTERFACES(Kiss::Target::Interface)
public:
	ComputerInterface();
	~ComputerInterface();
	
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
