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

#include "UiEventManager.h"

#include <QDebug>

void UiEventManager::addListener(UiEventListener* listener)
{
	if(m_listeners.contains(listener)) return;
	m_listeners.append(listener);
}

void UiEventManager::removeListener(UiEventListener* listener)
{
	m_listeners.removeAll(listener);
}

void UiEventManager::sendEvent(const QString& name, const QStringList& args)
{
	foreach(UiEventListener* l, m_listeners) l->event(name, args);
}