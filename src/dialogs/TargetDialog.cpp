#include "TargetDialog.h"

#include "Interface.h"
#include "InterfaceManager.h"

TargetDialog::TargetDialog(InterfaceManager* manager, QWidget* parent)
	: QDialog(parent), m_manager(manager),
	m_model(m_manager), m_interfaceModel(m_manager)
{
	setupUi(this);
	ui_targets->setModel(&m_model);
	m_interfaceModel.setAllInterface(true);
	ui_interfaces->setModel(&m_interfaceModel);
	
	ui_targetInfo->hide();
	
	connect(ui_targets->selectionModel(),
		SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
		SLOT(currentTargetChanged(const QModelIndex&)));
		
	connect(ui_targets,
		SIGNAL(doubleClicked(const QModelIndex&)),
		SLOT(targetChosen(const QModelIndex&)));
		
	ui_buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
	
	on_ui_refresh_clicked();
}

TargetDialog::~TargetDialog()
{
	foreach(Interface *interface, m_manager->interfaces())
		interface->invalidateResponder();
}

TargetPtr TargetDialog::target() const
{
	return m_model.indexToTarget(ui_targets->selectionModel()->currentIndex());
}

void TargetDialog::currentTargetChanged(const QModelIndex& index)
{
	TargetPtr target = m_model.indexToTarget(index);
	const QString unknown = "Unknown";
	ui_deviceName->setText(target.get() ? target->displayName() : unknown);
	ui_deviceType->setText(target.get() ? target->type() : unknown);
	ui_commPort->setText(target.get() ? target->commPort() : unknown);
	ui_serialNumber->setText(target.get() ? target->serial() : unknown);
	ui_interface->setText(target.get() ? target->interface()->name() : unknown);
	ui_version->setText(target.get() ? target->version() : unknown);
	ui_buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void TargetDialog::targetChosen(const QModelIndex& index)
{
	accept();
}

void TargetDialog::on_ui_interfaces_currentIndexChanged(int index)
{
	currentTargetChanged(QModelIndex());
}

void TargetDialog::on_ui_refresh_clicked()
{
	m_model.clear();
	foreach(Interface *interface, m_manager->interfaces())
		interface->scan(&m_model);
}
