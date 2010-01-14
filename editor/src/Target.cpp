/**************************************************************************
 *  Copyright 2007,2008,2009 KISS Institute for Practical Robotics        *
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

#include "Target.h"
#include "TargetInterface.h"

#include <QtGlobal>
#include <QDir>
#include <QApplication>
#include <QStringListIterator>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

// Constructor & Destructor
Target::Target(QObject *parent) : QObject(parent), m_targetInterface(0), m_plugin(0)
{
}

Target::~Target()
{
	unloadPlugin();
}

// Sets the target file and loads the associated plugin file
bool Target::setTargetFile(QString filename)
{
	QString pluginName;

	/* Load the settings file */
	QSettings settings(filename, QSettings::IniFormat);

	unloadPlugin();

	/* Determine the plugin's name */
	if(settings.value("base_target").toString().length() > 0)
		pluginName = settings.value("base_target").toString();
	else
		pluginName = settings.value("name").toString();

	/* Load the plugin */
	if(!loadPlugin(pluginName))
		return false;

	/* Pass the file name of the target file to our plugin */
	m_targetInterface->setTargetFile(filename);
	m_targetFileName = filename;

	return true;
}

/* Begin GET methods */

QStringList Target::getErrorMessages()
{
	if(m_targetInterface)
		return m_targetInterface->getErrorMessages();
	else {
		qWarning("Target::getErrorList: Invalid m_targetInterface");
		return QStringList();
	}
}

QStringList Target::getWarningMessages()
{
	if(m_targetInterface)
		return m_targetInterface->getWarningMessages();
	else {
		qWarning("Target::getWarningList: Invalid m_targetInterface");
		return QStringList();
	}
}

QStringList Target::getLinkerMessages()
{
	if(m_targetInterface)
		return m_targetInterface->getLinkerMessages();
	else {
		qWarning("Target::getLinkErrorList: Invalid m_targetInterface");
		return QStringList();
	}
}

QStringList Target::getVerboseMessages()
{
	if(m_targetInterface)
		return m_targetInterface->getVerboseMessages();

	qWarning("Target::getVerboseMessages() Invalid m_targetInterface");
	return QStringList();
}

LexerSpec *Target::getLexerSpec()
{
	if(m_targetInterface)
		return &m_targetInterface->getLexerSpec();

	qWarning("Target::getLexerSpec() Invalid m_targetInterface");
	return 0;
}

QList<QAction*> Target::getActionList()
{
	if(m_targetInterface)
		return m_targetInterface->getActionList();
	
	qWarning("Target::getActionList() Invalid m_targetInterface");
	return QList<QAction*>();
}

QString Target::getSourceExtensions()
{
	QSettings settings(m_targetFileName, QSettings::IniFormat);
	QString extensions = settings.value("extensions").toString();
	
	return extensions.split(",").join("\n") + "\n";
}

QString Target::getSourceTemplate()
{
	QSettings settings(m_targetFileName, QSettings::IniFormat);
	QString filePath = settings.value("template").toString();
	
	if(!QDir::isAbsolutePath(filePath))
		filePath = QDir::currentPath() + "/" + filePath;
	
	QFile file(filePath);
	
	if(!file.open(QIODevice::ReadOnly))
		return QString();
	QTextStream fileStream(&file);
	QString text = fileStream.readAll(); 
	return text;
}

QString Target::getTargetManualPath()
{
	QSettings settings(m_targetFileName, QSettings::IniFormat);
	QString filePath = settings.value("manual").toString();
	
	if(filePath == "")
		return QString();
	
	if(!QDir::isAbsolutePath(filePath))
		filePath = QDir::currentPath() + "/" + filePath;
	
	return filePath;
}

/* End GET Methods */

/* Begin HAS Methods */

bool Target::hasDownload()
{
	if(m_targetInterface && m_targetInterface->hasDownload())
		return true;
	return false;
}

bool Target::hasCompile()
{
	if(m_targetInterface && m_targetInterface->hasCompile()) {
		return true;
	}
	return false;
}

bool Target::hasRun()
{
	if(m_targetInterface && m_targetInterface->hasRun())
		return true;
	return false;
}

bool Target::hasStop()
{
	if(m_targetInterface && m_targetInterface->hasStop())
		return true;
	return false;
}

bool Target::hasSimulate()
{
  if(m_targetInterface && m_targetInterface->hasSimulate())
    return true;
  return false;
}

/* End HAS Methods */

/* Begin action Methods */

bool Target::compile(QString filename)
{
	if(!hasCompile())
		return false;
	return m_targetInterface->compile(filename);
}

bool Target::download(QString filename)
{
	if(!hasDownload())
		return false;
	return m_targetInterface->download(filename);
}

bool Target::run(QString filename)
{
	if(!hasRun())
		return false;
	return m_targetInterface->run(filename);
}

void Target::stop()
{
	if(!hasStop())
		return;
	m_targetInterface->stop();
}

bool Target::simulate(QString filename)
{
  if(!hasSimulate())
    return false;
  return m_targetInterface->simulate(filename);
}

/* End action Methods */

/* Serial Port set/get routines */
void Target::setPort(QString port)
{
	if(m_targetInterface)
		m_targetInterface->setPort(port);
	else	
		qWarning("Target::setPort() invalid target interface");
}

QString Target::getPort()
{
	if(m_targetInterface)
		return m_targetInterface->getPort();
	
	qWarning("Target::getPort() invalid target interface");
	return QString();
}

/* End serial port set/get routines */


/* These last two load/unload a plugin, determining the file name based on the target name */
bool Target::loadPlugin(QString targetName)
{
	// Create the QPluginLoader and start constructing the file name
	m_plugin = new QPluginLoader();

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
	m_plugin->setFileName(pluginPathString);
	if(!m_plugin->load()) {
		qWarning("Target::loadPlugin: %s", qPrintable(m_plugin->errorString()));
		return false;
	}
	
	// The plugin was loaded, attempt to cast it
	m_targetInterface = qobject_cast<TargetInterface *>(m_plugin->instance());
	if(!m_targetInterface) {
		m_plugin->unload();
		delete m_plugin;
		m_plugin = 0;
		qWarning("Target::loadPlugin: Plugin for target \"%s\" failed qobject_cast", qPrintable(targetName));
		return false;
	}
	
	// Check if the plugin is a QObject derivative, if so try to load the requestPort() signal
	QObject *qoPlugin = qobject_cast<QObject *>(m_plugin->instance());
	if(qoPlugin)
		connect(qoPlugin, SIGNAL(requestPort()), this, SIGNAL(requestPort()));
	
	// Everything worked!
	return true;
}

// Simply delete the plugin and unset everything
void Target::unloadPlugin()
{
	delete m_targetInterface;
	m_targetInterface = 0;
	if(m_plugin) {
		m_plugin->unload();
		delete m_plugin;
		m_plugin = 0;
	}
}

void Target::setCurrentFile(SourceFile *sourceFile)
{
    if(m_targetInterface)
        m_targetInterface->setCurrentFile(sourceFile);
    else
        qWarning("Target::setCurrentFile() invalid target interface");
}


