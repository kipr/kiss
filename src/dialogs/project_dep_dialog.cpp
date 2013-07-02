#include "project_dep_dialog.hpp"
#include "ui_ProjectDepDialog.h"

#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QFileDialog>

using namespace Kiss::Dialog;

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
	QSettings settings;
	QString openPath = settings.value("openpath", QDir::homePath()).toString();
	const QString &fullPath = QFileDialog::getOpenFileName(this, tr("Add Dependency"), openPath, tr("KISS Projects (*.kissproj)"));
	if(fullPath.isNull()) return;
	const QString &dirPath = QFileInfo(fullPath).absolutePath();
	add(dirPath);
}

void ProjectDepDialog::remove()
{
}

void ProjectDepDialog::add(const QString &dirPath)
{
	const QString &name = QDir(dirPath).dirName();

	const int &pos = ui_depsTable->rowCount();
	ui_depsTable->insertRow(ui_depsTable->rowCount());
	ui_depsTable->setItem(pos, 0, new QTableWidgetItem(name));
	ui_depsTable->setItem(pos, 1, new QTableWidgetItem(dirPath));
}
