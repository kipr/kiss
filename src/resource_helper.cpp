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

#include "resource_helper.hpp"

#include "log.hpp"

#include <QDir>
#include <QDebug>

using namespace Kiss;

ResourceHelper::ResourceHelper()
{
	addSearchLocation(":/shortcuts/target/icon_set/icons/");
	addSearchLocation(":/shortcuts/edit/icon_set/icons/");
	addSearchLocation(":/debugger/icon_set/icons/");
	addSearchLocation(":/shortcuts/file/icon_set/icons/");
	addSearchLocation(":/browser/icon_set/icons/");
	addSearchLocation(":/icon_set/icons/");
	addSearchLocation(":/welcome/");
	addSearchLocation(":/sourcefile/icon_set/icons/");
	addSearchLocation(":/targets/");
	addSearchLocation(":/");
	
	addSearchLocation("resources"); // For user resources
}

QString ResourceHelper::lookup(const QString& name)
{
	if(name.isEmpty()) return "";
	
	QMap<QString, QString>::iterator it = m_cache.find(name);
	if(it != m_cache.end()) return *it;
	
	QDir dir;
	foreach(const QString& location, m_locations) {
		dir.setPath(location);
		QFileInfoList entries = dir.entryInfoList(QStringList() << QString("*") + name + "*");
		if(entries.size() == 0) continue;
		const QString& path = entries[0].canonicalFilePath();
		if(entries.size() > 1) {
			Log::ref().warning(QString("More than one resource in %1 matches pattern %2. Using %3")
				.arg(dir.path()).arg(name).arg(path));
		}
		m_cache[name] = path;
		return path;
	}
	
	qWarning() << "ResourceHelper Failed to lookup resource" << name
		<< "(tried" << m_locations.size() << "locations)";
	
	return "";
}

QIcon ResourceHelper::icon(const QString& name)
{
	return QIcon(lookup(name));
}

QPixmap ResourceHelper::pixmap(const QString& name)
{
	return QPixmap(lookup(name));
}

const QStringList& ResourceHelper::searchLocations()
{
	return m_locations;
}

void ResourceHelper::addSearchLocation(const QString& location)
{
	m_locations.append(location);
}
