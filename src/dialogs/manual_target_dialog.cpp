#include "manual_target_dialog.hpp"
#include "ui_manual_target_dialog.h"

#include "interface_model.hpp"
#include "interface_manager.hpp"
#include "interface.hpp"


using namespace Kiss::Dialog;

ManualTarget::ManualTarget(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ManualTargetDialog),
	m_model(new Kiss::Target::InterfaceModel(&Target::InterfaceManager::ref(), this))
{
	ui->setupUi(this);
	ui->targetInterface->setModel(m_model);
}

ManualTarget::~ManualTarget()
{
	delete ui;
}

Kiss::Target::TargetPtr ManualTarget::target() const
{
	Target::Interface *interface = m_model->rowToInterface(ui->targetInterface->currentIndex());
	if(!interface) return Kiss::Target::TargetPtr();
	return interface->createTarget(ui->address->text());
}