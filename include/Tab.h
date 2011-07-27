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

/*! \class Tab
 * \brief Provides an interface all Tabs must implement
 *
 * A Tab must implement both this interface and QWidget, which is expected once passed to MainWindow for processing.
 */
class Tab
{
public:	
	/*!
	 * Called every time this Tab is switched to. Useful for setting window title.
	 */
	virtual void activate() = 0;
	
	/*!
	 * Allows a Tab to add actions to the "File" menu in the menu bar.
	 * \param file Pointer to current file menu
	 */
	virtual void addActionsFile(QMenu* file) = 0;
	
	/*!
	 * Allows a Tab to add actions to the "Edit" menu in the menu bar.
	 * \param edit Pointer to current edit menu
	 */
	virtual void addActionsEdit(QMenu* edit) = 0;
	
	/*!
	 * Allows a Tab to add actions to the "Help" menu in the menu bar.
	 * \param help Pointer to current help menu
	 */
	virtual void addActionsHelp(QMenu* help) = 0;
	
	/*!
	 * Allows a Tab to create menus and add actions to them.
	 * \param menuBar Pointer to current menu bar
	 */
	virtual void addOtherActions(QMenuBar* menuBar) = 0;
	
	/*!
	 * Allows a Tab to add actions to the tool bar.
	 * \param toolbar Pointer to current tool bar
	 */
	virtual void addToolbarActions(QToolBar* toolbar) = 0;
	
	/*!
	 * Begin setup is called before a Tab is added to the Tab Widget. 
	 * \return false cancels adding this Tab, true continues setup process
	 */
	virtual bool beginSetup() = 0;
	
	/*!
	 * The Tab has been added to the Tab Widget. Set the Tab Name here.
	 */
	virtual void completeSetup() = 0;
	
	/*!
	 * Requests a Tab to close
	 * \return false cancels the close operation, true closes the Tab
	 */
	virtual bool close() = 0;
	
	virtual void refreshSettings() = 0;
protected:
};

#endif