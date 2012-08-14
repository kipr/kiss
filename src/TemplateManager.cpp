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
#include <kiss-compiler/QTinyArchiveStream.h>

#define TEMPLATE_EXT_WILDCARD (QString("*.") + TEMPLATE_EXT)

QString TemplateManager::m_defaultName = "Default";

TemplateManager::TemplateManager()
{
	qDebug() << "Scanning" << packPath() << "for packs.";
	QFileInfoList entries = QDir(packPath()).entryInfoList(QStringList() << ("*." KISS_TEMPLATE_PACK_EXT), QDir::NoDot | QDir::NoDotDot | QDir::Files);
	foreach(const QFileInfo& entry, entries) {
		qDebug() << "Reading" << entry.canonicalFilePath();
		QFile file(entry.canonicalFilePath());
		if(!file.open(QIODevice::ReadOnly)) {
			qWarning() << "Template pack" << entry.completeBaseName() << "cannot be opened. Ignoring.";
			continue;
		}
		
		QTinyArchiveStream in(&file);
		TinyArchive *archive = TinyArchive::read(&in);
		file.close();
		
		if(!archive) {
			qWarning() << "Template pack" << entry.completeBaseName() << "is malformed. Ignoring.";
			continue;
		}
		// Assuming this is null terminated
		m_packs[entry.completeBaseName()] = archive;
	}
}

TemplateManager::~TemplateManager()
{
	QList<TinyArchive *>::iterator it = m_packs.values().begin();
	for(; it != m_packs.values().end(); ++it) {
		delete *it;
	}
}

QStringList TemplateManager::types() const
{
	QDir typeDir(QDir::currentPath() + "/" + TEMPLATES_FOLDER);
	const QStringList& entries = typeDir.exists()
		? typeDir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot)
		: QStringList();
	return entries;
}

QStringList TemplateManager::templateFolders(const QString& type, const QString& _template)
{
	QDir typeDir(pathForTemplate(type, _template));
	QStringList entries = typeDir.exists() ? typeDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot) : QStringList();
	
	// If no extra path, we're done
	if(_template.isEmpty()) return entries;
	
	// Add template path prefix
	QStringList ret;
	foreach(const QString& entry, entries) ret.append(_template + "/" + entry);

	return ret;
}

QStringList TemplateManager::templates(const QString& type, const QString& _template)
{
	if(!isTemplateDirectory(type, _template)) return QStringList();
	
	QDir typeDir(pathForTemplate(type, _template));
	QStringList entries = typeDir.entryList(QStringList() << TEMPLATE_EXT_WILDCARD, QDir::Files);
	
	// If no extra path, we're done
	if(_template.isEmpty()) return entries;
	
	// Add template path prefix
	QStringList ret;
	foreach(const QString& entry, entries) ret.append(_template + "/" + entry);
	return ret;
}

QIcon TemplateManager::templateIcon(const QString& type, const QString& _template)
{
	QFileInfo templateInfo(_template);
	const QString& base = pathForTemplate(type, templateInfo.path() + "/" + templateInfo.baseName());
	QFileInfo file(base + ".png");
	if(!file.exists()) file.setFile(file.path() + "/" + defaultTemplateName() + ".png");
	return QIcon(file.filePath());
}

QStringList TemplateManager::userTemplates(const QString& type)
{
	return QDir(pathForUserTemplate(type, "")).entryList(QDir::Files);
}

bool TemplateManager::addUserTemplate(const QString& type, const QString& name, const QString& content)
{
	ensureUserTemplateDirExists(type);
	QFile file(pathForUserTemplate(type, name) + "." + TEMPLATE_EXT);
	
	if(!file.open(QFile::WriteOnly | QFile::Truncate)) return false;
	
	QTextStream out(&file);
	out << content;
	out.flush();
	
	file.close();
	return true;
}

bool TemplateManager::deleteUserTemplate(const QString& type, const QString& name)
{
	return QFile(pathForUserTemplate(type, name)).remove();
}

QString TemplateManager::pathForUserTemplate(const QString& type, const QString& _template)
{
	return QDir::currentPath() + "/" + USER_FOLDER + "/" + TEMPLATES_FOLDER + "/" + type + (_template.isEmpty() ? "" : "/" + _template);
}

QStringList TemplateManager::packs() const
{
	return QStringList(m_packs.keys());
}

QStringList TemplateManager::packTemplates(const QString& pack) const
{
	QStringList ret;
	TinyArchive *archive = m_packs.value(pack, 0);
	if(!archive) {
		qWarning() << "Failed to lookup template pack" << pack;
		return ret;
	}
	
	QVector<TinyNode *> children = QVector<TinyNode *>::fromStdVector(archive->lookup("/")->children());
	QVector<TinyNode *>::const_iterator it = children.begin();
	for(; it != children.end(); ++it) {
		TinyNode *child = *it;
		if(!child->name()) {
			qWarning() << "TinyNode::name null during template pack lookup for" << pack;
			continue;
		}
		qDebug() << "Found pack template" << child->name();
		ret << child->name();
	}
	
	return ret;
}

QIcon TemplateManager::packTemplateIcon(const QString& pack, const QString& _template) const
{
	return QIcon();
}

QByteArray TemplateManager::packTemplateData(const QString& pack, const QString& _template) const
{
	TinyArchive *archive = m_packs.value(pack, 0);
	if(!archive) return QByteArray();
	
	const TinyNode *node = archive->lookup(_template.toStdString());
	
	return QByteArray(reinterpret_cast<const char *>(node->data()), node->length());
}

bool TemplateManager::installPack(const QString& name, const TinyArchive *archive)
{
	QFile file(QDir(packPath()).filePath(name + ("." KISS_TEMPLATE_PACK_EXT)));
	if(file.exists()) return false;
	if(!file.open(QIODevice::WriteOnly)) return false;
	
	QTinyArchiveStream stream(&file);
	archive->write(&stream);
	
	file.close();
	
	return true;
}

QString TemplateManager::pathForTemplate(const QString& type, const QString& _template)
{
	return QDir::currentPath() + "/" + TEMPLATES_FOLDER + "/" + type + (_template.isEmpty() ? "" : "/" + _template);
}

bool TemplateManager::isTemplateDirectory(const QString& type, const QString& _template)
{
	return isTemplateDirectory(pathForTemplate(type, _template));
}

bool TemplateManager::isTemplateDirectory(const QString& path) { return QFileInfo(path).isDir(); }
const QString& TemplateManager::defaultTemplateName() { return m_defaultName; }

void TemplateManager::ensureUserTemplateDirExists(const QString& type)
{
	QDir().mkpath(pathForUserTemplate(type, ""));
}

QString TemplateManager::packPath() const
{
	return QDir::currentPath() + "/" + USER_FOLDER + "/" + TEMPLATES_FOLDER;
}