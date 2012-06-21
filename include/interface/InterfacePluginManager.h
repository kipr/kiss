#ifndef _INTERFACEPLUGINMANAGER_H_
#define _INTERFACEPLUGINMANAGER_H_

#include "PluginManager.h"
#include "Interface.h"

class InterfacePluginManager : public PluginManager<InterfacePluginManager, Interface>
{
	virtual QString getExpectedLocation(const QString& name) const;
	virtual void pluginLoaded(Interface* plugin);
	virtual void pluginUnloaded(Interface* plugin);
};

#endif
