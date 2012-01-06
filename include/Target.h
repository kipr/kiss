/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
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
#include <QMap>

/*! \class Target
 * \brief Handles loading the TargetInterface through TargetManager.
 *
 * This class also has methods to check the availability offered by the currently loaded plugin.
 */
class Target : public QObject
{
Q_OBJECT

public:
	//Constructor & Destructor
	Target(QObject *parent = 0);

	// Sets the current target file (and loads the QPluginLoader object)
	bool setTargetFile(const QString& filename);
	QMap<QString, QString> targetManualPaths();
	QString requestFilePath();

	// These pass straight through to the TargetInterface if it is loaded
	QStringList 	errorMessages();
	QStringList 	warningMessages();
	QStringList 	linkerMessages();
	QStringList 	verboseMessages();
	QList<QAction*> actionList();
	
	// These two load some settings from the target file
	QStringList sourceExtensions();
	QString defaultExtension();
	bool cStyleBlocks();

	/*! \return True if the TargetInterface exists and supports downloading */
	bool hasDownload();
	/*! \return True if the TargetInterface exists and supports downloading without check */
	bool hasRawDownload();
	/*! \return True if the TargetInterface exists and supports compiling */
	bool hasCompile();
	/*! \return True if the TargetInterface exists and supports running */
	bool hasRun();
	/*! \return True if the TargetInterface exists and supports stopping */
	bool hasStop();
	/*! \return True if the TargetInterface exists and supports simulating */
	bool hasSimulate();
	/*! \return True if the TargetInterface exists and supports debugging */
	bool hasDebug();
	/*! \return True if the TargetInterface exists and supports a UI at runtime */
	bool hasUi();
	
	/*! Compiles a file
	 * \param filename Absolute path to a file
	 * \return true if the TargetInterface exists and compile was successful
	 */
	bool compile(const QString& filename);
	
	/*! Downloads a file
	 * \param filename Absolute path to a file
	 * \return true if the TargetInterface exists and download was successful
	 */
	int download(const QString& filename);
	
	/*! Downloads a file without checking it's validity
	 * \param filename Absolute path to a file
	 * \return true if the TargetInterface exists and download was successful
	 */
	bool rawDownload(const QString& filename);
	
	/*! Runs a file
	 * \param filename Absolute path to a file
	 * \return true if the TargetInterface exists and run was successful
	 */
	bool run(const QString& filename);
	
	/*! Stops current run. No effect if no file running. */
	void stop();
	
	/*! Simulates a file
	 * \param filename Absolute path to a file
	 * \return true if the TargetInterface exists and simulate was successful
	 */
	bool simulate(const QString& filename);
	
	bool debugConsole(const QString& filename, const QList<Location>& bkpts);
	
	/*! Sets up a debugger for a file
	 * \param filename Absolute path to a file
	 * \return Pointer to DebuggerInterface if the TargetInterface exists and debug setup was successful.
	 */
	DebuggerInterface* debug(const QString& filename);
	Tab* ui();
	
	bool hasScreenGrab();
	QByteArray screenGrab();
	
	bool hasRequestFile();
	QStringList requestDir(const QString&);
	QByteArray requestFile(const QString&);
	
	bool error();
	
	//! \return true if target expects port
	bool hasPort();

	//! \param port Port to associate with Target
	void setPort(const QString& port);
	
	//! \return currently set port
	QString port() const;
	
	QString name() const;

private:
	TargetInterface* get() const;
	bool m_got;
	
	QPluginLoader *m_plugin;
	QString m_targetFileName;
	QString m_targetName;
	QString m_port;
};

#endif
