#ifndef _TEMPLATE_TAB_HPP_
#define _TEMPLATE_TAB_HPP_

#include "tab.hpp"
#include "template_pack.hpp"

#include <QItemSelection>

namespace Ui
{
	class TemplateTab;
}

namespace Kiss
{
	namespace Template
	{
		class Manager;
		class Pack;
		class Model;
	}
	
	namespace Widget
	{
		class TemplateTab : public QWidget, public Tab
		{
		Q_OBJECT
		public:
			TemplateTab(const Template::PackPtr& pack, MainWindow *mainWindow);
			TemplateTab(const QString& path, MainWindow *mainWindow);
			~TemplateTab();
			
			virtual void activate();
			virtual bool beginSetup();
			virtual void completeSetup();
			virtual bool close();
			virtual void refreshSettings();
			
			bool saveAs(const QString& file);
			
			bool addFile(const QString& path);
			
		public slots:
			bool save();
			bool saveAs();
			
			void test();
			
			void addFile();
			void deleteSelection();
			
		private slots:
			void selectionChanged(const QItemSelection& selection, const QItemSelection& prev);
			
			void dirtyDesc();
			void dirty();
			
		private:
			void updateTitle();
			void init();
			
			bool m_dirty;
			
			Template::Manager *m_manager;
			Template::PackPtr m_pack;
			Template::Model *m_model;
			
			Ui::TemplateTab *ui;
		};
	}
}

#endif
