#include "communication_widget.hpp"

#include "ui_communication_widget.h"

#include "communication_queue_model.hpp"
#include "communication_manager.hpp"

using namespace Kiss;
using namespace Kiss::Widget;

CommunicationWidget::CommunicationWidget(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::CommunicationWidget)
{
	ui->setupUi(this);
	
	ui->queue->setModel(new Target::CommunicationQueueModel(&Target::CommunicationManager::ref(), this));
	
	connect(ui->hide, SIGNAL(clicked()), SLOT(hide()));
}

CommunicationWidget::~CommunicationWidget()
{
}