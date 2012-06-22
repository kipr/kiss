#ifndef _LEXERPLUGINMANAGER_H_
#define _LEXEREPLUGINMANAGER_H_

#include "PluginManager.h"
#include "LexerFactory.h"

class LexerPluginManager : public PluginManager<LexerPluginManager, Lexer::Constructor>
{
	virtual QString getExpectedLocation(const QString& name) const;
	virtual void pluginLoaded(Lexer::Constructor* plugin);
	virtual void pluginUnloaded(Lexer::Constructor* plugin);
};

#endif
