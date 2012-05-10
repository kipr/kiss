#include "NewProjectWizard.h"

#include "ProjectType.h"
#include "TargetManager.h"

#include <QFileDialog>

NewProjectWizard::NewProjectWizard(QWidget* parent) : QDialog(parent)
{
	setupUi(this);
	
	foreach(ProjectType* projectType, ProjectTypes::ref().projectTypes())
		ui_projectTypes->addItem(projectType->name(), QVariant::fromValue(projectType));
	
	const QStringList& targets = TargetManager::ref().targets();
	foreach(const QString& target, targets)
		ui_targetPlatforms->addItem(TargetManager::ref().displayName(target), target);
		
	m_savePath = QDir::homePath();
	updateSaveLocation();
}

void NewProjectWizard::setTargetPlatformEnabled(bool enabled)
{
	ui_targetPlatformLabel->setVisible(enabled);
	ui_targetPlatforms->setVisible(enabled);
}

QString NewProjectWizard::projectName() const
{
	return ui_projectName->text();
}

QString NewProjectWizard::saveLocation() const
{
	return ui_saveLocation->text();
}

ProjectType* NewProjectWizard::projectType() const
{
	int i = ui_projectTypes->currentIndex();
	return ui_projectTypes->itemData(i).value<ProjectType*>();
}

QString NewProjectWizard::targetPlatform() const
{
	int i = ui_targetPlatforms->currentIndex();
	return ui_targetPlatforms->itemData(i).toString();
}

void NewProjectWizard::on_ui_browse_clicked()
{
	m_savePath = QFileDialog::getExistingDirectory(this, tr("Choose a Path"));
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