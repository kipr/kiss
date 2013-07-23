#include "manual_target_dialog.hpp"
#include "ui_manual_target_dialog.h"

#include "interface_model.hpp"
#include "interface_manager.hpp"
#include "interface.hpp"


using namespace kiss::dialog;

ManualTarget::ManualTarget(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ManualTargetDialog),
	m_model(new kiss::target::InterfaceModel(&target::InterfaceManager::ref(), this))
{
	ui->setupUi(this);
	ui->targetInterface->setModel(m_model);
}

ManualTarget::~ManualTarget()
{
	delete ui;
}

kiss::target::TargetPtr ManualTarget::target() const
{
	target::Interface *interface = m_model->rowToInterface(ui->targetInterface->currentIndex());
	if(!interface) return kiss::target::TargetPtr();
	return interface->createTarget(ui->address->text());
}