#include "Target.h"

Target::Target(const QString& name, const QList<DeviceInterface*> interfaces)
	: Named(name), m_interfaces(interfaces)
{
	
}

Target::~Target()
{
	qDeleteAll(m_interfaces);
}

const QList<DeviceInterface*>& Target::interfaces() const
{
	return m_interfaces;
}