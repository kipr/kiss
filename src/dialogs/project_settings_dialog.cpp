#include "project_settings_dialog.hpp"
#include "ui_ProjectSettingsDialog.h"
#include "file_utils.hpp"

using namespace kiss::dialog;

ProjectSettingsDialog::ProjectSettingsDialog(project::ProjectPtr project, QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);

	connect(ui_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(ui_listContents, SIGNAL(currentRowChanged(int)), ui_stackedWidget, SLOT(setCurrentIndex(int)));
	//connect(ui_buttonAddDep, SIGNAL(clicked()), this, SLOT(addDep()));
	//connect(ui_buttonRemoveDep, SIGNAL(clicked()), this, SLOT(removeDep()));

	foreach(const QString &dep, project->dependencies()) addDep(dep);
	
	const Compiler::Options &settings = project->settings();
	ui_tableCompile->setRowCount(settings.keys().size());
	quint16 i = 0;
	foreach(const QString &key, settings.keys()) {
		ui_tableCompile->setItem(i, 0, new QTableWidgetItem(key));
		ui_tableCompile->setItem(i, 1, new QTableWidgetItem(settings[key]));
		++i;
	}

}

ProjectSettingsDialog::~ProjectSettingsDialog()
{
}

QStringList ProjectSettingsDialog::depNames()
{
	QStringList list;
	for(int row = 0; row < ui_tableDeps->rowCount(); ++row) {
		list << ui_tableDeps->item(row, 0)->text();
	}

	return list;
}

QStringList ProjectSettingsDialog::depPaths()
{
	QStringList list;
	for(int row = 0; row < ui_tableDeps->rowCount(); ++row) {
		list << ui_tableDeps->item(row, 1)->text();
	}

	return list;
}

Compiler::Options ProjectSettingsDialog::compilerSettings()
{
	Compiler::Options ret;
	for(int row = 0; row < ui_tableCompile->rowCount(); ++row) {
		ret.insert(ui_tableCompile->item(row, 0)->text(), ui_tableCompile->item(row, 1)->text());
	}
	
	return ret;
}

void ProjectSettingsDialog::addDep(const QString &dirPath)
{
	const QString &name = QDir(dirPath).dirName();

	const int &pos = ui_tableDeps->rowCount();
	ui_tableDeps->insertRow(ui_tableDeps->rowCount());
	ui_tableDeps->setItem(pos, 0, new QTableWidgetItem(name));
	ui_tableDeps->setItem(pos, 1, new QTableWidgetItem(dirPath));
}

void ProjectSettingsDialog::on_ui_buttonAddDep_clicked()
{
	const QString &fullPath = FileUtils::getOpenFileName(this, tr("Add Dependency"), tr("KISS Projects (*.kissproj)"));
	if(fullPath.isNull()) return;
	addDep(QFileInfo(fullPath).absolutePath());
}

void ProjectSettingsDialog::on_ui_buttonRemoveDep_clicked()
{
	ui_tableDeps->removeRow(ui_tableDeps->currentRow());
}

void ProjectSettingsDialog::on_ui_buttonAddSetting_clicked()
{
	ui_tableCompile->insertRow(ui_tableCompile->rowCount());
}

void ProjectSettingsDialog::on_ui_buttonRemoveSetting_clicked()
{
	int row = ui_tableCompile->currentRow();
	QTableWidgetItem *key = ui_tableCompile->item(row, 0);
	QString keyString = key ? key->text() : "";
	ui_tableCompile->removeRow(row);
}