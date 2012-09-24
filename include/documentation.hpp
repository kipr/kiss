#ifndef _DOCUMENTATION_H_
#define _DOCUMENTATION_H_

#include "build_options.hpp"

#include <QObject>

namespace Kiss
{
	namespace Widget
	{
		class Documentation;
	}
}

#ifdef BUILD_DOCUMENTATION_TAB

#include "tab.hpp"
#include <QListWidgetItem>

#define UI_EVENT_OPEN_MANUAL "openManual"

namespace Kiss
{
	namespace Widget
	{
		class MainWindow;

		class Documentation : public QObject, public Tab
		{
		Q_OBJECT
		public:
			Documentation(MainWindow *parent);

			void activate();
			bool beginSetup();
			void completeSetup();
			bool close();
			void refreshSettings();
		private slots:
			void itemDoubleClicked(QListWidgetItem* item);
		};
	}
}


#endif

#endif
