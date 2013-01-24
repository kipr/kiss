#include "activatable.hpp"
#include <QDebug>
#include <QMetaMethod>

ActivatableObject::ActivatableObject()
	: m_connected(0)
{
}

ActivatableObject::~ActivatableObject()
{

}

void ActivatableObject::activated()
{
	QObject *us = dynamic_cast<QObject *>(this);
	if(!us) return;
	
	QObject *connect = active();
	const QMetaObject* meta = connect->metaObject();
	if(meta->indexOfSignal("updateActivatable()") < 0) {
		qWarning() << "?";
		return;
	}
	qDebug() << "Activated" << us->metaObject()->className() << "on" << meta->className();
	m_connected = connect;
	
	QMetaObject::invokeMethod(us, "update");
	QObject::connect(m_connected, SIGNAL(updateActivatable()), us, SLOT(update()));
}

void ActivatableObject::deactivated()
{
	m_connected = 0;
	QObject *us = dynamic_cast<QObject *>(this);
	if(!us) return;
	if(!m_connected) return;

	const QMetaObject* meta = m_connected->metaObject();
	
	if(meta->indexOfSignal(SIGNAL(updateActivatable())) < 0) {
		qWarning() << "?";
		return;
	}

	QObject::disconnect(m_connected, SIGNAL(updateActivatable()), us, SLOT(update()));
}
