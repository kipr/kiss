#ifndef _OHAIYO_H_
#define _OHAIYO_H_

#include <string>
#include <list>
#include <map>

#include "Socket.h"

class Ohaiyo;

class OhaiyoListener
{
public:
	virtual ~OhaiyoListener() {}
	virtual void discoveryStarted(Ohaiyo *ohaiyo) = 0;
	virtual void deviceDiscovered(Ohaiyo *ohaiyo, const Address& address) = 0;
	virtual void discoveryEnded(Ohaiyo *ohaiyo) = 0;
private:
	
};

class Ohaiyo
{
public:
	Ohaiyo(const std::string& deviceType);
	
	void addListener(OhaiyoListener *listener);
	void removeListener(OhaiyoListener *listener);
	
	bool discover(const time_t& timeout = 5);
	bool update();
private:
	
	void notifyDiscoveryStarted();
	void notifyDeviceDiscovered(const Address& address);
	void notifyDiscoveryEnded();
	
	std::list<OhaiyoListener *> m_listeners;
	
	Socket m_regular;
	Socket m_multicast;
	
	std::string m_deviceType;
};

#endif
