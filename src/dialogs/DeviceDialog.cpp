#include "DeviceDialog.h"

#include "Interface.h"

DeviceDialog::DeviceDialog(InterfaceManager* manager, QWidget* parent)
	: QDialog(parent), m_model(manager), m_interfaceModel(manager)
{
	setupUi(this);
	ui_devices->setModel(&m_model);
	m_interfaceModel.setAllInterface(true);
	ui_interfaces->setModel(&m_interfaceModel);
	
	ui_deviceInfo->hide();
	
	connect(ui_devices->selectionModel(),
		SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
		SLOT(currentDeviceChanged(const QModelIndex&)));
	connect(ui_devices,
		SIGNAL(doubleClicked(const QModelIndex&)),
		SLOT(deviceChosen(const QModelIndex&)));
}

DevicePtr DeviceDialog::device() const
{
	return m_model.indexToDevice(ui_devices->selectionModel()->currentIndex());
}

void DeviceDialog::currentDeviceChanged(const QModelIndex& index)
{
	DevicePtr device = m_model.indexToDevice(index);
	const QString unknown = "?";
	ui_deviceName->setText(device.get() ? device->displayName() : unknown);
	ui_deviceType->setText(device.get() ? device->type() : unknown);
	ui_downloadType->setText(device.get() ? (device->downloadType() == Device::Source ? "Source" : "Binary") : unknown);
	ui_serialNumber->setText(device.get() ? device->serial() : unknown);
	ui_interface->setText(device.get() ? device->interface()->name() : unknown);
}

void DeviceDialog::deviceChosen(const QModelIndex& index)
{
	
	accept();
}

void DeviceDialog::on_ui_interfaces_currentIndexChanged(int index)
{
	m_model.filter(m_interfaceModel.rowToInterface(index));
	currentDeviceChanged(QModelIndex());
}

void DeviceDialog::on_ui_refresh_clicked()
{
	m_model.refresh();
}