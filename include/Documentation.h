#ifndef _DOCUMENTATION_H_
#define _DOCUMENTATION_H_

#include "Tab.h"
#include <QListWidgetItem>


#define UI_EVENT_OPEN_MANUAL "openManual"

class MainWindow;

class Documentation : public QObject, public TabbedWidget
{
Q_OBJECT
public:
	Documentation(MainWindow* parent);
	
	void activate();
	bool beginSetup();
	void completeSetup();
	bool close();
	void refreshSettings();
private slots:
	void itemDoubleClicked(QListWidgetItem* item);
};

#endif
