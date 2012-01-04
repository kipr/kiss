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

#include "TemplateManager.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#define TEMPLATE_EXT_WILDCARD (QString("*.") + TEMPLATE_EXT)

QString TemplateManager::m_defaultName = "Default";

QStringList TemplateManager::templateFolders(const QString& target, const QString& _template)
{
	QDir targetDir(pathForTemplate(target, _template));
	QStringList entries = targetDir.exists() ? targetDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot) : QStringList();
	
	// If no extra path, we're done
	if(_template.isEmpty()) return entries;
	
	// Add template path prefix
	QStringList ret;
	foreach(const QString& entry, entries) ret.append(_template + "/" + entry);

	return ret;
}

QStringList TemplateManager::templates(const QString& target, const QString& _template)
{
	if(!isTemplateDirectory(target, _template)) return QStringList();
	
	QDir targetDir(pathForTemplate(target, _template));
	QStringList entries = targetDir.entryList(QStringList() << TEMPLATE_EXT_WILDCARD, QDir::Files);
	
	// If no extra path, we're done
	if(_template.isEmpty()) return entries;
	
	// Add template path prefix
	QStringList ret;
	foreach(const QString& entry, entries) ret.append(_template + "/" + entry);
	return ret;
}

QIcon TemplateManager::templateIcon(const QString& target, const QString& _template)
{
	QFileInfo templateInfo(_template);
	const QString& base = pathForTemplate(target, templateInfo.path() + "/" + templateInfo.baseName());
	QFileInfo file(base + ".png");
	if(!file.exists()) file.setFile(file.path() + "/" + defaultTemplateName() + ".png");
	return QIcon(file.filePath());
}

QStringList TemplateManager::userTemplates(const QString& target)
{
	return QDir(pathForUserTemplate(target, "")).entryList(QDir::Files);
}

bool TemplateManager::addUserTemplate(const QString& target, const QString& name, const QString& content)
{
	ensureUserTemplateDirExists(target);
	QFile file(pathForUserTemplate(target, name) + "." + TEMPLATE_EXT);
	
	if(!file.open(QFile::WriteOnly | QFile::Truncate)) return false;
	
	QTextStream out(&file);
	out << content;
	out.flush();
	
	file.close();
	return true;
}

bool TemplateManager::deleteUserTemplate(const QString& target, const QString& name)
{
	return QFile(pathForUserTemplate(target, name)).remove();
}

QString TemplateManager::pathForUserTemplate(const QString& target, const QString& _template)
{
	return QDir::currentPath() + "/" + USER_FOLDER + "/" + TEMPLATES_FOLDER + "/" + target + (_template.isEmpty() ? "" : "/" + _template);
}

QString TemplateManager::pathForTemplate(const QString& target, const QString& _template)
{
	return QDir::currentPath() + "/" + TARGET_FOLDER + "/" + target + "/" + TEMPLATES_FOLDER + (_template.isEmpty() ? "" : "/" + _template);
}

bool TemplateManager::isTemplateDirectory(const QString& target, const QString& _template)
{
	return isTemplateDirectory(pathForTemplate(target, _template));
}

bool TemplateManager::isTemplateDirectory(const QString& path) { return QFileInfo(path).isDir(); }
const QString& TemplateManager::defaultTemplateName() { return m_defaultName; }

void TemplateManager::ensureUserTemplateDirExists(const QString& target)
{
	QDir().mkpath(pathForUserTemplate(target, ""));
}