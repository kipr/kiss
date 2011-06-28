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

#include "LexerSpecManager.h"
#include "LexerSpec.h"
#include "LexerSpecProvider.h"
#include <QSettings>
#include <QDir>
#include <QDebug>

LexerSpecManager& LexerSpecManager::ref()
{
	static LexerSpecManager instance;
	return instance;
}

LexerSpec* LexerSpecManager::lexerSpec(const QString& ext)
{
	return m_lexers.contains(ext) ? qobject_cast<LexerSpecProvider*>(m_lexers[ext]->instance())->lexerSpec() : 0;
}

LexerSpecManager::LexerSpecManager()
{
	loadLexers();
}

LexerSpecManager::~LexerSpecManager()
{
	QList<QPluginLoader*> lexers = m_lexers.values();
	while (lexers.size() > 0) {
		QPluginLoader* loader = lexers.at(0);
		lexers.removeAll(loader);
		delete loader;
	}
}

void LexerSpecManager::loadLexers()
{
	QStringList exts;
#ifdef Q_OS_WIN
	exts << "*.dll";
#elif defined(Q_OS_MAC)
	exts << "*.dylib";
#else
	exts << ".so";
#endif
	QStringList lexers = QDir("lexers").entryList(exts);
	qWarning() << "Lexers:" << lexers;
	foreach(const QString& str, lexers) {
		qWarning() << str;
		loadLexer(str);
	}
	qWarning() << m_lexers;
}

/* These last two load/unload a plugin, determining the file name based on the target name */
void LexerSpecManager::loadLexer(const QString& lexer)
{	
	// Create the QPluginLoader and start constructing the file name
	QPluginLoader* plugin = new QPluginLoader();

	QDir pluginPath(QDir::currentPath()  + "/lexers");
	QString pluginPathString;

	pluginPathString = pluginPath.absoluteFilePath(lexer);

	// Attempts to load the plugin
	plugin->setFileName(pluginPathString);
	if(!plugin->load()) {
		qWarning("LexerSpecManager::loadLexer: %s", qPrintable(plugin->errorString()));
		return;
	}
	
	// The plugin was loaded, attempt to cast it
	if(!qobject_cast<LexerSpecProvider*>(plugin->instance())) {
		plugin->unload();
		delete plugin;
		qWarning("LexerSpecManager::loadLexer: Plugin for lexer \"%s\" failed qobject_cast", qPrintable(lexer));
		return;
	}
	
	qWarning() << "Loaded" << lexer;
	
	LexerSpecProvider* provider = qobject_cast<LexerSpecProvider*>(plugin->instance());
	provider->init();
	
	foreach(const QString& str, provider->extension().split(" ")) {
		m_lexers[str] = plugin;
	}
}