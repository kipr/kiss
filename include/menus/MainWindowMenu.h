#ifndef _MAINWINDOWMENU_H_
#define _MAINWINDOWMENU_H_

#include "Singleton.h"
#include "Activatable.h"
#include "MenuManager.h"
#include "SourceFile.h"
#include "ConcreteMenuable.h"

class MainWindowMenu : public ConcreteMenuable, public ActivatableObject
{
Q_OBJECT
public:
	MainWindowMenu(MainWindow* mainWindow);
	
	MenuNode* fileOperations() const;
	MenuNode* nextNode() const;
	MenuNode* prevNode() const;
	MenuNode* closeNode() const;
	
	static QString menuName();
	
protected slots:
	virtual void triggered();
	
private:
	QAction* newFile;
	QAction* open;
	QAction* next;
	QAction* prev;
	QAction* close;
	
	QAction* quit;
	
	QAction* repo;
	QAction* install;
	
	MenuNode* m_nextNode;
	MenuNode* m_prevNode;
	MenuNode* m_closeNode;
	
	MenuNode* m_fileOperations;
	
	MainWindow* m_mainWindow;
};

#endif
