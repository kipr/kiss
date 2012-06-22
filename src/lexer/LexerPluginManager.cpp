#include "LexerPluginManager.h"

QString LexerPluginManager::getExpectedLocation(const QString& name) const
{
	return "lexers";
}

void LexerPluginManager::pluginLoaded(Lexer::Constructor* plugin)
{
	Lexer::Factory::ref().registerLexerConstructor(plugin);
}

void LexerPluginManager::pluginUnloaded(Lexer::Constructor* plugin)
{
	Lexer::Factory::ref().unregisterLexerConstructor(plugin);
}