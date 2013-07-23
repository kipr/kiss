#include "lexer_plugin_manager.hpp"

using namespace kiss::lexer;

QString PluginManager::getExpectedLocation(const QString &name) const
{
	return "lexers";
}

void PluginManager::pluginLoaded(Constructor *plugin)
{
	lexer::Factory::ref().registerLexerConstructor(plugin);
}

void PluginManager::pluginUnloaded(Constructor *plugin)
{
	lexer::Factory::ref().unregisterLexerConstructor(plugin);
}