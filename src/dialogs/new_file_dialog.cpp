#include "new_file_dialog.hpp"
#include "ui_new_file_dialog.h"

#include "template_manager.hpp"
#include "template_model.hpp"
#include "template_pack.hpp"

#include <QItemSelection>
#include <QItemDelegate>
#include <QDebug>

using namespace kiss;
using namespace kiss::dialog;

class ItemDelegate : public QItemDelegate
{
public:
	ItemDelegate(QObject *parent = 0)
		: QItemDelegate(parent)
	{
	}
	
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		return QSize(100, 22);
	}
};

NewFile::NewFile(kiss::templates::Manager *manager, QWidget *parent)
	: QDialog(parent),
	m_manager(manager),
	m_model(0),
	ui(new Ui::NewFileDialog())
{
	ui->setupUi(this);
	m_model = new kiss::templates::Model(m_manager, ui->templates);
	m_model->setReadOnly(true);
	ui->templates->setModel(m_model);
	ui->templates->setItemDelegate(new ItemDelegate(this));
	ui->templates->expandAll();
	ui->removePack->setEnabled(false);
  m_helpText = ui->description->toPlainText();
  updateAcceptable();
  
	connect(ui->templates->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
		this, SLOT(selectionChanged(QItemSelection)));
  connect(ui->templates->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
  	this, SLOT(updateAcceptable()));
  connect(ui->fileName, SIGNAL(textChanged(QString)), this, SLOT(updateAcceptable()));
	connect(ui->removePack, SIGNAL(clicked()), this, SLOT(removeSelectedPack()));
}

NewFile::~NewFile()
{
	delete ui;
}

QString NewFile::fileName() const
{
  return ui->fileName->text();
}

kiss::templates::File NewFile::templateFile() const
{
	QItemSelection selection = ui->templates->selectionModel()->selection();
	if(selection.indexes().isEmpty()) return kiss::templates::File();
	return m_model->indexToFile(selection.indexes()[0]);
}

void NewFile::selectionChanged(const QItemSelection &selection)
{
	const bool singular = selection.indexes().size() == 1;
	
	if(!singular) {
		ui->description->setPlainText(m_helpText);
		ui->removePack->setEnabled(false);
		return;
	}
	
	QModelIndex index = selection.indexes()[0];
	kiss::templates::Pack *pack = m_model->indexToPack(index);
	
	ui->removePack->setEnabled(m_model->isIndexPack(index));
	ui->buttons->button(QDialogButtonBox::Ok)->setEnabled(!m_model->isIndexPack(index));
	
	if(!pack) {
		ui->description->setPlainText(m_helpText);
		return;
	}
	
	QString text = m_model->itemFromIndex(index)->text();
	QString path = (m_model->isIndexPack(index) ? QString() : text);
	ui->description->setText(pack->description(path));
	ui->description->setVisible(true);
}

void NewFile::removeSelectedPack()
{
	QItemSelection selection = ui->templates->selectionModel()->selection();
	if(selection.indexes().size() != 1) return;
	QModelIndex index = selection.indexes()[0];
	kiss::templates::Pack *pack = m_model->indexToPack(index);
	if(!pack) return;
	m_manager->removePack(pack, true);
}

void NewFile::updateAcceptable()
{
  const bool enabled = ui->templates->selectionModel()->hasSelection() && !fileName().isEmpty();
  ui->buttons->button(QDialogButtonBox::Ok)->setEnabled(enabled);
}