#ifndef _INTERFACERESPONDER_H_
#define _INTERFACERESPONDER_H_

#include "Target.h"

class Interface;

class InterfaceResponder
{
public:
	virtual void targetScanStarted(Interface *interface) = 0;
	virtual void targetFound(Interface *interface, TargetPtr target) = 0;
};

#endif
