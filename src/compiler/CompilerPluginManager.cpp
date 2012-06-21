#include "CompilerPluginManager.h"
#include "CompilerManager.h"

QString CompilerPluginManager::getExpectedLocation(const QString& name) const
{
	return "compilers";
}

void CompilerPluginManager::pluginLoaded(Compiler* plugin)
{
	CompilerManager::ref().addCompiler(plugin);
}

void CompilerPluginManager::pluginUnloaded(Compiler* plugin)
{
	CompilerManager::ref().removeCompiler(plugin);
}