#ifndef PROJECT_SETTINGS_DIALOG_HPP
#define PROJECT_SETTINGS_DIALOG_HPP

#include <QDialog>
#include <QPushButton>

#include "ui_ProjectSettingsDialog.h"
#include "project.hpp"

namespace kiss
{
	namespace dialog
	{
		class ProjectSettingsDialog : public QDialog, private Ui::ProjectSettingsDialog
		{
			Q_OBJECT
			
		public:
			ProjectSettingsDialog(project::ProjectPtr project, QWidget *parent = 0);
			~ProjectSettingsDialog();

			QStringList depNames();
			QStringList depPaths();
			Compiler::Options compilerSettings();

		private slots:
			void on_ui_buttonAddDep_clicked();
			void on_ui_buttonRemoveDep_clicked();
			void on_ui_buttonAddSetting_clicked();
			void on_ui_buttonRemoveSetting_clicked();

		private:
			void addDep(const QString &fullPath);

			QPushButton *ui_buttonAdd;
			QPushButton *ui_buttonRemove;
		};
	}
}

#endif // PROJECT_SETTINGS_DIALOG_H
