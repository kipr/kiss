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
#include "Kiss.h"

#include <QSettings>
#include <QDir>

#define DISPLAY_NAME "display_name"
#define EXTENSIONS "extensions"

TargetManager::~TargetManager() { unloadAll(); }

QStringList TargetManager::targets()
{
	QDir targetDir(QDir::currentPath() + "/" + TARGET_FOLDER);

	// Choke if we can't find the target directory
	if(!targetDir.exists()) {
		qWarning("ChooseTargetDialog::scanTargetDirectory: Could not find targets directory");
		return QStringList();
	}
	
	QStringList targetList;
	
	// Get a list of the possible target directories and check through them
	const QStringList& targetDirs = targetDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	QStringListIterator i(targetDirs);

	while(i.hasNext()) {
		QString dirName = i.next();
		targetDir.cd(dirName);
		
		// The target file naming scheme is <dirname>.target
		QFileInfo targetFile(targetDir, dirName + "." + TARGET_EXT);

		// If we can't find a target file, skip this directory
		if(!targetFile.exists()) {
			qWarning("ChooseTargetDialog::scanTargetDirectory: Invalid target directory %s", qPrintable(targetDir.absolutePath()));
			targetDir.cdUp();
			continue;
		}
		
		targetList << dirName;
		targetDir.cdUp();
	}

	// Didn't find any targets, quit
	if(targetList.count() == 0) {
		qWarning("ChooseTargetDialog::scanTargetDirectory: No targets to display!\n");
		return QStringList();
	}

	return targetList;
}

QString TargetManager::displayName(const QString& target)
{
	return QSettings(QFileInfo(targetPath(target), target + "." + TARGET_EXT).absoluteFilePath(), 
		QSettings::IniFormat).value(DISPLAY_NAME).toString();
}

QString TargetManager::targetPath(const QString& target) { return QDir::currentPath() + "/" + TARGET_FOLDER + "/" + target; }

QStringList TargetManager::templateFolders(const QString& target)
{
	QDir targetDir(QDir::currentPath() + "/" + TARGET_FOLDER + "/" + target + "/" + TEMPLATES_FOLDER);
	return targetDir.exists() ? targetDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot) : QStringList();
}

QStringList TargetManager::templates(const QString& target, const QString& folder)
{
	QDir targetDir(QDir::currentPath() + "/" + TARGET_FOLDER + "/" + target + "/" + TEMPLATES_FOLDER + 
		(folder.isEmpty() ? "" : (QString("/") + folder)));
	
	return targetDir.entryList(QStringList() << (QString("*.") + TEMPLATE_EXT), QDir::Files); 
}

QIcon TargetManager::templateIcon(const QString& target, const QString& _template, const QString& folder)
{	
	return QIcon(QDir::currentPath() + "/" + TARGET_FOLDER + "/" + 
		target + "/" + TEMPLATES_FOLDER + "/" + (folder.isEmpty() ? "" : (folder + "/")) + 
		_template + ".png");
}

QStringList TargetManager::allSupportedExtensions()
{
	QDir targetDir(QDir::currentPath() + "/" + TARGET_FOLDER);
	QStringList extensionList;

	// Choke if we can't find the target directory
	if(!targetDir.exists()) return extensionList;
	
	// Get a list of the possible target directories and check through them
	const QStringList& targetDirs = targetDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	QStringListIterator i(targetDirs);

	while(i.hasNext()) {
		QString dirName = i.next();
		targetDir.cd(dirName);
		QFileInfo targetFile(targetDir, dirName + "." + TARGET_EXT);
		if(targetFile.exists()) {
			QSettings target(targetFile.absoluteFilePath(), QSettings::IniFormat);
			extensionList << target.value(EXTENSIONS).toString().split("|");
		}
		
		targetDir.cdUp();
	}
	return extensionList;
}

QString TargetManager::getExpectedLocation(const QString& name) const { return QString(TARGET_FOLDER) + "/" + name; }