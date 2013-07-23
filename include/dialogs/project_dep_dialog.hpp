#ifndef PROJECT_DEP_DIALOG_HPP
#define PROJECT_DEP_DIALOG_HPP

#include <QDialog>
#include <QPushButton>

#include "ui_ProjectDepDialog.h"

namespace kiss
{
	namespace dialog
	{
		class ProjectDepDialog : public QDialog, private Ui::ProjectDepDialog
		{
			Q_OBJECT
			
		public:
			ProjectDepDialog(const QStringList &deps, QWidget *parent = 0);
			~ProjectDepDialog();

			QStringList names();
			QStringList paths();

		private slots:
			void add();
			void remove();

		private:
			void add(const QString &fullPath);

			QPushButton *ui_buttonAdd;
			QPushButton *ui_buttonRemove;
		};
	}
}

#endif // PROJECT_DEP_DIALOG_H
