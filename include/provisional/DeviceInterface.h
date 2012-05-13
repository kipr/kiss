#ifndef _DEVICEINTERFACE_H_
#define _DEVICEINTERFACE_H_

#include "Named.h"
#include "Device.h"

#include <QList>
#include <QString>

class DeviceInterface : public Named
{
public:
	DeviceInterface(const QString& name);
	
	virtual QList<DevicePtr> devices() = 0;
};

#endif
