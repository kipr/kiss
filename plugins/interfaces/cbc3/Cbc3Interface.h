#ifndef _CBC3INTERFACE_H_
#define _CBC3INTERFACE_H_

#include <QtPlugin>
#include "Interface.h"

class Ohaiyo;
class OhaiyoListener;

class Cbc3Interface : public QObject, public Interface
{
Q_OBJECT
Q_INTERFACES(Interface)
public:
	Cbc3Interface();
	~Cbc3Interface();
	
	virtual QList<DevicePtr> devices();
	virtual const bool local();
private:
	Ohaiyo *m_ohaiyo;
	OhaiyoListener *m_listener;
};

#endif
