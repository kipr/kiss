#ifndef _TARGET_H_
#define _TARGET_H_

#include "Named.h"
#include <QList>

class Target : public Named
{
public:
	Target(const QString& name, const QList<DeviceInterface*> interfaces);
	~Target();
	
	const QList<DeviceInterface*>& interfaces() const;
private:
	QList<DeviceInterface*> m_interfaces;
};

#endif
