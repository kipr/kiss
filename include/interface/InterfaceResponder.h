#ifndef _INTERFACERESPONDER_H_
#define _INTERFACERESPONDER_H_

#include "Device.h"

class Interface;

class InterfaceResponder
{
public:
	virtual void deviceScanStarted(Interface *interface) = 0;
	virtual void deviceFound(Interface *interface, DevicePtr device) = 0;
};

#endif
