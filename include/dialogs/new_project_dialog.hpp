#ifndef _NEW_PROJECT_DIALOG_HPP_
#define _NEW_PROJECT_DIALOG_HPP_

#include <QDialog>

#include "ui_NewProjectDialog.h"

namespace Kiss
{
	namespace Dialog
	{
		class NewProjectDialog : public QDialog, private Ui::NewProjectDialog
		{
		Q_OBJECT
		public:
			NewProjectDialog(QWidget *parent = 0);

			QString projectName() const;
			QString saveLocation() const;

		public slots:
			void on_ui_browse_clicked();
			void on_ui_projectName_textChanged(const QString &text);
			
		private:
			void updateSaveLocation();

			QString m_savePath;
		};
	}
}


#endif
