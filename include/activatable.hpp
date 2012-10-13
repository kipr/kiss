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

#ifndef _ACTIVATABLE_H_
#define _ACTIVATABLE_H_

#include <QObject>

template<typename T>
class Activatable
{
public:
	Activatable()
		: m_active(0)
	{
	}
	
	void setActive(T *active)
	{
		m_active = active;
		if(active) activated();
		else deactivated();
	}
	
	T *active()
	{
		return m_active;
	}
	
	bool isActive()
	{
		return m_active;
	}
	
	void deactivateIfActive(T *active)
	{
		if(m_active == active) setActive(0);
	}
	
protected:
	virtual void activated()
	{
	}
	
	virtual void deactivated()
	{
	}
	
private:
	T *m_active;
};


class ActivatableObject : public Activatable<QObject>
{
public:
	ActivatableObject();
	ActivatableObject(const char *slot);
	~ActivatableObject();
	
	virtual void activated();
	virtual void deactivated();
	
	friend class ObjectDestroyed;
	
private:
	void destroyed(QObject *obj);
	
	QObject *m_connected;
	
	const char *m_signature;
};

#endif
