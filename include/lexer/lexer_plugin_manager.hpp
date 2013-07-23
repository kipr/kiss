#ifndef _LEXERPLUGINMANAGER_H_
#define _LEXEREPLUGINMANAGER_H_

#include "plugin_manager.hpp"
#include "lexer_factory.hpp"

namespace kiss
{
	namespace lexer
	{
		class PluginManager : public plugin::Manager<lexer::PluginManager, Constructor>
		{
			virtual QString getExpectedLocation(const QString &name) const;
			virtual void pluginLoaded(Constructor *plugin);
			virtual void pluginUnloaded(Constructor *plugin);
		};
	}
}

#endif
