#ifndef _WEBTABMENU_H_
#define _WEBTABMENU_H_

#include "Activatable.h"
#include "Singleton.h"
#include "MenuManager.h"
#include "WebTab.h"
#include "ConcreteMenuable.h"

class WebTabMenu : public ConcreteMenuable, public ActivatableObject
{
Q_OBJECT
public:
	WebTabMenu();
	
	static QString menuName();
	
protected:
	void activated();
	void deactivated();
	
protected slots:
	virtual void triggered();
	
private:	
	QAction* copy;
	QAction* cut;
	QAction* paste;
	
	QAction* back;
	QAction* forward;
	QAction* reload;
	QAction* open;
};

#endif
