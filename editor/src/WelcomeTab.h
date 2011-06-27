#ifndef __WELCOME_TAB_H__
#define __WELCOME_TAB_H__

#include "WebTab.h"

class WelcomeTab : public WebTab 
{
	Q_OBJECT
public:
	WelcomeTab(QWidget* parent = 0);	
	
	virtual void completeSetup();
	
private slots:
	void linkClicked(const QUrl& url);
};

#endif