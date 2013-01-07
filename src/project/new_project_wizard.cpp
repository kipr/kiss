#include "new_project_wizard.hpp"

#include <QFileDialog>
#include <QSettings>

#define SETTINGS_GROUP "New Project Wizard"
#define SETTINGS_KEY "Save Location"

using namespace Kiss::Wizard;

NewProject::NewProject(QWidget* parent)
	: QDialog(parent)
{
	setupUi(this);
		
	QSettings settings;
	settings.beginGroup(SETTINGS_GROUP);
	m_savePath = settings.value(SETTINGS_KEY, QDir::homePath()).toString();
	settings.endGroup();
	updateSaveLocation();
}

QString NewProject::projectName() const
{
	return ui_projectName->text();
}

QString NewProject::saveLocation() const
{
	return ui_saveLocation->text();
}

void NewProject::on_ui_browse_clicked()
{
	m_savePath = QFileDialog::getExistingDirectory(this, tr("Choose a Path"));
	QSettings settings;
	settings.beginGroup(SETTINGS_GROUP);
	settings.setValue(SETTINGS_KEY, m_savePath);
	settings.endGroup();
	updateSaveLocation();
}

void NewProject::on_ui_projectName_textChanged(const QString&)
{
	updateSaveLocation();
}

void NewProject::updateSaveLocation()
{
	ui_saveLocation->setText(m_savePath + "/" + ui_projectName->text() + ".kissproj");
}
