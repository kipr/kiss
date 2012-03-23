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

#include "TinyArchiveProjectFile.h"

#include <QFileInfo>

#include <QObject>

Project::Project(ProjectFile* projectFile) : WorkingUnit("Project"), m_projectFile(projectFile)
{
	setName(QFileInfo(projectFile->path()).fileName());
	connect(projectFile, SIGNAL(fileCreated(const QString&)), SIGNAL(updated()));
	connect(projectFile, SIGNAL(fileChanged(const QString&)), SIGNAL(updated()));
	connect(projectFile, SIGNAL(fileRemoved(const QString&)), SIGNAL(updated()));
}

bool Project::addFile(const QString& path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly)) return false;
	QDataStream in(&file);
	m_projectFile->addFile(QFileInfo(file).fileName(), in);
	file.close();
	emit updated();
	return m_projectFile->sync();
}

bool Project::createFile(const QString& path)
{
	m_projectFile->addFile(path, QByteArray());
	emit updated();
	return m_projectFile->sync();
}

void Project::setName(const QString& name)
{
	m_name = name;
	emit updated();
}

void Project::refresh()
{
	m_projectFile->sync();
}

QStringList Project::files() const
{
	return m_projectFile->list();
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

ProjectFile* Project::projectFile() const
{
	return m_projectFile;
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

void Project::fileAdded(const QString& file)
{
	emit updated();
}

void Project::fileRemoved(const QString& file)
{
	emit updated();
}

Project* Project::load(const QString& path)
{
	Project* ret = new Project(new TinyArchiveProjectFile(path));
	return ret;
}

Project* Project::create(const QString& path)
{
	Project* ret = new Project(new TinyArchiveProjectFile(path));
	return ret;
}