#ifndef _INTERFACE_PLUGIN_MANAGER_HPP_
#define _INTERFACE_PLUGIN_MANAGER_HPP_

#include "plugin_manager.hpp"
#include "interface.hpp"

namespace Kiss
{
	namespace Target
	{
		class InterfacePluginManager : public Plugin::Manager<InterfacePluginManager, Interface>
		{
			virtual QString getExpectedLocation(const QString& name) const;
			virtual void pluginLoaded(Interface *plugin);
			virtual void pluginUnloaded(Interface *plugin);
		};
	}
}

#endif
