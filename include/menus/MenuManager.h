#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

#include "Singleton.h"
#include "ResourceHelper.h"

#include <QList>
#include <QSet>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QObject>

typedef QList<QAction*> QActionList;

struct Menuable;

struct MenuableActionPair
{
	MenuableActionPair(Menuable* menuable, QAction* action) : menuable(menuable), action(action) {}
	
	Menuable* menuable;
	QAction* action;
};

struct MenuNode
{
	MenuNode();
	MenuNode(const QString& name);
	MenuNode(const QString& name, Menuable* menuable, QAction* action);
	MenuNode(Menuable* menuable, QAction* action);
	~MenuNode();
	
	static QList<MenuNode*> unify(QList<MenuNode*> nodes);
	static MenuNode* insertionPoint();
	static MenuNode* separator();
	
	QActionList allActions();
	
	QAction* rawAction;
	
	QString name;
	QList<MenuNode*> children;
	
	QList<MenuableActionPair> registers;
	
	bool _insertionPoint;
	bool activeControl;
	
private:
	static QString uniqueName();
};

typedef QList<MenuNode*> MenuNodeList;
class MenuManager;

class Menuable
{
public:
	Menuable(const QString& name) : m_name(name), m_menuManager(0) {}
	
	virtual MenuNodeList actionsFile() = 0;
	virtual MenuNodeList actionsEdit() = 0;
	virtual MenuNodeList actionsHelp() = 0;
	virtual MenuNodeList actions() = 0;
	virtual MenuNodeList toolbarActions() = 0;
	
	const QString& name() { return m_name; }
	
	MenuManager* menuManager() { return m_menuManager; }
	void setMenuManager(MenuManager* menuManager) { m_menuManager = menuManager; }
	
private:
	QString m_name;
	
	MenuManager* m_menuManager;
};

class MenuManager : public QObject
{
Q_OBJECT
public:
	MenuManager();
	~MenuManager();
	
	void registerMenus(Menuable* menuable);
	void unregisterMenus(Menuable* menuable);
	
	void activate(QList<Menuable*> menus);
	void addActivation(Menuable* menuable);
	void removeActivation(Menuable* menuable);
	
	void construct(QMenuBar* menuBar, QToolBar* toolBar);
	bool isConstructed() const;

	void refreshToolbar();

private slots:
	void triggered();
	
private:
	void addChildren(MenuNode* parent, const MenuNodeList& nodes);
	QList<MenuableActionPair> actionsToPair(Menuable* menuable, const QActionList& actions);
	void unregisterMenus(Menuable* menuable, MenuNode* node);
	void construct(QMenu* menu, MenuNodeList nodes);
	void construct(QToolBar* tool, MenuNodeList nodes);
	void construct(QMenu* menu, MenuNode* node);
	void construct(QToolBar* toolbar, MenuNode* node);
	
	void flattenInsertions(MenuNode* node);
	
	void refresh();
	void refresh(MenuNode* node);
	
	bool isActivatable(MenuNode* node) const;
	
	MenuNode* lookup(QAction* action);
	
	MenuNode* m_root;
	
	MenuNode* m_file;
	MenuNode* m_edit;
	MenuNode* m_help;
	
	MenuNode* m_tool;
	
	QList<Menuable*> m_active;
	
	QMenuBar* m_menuBar;
	QToolBar* m_toolBar;
};

#endif
