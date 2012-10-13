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

namespace Kiss
{
	namespace Template
	{
		class Manager;
		class Model;
	}
	
	namespace Dialog
	{
		class Template : public QDialog
		{
		Q_OBJECT
		public:
			Template(Kiss::Template::Manager *manager, QWidget *parent = 0);
			~Template();
			
			Kiss::Template::File file() const;
			
		private slots:
			void selectionChanged(const QItemSelection& selection);
			void removeSelectedPack();
			
		private:
			Kiss::Template::Manager *m_manager;
			Kiss::Template::Model *m_model;
			Ui::TemplateDialog *ui;
			
			QString m_helpText;
		};
	}
}

#endif
