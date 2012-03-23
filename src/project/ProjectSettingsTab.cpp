#include "ProjectSettingsTab.h"

#include "MainWindow.h"
#include "Activatable.h"
#include "ProjectFile.h"

#include <QFileInfo>

ProjectSettingsTab::ProjectSettingsTab(MainWindow* mainWindow) : QWidget(mainWindow), TabbedWidget(this, mainWindow), m_projectFile(0)
{
	setupUi(this);
}

void ProjectSettingsTab::setProjectFile(ProjectFile* projectFile)
{
	if(m_projectFile) disconnect(this, SIGNAL(settingUpdated(const QString&, const QString&)),
		m_projectFile, SLOT(settingUpdated(const QString&, const QString&)));
	m_projectFile = projectFile;
	connect(this, SIGNAL(settingUpdated(const QString&, const QString&)),
		m_projectFile, SLOT(settingUpdated(const QString&, const QString&)));
	
	qWarning() << "Set to" << projectFile->path();
	
	
	QMap<QString, QString> data = projectFile->projectSettings();
	
	ui_settingsTable->clearContents();
	ui_settingsTable->setRowCount(data.keys().size());
	quint16 i = 0;
	foreach(const QString& key, data.keys()) {
		ui_settingsTable->setItem(i, 0, new QTableWidgetItem(key));
		ui_settingsTable->setItem(i, 1, new QTableWidgetItem(data[key]));
		
		++i;
	}
}

ProjectFile* ProjectSettingsTab::projectFile() const
{
	return m_projectFile;
}

void ProjectSettingsTab::activate()
{
	QList<Menuable*> menus = mainWindow()->menuablesExcept(mainWindow()->standardMenus());
	foreach(Menuable* menu, menus) {
		ActivatableObject* activatable = dynamic_cast<ActivatableObject*>(menu);
		if(activatable) activatable->setActive(0);
	}

	mainWindow()->showProjectDock();
}

bool ProjectSettingsTab::beginSetup()
{
	return true;
}

void ProjectSettingsTab::completeSetup()
{
	if(m_projectFile) mainWindow()->setTabName(this, QFileInfo(m_projectFile->path()).fileName());
}

bool ProjectSettingsTab::close()
{
	m_projectFile->sync();
	return true;
}

void ProjectSettingsTab::refreshSettings()
{
	
}

void ProjectSettingsTab::on_ui_settingsTable_cellChanged(int row, int column)
{
	QTableWidgetItem* key = ui_settingsTable->item(row, 0);
	QTableWidgetItem* value = ui_settingsTable->item(row, 1);
	if(key && value) emit settingUpdated(key->text(), value->text());
}

void ProjectSettingsTab::on_ui_add_clicked()
{
	ui_settingsTable->setRowCount(ui_settingsTable->rowCount() + 1);
}

void ProjectSettingsTab::on_ui_remove_clicked()
{
	ui_settingsTable->removeRow(ui_settingsTable->currentRow());
}