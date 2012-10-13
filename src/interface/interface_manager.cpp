#include "interface_manager.hpp"

#include "interface.hpp"

using namespace Kiss::Target;

InterfaceManager::InterfaceManager()
{
	
}

InterfaceManager::~InterfaceManager()
{
	// TODO: This is a memory leak for non-plugin interfaces.
	// qDeleteAll(m_interfaces);
}

void InterfaceManager::addInterface(Interface* interface)
{
	m_interfaces.push_back(interface);
}

void InterfaceManager::removeInterface(Interface* interface)
{
	m_interfaces.removeAll(interface);
}

const QList<Interface *>& InterfaceManager::interfaces() const
{
	return m_interfaces;
}

Interface *InterfaceManager::interface(const QString& name)
{
	foreach(Interface *interface, m_interfaces) {
		if(interface->name() == name) return interface;
	}
	
	return 0;
}