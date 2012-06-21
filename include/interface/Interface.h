#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "Named.h"
#include "Device.h"

#include <QList>
#include <QString>

class Interface : public Named
{
public:
	Interface(const QString& name);
	
	virtual QList<DevicePtr> devices() = 0;
	virtual const bool local() = 0;
};

Q_DECLARE_INTERFACE(Interface, "com.kipr.kiss.Interface/1.0");

#endif
