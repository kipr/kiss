#ifndef _LANGUAGE_HELPER_PLUGIN_MANAGER_HPP_
#define _LANGUAGE_HELPER_PLUGIN_MANAGER_HPP_

#include "plugin_manager.hpp"
#include "language_helper.hpp"

namespace Kiss
{
	class LanguageHelperPluginManager : public Plugin::Manager<LanguageHelperPluginManager, LanguageHelper>
	{
		virtual QString getExpectedLocation(const QString& name) const;
		virtual void pluginLoaded(LanguageHelper *plugin);
		virtual void pluginUnloaded(LanguageHelper *plugin);
	};
}

#endif
