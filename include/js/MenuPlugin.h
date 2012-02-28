#ifndef _MENUPLUGIN_H_
#define _MENUPLUGIN_H_

#include <QString>
#include <QStringList>
#include <QScriptValue>
#include <QScriptable>

#include "MenuManager.h"

class MenuPlugin : public QObject, public QScriptable//, public Menuable
{
Q_OBJECT
public:
	MenuPlugin(const QString& name);
	
public slots:
	void addAction(const QScriptValue& obj);
	
private:
	QMap<QString, MenuNodeList> m_nodes;
	
};

#endif
