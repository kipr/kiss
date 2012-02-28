#include "ConcreteMenuable.h"

#include <QDebug>

ConcreteMenuable::ConcreteMenuable(const QString& name) : Menuable(name) {}
ConcreteMenuable::~ConcreteMenuable() {
	foreach(QAction* action, m_rawActions) delete action;
}

MenuNodeList ConcreteMenuable::actionsFile() { return m_file; }
MenuNodeList ConcreteMenuable::actionsEdit() { return m_edit; }
MenuNodeList ConcreteMenuable::actionsHelp() { return m_help; }
MenuNodeList ConcreteMenuable::actions() { return m_actions; }
MenuNodeList ConcreteMenuable::toolbarActions() { return m_toolbar; }

void ConcreteMenuable::activeTriggered() {
	QAction* _ = (QAction*)sender();
	QMap<QAction*, Invokable>::iterator it = m_activatables.find(_);
	if(it == m_activatables.end()) return;
	if(!QMetaObject::invokeMethod(it->obj->active(), it->slot, Qt::DirectConnection))
		qWarning() << "Failed to call" << it->slot << "on" << it->obj->active();

}

void ConcreteMenuable::activeToggled(bool state) {
	QAction* _ = (QAction*)sender();
	QMap<QAction*, Invokable>::iterator it = m_activatables.find(_);
	if(it == m_activatables.end()) return;

	qWarning() << "toggle";
	if(!QMetaObject::invokeMethod(it->obj->active(), it->slot, Qt::DirectConnection, Q_ARG(bool, state)))
		qWarning() << "Failed to call" << it->slot << "on" << it->obj->active();

}

MenuNode* ConcreteMenuable::node(QAction* action) {
	return new MenuNode(this, action);
}

QAction* ConcreteMenuable::activeAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot) {
	QAction* ret = new QAction(icon, text, 0);
	ret->setShortcut(shortcut);
	QObject::connect(ret, SIGNAL(triggered()), this, SLOT(activeTriggered()));
	m_activatables.insert(ret, Invokable(activatable, slot));
	return ret;
}

QAction* ConcreteMenuable::activeCheckedAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot) {
	QAction* ret = new QAction(icon, text, 0);
	ret->setCheckable(true);
	ret->setShortcut(shortcut);
	QObject::connect(ret, SIGNAL(triggered(bool)), this, SLOT(activeToggled(bool)));
	m_activatables.insert(ret, Invokable(activatable, slot));
	return ret;
}

QAction* ConcreteMenuable::activeAction(const QString& iconRes, const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot) {
	return activeAction(ResourceHelper::ref().icon(iconRes), text, shortcut, activatable, slot);
}

QAction* ConcreteMenuable::activeAction(const QString& text, const QKeySequence& shortcut, ActivatableObject* activatable, const char* slot) {
	return activeAction("", text, shortcut, activatable, slot);
}

QAction* ConcreteMenuable::action(const QIcon& icon, const QString& text, const QKeySequence& shortcut) {
	QAction* ret = new QAction(icon, text, 0);
	ret->setShortcut(shortcut);
	QObject::connect(ret, SIGNAL(triggered()), this, SLOT(triggered()));
	return ret;
}

QAction* ConcreteMenuable::action(const QString& iconRes, const QString& text, const QKeySequence& shortcut) {
	return action(ResourceHelper::ref().icon(iconRes), text, shortcut);
}

QAction* ConcreteMenuable::action(const QString& text, const QKeySequence& shortcut) {
	return action("", text, shortcut);
}

// Preps for action deletion
void ConcreteMenuable::finish() {
	finishList(m_file);
	finishList(m_edit);
	finishList(m_help);
	finishList(m_actions);
	finishList(m_toolbar);
}

void ConcreteMenuable::finishList(const MenuNodeList& nodes) {
	foreach(MenuNode* node, nodes) m_rawActions += QSet<QAction*>::fromList(node->allActions());
}
