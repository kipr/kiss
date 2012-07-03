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
	ui_buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
}

DevicePtr DeviceDialog::device() const
{
	return m_model.indexToDevice(ui_devices->selectionModel()->currentIndex());
}

void DeviceDialog::currentDeviceChanged(const QModelIndex& index)
{
	DevicePtr device = m_model.indexToDevice(index);
	const QString unknown = "Unknown";
	ui_deviceName->setText(device.get() ? device->displayName() : unknown);
	ui_deviceType->setText(device.get() ? device->type() : unknown);
	ui_commPort->setText(device.get() ? device->commPort() : unknown);
	ui_serialNumber->setText(device.get() ? device->serial() : unknown);
	ui_interface->setText(device.get() ? device->interface()->name() : unknown);
	ui_buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void DeviceDialog::deviceChosen(const QModelIndex& index)
{
	accept();
}

void DeviceDialog::on_ui_interfaces_currentIndexChanged(int index)
{
	currentDeviceChanged(QModelIndex());
}

void DeviceDialog::on_ui_refresh_clicked()
{
	m_model.refresh();
}