/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
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

#ifndef __TAB_H__
#define __TAB_H__

class MainWindow;
class QMenu;
class QMenuBar;
class QToolBar;

class Tab
{
public:	
	virtual void activate() = 0;
	
	// All of these are hooks to add menu items
	virtual void addActionsFile(QMenu* file) = 0;
	virtual void addActionsEdit(QMenu* edit) = 0;
	virtual void addActionsHelp(QMenu* help) = 0;
	virtual void addOtherActions(QMenuBar* menuBar) = 0;
	virtual void addToolbarActions(QToolBar* toolbar) = 0;
	
	virtual bool beginSetup() = 0;
	virtual void completeSetup() = 0;
	
	virtual bool close() = 0;
	
	virtual void refreshSettings() = 0;
	virtual void moveTo(int line, int pos) = 0;
	
	virtual bool exclusive() { return false; }
protected:
};

#endif