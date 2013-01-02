#ifndef _COMPUTER_INTERFACE_HPP_
#define _COMPUTER_INTERFACE_HPP_

#include <QtPlugin>
#include "interface.hpp"

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
	Kiss::Target::InterfaceResponder *m_responder;
};

#endif
