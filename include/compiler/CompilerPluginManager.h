#ifndef _COMPILERPLUGINMANAGER_H_
#define _COMPILERPLUGINMANAGER_H_

#include "PluginManager.h"
#include "Compiler.h"

class CompilerPluginManager : public PluginManager<CompilerPluginManager, Compiler>
{
	virtual QString getExpectedLocation(const QString& name) const;
	virtual void pluginLoaded(Compiler* plugin);
	virtual void pluginUnloaded(Compiler* plugin);
};

#endif
