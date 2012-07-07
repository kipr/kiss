#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "Named.h"
#include "InterfaceResponder.h"

#include <QList>
#include <QString>

class Interface : public Named
{
public:
	Interface(const QString& name);
	
	virtual const bool scan(InterfaceResponder *responder) = 0;
	virtual void invalidateResponder() = 0;
};

Q_DECLARE_INTERFACE(Interface, "com.kipr.kiss.Interface/1.1");

#endif
