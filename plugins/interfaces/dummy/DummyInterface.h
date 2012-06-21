#ifndef _DUMMYINTERFACE_H_
#define _DUMMYINTERFACE_H_

#include <QtPlugin>
#include "Interface.h"

class DummyInterface : public QObject, public Interface
{
Q_OBJECT
Q_INTERFACES(Interface)
public:
	DummyInterface();
	virtual QList<DevicePtr> devices();
	virtual const bool local();
};

#endif
