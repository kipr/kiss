#include "Device.h"

Device::Device(Interface* interface, const QString& name)
	: Named(name), m_interface(interface)
{
	
}

Device::~Device()
{
	
}

Interface* Device::interface() const
{
	return m_interface;
}

const QString Device::displayName() const
{
	return information().value(DISPLAY_NAME);
}

const QString Device::type() const
{
	return information().value(DEVICE_TYPE);
}

const QString Device::serial() const
{
	return information().value(SERIAL);
}

const int Device::downloadType() const
{
	const QString& type = information().value(DOWNLOAD_TYPE);
	return type == DOWNLOAD_BINARY ? Binary : Source;
}