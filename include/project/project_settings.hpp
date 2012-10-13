#ifndef _PROJECT_SETTINGS_HPP_
#define _PROJECT_SETTINGS_HPP_

#include "tab.hpp"
#include "project.hpp"

#include "ui_ProjectSettingsTab.h"

#include <QWidget>

namespace Kiss
{
	namespace Widget
	{
		class MainWindow;
		
		class ProjectSettings : public QWidget, public Tab, private Ui::ProjectSettingsTab
		{
		Q_OBJECT
		public:
			ProjectSettings(const Project::ProjectPtr& project, MainWindow *mainWindow);

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
	}
}

#endif
