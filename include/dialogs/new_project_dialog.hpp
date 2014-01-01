#ifndef _NEW_PROJECT_DIALOG_HPP_
#define _NEW_PROJECT_DIALOG_HPP_

#include <QDialog>

namespace Ui
{
  class NewProjectDialog;
}

namespace kiss
{
	namespace dialog
	{
		class NewProjectDialog : public QDialog
		{
		Q_OBJECT
		public:
			NewProjectDialog(QWidget *parent = 0);
      
      void limitTypeButtons();

			QString projectName() const;
			QString saveLocation() const;
      bool emptyProject() const;
      bool newFile() const;
      bool existingFile() const;

		public slots:
			void on_browse_clicked();
			void on_projectName_textChanged(const QString &text);
			
		private:
			void updateSaveLocation();

			QString m_savePath;
      Ui::NewProjectDialog *ui;
		};
	}
}


#endif
