#ifndef _INTERFACEMANAGER_H_
#define _INTERFACEMANAGER_H_

#include "Singleton.h"

#include <QObject>
#include <QList>

class Interface;

class InterfaceManager : public QObject, public Singleton<InterfaceManager>
{
Q_OBJECT
public:
	InterfaceManager();
	~InterfaceManager();
	
	void addInterface(Interface* interface);
	void removeInterface(Interface* interface);
	
	const QList<Interface*>& interfaces() const;
	Interface* interface(const QString& name);
	
signals:
	void interfaceAdded(Interface* interface);
	void interfaceRemoved(Interface* interface);

private:
	QList<Interface*> m_interfaces;
};

#endif
