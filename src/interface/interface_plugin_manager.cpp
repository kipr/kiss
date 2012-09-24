#include "interface_plugin_manager.hpp"
#include "interface_manager.hpp"

using namespace Kiss::Target;

QString InterfacePluginManager::getExpectedLocation(const QString& name) const
{
	return "interfaces";
}

void InterfacePluginManager::pluginLoaded(Interface *plugin)
{
	InterfaceManager::ref().addInterface(plugin);
}

void InterfacePluginManager::pluginUnloaded(Interface *plugin)
{
	InterfaceManager::ref().removeInterface(plugin);
}