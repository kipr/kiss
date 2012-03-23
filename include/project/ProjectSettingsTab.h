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
	ProjectSettingsTab(MainWindow* mainWindow);
	
	void setProjectFile(ProjectFile* file);
	ProjectFile* projectFile() const;
	
	virtual void activate();
	
	virtual bool beginSetup();
	virtual void completeSetup();
	
	virtual bool close();
	
	virtual void refreshSettings();
	
private slots:
	void on_ui_settingsTable_cellChanged(int row, int column);
	void on_ui_add_clicked();
	void on_ui_remove_clicked();
	
signals:
	void settingUpdated(const QString& key, const QString& value);
private:
	ProjectFile* m_projectFile;
};

#endif
