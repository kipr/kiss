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

#ifndef _RESOURCE_HELPER_HPP_
#define _RESOURCE_HELPER_HPP_

#include "singleton.hpp"

#include <QStringList>
#include <QMap>
#include <QIcon>
#include <QPixmap>

namespace Kiss
{
	struct ResourceHelper : Singleton<ResourceHelper>
	{
		ResourceHelper();

		QString lookup(const QString& name);
		QIcon icon(const QString& name);
		QPixmap pixmap(const QString& name);

		const QStringList& searchLocations();
		void addSearchLocation(const QString& location);

	private:
		QMap<QString, QString> m_cache;

		QStringList m_locations;
	};
}

#endif
