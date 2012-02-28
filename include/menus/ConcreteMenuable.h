#ifndef _CONCRETEMENUABLE_H_
#define _CONCRETEMENUABLE_H_

#include "Activatable.h"
#include "MenuManager.h"

struct Invokable
{
	Invokable(ActivatableObject* obj, const char* slot) : obj(obj), slot(slot) {}
	
	ActivatableObject* obj;
	const char* slot;
};

struct ConcreteMenuable : virtual QObject, Menuable
{
Q_OBJECT
public:
	ConcreteMenuable(const QString& name);
	~ConcreteMenuable();
	
	MenuNodeList actionsFile();
	MenuNodeList actionsEdit();
	MenuNodeList actionsHelp();
	MenuNodeList actions();
	MenuNodeList toolbarActions();
	
protected slots:
	virtual void triggered() = 0;
	virtual void activeTriggered();
	virtual void activeToggled(bool state);
	
protected:
	MenuNodeList m_file;
	MenuNodeList m_edit;
	MenuNodeList m_help;
	MenuNodeList m_actions;
	MenuNodeList m_toolbar;
	
	MenuNode* node(QAction* action);
	
	QAction* activeAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);
	QAction* activeCheckedAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);
	QAction* activeAction(const QString& iconRes, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);
	QAction* activeAction(const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot);
	
	QAction* action(const QIcon& icon, const QString& text, const QKeySequence& shortcut = QKeySequence::UnknownKey);
	
	QAction* action(const QString& iconRes, const QString& text, const QKeySequence& shortcut = QKeySequence::UnknownKey);
	
	QAction* action(const QString& text, const QKeySequence& shortcut = QKeySequence::UnknownKey);
	
	// Preps for action deletion
	void finish();
	
private:
	void finishList(const MenuNodeList& nodes);
	
	// Sets don't allow duplicates.
	QSet<QAction*> m_rawActions;
	
	QMap<QAction*, Invokable> m_activatables;
};

#endif
