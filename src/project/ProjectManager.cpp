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

#include "ProjectManager.h"

#include "Project.h"
#include "ArchiveWriter.h"
#include <kiss-compiler/Temporary.h>

ProjectManager::~ProjectManager()
{
	foreach(Project* p, m_projects) {
		p->sync();
		delete p;
	}
}

void ProjectManager::openProject(Project* project)
{
	m_projects.append(project);
	m_writers[project] = new ArchiveWriter(project->archive(), Temporary::subdir(project->name()));
	emit projectOpened(project);
	connect(project, SIGNAL(updated()), SLOT(projectUpdated()));
}

void ProjectManager::closeProject(Project* project)
{
	m_projects.removeAll(project);
	delete archiveWriter(project);
	m_writers.remove(project);
	emit projectClosed(project);
	delete project;
}

const QList<Project*>& ProjectManager::projects() { return m_projects; }

ArchiveWriter* ProjectManager::archiveWriter(Project* project)
{
	QMap<Project*, ArchiveWriter*>::const_iterator it = m_writers.find(project);
	if(it == m_writers.end()) return 0;
	return *it;
}

void ProjectManager::projectUpdated()
{
	emit projectChanged(static_cast<Project*>(sender()));
}