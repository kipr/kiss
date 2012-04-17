#include "ProjectSettingsTab.h"

#include "MainWindow.h"
#include "Activatable.h"
#include "ProjectFile.h"
#include "Project.h"

#include <QFileInfo>

ProjectSettingsTab::ProjectSettingsTab(Project* project, MainWindow* mainWindow) : QWidget(mainWindow), TabbedWidget(this, mainWindow)
{
	setupUi(this);
	
	setAssociatedProject(project);
	
	ProjectFile* projectFile = associatedProject()->projectFile();
	
	connect(this, SIGNAL(settingUpdated(const QString&, const QString&)),
		projectFile, SLOT(settingUpdated(const QString&, const QString&)));
		
	connect(this, SIGNAL(settingRemoved(const QString&)),
		projectFile, SLOT(settingRemoved(const QString&)));
	reload();
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
	mainWindow()->setTabName(this, QFileInfo(associatedProject()->projectFile()->path()).fileName() + " settings");
}

bool ProjectSettingsTab::close()
{
	associatedProject()->projectFile()->sync();
	return true;
}

void ProjectSettingsTab::refreshSettings()
{
	
}

void ProjectSettingsTab::reload()
{
	QMap<QString, QString> data = associatedProject()->projectFile()->projectSettings();
	
	ui_settingsTable->clearContents();
	ui_settingsTable->setRowCount(data.keys().size());
	quint16 i = 0;
	foreach(const QString& key, data.keys()) {
		ui_settingsTable->setItem(i, 0, new QTableWidgetItem(key));
		ui_settingsTable->setItem(i, 1, new QTableWidgetItem(data[key]));
		
		++i;
	}
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
	int row = ui_settingsTable->currentRow();
	QTableWidgetItem* key = ui_settingsTable->item(row, 0);
	QString keyString = key ? key->text() : "";
	ui_settingsTable->removeRow(row);
	emit settingRemoved(keyString);
}