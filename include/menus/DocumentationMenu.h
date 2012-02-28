#ifndef _DOCUMENTATIONMENU_H_
#define _DOCUMENTATIONMENU_H_

#include <QObject>
#include "MenuManager.h"
#include "ConcreteMenuable.h"

class MainWindow;

class DocumentationMenu : public ConcreteMenuable
{
Q_OBJECT
public:
	DocumentationMenu(MainWindow* mainWindow);
	
	static QString menuName();
protected slots:
	virtual void triggered();
private:
	MainWindow* m_mainWindow;
};

#endif
