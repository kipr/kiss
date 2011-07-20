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

#ifndef __TARGET_H__
#define __TARGET_H__

#include "TargetInterface.h"
#include "DebuggerInterface.h"

#include <QPluginLoader>
#include <QObject>
#include <QWidget>
#include <QSettings>

/** 
 * The Target class handles loading the TargetInterface through
 * QPluginLoader.  This class also has methods to check the availability
 * offered by the currently loaded plugin						 
 */
class Target : public QObject
{
Q_OBJECT

public:
	//Constructor & Destructor
	Target(QObject *parent = 0);

	// Sets the current target file (and loads the QPluginLoader object)
	bool setTargetFile(const QString& filename);
	QString targetManualPath();

	// These pass straight through to the TargetInterface if it is loaded
	QStringList 	errorMessages();
	QStringList 	warningMessages();
	QStringList 	linkerMessages();
	QStringList 	verboseMessages();
	QList<QAction*> actionList();
	
	// These two load some settings from the target file
	QStringList sourceExtensions();
	QString defaultExtension();

	// These methods check the availability of certain features
	bool hasDownload();
	bool hasCompile();
	bool hasRun();
	bool hasStop();
	bool hasSimulate();
	bool hasDebug();
	bool hasUi();

	// Again, these pass through to the TargetInterface if loaded
	bool compile(const QString& filename);
	bool download(const QString& filename);
	bool run(const QString& filename);
	void stop();
	bool simulate(const QString& filename);
	DebuggerInterface* debug(const QString& filename);
	Tab* ui();

	// Set/Get the current serial port
	void setPort(const QString& port);
	const QString& port() const;

private:
	TargetInterface* get() const;
	
	QPluginLoader *m_plugin;
	QString m_targetFileName;
	QString m_targetName;
	QString m_port;
};

#endif
