#ifndef _PROJECTSETTINGSTAB_H_
#define _PROJECTSETTINGSTAB_H_

#include <QWidget>
#include "Tab.h"

struct ProjectFile;

#include "ui_ProjectSettingsTab.h"

class ProjectSettingsTab : public QWidget, public TabbedWidget, private Ui::ProjectSettingsTab
{
Q_OBJECT
public:
	ProjectSettingsTab(Project* project, MainWindow* mainWindow);
	
	virtual void activate();
	
	virtual bool beginSetup();
	virtual void completeSetup();
	
	virtual bool close();
	
	virtual void refreshSettings();
	
public slots:
	void reload();
	
private slots:
	void on_ui_settingsTable_cellChanged(int row, int column);
	void on_ui_add_clicked();
	void on_ui_remove_clicked();
	
signals:
	void settingRemoved(const QString& key);
	void settingUpdated(const QString& key, const QString& value);
	
private:
	bool m_block;

};

#endif
