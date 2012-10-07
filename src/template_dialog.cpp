#include "template_dialog.hpp"
#include "ui_template_dialog.h"

#include "template_manager.hpp"
#include "template_model.hpp"
#include "template_pack.hpp"

#include <QItemSelection>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Dialog;

Dialog::Template::Template(Kiss::Template::Manager *manager, QWidget *parent)
	: QDialog(parent),
	m_manager(manager),
	m_model(0),
	ui(new Ui::TemplateDialog())
{
	ui->setupUi(this);
	m_model = new Kiss::Template::Model(m_manager, ui->templates);
	m_model->setReadOnly(true);
	ui->templates->setModel(m_model);
	ui->description->setVisible(false);
	connect(ui->templates->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
		this, SLOT(selectionChanged(QItemSelection)));
}

Dialog::Template::~Template()
{
	delete ui;
}

Kiss::Template::File Dialog::Template::file() const
{
	QItemSelection selection = ui->templates->selectionModel()->selection();
	if(selection.indexes().isEmpty()) return Kiss::Template::File();
	return m_model->indexToFile(selection.indexes()[0]);
}

void Dialog::Template::selectionChanged(const QItemSelection& selection)
{
	const bool singular = selection.indexes().size() == 1;
	
	if(!singular) {
		qDebug() << "Not singular";
		ui->description->setVisible(false);
		return;
	}
	
	QModelIndex index = selection.indexes()[0];
	Kiss::Template::Pack *pack = m_model->indexToPack(index);
	
	if(!pack) {
		qDebug() << "Pack null";
		ui->description->setVisible(false);
		return;
	}
	
	QString text = m_model->itemFromIndex(index)->text();
	QString path = (m_model->isIndexPack(index) ? QString() : text);
	ui->description->setText(pack->description(path));
	ui->description->setVisible(true);
}