#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "Kiss.h"
#include "Singleton.h"

#include <QMap>
#include <QPluginLoader>
#include <QString>
#include <QDir>
#include <QDebug>

template<typename M, typename T>
class PluginManager : public Singleton<M>
{
public:
	T* get(const QString& name) {	
		if(!m_plugins.contains(name)) 
			if(!loadPlugin(name)) return 0;
		return qobject_cast<T*>(m_plugins[name]->instance());
	}

	void unloadAll() {
		QMap<QString, QPluginLoader*>::iterator i = m_plugins.begin();
		while (i != m_plugins.end()) {
			unloadPlugin(i.key());
			i = m_plugins.erase(i);
		}
	}
	
	bool loadPlugin(const QString& name) {
		if(m_plugins.contains(name)) return false;

		QPluginLoader* plugin = new QPluginLoader();
		QDir pluginPath(QDir::currentPath());
		pluginPath.cd(getExpectedLocation(name));
		

		const QString& pluginPathString = pluginPath.absoluteFilePath("lib" + name + "_plugin." + OS_LIB_EXT);
		qWarning() << "Path:" << pluginPathString;

		plugin->setFileName(pluginPathString);
		if(!plugin->load()) {
			qWarning("PluginManager::loadPlugin: %s", qPrintable(plugin->errorString()));
			delete plugin;
			return false;
		}

		T* instance = qobject_cast<T*>(plugin->instance());
		if(!instance) {
			plugin->unload();
			delete plugin;
			qWarning("PluginManager::loadPlugin: Plugin \"%s\" failed qobject_cast", qPrintable(name));
			return false;
		}

		m_plugins[name] = plugin;
		
		qWarning() << "Calling pluginLoaded";
		pluginLoaded(instance);
		return true;
	}

	void unloadPlugin(const QString& name) {
		if(!m_plugins.contains(name)) return;
		pluginUnloaded(get(name));
		QPluginLoader* plugin = m_plugins.take(name);
		if(plugin) {
			plugin->unload();
			delete plugin;
		}
	}
	
protected:
	virtual QString getExpectedLocation(const QString& name) const = 0;
	virtual void pluginLoaded(T* plugin) = 0;
	virtual void pluginUnloaded(T* plugin) = 0;
	
private:
	QMap<QString, QPluginLoader*> m_plugins;
};

#endif 
