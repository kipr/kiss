#include "DeviceModel.h"

#include "Device.h"
#include "Interface.h"
#include "InterfaceManager.h"

#include "ResourceHelper.h"

#include <QDebug>

class DeviceItem : public QStandardItem
{
public:
	DeviceItem(DevicePtr device)
		: QStandardItem(device->displayName()), m_device(device)
	{
		setEditable(false);
		setIcon(ResourceHelper::ref().icon("default_device"));
	}
	
	DevicePtr device() {
		return m_device;
	}
	
	template<typename T>
	static DevicePtr device_cast(T* t) {
		DeviceItem* item = dynamic_cast<DeviceItem*>(t);
		return item ? item->device() : DevicePtr();
	}
	
	template<typename T>
	static Interface* interface_cast(T* t) {
		DevicePtr device = device_cast(t);
		return device.get() ? device->interface() : 0;
	}
private:
	DevicePtr m_device;
};

DeviceModel::DeviceModel(InterfaceManager* manager) : m_manager(manager)
{
	connect(m_manager, SIGNAL(interfaceRemoved(Interface *)), SLOT(interfaceRemoved(Interface *)), Qt::QueuedConnection);
	connect(this, SIGNAL(interfaceStartedScan(Interface *)), SLOT(interfaceRemoved(Interface *)), Qt::QueuedConnection);
	connect(this, SIGNAL(foundDevice(DevicePtr)), SLOT(addDevice(DevicePtr)), Qt::QueuedConnection);
}

DevicePtr DeviceModel::indexToDevice(const QModelIndex& index) const
{
	return DeviceItem::device_cast(itemFromIndex(index));
}

Interface* DeviceModel::indexToInterface(const QModelIndex& index) const
{
	return DeviceItem::interface_cast(itemFromIndex(index));
}

void DeviceModel::refresh()
{
	clear();
	// foreach(Interface *interface, InterfaceManager::ref().interfaces()) interface->scan(this);
}

void DeviceModel::addDevice(DevicePtr device)
{
	appendRow(new DeviceItem(device));
}

void DeviceModel::interfaceRemoved(Interface* interface)
{
	for(int i = 0; i < rowCount(); ++i) {
		if(DeviceItem::interface_cast(item(i)) == interface) delete takeItem(i--);
	}
}

void DeviceModel::deviceScanStarted(Interface *interface)
{
	emit interfaceStartedScan(interface);
}

void DeviceModel::deviceFound(Interface *interface, DevicePtr device)
{
	emit foundDevice(device);
}
