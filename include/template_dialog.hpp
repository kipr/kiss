#ifndef _TEMPLATE_DIALOG_HPP_
#define _TEMPLATE_DIALOG_HPP_

#include "template_file.hpp"

#include <QDialog>
#include <QWidget>
#include <QItemSelection>

namespace Ui
{
	class TemplateDialog;
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
		class Template : public QDialog
		{
		Q_OBJECT
		public:
			Template(kiss::templates::Manager *manager, QWidget *parent = 0);
			~Template();
			
			kiss::templates::File file() const;
			
		private slots:
			void selectionChanged(const QItemSelection &selection);
			void removeSelectedPack();
			
		private:
			kiss::templates::Manager *m_manager;
			kiss::templates::Model *m_model;
			Ui::TemplateDialog *ui;
			
			QString m_helpText;
		};
	}
}

#endif
