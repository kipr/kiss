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

#include "Project.h"

#include "QTinyArchive.h"
#include "TargetManager.h"

#include <QFileInfo>
#include <QDebug>

#define SETTINGS_FILE "settings:"

#define PROJECT_NAME_SETTING "PROJECT_NAME"

class ReadFailedException : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Unable to read archive";
	}
};

Project::Project(TinyArchiveReader* reader, TinyArchiveWriter* writer)
	: WorkingUnit("Project"), m_writer(writer)
{
	m_archive = (QTinyArchive*)TinyArchive::read(reader);
	if(!m_archive) throw ReadFailedException();
	
	processSettings(settings()); // Initial setting processing
	setName(settings()[PROJECT_NAME_SETTING]);
}

Project::Project(TinyArchiveWriter* writer)
	: WorkingUnit("Project"), m_writer(writer)
{
	m_archive = new QTinyArchive();
	m_archive->add(SETTINGS_FILE, SETTINGS_ID);
	setName(settings()[PROJECT_NAME_SETTING]);
}

Project::~Project()
{
	delete m_writer;
	if(m_archive) delete m_archive;
}

bool Project::addFile(const QString& path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly)) return false;
	m_archive->add(QFileInfo(file).fileName(), file.readAll());
	file.close();
	return true;
}

void Project::setName(const QString& name)
{
	m_name = name;
}

const bool Project::sync()
{
	return m_archive->write(m_writer);
}

QStringList Project::files() const
{
	QStringList ret = m_archive->files();
	return ret;
}

QStringList Project::resources() const
{
	return files(); // TODO: NYI
}

QStringList Project::sources() const
{
	return files(); // TODO: NYI
}

QString Project::outputPath() const
{
	return "";
}

const QString& Project::name()
{
	return m_name;
}

QTinyArchive* Project::archive() const
{
	return m_archive;
}

void Project::setAssociatedPort(const QString& associatedPort)
{
	m_associatedPort = associatedPort;
}

const QString& Project::associatedPort() const
{
	return m_associatedPort;
}

bool Project::isAssociatedWithPort() const
{
	return m_associatedPort.isEmpty();
}

const bool Project::updateSetting(const QString& key, const QString& value)
{
	QStringMap current = settings();
	current[key] = value;
	setSettings(current);
	
	qWarning() << "Setting update emitted";
	emit settingUpdated(key);
	
	return true;
}

const bool Project::removeSetting(const QString& key)
{
	QStringMap current = settings();
	QStringMap::iterator it = current.find(key);
	if(it == current.end()) return false;
	current.erase(it);
	setSettings(current);
	
	emit settingRemoved(key);
	
	return true;
}

void Project::setSettings(const QStringMap& settings)
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << settings;
	m_archive->put(SETTINGS_FILE, data, SETTINGS_ID);
	processSettings(settings);
	emit settingsChanged();
}

QStringMap Project::settings() const
{
	QByteArray data = QTinyNode::data(m_archive->lookup(SETTINGS_FILE));
	QDataStream stream(&data, QIODevice::ReadOnly);
	
	QStringMap ret;
	stream >> ret;
	return ret;
}

void Project::setTargetName(const QString& target)
{
	updateSetting(TARGET_KEY, target);
}

Project* Project::load(const QString& path)
{
	TinyArchiveFile* file = new TinyArchiveFile(path.toStdString());
	Project* ret = 0;
	try {
		ret = new Project(file, file);
	} catch(const ReadFailedException& e) {
		qWarning() << e.what();
		return 0;
	}
	
	ret->setName(QFileInfo(path).baseName());
	
	return ret;
}

Project* Project::create(const QString& path)
{
	TinyArchiveFile* file = new TinyArchiveFile(path.toStdString());
	Project* ret = new Project(file);	
	ret->setName(QFileInfo(path).baseName());
	return ret;
}

void Project::processSettings(const QStringMap& settings)
{
	foreach(const QString& key, settings.keys()) {
		if(key == TARGET_KEY) {
			const QString& path = TargetManager::ref().targetFilePath(settings[key]);
			qWarning() << "Set target to" << path;
			target()->setTargetFile(path);
		}
	}
}