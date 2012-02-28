#ifndef _DEVELOPERMENU_H_
#define _DEVELOPERMENU_H_

#include "Activatable.h"
#include "MenuManager.h"
#include "SourceFile.h"
#include "ConcreteMenuable.h"

class DeveloperMenu : public ConcreteMenuable
{
Q_OBJECT
public:
	DeveloperMenu();
	
	static QString menuName();
	
protected slots:
	virtual void triggered();
	
private:
	QAction* injectScript;
	QAction* uninstallAll;
};

#endif
