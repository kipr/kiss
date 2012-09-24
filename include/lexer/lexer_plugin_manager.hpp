#ifndef _LEXERPLUGINMANAGER_H_
#define _LEXEREPLUGINMANAGER_H_

#include "plugin_manager.hpp"
#include "lexer_factory.hpp"

namespace Kiss
{
	namespace Lexer
	{
		class PluginManager : public Plugin::Manager<Lexer::PluginManager, Constructor>
		{
			virtual QString getExpectedLocation(const QString& name) const;
			virtual void pluginLoaded(Constructor *plugin);
			virtual void pluginUnloaded(Constructor *plugin);
		};
	}
}

#endif
