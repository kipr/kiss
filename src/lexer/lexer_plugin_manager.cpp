#include "lexer_plugin_manager.hpp"

using namespace Kiss::Lexer;

QString PluginManager::getExpectedLocation(const QString& name) const
{
	return "lexers";
}

void PluginManager::pluginLoaded(Constructor *plugin)
{
	Lexer::Factory::ref().registerLexerConstructor(plugin);
}

void PluginManager::pluginUnloaded(Constructor *plugin)
{
	Lexer::Factory::ref().unregisterLexerConstructor(plugin);
}