#include "ProjectSettingsTab.h"

#include "MainWindow.h"
#include "Activatable.h"
#include "Project.h"
#include "Log.h"

#include <QFileInfo>
#include <QDebug>

ProjectSettingsTab::ProjectSettingsTab(Project* project, MainWindow* mainWindow)
	: QWidget(mainWindow), TabbedWidget(this, mainWindow), m_block(false)
{
	setupUi(this);
	
	setAssociatedProject(project);
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
	mainWindow()->setTabName(this, associatedProject()->name() + " Settings");
	connect(associatedProject(), SIGNAL(settingsChanged()), SLOT(reload()));
}

bool ProjectSettingsTab::close()
{
	associatedProject()->sync();
	return true;
}

void ProjectSettingsTab::refreshSettings()
{
	
}

void ProjectSettingsTab::reload()
{
	const QStringMap& settings = associatedProject()->settings();
	
	Log::ref().debug("Reloaded settings tab");
	
	m_block = true;
	
	ui_settingsTable->clearContents();
	ui_settingsTable->setRowCount(settings.keys().size());
	quint16 i = 0;
	foreach(const QString& key, settings.keys()) {
		ui_settingsTable->setItem(i, 0, new QTableWidgetItem(key));
		ui_settingsTable->setItem(i, 1, new QTableWidgetItem(settings[key]));
		
		++i;
	}
	
	m_block = false;
}

void ProjectSettingsTab::on_ui_settingsTable_cellChanged(int row, int column)
{
	if(m_block) return;
	
	QTableWidgetItem* key = ui_settingsTable->item(row, 0);
	QTableWidgetItem* value = ui_settingsTable->item(row, 1);
	
	if(key && value) associatedProject()->updateSetting(key->text(), value->text());
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
	
	associatedProject()->removeSetting(keyString);
}