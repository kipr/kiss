#ifndef _SOURCEFILEMENU_H_
#define _SOURCEFILEMENU_H_

#include "Activatable.h"
#include "MenuManager.h"
#include "SourceFile.h"
#include "ConcreteMenuable.h"

class MainWindow;

class SourceFileMenu : public ConcreteMenuable, public ActivatableObject
{
Q_OBJECT
public:
	SourceFileMenu(MainWindow* mainWindow);
	
	MenuNode* breakpointToggle() const;
	
	static QString menuName();
	
protected:
	void activated();
	void deactivated();
	
protected slots:
	virtual void triggered();
	
private:
	MenuNode* breakpoint;
	
	MenuNode* undo;
	MenuNode* redo;
};

#endif
