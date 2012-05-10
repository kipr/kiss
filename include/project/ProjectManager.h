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

#ifndef _PROJECTMANAGER_H_
#define _PROJECTMANAGER_H_

#include "Singleton.h"

#include <QObject>
#include <QList>
#include <QMap>

class Project;
class ArchiveWriter;

class ProjectManager : public QObject, public Singleton<ProjectManager>
{
Q_OBJECT
public:
	~ProjectManager();
	
	void openProject(Project* project);
	void closeProject(Project* project);
	
	const QList<Project*>& projects();
	ArchiveWriter* archiveWriter(Project* project);
	
signals:
	void projectOpened(Project* project);
	void projectClosed(Project* project);
	void projectChanged(Project* project);

private slots:
	void projectUpdated();

private:
	QList<Project*> m_projects;
	QMap<Project*, ArchiveWriter*> m_writers;
};

#endif
