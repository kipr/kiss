#include "Ohaiyo.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define DISCOVER "discover"
#define BUFFSIZE 256

#define OHAIYO_MULTICAST_PORT 8374
#define OHAIYO_PORT 8375
#define OHAIYO_GROUP "225.0.0.37"

Ohaiyo::Ohaiyo(const std::string& deviceType)
	: m_deviceType(deviceType)
{
	m_regular.setReusable(true);
	m_regular.setBlocking(false);
	m_regular.bind(Address((unsigned long)INADDR_ANY, OHAIYO_PORT));
	
	m_multicast.setReusable(true);
	m_multicast.setBlocking(false);
	m_multicast.bind(Address((unsigned long)INADDR_ANY, OHAIYO_MULTICAST_PORT));
	m_multicast.joinMulticastGroup(Address(OHAIYO_GROUP, OHAIYO_MULTICAST_PORT));
}

void Ohaiyo::addListener(OhaiyoListener *listener)
{
	m_listeners.push_back(listener);
}

void Ohaiyo::removeListener(OhaiyoListener *listener)
{
	std::list<OhaiyoListener *>::iterator it = m_listeners.begin();
	for(; it != m_listeners.end(); ++it) if(*it == listener) break;
	if(it == m_listeners.end()) return;
	m_listeners.erase(it);
}

bool Ohaiyo::discover(const time_t& timeout)
{
	notifyDiscoveryStarted();
	
	printf("Sending out request for peers...\n");
	m_multicast.sendto(m_deviceType.c_str(), m_deviceType.size() + 1, Address(OHAIYO_GROUP, OHAIYO_MULTICAST_PORT));
	
	time_t seconds = time(NULL);
	char *buffer = 0;
	size_t size = 0;
	Address from;
	while(time(NULL) - seconds <= timeout) {
		if(m_multicast.recvfrom(buffer, size, from) > 0) {
			printf("discover: peer replied %s\n", inet_ntoa(from.address().sin_addr));
			notifyDeviceDiscovered(from);
			free(buffer);
		}
		update();
	}
	
	notifyDiscoveryEnded();
	
	return true;
}

bool Ohaiyo::update()
{
	char *buffer = 0;
	size_t size = 0;
	Address from;
	if(m_multicast.recvfrom(buffer, size, from) > 0) {
		printf("update: sending our data back %s\n", inet_ntoa(from.address().sin_addr));
		m_multicast.sendto(buffer, size, from);
		free(buffer);
	}
	
	return true;
}

void Ohaiyo::notifyDiscoveryStarted()
{
	std::list<OhaiyoListener *>::iterator it = m_listeners.begin();
	for(; it != m_listeners.end(); ++it) {
		(*it)->discoveryStarted(this);
	}
}

void Ohaiyo::notifyDeviceDiscovered(const Address& address)
{
	std::list<OhaiyoListener *>::iterator it = m_listeners.begin();
	for(; it != m_listeners.end(); ++it) {
		(*it)->deviceDiscovered(this, address);
	}
}

void Ohaiyo::notifyDiscoveryEnded()
{
	std::list<OhaiyoListener *>::iterator it = m_listeners.begin();
	for(; it != m_listeners.end(); ++it) {
		(*it)->discoveryEnded(this);
	}
}