#ifndef _SERIALINTERFACE_H_
#define _SERIALINTERFACE_H_

#include <QtPlugin>
#include "Interface.h"

class SerialInterface : public QObject, public Interface
{
Q_OBJECT
Q_INTERFACES(Interface)
public:
	SerialInterface();
	~SerialInterface();
	
	virtual const bool scan(InterfaceResponder *responder);
	virtual void invalidateResponder();
	
private:
	InterfaceResponder *m_responder;
};

#endif
