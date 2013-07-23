#include "project_dep_dialog.hpp"
#include "ui_ProjectDepDialog.h"
#include "file_utils.hpp"

using namespace kiss::dialog;

ProjectDepDialog::ProjectDepDialog(const QStringList &deps, QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);

	ui_buttonAdd = new QPushButton(tr("&Add"), this);
	ui_buttonRemove = new QPushButton(tr("&Remove"), this);
	ui_buttonBox->addButton(ui_buttonAdd, QDialogButtonBox::ActionRole);
	ui_buttonBox->addButton(ui_buttonRemove, QDialogButtonBox::ActionRole);

	connect(ui_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(ui_buttonAdd, SIGNAL(clicked()), this, SLOT(add()));
	connect(ui_buttonRemove, SIGNAL(clicked()), this, SLOT(remove()));

	foreach(const QString &dep, deps) add(dep);
}

ProjectDepDialog::~ProjectDepDialog()
{
	delete ui_buttonAdd;
	delete ui_buttonRemove;
}

QStringList ProjectDepDialog::names()
{
	QStringList list;
	for(int row = 0; row < ui_depsTable->rowCount(); ++row) {
		list << ui_depsTable->item(row, 0)->text();
	}

	return list;
}

QStringList ProjectDepDialog::paths()
{
	QStringList list;
	for(int row = 0; row < ui_depsTable->rowCount(); ++row) {
		list << ui_depsTable->item(row, 1)->text();
	}

	return list;
}

void ProjectDepDialog::add()
{
	const QString &fullPath = FileUtils::getOpenFileName(this, tr("Add Dependency"), tr("KISS Projects (*.kissproj)"));
	if(fullPath.isNull()) return;
	add(QFileInfo(fullPath).absolutePath());
}

void ProjectDepDialog::remove()
{
	ui_depsTable->removeRow(ui_depsTable->currentRow());
}

void ProjectDepDialog::add(const QString &dirPath)
{
	const QString &name = QDir(dirPath).dirName();

	const int &pos = ui_depsTable->rowCount();
	ui_depsTable->insertRow(ui_depsTable->rowCount());
	ui_depsTable->setItem(pos, 0, new QTableWidgetItem(name));
	ui_depsTable->setItem(pos, 1, new QTableWidgetItem(dirPath));
}
