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

InterfaceWorker::InterfaceWorker(Interface *interface, QStandardItemModel *model)
	: m_interface(interface), m_model(model)
{}

InterfaceWorker::~InterfaceWorker() {}

void InterfaceWorker::run()
{
	printf("Checking interface %s for devices...\n", m_interface->name().toStdString().c_str());
	QList<DevicePtr> devices = m_interface->devices();
	foreach(DevicePtr device, devices) emit foundDevice(device);
}


DeviceModel::DeviceModel(InterfaceManager* manager) : m_manager(manager)
{
	connect(m_manager, SIGNAL(interfaceAdded(Interface*)), SLOT(interfaceAdded(Interface*)));
	connect(m_manager, SIGNAL(interfaceRemoved(Interface*)), SLOT(interfaceRemoved(Interface*)), Qt::QueuedConnection);
}

DevicePtr DeviceModel::indexToDevice(const QModelIndex& index) const
{
	return DeviceItem::device_cast(itemFromIndex(index));
}

Interface* DeviceModel::indexToInterface(const QModelIndex& index) const
{
	return DeviceItem::interface_cast(itemFromIndex(index));
}

void DeviceModel::filter(Interface* interface)
{
	reload(interface);
}

void DeviceModel::refresh()
{
	clear();
	foreach(Interface *interface, InterfaceManager::ref().interfaces()) interfaceAdded(interface);
}

void DeviceModel::addDevice(DevicePtr device)
{
	appendRow(new DeviceItem(device));
}

void DeviceModel::interfaceAdded(Interface* interface)
{
	interfaceRemoved(interface);
	InterfaceWorker *worker = new InterfaceWorker(interface, this);
	connect(worker, SIGNAL(foundDevice(DevicePtr)), SLOT(addDevice(DevicePtr)));
	QThreadPool::globalInstance()->start(worker);
}

void DeviceModel::interfaceRemoved(Interface* interface)
{
	for(int i = 0; i < rowCount(); ++i) {
		if(DeviceItem::interface_cast(item(i)) == interface) delete takeItem(i--);
	}
}

void DeviceModel::reload(Interface* filter)
{
	bool filtering = filter;
	clear();
	foreach(Interface* interface, m_manager->interfaces()) {
		if(!filtering) filter = interface;
		if(interface == filter) interfaceAdded(interface);
	}
}