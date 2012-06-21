#ifndef _LOCALINTERFACE_H_
#define _LOCALINTERFACE_H_

#include <QtPlugin>
#include "Interface.h"

class LocalInterface : public QObject, public Interface
{
Q_OBJECT
Q_INTERFACES(Interface)
public:
	LocalInterface();
	
	virtual QList<DevicePtr> devices();
	virtual const bool local();
};

#endif
