#include "NewProjectWizard.h"

#include <QFileDialog>
#include <QSettings>

#define SETTINGS_GROUP "New Project Wizard"
#define SETTINGS_KEY "Save Location"

NewProjectWizard::NewProjectWizard(QWidget* parent) : QDialog(parent)
{
	setupUi(this);
		
	QSettings settings;
	settings.beginGroup(SETTINGS_GROUP);
	m_savePath = settings.value(SETTINGS_KEY, QDir::homePath()).toString();
	settings.endGroup();
	updateSaveLocation();
}

QString NewProjectWizard::projectName() const
{
	return ui_projectName->text();
}

QString NewProjectWizard::saveLocation() const
{
	return ui_saveLocation->text();
}

void NewProjectWizard::on_ui_browse_clicked()
{
	m_savePath = QFileDialog::getExistingDirectory(this, tr("Choose a Path"));
	QSettings settings;
	settings.beginGroup(SETTINGS_GROUP);
	settings.setValue(SETTINGS_KEY, m_savePath);
	settings.endGroup();
	updateSaveLocation();
}

void NewProjectWizard::on_ui_projectName_textChanged(const QString&)
{
	updateSaveLocation();
}

void NewProjectWizard::updateSaveLocation()
{
	ui_saveLocation->setText(m_savePath + "/" + ui_projectName->text() + ".kissproj");
}
