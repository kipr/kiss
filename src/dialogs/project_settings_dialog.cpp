#include "project_settings_dialog.hpp"
#include "ui_ProjectSettingsDialog.h"
#include "compiler_flag_dialog.hpp"
#include "file_utils.hpp"

using namespace kiss;
using namespace kiss::dialog;

ProjectSettingsDialog::ProjectSettingsDialog(project::ProjectPtr project, QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);

	connect(ui_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(ui_listContents, SIGNAL(currentRowChanged(int)), ui_stackedWidget, SLOT(setCurrentIndex(int)));

	foreach(const QString &dep, project->deps()) addDep(dep);
	
	const Compiler::Options &flags = project->compilerFlags();
	foreach(const QString &key, flags.keys()) addSetting(key, flags[key].toString());
	
	ui_buttonLibrary->setChecked(project->compileLib());
	ui_boxAutoDep->setChecked(project->autoCompileDeps());
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

Compiler::Options ProjectSettingsDialog::compilerFlags()
{
	Compiler::Options ret;
	for(int row = 0; row < ui_tableCompile->rowCount(); ++row) {
		ret.insert(ui_tableCompile->item(row, 0)->text(), ui_tableCompile->item(row, 1)->text());
	}
	
	return ret;
}

bool ProjectSettingsDialog::compileLib()
{
	return ui_buttonLibrary->isChecked();
}

bool ProjectSettingsDialog::autoCompileDeps()
{
	return ui_boxAutoDep->isChecked();
}

void ProjectSettingsDialog::addDep(const QString &dirPath)
{
	const QString &name = QDir(dirPath).dirName();

  QTableWidgetItem *projectItem = new QTableWidgetItem(name);
  QTableWidgetItem *pathItem = new QTableWidgetItem(dirPath);
  projectItem->setToolTip(projectItem->text());
  pathItem->setToolTip(pathItem->text());
  
	const int &pos = ui_tableDeps->rowCount();
	ui_tableDeps->insertRow(pos);
	ui_tableDeps->setItem(pos, 0, projectItem);
	ui_tableDeps->setItem(pos, 1, pathItem);
}

void ProjectSettingsDialog::addSetting(const QString &flag, const QString &value)
{
  QTableWidgetItem *flagItem = new QTableWidgetItem(flag);
  QTableWidgetItem *valueItem = new QTableWidgetItem(value);
  flagItem->setToolTip(flag);
  valueItem->setToolTip(value);
	
	const int numRows = ui_tableCompile->rowCount();
	ui_tableCompile->insertRow(numRows);
	ui_tableCompile->setItem(numRows, 0, flagItem);
	ui_tableCompile->setItem(numRows, 1, valueItem);
}

void ProjectSettingsDialog::editCompilerFlag(int row)
{
	if(row < 0) return;
	
	QTableWidgetItem *item1 = ui_tableCompile->item(row, 0);
	QTableWidgetItem *item2 = ui_tableCompile->item(row, 1);
	
	CompilerFlagDialog dialog(this, item1->text(), item2->text());
	if(dialog.exec() == QDialog::Rejected) return;
	
	item1->setText(dialog.flag());
	item2->setText(dialog.value());
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
	CompilerFlagDialog dialog(this);
	if(dialog.exec() == QDialog::Rejected) return;
  addSetting(dialog.flag(), dialog.value());
}

void ProjectSettingsDialog::on_ui_buttonEditSetting_clicked()
{
	editCompilerFlag(ui_tableCompile->currentRow());
}

void ProjectSettingsDialog::on_ui_buttonRemoveSetting_clicked()
{
	ui_tableCompile->removeRow(ui_tableCompile->currentRow());
}

void ProjectSettingsDialog::on_ui_tableCompile_cellDoubleClicked(int row, int column)
{
	editCompilerFlag(row);
}