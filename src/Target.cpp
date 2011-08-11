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

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "TargetManager.h"

#define TARGET_SETTINGS QSettings(m_targetFileName, QSettings::IniFormat)
#define EXTENSIONS "extensions"
#define DEFAULT_EXTENSION "default_extension"
#define DELIM "|"
#define NAME "name"
#define MANUALS "Manuals"
#define PORT_DIALOG "port_dialog"
#define C_STYLE_BLOCKS "c_style_blocks"
#define REQUEST_FILE_PATH "request_file_path"

Target::Target(QObject *parent) : QObject(parent), m_got(false), m_plugin(0) {}

// Sets the target file and loads the associated plugin file
bool Target::setTargetFile(const QString& filename)
{
	qWarning() << "Target Path:" << filename;
	m_targetFileName = filename;
	m_targetName = TARGET_SETTINGS.value(NAME).toString();
	m_got = get() != 0;
	if(!m_got) return false;
	get()->setTargetFile(filename);
	return true;
}

QMap<QString, QString> Target::targetManualPaths()
{
	QSettings settings(m_targetFileName, QSettings::IniFormat);
	settings.beginGroup(MANUALS);
	QMap<QString, QString> ret;
	foreach(const QString& str, settings.childKeys()) {
		const QString& f = settings.value(str).toString();
		ret[str] = f.isEmpty() ? "" : (QDir::isAbsolutePath(f) ? f : (QDir::currentPath() + "/" + f));
	}
	return ret;
}

QString Target::requestFilePath() { return TARGET_SETTINGS.value(REQUEST_FILE_PATH).toString(); }
QStringList Target::errorMessages() { return m_got ? get()->getErrorMessages() : QStringList(); }
QStringList Target::warningMessages() { return m_got ? get()->getWarningMessages() : QStringList(); }
QStringList Target::linkerMessages() { return m_got ? get()->getLinkerMessages() : QStringList(); }
QStringList Target::verboseMessages() { return m_got ? get()->getVerboseMessages() : QStringList(); }
QList<QAction*> Target::actionList() { return m_got ? get()->getActionList() : QList<QAction*>(); }
QStringList Target::sourceExtensions() { return TARGET_SETTINGS.value(EXTENSIONS).toString().split(DELIM); }
QString Target::defaultExtension() { return TARGET_SETTINGS.value(DEFAULT_EXTENSION).toString(); }
bool Target::cStyleBlocks() { return TARGET_SETTINGS.value(C_STYLE_BLOCKS).toBool(); }
bool Target::hasDownload() { return m_got && get()->hasDownload(); }
bool Target::hasCompile() { return m_got && get()->hasCompile(); }
bool Target::hasRun() { return m_got && get()->hasRun(); }
bool Target::hasStop() { return m_got && get()->hasStop(); }
bool Target::hasSimulate() { return m_got && get()->hasSimulate(); }
bool Target::hasDebug() { return m_got && get()->hasDebug(); }
bool Target::hasUi() { return m_got && get()->hasUi(); }
bool Target::compile(const QString& filename) {	return hasCompile() ? get()->compile(filename, m_port) : false; }
bool Target::download(const QString& filename) { return hasDownload() ? get()->download(filename, m_port) : false; }
bool Target::run(const QString& filename) { return hasRun() ? get()->run(filename, m_port) : false; }
void Target::stop() { if(!hasStop()) return; get()->stop(m_port); }
bool Target::simulate(const QString& filename) { return hasSimulate() ? get()->simulate(filename, m_port) : false; }
bool Target::debugConsole(const QString& filename, const QList<Location>& bkpts) { return hasDebug() ? get()->debugConsole(filename, m_port, bkpts) : false; }
DebuggerInterface* Target::debug(const QString& filename) { return hasDebug() ? get()->debug(filename, m_port) : 0; }
Tab* Target::ui() { return hasUi() ? get()->ui(m_port) : 0; }
bool Target::hasRequestFile() { return m_got && get()->hasFileRequest(); }
QStringList Target::requestDir(const QString& filename) { return hasRequestFile() ? get()->requestDir(filename, m_port) : QStringList(); }
QByteArray Target::requestFile(const QString& filename) { return hasRequestFile() ? get()->requestFile(filename, m_port) : QByteArray(); }
bool Target::error() { return m_got ? get()->error() : true; }
bool Target::hasPort() { return TARGET_SETTINGS.value(PORT_DIALOG).toBool(); }
void Target::setPort(const QString& port) { m_port = port; }
const QString& Target::port() const { return m_port; }

// For cleaner syntax in rest of file
inline TargetInterface* Target::get() const { return TargetManager::ref().get(m_targetName); }
