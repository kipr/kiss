#include "new_project_dialog.hpp"
#include "ui_NewProjectDialog.h"
#include "file_utils.hpp"

#include <QSettings>
#include <QDebug>

#define SETTINGS_GROUP "New Project Wizard"
#define SETTINGS_KEY "Save Location"

using namespace kiss::dialog;

NewProjectDialog::NewProjectDialog(QWidget *parent)
	: QDialog(parent),
  ui(new Ui::NewProjectDialog())
{
	ui->setupUi(this);
	ui->projectName->selectAll();
		
	QSettings settings;
	settings.beginGroup(SETTINGS_GROUP);
	m_savePath = settings.value(SETTINGS_KEY, QDir::homePath()).toString();
	settings.endGroup();
	updateSaveLocation();
  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!projectName().isEmpty());  
}

void NewProjectDialog::limitTypeButtons()
{
  ui->existingFile->setChecked(true);
  ui->existingFile->setEnabled(false);
  ui->newFile->setEnabled(false);
  ui->emptyProject->setEnabled(false);  
}

QString NewProjectDialog::projectName() const
{
	return ui->projectName->text();
}

QString NewProjectDialog::saveLocation() const
{
	return ui->saveLocation->text();
}

bool NewProjectDialog::emptyProject() const
{
  return ui->emptyProject->isChecked();
}

bool NewProjectDialog::newFile() const
{
  return ui->newFile->isChecked();
}

bool NewProjectDialog::existingFile() const
{
  return ui->existingFile->isChecked();
}

void NewProjectDialog::on_browse_clicked()
{
  const QString savePath = FileUtils::getExistingDirectory(this, tr("Choose a Save Location"));
  if(savePath.isEmpty()) return;
	m_savePath = savePath;
	QSettings settings;
	settings.beginGroup(SETTINGS_GROUP);
	settings.setValue(SETTINGS_KEY, m_savePath);
	settings.endGroup();
	updateSaveLocation();
}

void NewProjectDialog::on_projectName_textChanged(const QString &text)
{
  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
	updateSaveLocation();
}

void NewProjectDialog::updateSaveLocation()
{
	ui->saveLocation->setText(m_savePath + "/" + ui->projectName->text());
}
