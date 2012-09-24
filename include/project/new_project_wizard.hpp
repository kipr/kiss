#ifndef _NEW_PROJECT_WIZARD_HPP_
#define _NEW_PROJECT_WIZARD_HPP_

#include <QDialog>
#include <QList>

#include "ui_NewProjectWizard.h"

namespace Kiss
{
	namespace Wizard
	{
		class NewProject : public QDialog, private Ui::NewProjectWizard
		{
		Q_OBJECT
		public:
			NewProject(QWidget* parent = 0);

			QString projectName() const;
			QString saveLocation() const;

		public slots:
			void on_ui_browse_clicked();
			void on_ui_projectName_textChanged(const QString& text);
			
		private:
			void updateSaveLocation();

			QString m_savePath;
		};
	}
}


#endif
