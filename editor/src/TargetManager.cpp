/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "TargetManager.h"

#include <QSettings>
#include <QDir>

TargetManager& TargetManager::ref()
{
	static TargetManager manager;
	return manager;
}

TargetInterface* TargetManager::get(QString targetName)
{	
	if(!m_plugins.contains(targetName))
		if(!loadPlugin(targetName)) return 0;
	
	return qobject_cast<TargetInterface *>(m_plugins[targetName]->instance());
}

QStringList TargetManager::getAllSupportedExtensions()
{
	QDir targetDir(QDir::currentPath() + "/targets");
	QStringList targetDirs;
	QStringList extensionList;

	// Choke if we can't find the target directory
	if(!targetDir.exists()) {
		return extensionList;
	}
	
	// Get a list of the possible target directories and check through them
	targetDirs = targetDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	QStringListIterator i(targetDirs);

	while(i.hasNext()) {
		QString dirName = i.next();
		targetDir.cd(dirName);
		
		// The target file naming scheme is <dirname>.target
		QFileInfo targetFile(targetDir, dirName + ".target");

		// If we can't find a target file, skip this directory
		if(!targetFile.exists()) {
			targetDir.cdUp();
			continue;
		}
		
		QSettings target(targetFile.absoluteFilePath(), QSettings::IniFormat);
		extensionList << target.value("extensions").toString().split("|");
		targetDir.cdUp();
	}
	return extensionList;
}

TargetManager::TargetManager()
{
	
}

TargetManager::TargetManager(const TargetManager& that) {}

TargetManager::~TargetManager()
{
	QMap<QString, QPluginLoader*>::iterator i = m_plugins.begin();
	while (i != m_plugins.end()) {
		unloadPlugin(i.key());
		i = m_plugins.erase(i);
	}
}

/* These last two load/unload a plugin, determining the file name based on the target name */
bool TargetManager::loadPlugin(QString targetName)
{
	if(m_plugins.contains(targetName)) unloadPlugin(targetName);
	
	// Create the QPluginLoader and start constructing the file name
	QPluginLoader* plugin = new QPluginLoader();

	QDir pluginPath(QDir::currentPath()  + "/targets");
	QString pluginPathString;

	pluginPath.cd(targetName.toLocal8Bit());

	// These conditionals deal with different file names on different platforms
#ifdef Q_OS_MAC
	pluginPathString = pluginPath.absoluteFilePath("lib" + targetName + "_plugin.dylib");
#elif defined(Q_OS_LINUX)
	pluginPathString = pluginPath.absoluteFilePath("lib" + targetName + "_plugin.so");
#else
	pluginPathString = pluginPath.absoluteFilePath("lib" + targetName + "_plugin.dll");
#endif

	// Attempts to load the plugin
	plugin->setFileName(pluginPathString);
	if(!plugin->load()) {
		qWarning("Target::loadPlugin: %s", qPrintable(plugin->errorString()));
		return false;
	}
	
	// The plugin was loaded, attempt to cast it
	if(!qobject_cast<TargetInterface *>(plugin->instance())) {
		plugin->unload();
		delete plugin;
		qWarning("Target::loadPlugin: Plugin for target \"%s\" failed qobject_cast", qPrintable(targetName));
		return false;
	}
	
	// Everything worked!
	m_plugins[targetName] = plugin;
	return true;
}

// Simply delete the plugin and unset everything
void TargetManager::unloadPlugin(QString targetName)
{
	if(!m_plugins.contains(targetName)) return;
	QPluginLoader* plugin = m_plugins.take(targetName);
	if(plugin) {
		TargetInterface* targetInterface = qobject_cast<TargetInterface *>(plugin->instance());
		plugin->unload();
		delete plugin;
	}
}