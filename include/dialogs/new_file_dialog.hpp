#ifndef _NEW_FILE_DIALOG_HPP_
#define _NEW_FILE_DIALOG_HPP_

#include "template_file.hpp"

#include <QDialog>
#include <QWidget>
#include <QItemSelection>

namespace Ui
{
	class NewFileDialog;
}

namespace kiss
{
	namespace templates
	{
		class Manager;
		class Model;
	}
	
	namespace dialog
	{
		class NewFile : public QDialog
		{
		Q_OBJECT
		public:
			NewFile(kiss::templates::Manager *manager, QWidget *parent = 0);
			~NewFile();
			
      QString fileName() const;
			kiss::templates::File templateFile() const;
			
		private slots:
			void selectionChanged(const QItemSelection &selection);
			void removeSelectedPack();
      void updateAcceptable();
			
		private:
			kiss::templates::Manager *m_manager;
			kiss::templates::Model *m_model;
			Ui::NewFileDialog *ui;
			
			QString m_helpText;
		};
	}
}

#endif
