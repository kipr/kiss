#include "language_helper_plugin_manager.hpp"
#include "language_helper_manager.hpp"

using namespace Kiss;

QString LanguageHelperPluginManager::getExpectedLocation(const QString& name) const
{
	return "language_helpers";
}

void LanguageHelperPluginManager::pluginLoaded(LanguageHelper *plugin)
{
	qDebug() << "Added language helper plugin" << plugin->metaObject()->className();
	LanguageHelperManager::ref().addLanguageHelper(plugin);
}

void LanguageHelperPluginManager::pluginUnloaded(LanguageHelper *plugin)
{
	LanguageHelperManager::ref().removeLanguageHelper(plugin);
}